"use strict";
const iro = window.iro; // require("@jaames/iro");
(function(window, document) {

	let layout = document.querySelector(".layout");
	let menu = document.querySelector(".menu");
	let menuLink = document.querySelector(".hamburger");

	function toggleMenuEventHandler(event) {
		// It's a link, so don't activate the link but do some JS instead
		event.preventDefault();

		const active = "active";
		const menuIsOpen = menuLink.getAttribute("aria-expanded") === "true";

		if (menuIsOpen) {
			// Close menu
			console.log("is open, closing");
			menuLink.setAttribute("aria-expanded", "false");
			menuLink.setAttribute("aria-label", i18next.t("menu.aria-show-menu"));
			layout.classList.remove(active);
			menu.classList.remove(active);
		} else {
			console.log("is closed, opening");
			// Open menu
			menuLink.setAttribute("aria-expanded", "true");
			menuLink.setAttribute("aria-label", i18next.t("menu.aria-hide-menu"));
			layout.classList.add(active);
			menu.classList.add(active);
		}
	}

	if (menuLink) {
		menuLink.addEventListener("click", toggleMenuEventHandler);
	}
}(this, this.document));

// minified.js dependency removed
let debugMessageReconfigured = " was reconfigured.";
let websocket;
let ipEsp = "ws://192.168.4.1";
let debug = true;
let command = 1;
let hsb = [
	[0, 100, 50],
	[120, 100, 50],
	[240, 100, 50]
];
let colorPosition = 0;
let effectBri = 2;
let effectSpeed = 10;
let langVar = [0, 0, 0, 0, 0];
let layVar = [0, 0, 0, 0, 0, 0];
let itIsVar = 0;
let hasSpecialWordHappyBirthday = 0;
let autoBrightDisplay = 0;
let autoBrightEnabled = 0;
let autoBrightInterval = null;
let autoBrightMin = 10;
let autoBrightMax = 80;
let autoBrightPeak = 750;
let transitionType = 0;
let transitionDuration = 1;
let transitionSpeed = 30;
let transitionColorize = 1;
let transitionDemo = false;

const CMD = {
	// Modes
	MODE_WORD_CLOCK: 1,
	MODE_SECONDS: 2,
	MODE_SCROLLINGTEXT: 3,
	MODE_RAINBOWCYCLE: 4,
	MODE_RAINBOW: 5,
	MODE_COLOR: 6,
	MODE_DIGITAL_CLOCK: 7,
	MODE_SYMBOL: 8,
	MODE_TRANSITION: 10,

	// Settings
	SET_INITIAL_VALUES: 20,
	SET_TIME: 30,
	SET_BIRTHDAYS: 83,
	SET_LANGUAGE_VARIANT: 84,
	SET_MQTT: 85,
	SET_TIME_MANUAL: 86,
	SET_BUILDTYPE: 87,
	SET_COLORTYPE: 88,
	SET_UHRTYPE: 89,
	SET_WEATHER_DATA: 90,
	SET_WHITETYPE: 91,
	SET_HOSTNAME: 92,
	SET_SETTING_SECOND: 93,
	SET_MINUTE: 94,
	SET_BRIGHTNESS: 95,
	SET_SCROLLINGTEXT: 96,
	SET_TIMESERVER: 97,
	SET_WIFI_DISABLED: 98,
	SET_WIFI_AND_RESTART: 99,
	RESET: 100,
	SET_BOOT: 101,
	SET_AUTO_BRIGHT: 102,
	SET_LAYOUT_VARIANT: 103,
	SET_MQTT_HA_DISCOVERY: 104,
	SET_SYMBOL: 105,
	SET_IT_IS_VARIANT: 106,
	SPEED: 152,

	// Requests
	REQ_CONFIG_VALUES: 200,
	REQ_COLOR_VALUES: 201,
	REQ_WIFI_LIST: 202,
	REQ_AUTO_BRIGHT: 203,
	REQ_TRANSITION: 204,
	REQ_MQTT_VALUES: 205,
	REQ_BIRTHDAYS: 206
};

/**
 * Maps the mode to the corresponding input id on the functions page.
 *
 * @type {Map<number, String>}
 */
const MODE_TO_INPUT_ID = new Map();
MODE_TO_INPUT_ID.set(0, "mode-wordclock"); // Map CMD.IDLE to mode wordclock
MODE_TO_INPUT_ID.set(CMD.MODE_WORD_CLOCK, "mode-wordclock");
MODE_TO_INPUT_ID.set(CMD.MODE_SECONDS, "mode-seconds");
MODE_TO_INPUT_ID.set(CMD.MODE_SCROLLINGTEXT, "mode-scrollingtext");
MODE_TO_INPUT_ID.set(CMD.MODE_RAINBOWCYCLE, "mode-rainbow");
MODE_TO_INPUT_ID.set(CMD.MODE_RAINBOW, "mode-change"); // Color change
MODE_TO_INPUT_ID.set(CMD.MODE_COLOR, "mode-color");
MODE_TO_INPUT_ID.set(CMD.MODE_DIGITAL_CLOCK, "mode-digital-clock");
MODE_TO_INPUT_ID.set(CMD.MODE_SYMBOL, "mode-symbol");
MODE_TO_INPUT_ID.set(CMD.MODE_TRANSITION, "mode-wordclock");

// data that gets send back to the esp
const DATA_SCROLLINGTEXT_LENGTH = 30;
const DATA_TIMESERVER_TEXT_LENGTH = 30;
const DATA_MQTT_RESPONSE_TEXT_LENGTH = 30;
const DATA_HOST_TEXT_LENGTH = 30;

// color pickers
let colorPicker;

/**
 * Indicates local development of the webinterface.
 * @returns {boolean|boolean}
 */
function isLocalEnvironment() {
	// location.host will be an empty string if loading the HTML from file:/// protocol.
	return location.host === "" || location.host.includes("localhost");
}
function initConfigValues() {

	// Load from hardcoded IP when doing development locally.
	if (isLocalEnvironment()) {
		ipEsp = "ws://192.168.2.167/";
	} else {
		ipEsp = `ws://${location.host}`;
	}

	debug = true;
	command = 1;
	hsb = [
		[0, 100, 50],
		[120, 100, 50],
		[240, 100, 50]
	];
	effectBri = 2;
	effectSpeed = 10;
	langVar = [0, 0, 0, 0, 0];
	layVar = [0, 0, 0, 0, 0, 0];
	itIsVar = 0;
	hasSpecialWordHappyBirthday = 0;
	autoBrightDisplay = 0;
	autoBrightEnabled = 0;
	autoBrightMin = 10;
	autoBrightMax = 80;
	autoBrightPeak = 750;
	transitionType = 0;
	transitionDuration = 1;
	transitionSpeed = 30;
	transitionColorize = 1;
	transitionDemo = false;
}

/* eslint-disable no-console */
function debugMessage(debugMessage, someObject) {
	if (debug === true) {

		if (console !== undefined) {
			if (someObject) {
				console.log(debugMessage, someObject);
			} else {
				console.log(debugMessage);
			}
		}

		const outputEl = document.getElementById("output");
		if (outputEl) outputEl.textContent = debugMessage;
	}
}

/// only shows elements of class `cls` if `enbl` is true.
function enableSpecific(cls, enbl) {
	let items = document.getElementsByClassName(cls);
	for (const item of items) {
		item.style.display = enbl ? "block" : "none";
	}
}

function removeSpecificOption(cls, val, bool) {
	if (bool) {
		let selectobject = document.getElementById(cls);
		if (!selectobject) return;
		for (let i = 0; i < selectobject.length; i++) {
			if (selectobject.options[i].value === val) {
				selectobject.remove(i);
			}
		}
	}
}

// handle click events on the swatch

let swatchGrid = document.getElementById("swatch-grid");
if (swatchGrid) {
	swatchGrid.addEventListener("click", function(ext) {
		let clickTarget = ext.target;
		// read data-color attribute
		if (clickTarget.dataset.color) {
			// update the color picker
			colorPicker.color.set(clickTarget.dataset.color);
			changeColor(colorPicker.color);
		}
	});
}

function initWebsocket() {
	websocket = new WebSocket(ipEsp);

	websocket.onopen = function(event) {

		const statusEl = document.getElementById("status");
		if (statusEl) {
			statusEl.classList.add("online");
			statusEl.classList.remove("offline");
			statusEl.setAttribute("value", "Online");
		}

		document.querySelectorAll(".status-button").forEach(btn => {
			btn.textContent = "Disconnect";
			btn.value = "1";
		});

		const connLostEl = document.getElementById("section-connection-lost");
		if (connLostEl) connLostEl.style.display = "none";

		debugMessage("The connection with the websocket has been established.", event);

		sendCmd(CMD.REQ_COLOR_VALUES);
		sendCmd(CMD.REQ_TRANSITION);
	};

	websocket.onclose = function(event) {

		const statusEl = document.getElementById("status");
		if (statusEl) {
			statusEl.classList.remove("online");
			statusEl.classList.add("offline");
			statusEl.setAttribute("value", "Offline");
		}

		document.querySelectorAll(".status-button").forEach(btn => {
			btn.textContent = "Verbinden";
			btn.value = "0";
		});

		const connLostEl = document.getElementById("section-connection-lost");
		if (connLostEl) connLostEl.style.display = "block";

		autoBrightStop();

		debugMessage(`The connection with the websocket was closed (code ${event.code}).`, event);
	};

	websocket.onmessage = function(event) {

		const modeColorForm = document.getElementById("mode-color-form");

		let data = JSON.parse(event.data);

		debugMessage(`WebSocket response arrived (command ${data.command}).`, data);

		switch (data.command) {
			case "mqtt": {
				document.getElementById("mqtt-port").value = data.MQTT_Port;
				const mqttServer = document.getElementById("mqtt-server");
				mqttServer.value = data.MQTT_Server;
				mqttServer.setAttribute("maxlength", DATA_MQTT_RESPONSE_TEXT_LENGTH);
				document.getElementById("mqtt-state").checked = data.MQTT_State;
				const mqttUser = document.getElementById("mqtt-user");
				mqttUser.value = data.MQTT_User;
				mqttUser.setAttribute("maxlength", DATA_MQTT_RESPONSE_TEXT_LENGTH);
				const mqttPass = document.getElementById("mqtt-pass");
				mqttPass.value = data.MQTT_Pass;
				mqttPass.setAttribute("maxlength", DATA_MQTT_RESPONSE_TEXT_LENGTH);
				const mqttClientId = document.getElementById("mqtt-clientid");
				mqttClientId.value = data.MQTT_ClientId;
				mqttClientId.setAttribute("maxlength", DATA_MQTT_RESPONSE_TEXT_LENGTH);
				const mqttTopic = document.getElementById("mqtt-topic");
				mqttTopic.value = data.MQTT_Topic;
				mqttTopic.setAttribute("maxlength", DATA_MQTT_RESPONSE_TEXT_LENGTH);
				break;
			}
			case "birthdays":
				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;
				for (let i = 0; i < 5; i++) {
					const el = document.getElementById(`birthdays-date${i}`);
					if (el) el.value = data[`birthdayDate${i}`];
				}
				break;

			case "config": {
				document.getElementById("ssid").value = data.ssid;
				document.getElementById("timeserver").value = data.timeserver;
				document.getElementById("hostname").value = data.hostname;
				document.getElementById("scrollingtext").value = data.scrollingText;

				[6, 8, 12, 16, 18, 20, 22, 24].forEach(h => {
					const el = document.getElementById(`brightness-${h}`);
					if (el) el.value = data[`h${h}`];
				});

				for (let i = 0; i < 5; i++) {
					const langVarEl = document.getElementById(`langvar-${i}`);
					if (langVarEl) langVarEl.value = data[`langVar${i}`];
				}

				for (let i = 0; i < 6; i++) {
					const layvarEl = document.getElementById(`layvar-${i}`);
					if (layvarEl) layvarEl.checked = data[`layVar${i}`];
				}

				itIsVar = data.itIsVariant;
				document.getElementById("it-is-variant").value = data.itIsVariant;
				document.getElementById("slider-brightness").value = data.effectBri;
				document.getElementById("slider-speed").value = data.effectSpeed;
				document.getElementById("show-seconds").value = data.secondVariant;
				document.getElementById("show-minutes").value = data.minuteVariant;

				document.getElementById("owm-api-key").value = data.apiKey;
				document.getElementById("owm-city-id").value = data.cityid;

				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;
				document.getElementById("front-layout").value = data.UhrtypeDef;
				document.getElementById("buildtype").value = data.buildtype;
				document.getElementById("whitetype").value = data.wType;
				document.getElementById("colortype").value = data.colortype;

				document.getElementById("boot-show-led-blink").checked = data.bootLedBlink;
				document.getElementById("boot-show-led-sweep").checked = data.bootLedSweep;
				document.getElementById("boot-show-wifi").checked = data.bootShowWifi;
				document.getElementById("boot-show-ip").checked = data.bootShowIP;

				if (modeColorForm) {
					modeColorForm.style.gridTemplateColumns = data.hasSecondsFrame ? "1fr 1fr 1fr" : "1fr 1fr";
				}

				enableSpecific("specific-layout-0", !data.isRomanLanguage);
				enableSpecific("specific-layout-2", data.hasDreiviertel);
				enableSpecific("specific-layout-3", data.hasTwenty);
				enableSpecific("specific-layout-4", data.hasSecondsFrame);
				enableSpecific("specific-layout-5", data.hasWeatherLayout);
				if (data.UhrtypeDef === 10) {
					enableSpecific("specific-layout-6", true);
					enableSpecific("specific-layout-3", false);
					enableSpecific("specific-layout-1", false);
				}
				enableSpecific("specific-layout-7", data.hasSecondsFrame);
				enableSpecific("specific-colortype-4", data.colortype === 5);

				removeSpecificOption("show-minutes", "3", data.numOfRows !== 11);
				removeSpecificOption("show-minutes", "4", !data.hasMinuteInWords);
				removeSpecificOption("show-minutes", "1", data.UhrtypeDef === 13 || data.UhrtypeDef === 14);
				removeSpecificOption("show-minutes", "2", data.UhrtypeDef === 13 || data.UhrtypeDef === 14);

				autoBrightEnabled = data.autoBrightEnabled;
				const autoBrightSelect = document.getElementById("auto-bright-enabled");
				if (autoBrightEnabled === 1) {
					if (autoBrightSelect) autoBrightSelect.value = 1;
					autoBrightUpdater();
				} else {
					if (autoBrightSelect) autoBrightSelect.value = 0;
					autoBrightStop();
				}
				enableSpecific("specific-layout-brightness-man", autoBrightEnabled !== 1);
				enableSpecific("specific-layout-brightness-auto", autoBrightEnabled === 1);
				document.getElementById("auto-bright-min").value = autoBrightMin;
				document.getElementById("auto-bright-max").value = autoBrightMax;
				document.getElementById("auto-bright-peak").value = autoBrightPeak;
				break;
			}
			case "set": {
				hsb[0] = [data.hsb00, data.hsb01, data.hsb02];
				hsb[1] = [data.hsb10, data.hsb11, data.hsb12];
				effectBri = data.effectBri;
				effectSpeed = data.effectSpeed;
				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;

				if (modeColorForm) {
					modeColorForm.style.gridTemplateColumns = data.hasSecondsFrame ? "1fr 1fr 1fr" : "1fr 1fr";
				}

				command = data.prog === 0 ? CMD.MODE_WORD_CLOCK : data.prog;
				const inputID = MODE_TO_INPUT_ID.get(data.prog);
				debugMessage(`Mode is ${data.prog} (${inputID})`);

				const inputEl = document.getElementById(inputID);
				if (inputEl) inputEl.checked = true;

				setSliders();
				setColors();
				enableSpecific("specific-colortype-4", data.colortype === 5);
				enableSpecific("specific-layout-7", data.hasSecondsFrame);
				break;
			}
			case "wlan":
				document.getElementById("wlanlist").innerHTML = data.list;
				break;

			case "transition":
				transitionType = data.transitionType;
				transitionDuration = data.transitionDuration;
				transitionSpeed = data.transitionSpeed;
				transitionColorize = data.transitionColorize;
				transitionDemo = data.transitionDemo;
				setElementsForFunctionsMenu();
				break;

			case "autoBright":
				document.getElementById("auto-bright-enabled").value = data.autoBrightEnabled;
				document.getElementById("auto-bright-sensor").value = data.autoBrightSensor;
				document.getElementById("auto-bright-gain").value = data.autoBrightGain;
				if (data.autoBrightMin && data.autoBrightMax && data.autoBrightPeak) {
					autoBrightMin = data.autoBrightMin;
					autoBrightMax = data.autoBrightMax;
					autoBrightPeak = data.autoBrightPeak;
					document.getElementById("auto-bright-min").value = autoBrightMin;
					document.getElementById("auto-bright-max").value = autoBrightMax;
					document.getElementById("auto-bright-peak").value = autoBrightPeak;
				}

				if (data.autoBrightEnabled === 1) {
					autoBrightUpdater();
				} else {
					autoBrightStop();
				}
				break;
		}
	};
	websocket.onerror = function(event) {
		debugMessage("An error occurred while connecting to the websocket.", event);
	};
}

function changeColor(color) {
	hsb[colorPosition][0] = color.hue;
	hsb[colorPosition][1] = color.saturation;
	if (color.value !== 100) {
		hsb[colorPosition][2] = color.value;
	}

	setColors();
	sendColorData(command, nstr(1));
}

function createColorPicker() {
	colorPicker = new iro.ColorPicker("#color-picker", {
		wheelLightness: false,
		borderWidth: 3,
		borderColor: "#777",
		layout: [{
			component: iro.ui.Wheel
		}, {
			component: iro.ui.Slider,
			options: { sliderType: "value" }
		}, {
			component: iro.ui.Slider,
			options: { sliderType: "kelvin" }
		}]
	});
	colorPicker.on("input:change", changeColor);
}

function setColors() {
	let hsbFg = {
		h: hsb[colorPosition][0],
		s: hsb[colorPosition][1],
		v: hsb[colorPosition][2]
	};
	colorPicker.setColors([hsbFg]);
}

function setSliders() {
	const briSlider = document.getElementById("slider-brightness");
	if (briSlider) briSlider.value = effectBri;
	const speedSlider = document.getElementById("slider-speed");
	if (speedSlider) speedSlider.value = effectSpeed;

	const briValue = document.getElementById("slider-brightness-value");
	if (briValue) briValue.textContent = effectBri;
	const speedValue = document.getElementById("slider-speed-value");
	if (speedValue) speedValue.textContent = effectSpeed;
}

function setElementsForFunctionsMenu() {
	const modeWordclock = document.getElementById("mode-wordclock");
	const isWordclockChecked = modeWordclock && modeWordclock.checked;

	const transitionBox = document.getElementById("transition-box");
	if (transitionBox) {
		transitionBox.style.display = isWordclockChecked ? "block" : "none";
	}

	const functionsBirthdays = document.getElementById("functions-birthdays");
	if (functionsBirthdays) {
		functionsBirthdays.style.display = (hasSpecialWordHappyBirthday === true && isWordclockChecked) ? "block" : "none";
	}

	const transitionTypes = document.getElementById("transition-types");
	if (transitionTypes) transitionTypes.value = transitionType;

	const transitionDurationEl = document.getElementById("transition-duration");
	if (transitionDurationEl) transitionDurationEl.value = transitionDuration;

	const transitionSpeedValue = document.getElementById("transition-speed-value");
	if (transitionSpeedValue) transitionSpeedValue.textContent = transitionSpeed;

	const transitionSpeedEl = document.getElementById("transition-speed");
	if (transitionSpeedEl) transitionSpeedEl.value = transitionSpeed;

	const transitionColorizeEl = document.getElementById("transition-colorize");
	if (transitionColorizeEl) transitionColorizeEl.value = transitionColorize;

	const transitionDemoEl = document.getElementById("transition-demo");
	if (transitionDemoEl) transitionDemoEl.checked = transitionDemo;
}

function updateManualTimeInput() {
	const currentDate = new Date();
	const hours = currentDate.getHours().toString().padStart(2, "0");
	const minutes = currentDate.getMinutes().toString().padStart(2, "0");
	const timeEl = document.getElementById("time");
	if (timeEl) timeEl.value = `${hours}:${minutes}`;
}

function autoBrightUpdater() {
	if (autoBrightInterval !== null || (autoBrightEnabled !== 1 && autoBrightEnabled !== "1")) {
		return;
	}
	autoBrightInterval = setInterval(function() {
		const autoBrightEl = document.getElementById("auto-bright-enabled");
		if (autoBrightEl && autoBrightEl.value === "1") {
			sendCmd(CMD.REQ_AUTO_BRIGHT, 1);
		}
	}, 1000);
	debugMessage(`Start timer autoBrightInterval with ID ${autoBrightInterval}`);
}

function autoBrightStop() {
	if (autoBrightInterval === null) {
		return;
	}
	debugMessage(`Stop timer autoBrightInterval with ID ${autoBrightInterval}`);
	clearInterval(autoBrightInterval);
	autoBrightInterval = null;
}

function nstr5(number) {
	return Math.round(number).toString().padStart(5, "0");
}

function nstr(number) {
	return Math.round(number).toString().padStart(3, "0");
}

function getPaddedString(string, maxStringLength) {
	return string.padEnd(maxStringLength, " ");
}

function sendCmd(command, addData = "") {
	let data = nstr(command) + addData;
	debugMessage(`Send data: '${data}'`);
	if (websocket && websocket.readyState === WebSocket.OPEN) {
		websocket.send(data);
	}
}

function sendBrightnessData() {
	let payload = "";
	[6, 8, 12, 16, 18, 20, 22, 24].forEach(h => {
		let val = document.getElementById(`brightness-${h}`).value;
		window[`h${h}`] = val;
		payload += nstr(val);
	});
	sendCmd(CMD.SET_BRIGHTNESS, payload + nstr(effectBri));
	debugMessage(`Brightness${debugMessageReconfigured}`);
}

function sendColorData(command, addData = "") {
	sendCmd(command, nstr(colorPosition) +
	nstr(hsb[colorPosition][0]) +
	nstr(hsb[colorPosition][1]) +
	nstr(hsb[colorPosition][2]) +
	nstr(effectBri) +
	nstr(effectSpeed));
}

// Replaced $.ready with DOMContentLoaded
document.addEventListener("DOMContentLoaded", function() {

	initConfigValues();
	createColorPicker();
	setSliders();
	setElementsForFunctionsMenu();
	initWebsocket();
	setColors();

	document.querySelectorAll("input[name='colorwheel']").forEach(input => {
		input.addEventListener("change", function(event) {
			let id = event.target.id;
			if (id === "colorwheel-frame") {
				colorPosition = 2;
			} else if (id === "colorwheel-background") {
				colorPosition = 1;
			} else {
				colorPosition = 0;
			}
			console.log(`colorPosition: ${colorPosition}`);
			setColors();
		});
	});

	document.querySelectorAll(".status-button").forEach(btn => {
		btn.addEventListener("click", function(event) {
			event.preventDefault();
			let value = this.value;
			const statusEl = document.getElementById("status");
			if (statusEl) statusEl.textContent = "Verbinden ...";

			if (value === "1") {
				value = 0;
				document.querySelectorAll(".status-button").forEach(btn => { btn.value = value; });
				if (websocket) websocket.close();
			} else {
				value = 1;
				document.querySelectorAll(".status-button").forEach(btn => { btn.value = value; });
				initWebsocket();
			}
		});
	});

	const clockBtn = document.getElementById("_clock");
	if (clockBtn) {
		clockBtn.addEventListener("click", function() {
			let date = new Date();
			let timeZoneOffset = date.getTimezoneOffset();
			timeZoneOffset = timeZoneOffset / 60 * -1;
			let time = date.getTime() / 1000;

			sendCmd(CMD.SET_TIME, getPaddedString(nstr(timeZoneOffset) + time, 21));
			debugMessage("Clock data: ");
		});
	}

	/**
	 * A menu item has been clicked.
	 */
	document.querySelectorAll("a[data-navigation]").forEach(navLink => {
		navLink.addEventListener("click", function(event) {
			event.preventDefault();
			let navigation = this.dataset.navigation;

			// Remove classes and attributes
			document.querySelectorAll(".pure-menu-link").forEach(elem => {
				elem.classList.remove("pure-menu-selected");
				elem.removeAttribute("aria-current");
			});

			// Add classes and Attributes
			this.classList.add("pure-menu-selected");
			this.setAttribute("aria-current", "page");

			if (navigation === "functions") {
				sendCmd(CMD.REQ_BIRTHDAYS);
				setElementsForFunctionsMenu();
			}
			if (navigation === "smart-home") {
				sendCmd(CMD.REQ_MQTT_VALUES);
			}
			if (navigation === "frontoptions") {
				sendCmd(CMD.REQ_CONFIG_VALUES);
			}
			if (navigation === "settings" || navigation === "frontoptions") {
				sendCmd(CMD.REQ_CONFIG_VALUES);
				sendCmd(CMD.REQ_AUTO_BRIGHT);
				updateManualTimeInput();
				autoBrightUpdater();
			} else {
				autoBrightStop();
			}

			// show/hide sections
			document.querySelectorAll(".section").forEach(sec => { sec.style.display = "none"; });
			const targetSection = document.querySelector(`.section-${navigation}`);
			if (targetSection) targetSection.style.display = "block";
		});
	});

	/**
	 * The clock mode has been changed.
	 */
	document.querySelectorAll("input[name='mode']").forEach(input => {
		input.addEventListener("change", function(event) {
			let id = event.target.id;

			const MODE_MAP = {
				"mode-wordclock": { cmd: CMD.MODE_WORD_CLOCK, bri: false, speed: false, txt: false, symbol: false },
				"mode-seconds": { cmd: CMD.MODE_SECONDS, bri: false, speed: false, txt: false, symbol: false },
				"mode-scrollingtext": { cmd: CMD.MODE_SCROLLINGTEXT, bri: false, speed: true, txt: true, symbol: false },
				"mode-rainbow": { cmd: CMD.MODE_RAINBOWCYCLE, bri: true, speed: true, txt: false, symbol: false },
				"mode-change": { cmd: CMD.MODE_RAINBOW, bri: true, speed: true, txt: false, symbol: false },
				"mode-color": { cmd: CMD.MODE_COLOR, bri: false, speed: false, txt: false, symbol: false },
				"mode-digital-clock": { cmd: CMD.MODE_DIGITAL_CLOCK, bri: false, speed: false, txt: false, symbol: false },
				"mode-symbol": { cmd: CMD.MODE_SYMBOL, bri: false, speed: true, txt: false, symbol: true }
			};

			let selected = MODE_MAP[id];
			if (selected) {
				command = selected.cmd;
				setElementsForFunctionsMenu();

				document.querySelectorAll(".brightness").forEach(el => { el.style.display = selected.bri ? "block" : "none"; });
				document.querySelectorAll(".speed").forEach(el => { el.style.display = selected.speed ? "block" : "none"; });
				document.querySelectorAll(".functions-settings").forEach(el => { el.style.display = (selected.bri || selected.speed) ? "block" : "none"; });
				document.querySelectorAll(".text").forEach(el => { el.style.display = selected.txt ? "block" : "none"; });
				document.querySelectorAll(".symbol").forEach(el => { el.style.display = selected.symbol ? "block" : "none"; });

				sendColorData(command);
				setSliders();
				setColors();
			}
		});
	});

	let sliderTimeout = null;

	document.querySelectorAll("[id*='slider']").forEach(slider => {
		slider.addEventListener("input", function(event) {
			if (sliderTimeout) return;

			let id = event.target.id;
			if (id === "slider-brightness") {
				effectBri = event.target.value;
				sendBrightnessData();
			}
			if (id === "slider-speed") {
				effectSpeed = event.target.value;
				sendCmd(CMD.SPEED, nstr(effectSpeed));
			}
			setSliders();

			sliderTimeout = setTimeout(function() {
				sliderTimeout = null;
			}, 20);
		});
	});

	const transSpeedEl = document.getElementById("transition-speed");
	if (transSpeedEl) {
		transSpeedEl.addEventListener("input", function(event) {
			transitionSpeed = event.target.value;
			const tsv = document.getElementById("transition-speed-value");
			if (tsv) tsv.textContent = transitionSpeed;
		});
	}

	document.querySelectorAll("[id*='transition']").forEach(el => {
		el.addEventListener("change", function(event) {
			transitionType = document.getElementById("transition-types").value;
			transitionDuration = document.getElementById("transition-duration").value;
			transitionSpeed = document.getElementById("transition-speed").value;
			transitionColorize = document.getElementById("transition-colorize").value;
			transitionDemo = document.getElementById("transition-demo").checked;

			sendCmd(CMD.MODE_TRANSITION, nstr(transitionType) + nstr(transitionDuration) + nstr(transitionSpeed) + nstr(transitionColorize) + nstr(transitionDemo ? 1 : 0));
			debugMessage(`Transition${debugMessageReconfigured}`);
			setElementsForFunctionsMenu();
		});
	});

	const initValsBtn = document.getElementById("initial-values-button");
	if (initValsBtn) {
		initValsBtn.addEventListener("click", () => sendCmd(CMD.SET_INITIAL_VALUES));
	}

	const wifiBtn = document.getElementById("wifi-button");
	if (wifiBtn) {
		wifiBtn.addEventListener("click", function(event) {
			event.preventDefault();
			sendCmd(CMD.SET_WIFI_AND_RESTART);
			debugMessage(`WiFi${debugMessageReconfigured}`);
		});
	}

	document.querySelectorAll("[id*='auto-bright']").forEach(el => {
		el.addEventListener("change", function(event) {
			autoBrightEnabled = document.getElementById("auto-bright-enabled").value;
			autoBrightMin = document.getElementById("auto-bright-min").value;
			autoBrightMax = document.getElementById("auto-bright-max").value;

			if (parseInt(autoBrightMin) > parseInt(autoBrightMax)) {
				autoBrightMax = autoBrightMin;
				document.getElementById("auto-bright-max").value = autoBrightMax;
			}

			autoBrightPeak = document.getElementById("auto-bright-peak").value;
			sendCmd(CMD.SET_AUTO_BRIGHT, nstr(autoBrightEnabled) + nstr(autoBrightMin) + nstr(autoBrightMax) + nstr(autoBrightPeak));
			sendCmd(CMD.REQ_AUTO_BRIGHT);

			autoBrightDisplay = Number(document.getElementById("auto-bright-enabled").value);
			enableSpecific("specific-layout-brightness-man", !autoBrightDisplay);
			enableSpecific("specific-layout-brightness-auto", autoBrightDisplay);
		});
	});

	const wlanScanBtn = document.getElementById("_wlanscan");
	if (wlanScanBtn) {
		wlanScanBtn.addEventListener("click", function(event) {
			event.preventDefault();
			sendCmd(CMD.REQ_WIFI_LIST);
			document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";
		});
	}

	const timeServerBtn = document.getElementById("timeserver-button");
	if (timeServerBtn) {
		timeServerBtn.addEventListener("click", function(event) {
			event.preventDefault();
			sendCmd(CMD.SET_TIMESERVER, getPaddedString(document.getElementById("timeserver").value, DATA_TIMESERVER_TEXT_LENGTH));
			debugMessage(`Timeserver${debugMessageReconfigured}`);
		});
	}

	const scrollTextBtn = document.getElementById("scrollingtext-button");
	if (scrollTextBtn) {
		scrollTextBtn.addEventListener("click", function() {
			sendCmd(CMD.SET_SCROLLINGTEXT, getPaddedString(document.getElementById("scrollingtext").value, DATA_SCROLLINGTEXT_LENGTH));
			debugMessage(`ScrollingText${debugMessageReconfigured}`);
		});
	}

	document.querySelectorAll("[id*='brightness']").forEach(el => {
		el.addEventListener("change", () => sendBrightnessData(CMD.SET_BRIGHTNESS));
	});

	const weatherBtn = document.getElementById("weather-button");
	if (weatherBtn) {
		weatherBtn.addEventListener("click", function() {
			sendCmd(CMD.SET_WEATHER_DATA, document.getElementById("owm-city-id").value + " " + document.getElementById("owm-api-key").value);
			debugMessage(`OpenWeatherMap Login${debugMessageReconfigured}`);
		});
	}

	const showMinsBtn = document.getElementById("show-minutes");
	if (showMinsBtn) {
		showMinsBtn.addEventListener("change", function() {
			sendCmd(CMD.SET_MINUTE, nstr(this.value));
			debugMessage(`MinuteVariant${debugMessageReconfigured}`);
		});
	}

	const showSecsBtn = document.getElementById("show-seconds");
	if (showSecsBtn) {
		showSecsBtn.addEventListener("change", function() {
			sendCmd(CMD.SET_SETTING_SECOND, nstr(this.value));
			debugMessage(`SecondVariant${debugMessageReconfigured}`);
		});
	}

	const frontLayoutBtn = document.getElementById("front-layout");
	if (frontLayoutBtn) {
		frontLayoutBtn.addEventListener("change", function() {
			sendCmd(CMD.SET_UHRTYPE, nstr(this.value));
			sendCmd(CMD.REQ_CONFIG_VALUES);
			debugMessage(`FrontLayout${debugMessageReconfigured}`);
		});
	}

	const colorTypeBtn = document.getElementById("colortype-button");
	if (colorTypeBtn) {
		colorTypeBtn.addEventListener("click", function() {
			sendCmd(CMD.SET_COLORTYPE, nstr(document.getElementById("colortype").value));
			debugMessage(`Colortype${debugMessageReconfigured}`);
		});
	}

	document.querySelectorAll('input[name="symbol-choice"]').forEach(el => {
		el.addEventListener("change", function(event) {
			let selectedSymbol = event.target.value;
			sendCmd(CMD.SET_SYMBOL, nstr(selectedSymbol));
		});
	});

	const hostNameBtn = document.getElementById("hostname-button");
	if (hostNameBtn) {
		hostNameBtn.addEventListener("click", function() {
			sendCmd(CMD.SET_HOSTNAME, getPaddedString(document.getElementById("hostname").value, DATA_HOST_TEXT_LENGTH));
			debugMessage(`Hostname${debugMessageReconfigured}`);
		});
	}

	document.querySelectorAll("[id*='boot-show']").forEach(el => {
		el.addEventListener("change", function() {
			let blink = document.getElementById("boot-show-led-blink").checked ? 1 : 0;
			let sweep = document.getElementById("boot-show-led-sweep").checked ? 1 : 0;
			let wifi = document.getElementById("boot-show-wifi").checked ? 1 : 0;
			let ip = document.getElementById("boot-show-ip").checked ? 1 : 0;

			sendCmd(CMD.SET_BOOT, nstr(blink) + nstr(sweep) + nstr(wifi) + nstr(ip));
			debugMessage(`Bootoption${debugMessageReconfigured}`);
		});
	});

	const disableBtn = document.getElementById("disable-button");
	if (disableBtn) disableBtn.addEventListener("click", () => sendCmd(CMD.SET_WIFI_DISABLED));

	const resetBtn = document.getElementById("reset-button");
	if (resetBtn) resetBtn.addEventListener("click", () => sendCmd(CMD.RESET));

	document.querySelectorAll(".birthdays input").forEach(input => {
		input.addEventListener("input", function(event) {
			let val = event.target.value.replace(/\D/g, "");
			if (val.length > 2) {
				val = val.substring(0, 2) + "-" + val.substring(2, 4);
			}
			event.target.value = val;
		});
	});

	const bdaysStoreBtn = document.getElementById("birthdays-store-button");
	if (bdaysStoreBtn) {
		bdaysStoreBtn.addEventListener("click", function() {
			let payload = "";
			for (let i = 0; i < 5; i++) {
				payload += getPaddedString(document.getElementById(`birthdays-date${i}`).value, 5);
			}
			sendCmd(CMD.SET_BIRTHDAYS, payload);
		});
	}

	const uhrzeitBtn = document.getElementById("uhrzeit-button");
	if (uhrzeitBtn) {
		uhrzeitBtn.addEventListener("click", function() {
			const timeEl = document.getElementById("time");
			if (!timeEl.validity.valid) return;

			const timeString = timeEl.value;
			const [hours, minutes] = timeString.split(":");

			sendCmd(CMD.SET_TIME_MANUAL, nstr(hours) + nstr(minutes));
			debugMessage(`Time manually set to ${hours}:${minutes}`);
		});
	}

	const mqttBtn = document.getElementById("mqtt-button");
	if (mqttBtn) {
		mqttBtn.addEventListener("click", function() {
			const state = document.getElementById("mqtt-state").checked ? 1 : 0;
			const port = document.getElementById("mqtt-port").value;
			const server = document.getElementById("mqtt-server").value;
			const user = document.getElementById("mqtt-user").value;
			const pass = document.getElementById("mqtt-pass").value;
			const clientId = document.getElementById("mqtt-clientid").value;
			const topic = document.getElementById("mqtt-topic").value;

			const payload = nstr(state) +
							nstr5(port) +
							getPaddedString(server, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
							getPaddedString(user, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
							getPaddedString(pass, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
							getPaddedString(clientId, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
							getPaddedString(topic, DATA_MQTT_RESPONSE_TEXT_LENGTH);

			sendCmd(CMD.SET_MQTT, payload);
			debugMessage(`MQTT config${debugMessageReconfigured}`);
		});
	}

	const mqttDiscBtn = document.getElementById("mqtt-discovery-button");
	if (mqttDiscBtn) {
		mqttDiscBtn.addEventListener("click", function() {
			sendCmd(CMD.SET_MQTT_HA_DISCOVERY);
			debugMessage(`MQTT Discovery${debugMessageReconfigured}`);
		});
	}

	document.querySelectorAll("[id*='it-is-variant']").forEach(el => {
		el.addEventListener("change", function() {
			let payload = "";
			itIsVar = document.getElementById(`it-is-variant`).value;
			payload += nstr(itIsVar);
			sendCmd(CMD.SET_IT_IS_VARIANT, payload);
			debugMessage(`itIsVar${debugMessageReconfigured}`);
		});
	});

	document.querySelectorAll("[id*='langvar']").forEach(el => {
		el.addEventListener("change", function() {
			let payload = "";
			for (let i = 0; i < 5; i++) {
				langVar[i] = document.getElementById(`langvar-${i}`).value;
				payload += nstr(langVar[i]);
			}
			sendCmd(CMD.SET_LANGUAGE_VARIANT, payload);
			debugMessage(`langVar${debugMessageReconfigured}`);
		});
	});

	document.querySelectorAll("[id*='layvar']").forEach(el => {
		el.addEventListener("change", function() {
			let payload = "";
			for (let i = 0; i < 6; i++) {
				layVar[i] = document.getElementById(`layvar-${i}`).checked ? 1 : 0;
				payload += nstr(layVar[i]);
			}
			sendCmd(CMD.SET_LAYOUT_VARIANT, payload);
			debugMessage(`layVar${debugMessageReconfigured}`);
		});
	});

	document.querySelectorAll("[id*='buildtype']").forEach(el => {
		el.addEventListener("change", function(event) {
			sendCmd(CMD.SET_BUILDTYPE, nstr(event.target.value));
			debugMessage(`buildtype${debugMessageReconfigured}`);
		});
	});

	document.querySelectorAll("[id*='whitetype']").forEach(el => {
		el.addEventListener("change", function(event) {
			sendCmd(CMD.SET_WHITETYPE, nstr(event.target.value));
			debugMessage(`whitetype${debugMessageReconfigured}`);
		});
	});
});
