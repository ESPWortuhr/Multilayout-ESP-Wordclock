"use strict";
const iro = window.iro; // require("@jaames/iro");
(function(window, document) {

	let layout = document.getElementsByClassName("layout")[0];
	let menu = document.getElementsByClassName("menu")[0];
	let menuLink = document.getElementsByClassName("hamburger")[0];

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

	menuLink.addEventListener("click", toggleMenuEventHandler);

}(this, this.document));

var MINI = require("minified");
// eslint-disable-next-line one-var
var _ = MINI._,
	$ = MINI.$,
	$$ = MINI.$$,
	EE = MINI.EE,
	HTML = MINI.HTML;

var debugMessageReconfigured = " was reconfigured.";
var websocket;
var ipEsp = "ws://192.168.4.1";
var debug = true;
var command = 1;
var hsb = [
	[0, 100, 50],
	[120, 100, 50],
	[240, 100, 50]
];
var colorPosition = 0;
var effectBri = 2;
var effectSpeed = 10;
var dialect = [0, 0, 0, 0, 0, 0];
var layVar = [0, 0, 0, 0, 0, 0];
var hasSpecialWordHappyBirthday = 0;
var autoBrightDisplay = 0;
var autoBrightEnabled = 0;
var autoBrightInterval = null;
var autoBrightMin = 10;
var autoBrightMax = 80;
var autoBrightPeak = 750;
var transitionType = 0;
var transitionDuration = 1;
var transitionSpeed = 30;
var transitionColorize = 1;
var transitionDemo = false;

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

// colors
var COLOR_FOREGROUND = 0;
var COLOR_BACKGROUND = 1;

// data that gets send back to the esp
var DATA_SCROLLINGTEXT_LENGTH = 30;
var DATA_TIMESERVER_TEXT_LENGTH = 30;
var DATA_MQTT_RESPONSE_TEXT_LENGTH = 30;
var DATA_HOST_TEXT_LENGTH = 30;

// color pickers
var colorPicker;

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
		ipEsp = "ws://" + location.host;
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
	dialect = [0, 0, 0, 0, 0, 0];
	layVar = [0, 0, 0, 0, 0, 0];
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

		$("#output").fill(debugMessage);
	}
}

/// only shows elements of class `cls` if `enbl` is true.
function enableSpecific(cls, enbl) {
	let items = document.getElementsByClassName(cls);
	for (var item of items) {
		item.style.display = enbl ? "block" : "none";
	}
}

function removeSpecificOption(cls, val, bool) {
	if (bool) {
		var selectobject = document.getElementById(cls);
		for (var i = 0; i < selectobject.length; i++) {
			if (selectobject.options[i].value === val) {
				selectobject.remove(i);
			}
		}
	}
}

// handle click events on the swatch

var swatchGrid = document.getElementById("swatch-grid");

swatchGrid.addEventListener("click", function(ext) {
	var clickTarget = ext.target;
	// read data-color attribute
	if (clickTarget.dataset.color) {
	// update the color picker
		colorPicker.color.set(clickTarget.dataset.color);
		changeColor(colorPicker.color);
	}
});

function initWebsocket() {
	websocket = new WebSocket(ipEsp);

	websocket.onopen = function(event) {

		$("#status").set("+online");
		$("#status").set("-offline");
		$("#status").set("@value", "Online");
		$(".status-button").fill("Disconnect");
		$(".status-button").set("@value", "1");
		$("#section-connection-lost").set({
			$display: "none"
		});

		debugMessage("The connection with the websocket has been established.", event);

		sendCmd(CMD.REQ_COLOR_VALUES);
		sendCmd(CMD.REQ_TRANSITION);
	};

	websocket.onclose = function(event) {

		$("#status").set("-online");
		$("#status").set("+offline");
		$("#status").set("@value", "Offline");
		$(".status-button").fill("Verbinden");
		$(".status-button").set("@value", "0");
		$("#section-connection-lost").set({
			$display: "block"
		});
		autoBrightStop();

		debugMessage("The connection with the websocket was closed (code " + event.code + ").", event);
	};

	websocket.onmessage = function(event) {

		const modeColorForm = document.getElementById("mode-color-form");

		var data = JSON.parse(event.data);

		debugMessage("WebSocket response arrived (command " + data.command + ").", data);

		switch (data.command) {
			case "mqtt":
				$("#mqtt-port").set("value", data.MQTT_Port);
				$("#mqtt-server").set({ "value": data.MQTT_Server, "@maxlength": DATA_MQTT_RESPONSE_TEXT_LENGTH });
				document.getElementById("mqtt-state").checked = data.MQTT_State;
				$("#mqtt-user").set({ "value": data.MQTT_User, "@maxlength": DATA_MQTT_RESPONSE_TEXT_LENGTH });
				$("#mqtt-pass").set({ "value": data.MQTT_Pass, "@maxlength": DATA_MQTT_RESPONSE_TEXT_LENGTH });
				$("#mqtt-clientid").set({ "value": data.MQTT_ClientId, "@maxlength": DATA_MQTT_RESPONSE_TEXT_LENGTH });
				$("#mqtt-topic").set({ "value": data.MQTT_Topic, "@maxlength": DATA_MQTT_RESPONSE_TEXT_LENGTH });
				break;

			case "birthdays":
				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;
				for (let i = 0; i < 5; i++) {
					$(`#birthdays-date${i}`).set("value", data[`birthdayDate${i}`]);
				}
				break;

			case "config":

				$("#ssid").set("value", data.ssid);
				$("#timeserver").set("value", data.timeserver);
				$("#hostname").set("value", data.hostname);
				$("#scrollingtext").set("value", data.scrollingText);

				[6, 8, 12, 16, 18, 20, 22, 24].forEach(h => {
					$(`#brightness-${h}`).set("value", data[`h${h}`]);
				});

				for (let i = 0; i < 6; i++) {
					$(`#dialect-${i}`).set("value", data[`langVar${i}`]);
					document.getElementById(`layvar-${i}`).checked = data[`layVar${i}`];
				}

				$("#slider-brightness").set("value", data.effectBri);
				$("#slider-speed").set("value", data.effectSpeed);
				$("#show-seconds").set("value", data.secondVariant);
				$("#show-minutes").set("value", data.minuteVariant);

				$("#owm-api-key").set("value", data.apiKey);
				$("#owm-city-id").set("value", data.cityid);

				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;
				$("#front-layout").set("value", data.UhrtypeDef);
				$("#buildtype").set("value", data.buildtype);
				$("#whitetype").set("value", data.wType);
				$("#colortype").set("value", data.colortype);

				document.getElementById("boot-show-led-blink").checked = data.bootLedBlink;
				document.getElementById("boot-show-led-sweep").checked = data.bootLedSweep;
				document.getElementById("boot-show-wifi").checked = data.bootShowWifi;
				document.getElementById("boot-show-ip").checked = data.bootShowIP;

				modeColorForm.style.gridTemplateColumns = data.hasSecondsFrame ? "1fr 1fr 1fr" : "1fr 1fr";

				enableSpecific("specific-layout-0", !data.isRomanLanguage);
				enableSpecific("specific-layout-2", data.hasDreiviertel);
				enableSpecific("specific-layout-3", data.hasTwenty);
				enableSpecific("specific-layout-4", data.hasSecondsFrame);
				enableSpecific("specific-layout-5", data.hasWeatherLayout);
				if (data.UhrtypeDef === 10) { // (En10x11 exclusive)
					enableSpecific("specific-layout-6", true); // Add A-Quarter to
					enableSpecific("specific-layout-3", false);	// Remove "Twenty past" selector for En10x11
					enableSpecific("specific-layout-1", false); // Remove "Quater Ten" selector for En10x11
				}
				enableSpecific("specific-layout-7", data.hasSecondsFrame);
				enableSpecific("specific-colortype-4", data.colortype === 5);

				removeSpecificOption("show-minutes", "3", data.numOfRows !== 11); // MinuteVariant "Corners" only for DE11x11 Variants
				removeSpecificOption("show-minutes", "4", !data.hasMinuteInWords);
				removeSpecificOption("show-minutes", "1", data.UhrtypeDef === 13); // Remove "LED4x" for Ger16x8
				removeSpecificOption("show-minutes", "2", data.UhrtypeDef === 13); // Remove "LED7x" for Ger16x8

				autoBrightEnabled = data.autoBrightEnabled;
				if (autoBrightEnabled === 9) {
					// hide auto/manual select box
					$("#auto-bright-enabled").fill(EE("option", { "@value": "-1" }, "not available"));
					$("#auto-bright-enabled").set("value", -1);
					$("#auto-bright-enabled").up(".pure-control-group").hide();
					autoBrightStop();
				} else if (autoBrightEnabled === 1) {
					$("#auto-bright-enabled").set("value", 1);
					autoBrightUpdater();
				} else {
					$("#auto-bright-enabled").set("value", 0);
					autoBrightStop();
				}
				enableSpecific("specific-layout-brightness-man", autoBrightEnabled !== 1);
				enableSpecific("specific-layout-brightness-auto", autoBrightEnabled === 1);
				break;

			case "set": {
				hsb[0] = [data.hsb00, data.hsb01, data.hsb02];
				hsb[1] = [data.hsb10, data.hsb11, data.hsb12];
				effectBri = data.effectBri;
				effectSpeed = data.effectSpeed;
				hasSpecialWordHappyBirthday = data.hasSpecialWordHappyBirthday;

				modeColorForm.style.gridTemplateColumns = data.hasSecondsFrame ? "1fr 1fr 1fr" : "1fr 1fr";

				command = data.prog === 0 ? CMD.MODE_WORD_CLOCK : data.prog;
				const inputID = MODE_TO_INPUT_ID.get(data.prog);
				debugMessage("Mode is " + data.prog + " (" + inputID + ")");

				document.getElementById(inputID).checked = true;
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
				$("#auto-bright-enabled").set("value", data.autoBrightEnabled);
				$("#auto-bright-sensor").set("value", data.autoBrightSensor);
				$("#auto-bright-gain").set("value", data.autoBrightGain);
				$("#auto-bright-min").set("value", data.autoBrightMin);
				$("#auto-bright-max").set("value", data.autoBrightMax);
				$("#auto-bright-peak").set("value", data.autoBrightPeak);
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

/**
 * initialize background checkbox and color picker
 */
function setColors() {
	var hsbFg = {
		h: hsb[colorPosition][0],
		s: hsb[colorPosition][1],
		v: hsb[colorPosition][2]
	};
	var colors = [hsbFg];
	colorPicker.setColors(colors);
}

/**
 * Sets all sliders (the values) and their corresponding labels to
 * the currently stored config values.
 *
 * This function also updated the color area with the current rgbw values.
 */
function setSliders() {
	// sliders
	$("#slider-brightness").set("value", effectBri);
	$("#slider-speed").set("value", effectSpeed);

	// labels
	$("#slider-brightness-value").fill(effectBri);
	$("#slider-speed-value").fill(effectSpeed);
}

function setElementsForFunctionsMenu() {
	if (document.getElementById("mode-wordclock").checked) {
		$("#transition-box").set({
			$display: "block"
		});
	} else {
		$("#transition-box").set({
			$display: "none"
		});
	}

	if (hasSpecialWordHappyBirthday === true && document.getElementById("mode-wordclock").checked) {
		$("#functions-birthdays").set({
			$display: "block"
		});
	} else {
		$("#functions-birthdays").set({
			$display: "none"
		});
	}

	$("#transition-types").set("value", transitionType);
	$("#transition-duration").set("value", transitionDuration);
	$("#transition-speed-value").fill(transitionSpeed);
	$("#transition-speed").set("value", transitionSpeed);
	$("#transition-colorize").set("value", transitionColorize);
	document.getElementById("transition-demo").checked = transitionDemo;
}

/**
 * Sets the value of the manual time input to the current browser time on the settings page.
 */
function updateManualTimeInput() {
	const currentDate = new Date();
	// Pad the values with leading zeroes, so at five past eight in the morning it is 08:05 not 8:5.
	const hours = currentDate.getHours().toString().padStart(2, "0");
	const minutes = currentDate.getMinutes().toString().padStart(2, "0");
	$("#time").set("value", `${hours}:${minutes}`);
}

function autoBrightUpdater() {
	// note: autoBrightEnabled is a string if the value originates from an input (onChange event)
	if (autoBrightInterval !== null || (autoBrightEnabled !== 1 && autoBrightEnabled !== "1")) {
		return;
	}
	autoBrightInterval = setInterval(function() {
		if ($("#auto-bright-enabled").get("value") === "1") {
			sendCmd(CMD.REQ_AUTO_BRIGHT, 1);
		}
	}, 1000); // 1000 milliseconds intervall
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
	var data = nstr(command) + addData;
	debugMessage("Send data: '" + data + "'");
	websocket.send(data);
}

function sendBrightnessData() {
	let payload = "";
	[6, 8, 12, 16, 18, 20, 22, 24].forEach(h => {
		let val = $(`#brightness-${h}`).get("value");
		window[`h${h}`] = val;
		payload += nstr(val);
	});
	sendCmd(CMD.SET_BRIGHTNESS, payload + nstr(effectBri));
	debugMessage("Brightness" + debugMessageReconfigured);
}

function sendColorData(command, addData = "") {
	sendCmd(command, nstr(colorPosition) +
	nstr(hsb[colorPosition][0]) +
	nstr(hsb[colorPosition][1]) +
	nstr(hsb[colorPosition][2]) +
	nstr(effectBri) +
	nstr(effectSpeed));
}

$.ready(function() {

	initConfigValues();
	createColorPicker();
	setSliders();
	setElementsForFunctionsMenu();
	initWebsocket();
	setColors();

	$("input[name='colorwheel']").on("change", function() {
		var id = $(this).get("id");
		if (id === "colorwheel-frame") {
			colorPosition = 2;
		} else if (id === "colorwheel-background") {
			colorPosition = 1;
		} else {
			colorPosition = 0;
		}
		console.log("colorPosition: " + colorPosition);
		setColors();
	});

	$(".status-button").on("click", function() {
		var value = $(this).get("value");
		$("#status").fill("Verbinden ...");
		if (value === "1") {
			value = 0;
			$(".status-button").set("value", value);
			websocket.close();
		} else {
			value = 1;
			$(".status-button").set("value", value);
			initWebsocket();
		}
		return false;
	});

	$("#_clock").on("click", function() {

		var date = new Date();
		var timeZoneOffset = date.getTimezoneOffset();
		timeZoneOffset = timeZoneOffset / 60 * -1;
		var time = date.getTime() / 1000;

		sendCmd(CMD.SET_TIME, getPaddedString(nstr(timeZoneOffset) + time, 21));
		debugMessage("Clock data: ");
	});

	/**
	 * A menu item has been clicked.
	 */
	$("a[data-navigation]").on("click", function() {
		var navigation = $(this)[0].dataset.navigation;

		// Remove classes and attributes
		$(".pure-menu-link").set("-pure-menu-selected");
		Array.from(document.getElementsByClassName("pure-menu-link")).forEach(elem => {
			elem.removeAttribute("aria-current");
		});

		// Add classes and Attributes
		$(this).set("+pure-menu-selected");
		document.getElementsByClassName("pure-menu-selected")[0].setAttribute("aria-current", "page");

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
		$(".section").set({
			$display: "none"
		});
		$(".section-" + navigation).set({
			$display: "block"
		});
	});

	/**
	 * The clock mode has been changed.
	 */
	$("input[name='mode']").on("change", function() {
		var id = $(this).get("id");

		const MODE_MAP = {
			"mode-wordclock": { cmd: CMD.MODE_WORD_CLOCK, bri: false, speed: false, txt: false },
			"mode-seconds":	{ cmd: CMD.MODE_SECONDS, bri: false, speed: false, txt: false },
			"mode-scrollingtext": { cmd: CMD.MODE_SCROLLINGTEXT, bri: false, speed: true, txt: true },
			"mode-rainbow":	{ cmd: CMD.MODE_RAINBOWCYCLE, bri: true, speed: true, txt: false },
			"mode-change": { cmd: CMD.MODE_RAINBOW, bri: true, speed: true, txt: false },
			"mode-color": { cmd: CMD.MODE_COLOR, bri: false, speed: false, txt: false },
			"mode-digital-clock": { cmd: CMD.MODE_DIGITAL_CLOCK, bri: false, speed: false, txt: false },
			"mode-symbol": { cmd: CMD.MODE_SYMBOL,	bri: false, speed: true, txt: false }
		};

		let selected = MODE_MAP[id];
		if (selected) {
			command = selected.cmd;
			setElementsForFunctionsMenu();

			$(".brightness").set({ $display: selected.bri ? "block" : "none" });
			$(".speed").set({ $display: selected.speed ? "block" : "none" });
			$(".functions-settings").set({ $display: (selected.bri || selected.speed) ? "block" : "none" });
			$(".text").set({ $display: selected.txt ? "block" : "none" });

			sendColorData(command);
			setSliders();
			setColors();
		}
	});

	let sliderTimeout = null;

	$("[id*='slider']").onChange(function(event) {
		if (sliderTimeout) return false;

		var id = $(this).get("id");
		if (id === "slider-brightness") {
			effectBri = $(this).get("value");
			sendBrightnessData();
		}
		if (id === "slider-speed") {
			effectSpeed = $(this).get("value");
			sendCmd(CMD.SPEED, nstr(effectSpeed));
		}
		setSliders();

		sliderTimeout = setTimeout(function() {
			sliderTimeout = null;
		}, 20);

		return false;
	});

	$("#transition-speed").onChange(function(event) {
		transitionSpeed = $("#transition-speed").get("value");
		$("#transition-speed-value").fill(transitionSpeed);
	});

	$("[id*='transition']").on("change", function(event) {
		transitionType = $("#transition-types").get("value");
		transitionDuration = $("#transition-duration").get("value");
		transitionSpeed = $("#transition-speed").get("value");
		transitionColorize = $("#transition-colorize").get("value");
		transitionDemo = document.getElementById("transition-demo").checked;

		sendCmd(CMD.MODE_TRANSITION, nstr(transitionType) + nstr(transitionDuration) + nstr(transitionSpeed) + nstr(transitionColorize) + nstr(transitionDemo ? 1 : 0));
		debugMessage("Transition" + debugMessageReconfigured);
		setElementsForFunctionsMenu();
		return false;
	});
	$("#initial-values-button").on("click", function() {
		sendCmd(CMD.SET_INITIAL_VALUES);
	});
	$("#wifi-button").on("click", function() {
		sendCmd(CMD.SET_WIFI_AND_RESTART);
		debugMessage("WiFi" + debugMessageReconfigured);
		return false;
	});
	$("[id*='auto-bright']").on("change", function() {
		autoBrightEnabled = $("#auto-bright-enabled").get("value");
		autoBrightMin = $("#auto-bright-min").get("value");
		autoBrightMax = $("#auto-bright-max").get("value");
		if (autoBrightMin > autoBrightMax) {
			autoBrightMax = autoBrightMin;
			$("#auto-bright-max").set("value", autoBrightMax);
		}
		autoBrightPeak = $("#auto-bright-peak").get("value");
		sendCmd(CMD.SET_AUTO_BRIGHT, nstr(autoBrightEnabled) + nstr(autoBrightMin) + nstr(autoBrightMax) + nstr(autoBrightPeak));
		sendCmd(CMD.REQ_AUTO_BRIGHT);	// read back values

		autoBrightDisplay = Number($("#auto-bright-enabled").get("value"));
		enableSpecific("specific-layout-brightness-man", !autoBrightDisplay);
		enableSpecific("specific-layout-brightness-auto", autoBrightDisplay);
		return false;
	});
	$("#_wlanscan").on("click", function() {
		sendCmd(CMD.REQ_WIFI_LIST);
		document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";
		return false;
	});
	$("#timeserver-button").on("click", function() {
		sendCmd(CMD.SET_TIMESERVER, getPaddedString($("#timeserver").get("value"), DATA_TIMESERVER_TEXT_LENGTH));
		debugMessage("Timeserver" + debugMessageReconfigured);
		return false;
	});
	$("#scrollingtext-button").on("click", function() {
		sendCmd(CMD.SET_SCROLLINGTEXT, getPaddedString($("#scrollingtext").get("value"), DATA_SCROLLINGTEXT_LENGTH));
		debugMessage("ScrollingText" + debugMessageReconfigured);
	});
	$("[id*='brightness']").on("change", function() {
		sendBrightnessData(CMD.SET_BRIGHTNESS);
	});
	$("#weather-button").on("click", function() {
		sendCmd(CMD.SET_WEATHER_DATA, $("#owm-city-id").get("value") + " " + $("#owm-api-key").get("value"));
		debugMessage("OpenWeatherMap Login" + debugMessageReconfigured);
	});
	$("#show-minutes").on("change", function() {
		sendCmd(CMD.SET_MINUTE, nstr($("#show-minutes").get("value")));
		debugMessage("MinuteVariant" + debugMessageReconfigured);
	});
	$("#show-seconds").on("change", function() {
		sendCmd(CMD.SET_SETTING_SECOND, nstr($("#show-seconds").get("value")));
		debugMessage("SecondVariant" + debugMessageReconfigured);
	});
	$("#front-layout").on("change", function() {
		sendCmd(CMD.SET_UHRTYPE, nstr($("#front-layout").get("value")));
		sendCmd(CMD.REQ_CONFIG_VALUES);
		debugMessage("FrontLayout" + debugMessageReconfigured);
	});
	$("#colortype-button").on("click", function() {
		sendCmd(CMD.SET_COLORTYPE, nstr($("#colortype").get("value")));
		debugMessage("Colortype" + debugMessageReconfigured);
	});
	$("#hostname-button").on("click", function() {
		sendCmd(CMD.SET_HOSTNAME, getPaddedString($("#hostname").get("value"), DATA_HOST_TEXT_LENGTH));
		debugMessage("Hostname" + debugMessageReconfigured);
	});
	$("[id*='boot-show']").on("change", function() {
		let blink = $("#boot-show-led-blink").get("checked") | 0;
		let sweep = $("#boot-show-led-sweep").get("checked") | 0;
		let wifi = $("#boot-show-wifi").get("checked") | 0;
		let ip = $("#boot-show-ip").get("checked") | 0;

		sendCmd(CMD.SET_BOOT, nstr(blink) + nstr(sweep) + nstr(wifi) + nstr(ip));
		debugMessage("Bootoption" + debugMessageReconfigured);
	});
	$("#disable-button").on("click", function() {
		sendCmd(CMD.SET_WIFI_DISABLED);
	});
	$("#reset-button").on("click", function() {
		sendCmd(CMD.RESET);
	});
	$(".birthdays input").on("input", function() {
		let val = $(this).get("value").replace(/\D/g, "");
		if (val.length > 2) {
			val = val.substring(0, 2) + "-" + val.substring(2, 4);
		}
		$(this).set("value", val);
	});
	$("#birthdays-store-button").on("click", function() {
		let payload = "";
		for (let i = 0; i < 5; i++) {
			payload += getPaddedString($(`#birthdays-date${i}`).get("value"), 5);
		}
		sendCmd(CMD.SET_BIRTHDAYS, payload);
	});
	$("#uhrzeit-button").on("click", function() {

		if (!$("#time")[0].validity.valid) {
			return;
		}
		const timeString = $("#time").get("value");
		const [hours, minutes] = timeString.split(":");

		sendCmd(CMD.SET_TIME_MANUAL, nstr(hours) + nstr(minutes));
		debugMessage(`Time manually set to ${hours}:${minutes}`);
	});
	$("#mqtt-button").on("click", function() {
		const state = $("#mqtt-state").get("checked") | 0;
		const port = $("#mqtt-port").get("value");
		const server = $("#mqtt-server").get("value");
		const user = $("#mqtt-user").get("value");
		const pass = $("#mqtt-pass").get("value");
		const clientId = $("#mqtt-clientid").get("value");
		const topic = $("#mqtt-topic").get("value");

		const payload = nstr(state) +
						nstr5(port) +
						getPaddedString(server, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
						getPaddedString(user, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
						getPaddedString(pass, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
						getPaddedString(clientId, DATA_MQTT_RESPONSE_TEXT_LENGTH) +
						getPaddedString(topic, DATA_MQTT_RESPONSE_TEXT_LENGTH);

		sendCmd(CMD.SET_MQTT, payload);
		debugMessage("MQTT config" + debugMessageReconfigured);
	});
	$("#mqtt-discovery-button").on("click", function() {
		sendCmd(CMD.SET_MQTT_HA_DISCOVERY);
		debugMessage("MQTT Discovery" + debugMessageReconfigured);
	});
	$("[id*='dialect']").on("change", function() {
		let payload = "";
		for (let i = 0; i < 6; i++) {
			dialect[i] = $(`#dialect-${i}`).get("value");
			payload += nstr(dialect[i]);
		}
		sendCmd(CMD.SET_LANGUAGE_VARIANT, payload);
		debugMessage("langVar" + debugMessageReconfigured);
	});
	$("[id*='layvar']").on("change", function() {
		let payload = "";
		for (let i = 0; i < 6; i++) {
			layVar[i] = $(`#layvar-${i}`).get("checked") | 0;
			payload += nstr(layVar[i]);
		}
		sendCmd(CMD.SET_LAYOUT_VARIANT, payload);
		debugMessage("layVar" + debugMessageReconfigured);
	});
	$("[id*='buildtype']").on("change", function() {
		sendCmd(CMD.SET_BUILDTYPE, nstr($(this).get("value")));
		debugMessage("buildtype" + debugMessageReconfigured);
	});
	$("[id*='whitetype']").on("change", function() {
		sendCmd(CMD.SET_WHITETYPE, nstr($(this).get("value")));
		debugMessage("whitetype" + debugMessageReconfigured);
	});
});
