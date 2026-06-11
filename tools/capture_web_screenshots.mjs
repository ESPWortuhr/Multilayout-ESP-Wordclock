#!/usr/bin/env node
import { mkdir, mkdtemp, rm, writeFile } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { spawn } from "node:child_process";

const CHROMIUM = "/Applications/Chromium.app/Contents/MacOS/Chromium";
const BASE_URL = process.argv[2] ?? "http://127.0.0.1:8080/";
const OUTPUT_DIR = new URL("../docs/Logos/", import.meta.url);
const WIDTH = 1280;
const HEIGHT = 720;

let nextId = 1;

function delay(ms) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

async function waitForJson(url, timeoutMs = 10000) {
	const started = Date.now();
	while (Date.now() - started < timeoutMs) {
		try {
			const response = await fetch(url);
			if (response.ok) return await response.json();
		} catch {
			// Chromium is still starting.
		}
		await delay(100);
	}
	throw new Error(`Timed out waiting for ${url}`);
}

function send(socket, method, params = {}) {
	const id = nextId++;
	socket.send(JSON.stringify({ id, method, params }));
	return new Promise((resolve, reject) => {
		const onMessage = event => {
			const message = JSON.parse(event.data);
			if (message.id !== id) return;
			socket.removeEventListener("message", onMessage);
			if (message.error) {
				reject(new Error(`${method}: ${message.error.message}`));
				return;
			}
			resolve(message.result);
		};
		socket.addEventListener("message", onMessage);
	});
}

async function connectToPage() {
	const tabs = await waitForJson("http://127.0.0.1:9222/json/list");
	const page = tabs.find(tab => tab.type === "page");
	if (!page?.webSocketDebuggerUrl) throw new Error("No Chromium page target found");

	const socket = new WebSocket(page.webSocketDebuggerUrl);
	await new Promise((resolve, reject) => {
		socket.addEventListener("open", resolve, { once: true });
		socket.addEventListener("error", reject, { once: true });
	});
	return socket;
}

async function navigate(socket) {
	await send(socket, "Page.enable");
	await send(socket, "Runtime.enable");
	await send(socket, "Emulation.setDeviceMetricsOverride", {
		width: WIDTH,
		height: HEIGHT,
		deviceScaleFactor: 1,
		mobile: false,
	});
	await send(socket, "Page.navigate", { url: BASE_URL });
	await delay(2000);
	await send(socket, "Runtime.evaluate", {
		expression: `
			document.documentElement.style.scrollBehavior = 'auto';
			document.body.style.scrollBehavior = 'auto';
		`,
		awaitPromise: false,
	});
	await assertPageLoaded(socket);
}

async function navigateSection(socket, section) {
	await send(socket, "Runtime.evaluate", {
		expression: `
			(() => {
				document.querySelectorAll('.section').forEach(sec => { sec.style.display = 'none'; });
				const target = document.querySelector(${JSON.stringify(`.section-${section}`)});
				if (!target) throw new Error('Missing section: ${section}');
				target.style.display = 'block';

				document.querySelectorAll('.pure-menu-link').forEach(link => {
					link.classList.remove('pure-menu-selected');
					link.removeAttribute('aria-current');
				});
				const nav = document.querySelector(${JSON.stringify(`a[data-navigation="${section}"]`)});
				if (nav) {
					nav.classList.add('pure-menu-selected');
					nav.setAttribute('aria-current', 'page');
				}
				window.scrollTo(0, 0);
			})()
		`,
		awaitPromise: false,
	});
	await delay(400);
}

async function assertPageLoaded(socket) {
	const result = await send(socket, "Runtime.evaluate", {
		expression: `
			(() => {
				const bodyText = document.body ? document.body.innerText : '';
				return {
					title: document.title,
					hasLayout: Boolean(document.querySelector('.layout')),
					hasTranslatedText: bodyText.includes('Funktionen') || bodyText.includes('Einstellungen') || bodyText.includes('Anzeigeoptionen'),
					isChromiumErrorPage: bodyText.includes('Die Website ist nicht erreichbar') || bodyText.includes('ERR_ADDRESS_UNREACHABLE')
				};
			})()
		`,
		awaitPromise: false,
		returnByValue: true,
	});
	const value = result.result?.value;
	if (!value?.hasLayout || value.isChromiumErrorPage || !value.hasTranslatedText) {
		throw new Error(`Web UI did not load correctly: ${JSON.stringify(value)}`);
	}
}

async function jumpTo(socket, selector, offset = 16) {
	await send(socket, "Runtime.evaluate", {
		expression: `
			(() => {
				document.querySelectorAll('.section-connection-lost, .section-reboot-recommended')
					.forEach(sec => { sec.style.display = 'none'; });
				const element = document.querySelector(${JSON.stringify(selector)});
				if (!element) throw new Error('Missing selector: ${selector}');
				const top = element.getBoundingClientRect().top + window.scrollY - ${offset};
				window.scrollTo(0, Math.max(0, top));
			})()
		`,
		awaitPromise: false,
	});
	await delay(250);
}

async function screenshot(socket, filename) {
	const result = await send(socket, "Page.captureScreenshot", {
		format: "png",
		fromSurface: true,
		captureBeyondViewport: false,
	});
	await writeFile(new URL(filename, OUTPUT_DIR), Buffer.from(result.data, "base64"));
}

async function main() {
	await mkdir(OUTPUT_DIR, { recursive: true });
	const userDataDir = await mkdtemp(join(tmpdir(), "wortuhr-chromium-"));
	const chromium = spawn(CHROMIUM, [
		"--headless=new",
		"--remote-debugging-port=9222",
		`--user-data-dir=${userDataDir}`,
		"--disable-gpu",
		"--no-first-run",
		"--no-default-browser-check",
		`--window-size=${WIDTH},${HEIGHT}`,
		"about:blank",
	], {
		stdio: ["ignore", "ignore", "pipe"],
	});

	try {
		const socket = await connectToPage();
		await navigate(socket);

		await navigateSection(socket, "functions");
		await jumpTo(socket, ".section-functions", 80);
		await screenshot(socket, "WebFrontendFunctionsModes.png");

		await send(socket, "Runtime.evaluate", {
			expression: `
				(() => {
					const modeSymbol = document.getElementById('mode-symbol');
					if (modeSymbol) modeSymbol.checked = true;
					const symbolBox = document.getElementById('symbol-box');
					if (symbolBox) symbolBox.style.display = 'block';
				})()
			`,
			awaitPromise: false,
		});
		await delay(250);
		await jumpTo(socket, "#symbol-box", 120);
		await screenshot(socket, "WebFrontendSymbolMode.png");

		await navigateSection(socket, "frontoptions");
		await jumpTo(socket, "#buildtype", 300);
		await screenshot(socket, "WebFrontendFrontBuildType.png");

		await navigateSection(socket, "settings");
		await jumpTo(socket, "#led-pin", 150);
		await screenshot(socket, "WebFrontendHardwarePins.png");

		await jumpTo(socket, "#timeserver", 150);
		await screenshot(socket, "WebFrontendTimeServer.png");

		await jumpTo(socket, "#timezone", 150);
		await screenshot(socket, "WebFrontendTimezone.png");

		await jumpTo(socket, "#auto-bright-enabled", 150);
		await screenshot(socket, "WebFrontendAutoBrightness.png");

		await jumpTo(socket, "#ssid", 150);
		await screenshot(socket, "WebFrontendWifi.png");

		socket.close();
	} finally {
		chromium.kill("SIGTERM");
		await delay(300);
		await rm(userDataDir, { recursive: true, force: true });
	}
}

main().catch(error => {
	console.error(error);
	process.exit(1);
});
