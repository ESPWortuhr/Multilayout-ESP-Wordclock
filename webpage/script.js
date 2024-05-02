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
	[120, 100, 50]
];
var effectBri = 2;
var effectSpeed = 10;
var sleep = 0;
var sleeptime = 1;
var color = 0;
var h6 = 100;
var h8 = 100;
var h12 = 100;
var h16 = 100;
var h18 = 100;
var h20 = 100;
var h22 = 100;
var h24 = 100;
var dialect = [0, 0, 0, 0, 0, 0];
var layVar = [0, 0, 0];
var showSeconds = 0;
var showMinutes = 0;
var UhrtypeDef = 0;
var buildtype = 0;
var wType = 0;
var colortype = 0;
var MQTTState = 0;
var MQTTPort = 0;
var MQTTServer = 0;
var MQTTUser = 0;
var MQTTPass = 0;
var MQTTClientId = 0;
var MQTTTopic = 0;
var bootLedBlink = 0;
var bootLedSweep = 0;
var bootShowWifi = 0;
var bootShowIP = 0;
var autoLdrInterval = null;
var autoLdrEnabled = 0;
var displayAutoLdr = 0;
var autoLdrBright = " ";
var autoLdrDark = " ";
var transitionType = 0;
var transitionDuration = 1;
var transitionSpeed = 30;
var transitionColorize = 1;
var transitionDemo = false;

// operation modes
var COMMAND_MODE_WORD_CLOCK = 1;
var COMMAND_MODE_SECONDS = 2;
var COMMAND_MODE_SCROLLINGTEXT = 3;
var COMMAND_MODE_RAINBOWCYCLE = 4;
var COMMAND_MODE_RAINBOW = 5;
var COMMAND_MODE_COLOR = 6;
var COMMAND_MODE_DIGITAL_CLOCK = 7;
var COMMAND_MODE_SYMBOL = 8;
var COMMAND_MODE_TRANSITION = 10;

/**
 * Maps the mode to the corresponding input id on the functions page.
 *
 * @type {Map<number, String>}
 */
const MODE_TO_INPUT_ID = new Map();
MODE_TO_INPUT_ID.set(0, "mode-wordclock"); // Map COMMAND_IDLE to mode wordclock
MODE_TO_INPUT_ID.set(COMMAND_MODE_WORD_CLOCK, "mode-wordclock");
MODE_TO_INPUT_ID.set(COMMAND_MODE_SECONDS, "mode-seconds");
MODE_TO_INPUT_ID.set(COMMAND_MODE_SCROLLINGTEXT, "mode-scrollingtext");
MODE_TO_INPUT_ID.set(COMMAND_MODE_RAINBOWCYCLE, "mode-rainbow");
MODE_TO_INPUT_ID.set(COMMAND_MODE_RAINBOW, "mode-change"); // Color change
MODE_TO_INPUT_ID.set(COMMAND_MODE_COLOR, "mode-color");
MODE_TO_INPUT_ID.set(COMMAND_MODE_DIGITAL_CLOCK, "mode-digital-clock");
MODE_TO_INPUT_ID.set(COMMAND_MODE_SYMBOL, "mode-symbol");
MODE_TO_INPUT_ID.set(COMMAND_MODE_TRANSITION, "mode-wordclock");

// other commands
var COMMAND_SET_INITIAL_VALUES = 20;
var COMMAND_SET_TIME = 30;
var COMMAND_SET_LANGUAGE_VARIANT = 84;
var COMMAND_SET_MQTT = 85;
var COMMAND_SET_TIME_MANUAL = 86;
var COMMAND_SET_BUILDTYPE = 87;
var COMMAND_SET_COLORTYPE = 88;
var COMMAND_SET_UHRTYPE = 89;
var COMMAND_SET_WEATHER_DATA = 90;
var COMMAND_SET_WHITETYPE = 91;
var COMMAND_SET_HOSTNAME = 92;
var COMMAND_SET_SETTING_SECOND = 93;
var COMMAND_SET_MINUTE = 94;
var COMMAND_SET_BRIGHTNESS = 95;
var COMMAND_SET_SCROLLINGTEXT = 96;
var COMMAND_SET_TIMESERVER = 97;
var COMMAND_SET_WIFI_DISABLED = 98;
var COMMAND_SET_WIFI_AND_RESTART = 99;
var COMMAND_RESET = 100;
var COMMAND_SET_BOOT = 101;
var COMMAND_SET_AUTO_LDR = 102;
var COMMAND_SET_LAYOUT_VARIANT = 103;
var COMMAND_SET_MQTT_HA_DISCOVERY = 104;

var COMMAND_SPEED = 152;

var COMMAND_REQUEST_CONFIG_VALUES = 200;
var COMMAND_REQUEST_COLOR_VALUES = 201;
var COMMAND_REQUEST_WIFI_LIST = 202;
var COMMAND_REQUEST_AUTO_LDR = 203;
var COMMAND_REQUEST_TRANSITION = 204;
var COMMAND_REQUEST_MQTT_VALUES = 205;

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
		[120, 100, 50]
	];
	effectBri = 2;
	effectSpeed = 10;
	sleep = 0;
	sleeptime = 1;
	color = 0;
	h6 = 100;
	h8 = 100;
	h12 = 100;
	h16 = 100;
	h18 = 100;
	h20 = 100;
	h22 = 100;
	h24 = 100;
	dialect = [0, 0, 0, 0, 0, 0];
	layVar = [0, 0, 0];
	showSeconds = 0;
	showMinutes = 0;
	buildtype = 0;
	wType = 0;
	UhrtypeDef = 0;
	colortype = 0;
	MQTTState = 0;
	MQTTPort = 0;
	MQTTServer = 0;
	MQTTUser = 0;
	MQTTPass = 0;
	MQTTClientId = 0;
	MQTTTopic = 0;
	bootLedBlink = 0;
	bootLedSweep = 0;
	bootShowWifi = 0;
	bootShowIP = 0;
	displayAutoLdr = 0;
	autoLdrEnabled = 0;
	autoLdrBright = " ";
	autoLdrDark = " ";
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

		sendCmd(COMMAND_REQUEST_COLOR_VALUES);
		sendCmd(COMMAND_REQUEST_TRANSITION);
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
		autoLdrStop();

		debugMessage("The connection with the websocket was closed (code " + event.code + ").", event);
	};

	websocket.onmessage = function(event) {

		var data = JSON.parse(event.data);

		debugMessage("WebSocket response arrived (command " + data.command + ").", data);

		if (data.command === "mqtt") {
			$("#mqtt-port").set("value", data.MQTT_Port);
			$("#mqtt-server").set("value", data.MQTT_Server);
			document.getElementById("mqtt-state").checked = data.MQTT_State;
			$("#mqtt-user").set("value", data.MQTT_User);
			$("#mqtt-pass").set("value", data.MQTT_Pass);
			$("#mqtt-clientid").set("value", data.MQTT_ClientId);
			$("#mqtt-topic").set("value", data.MQTT_Topic);
		}

		if (data.command === "config") {

			$("#ssid").set("value", data.ssid);

			$("#timeserver").set("value", data.timeserver);
			$("#hostname").set("value", data.hostname);
			$("#scrollingtext").set("value", data.scrollingText);

			$("#brightness-6").set("value", data.h6);
			$("#brightness-8").set("value", data.h8);
			$("#brightness-12").set("value", data.h12);
			$("#brightness-16").set("value", data.h16);
			$("#brightness-18").set("value", data.h18);
			$("#brightness-20").set("value", data.h20);
			$("#brightness-22").set("value", data.h22);
			$("#brightness-24").set("value", data.h24);

			$("#dialect-0").set("value", data.langVar0);
			$("#dialect-1").set("value", data.langVar1);
			$("#dialect-2").set("value", data.langVar2);
			$("#dialect-3").set("value", data.langVar3);
			document.getElementById("dialect-4").checked = data.langVar4;
			$("#dialect-5").set("value", data.langVar5);

			document.getElementById("layvar-0").checked = data.layVar0;
			document.getElementById("layvar-1").checked = data.layVar1;
			document.getElementById("layvar-2").checked = data.layVar2;

			$("#slider-brightness").set("value", data.effectBri);
			$("#slider-speed").set("value", data.effectSpeed); // TODO: there is no property effectSpeed!
			$("#show-seconds").set("value", data.secondVariant);
			$("#show-minutes").set("value", data.minuteVariant);

			$("#owm-api-key").set("value", data.apiKey);
			$("#owm-city-id").set("value", data.cityid);

			$("#front-layout").set("value", data.UhrtypeDef);
			$("#buildtype").set("value", data.buildtype);
			$("#whitetype").set("value", data.wType);
			$("#colortype").set("value", data.colortype);

			document.getElementById("boot-show-led-blink").checked = data.bootLedBlink;
			document.getElementById("boot-show-led-sweep").checked = data.bootLedSweep;
			document.getElementById("boot-show-wifi").checked = data.bootShowWifi;
			document.getElementById("boot-show-ip").checked = data.bootShowIP;

			enableSpecific("specific-layout-1", !data.isRomanLanguage);
			enableSpecific("specific-layout-2", data.hasDreiviertel);
			enableSpecific("specific-layout-3", data.hasZwanzig);
			enableSpecific("specific-layout-4", data.hasSecondsFrame);
			enableSpecific("specific-layout-5", data.hasWeatherLayout);
			enableSpecific("specific-layout-6", data.UhrtypeDef === 10); // Add A-Quarter to (En10x11 exclusive)

			enableSpecific("specific-colortype-4", data.colortype === 4);

			removeSpecificOption("show-minutes", "3", data.numOfRows !== 11); // MinuteVariant "Corners" only for DE11x11 Variants
			removeSpecificOption("show-minutes", "4", !data.hasMinuteInWords);
			removeSpecificOption("show-minutes", "1", data.UhrtypeDef === 13); // Remove "LED4x" for Ger16x8
			removeSpecificOption("show-minutes", "2", data.UhrtypeDef === 13); // Remove "LED7x" for Ger16x8

			autoLdrEnabled = data.autoLdrEnabled;
			$("#auto-ldr-enabled").set("value", autoLdrEnabled);
			enableSpecific("specific-layout-brightness-man", autoLdrEnabled === 0);
			enableSpecific("specific-layout-brightness-auto", autoLdrEnabled === 1);
		}
		if (data.command === "set") {
			hsb[0][0] = data.hsb00;
			hsb[0][1] = data.hsb01;
			hsb[0][2] = data.hsb02;
			hsb[1][0] = data.hsb10;
			hsb[1][1] = data.hsb11;
			hsb[1][2] = data.hsb12;
			effectBri = data.effectBri;
			effectSpeed = data.effectSpeed;
			colortype = data.colortype;

			var prog = data.prog;
			command = prog === 0 ? COMMAND_MODE_WORD_CLOCK : prog;	// 0 == COMMAND_IDLE
			const inputID = MODE_TO_INPUT_ID.get(prog);
			debugMessage("Mode is " + prog + " (" + inputID + ")");

			document.getElementById(inputID).checked = true;
			setSliders();
			setColors();
			enableSpecific("specific-colortype-4", data.colortype === 4);
		}
		if (data.command === "wlan") {
			document.getElementById("wlanlist").innerHTML = data.list;
		}
		if (data.command === "transition") {
			transitionType = data.transitionType;
			transitionDuration = data.transitionDuration;
			transitionSpeed = data.transitionSpeed;
			transitionColorize = data.transitionColorize;
			transitionDemo = data.transitionDemo;
			setTransition();
		}
		if (data.command === "autoLdr") {
			$("#auto-ldr-enabled").set("value", data.autoLdrEnabled);
			$("#auto-ldr-value").set("value", data.autoLdrValue);
			$("#auto-ldr-bright").set("value", data.autoLdrBright);
			$("#auto-ldr-dark").set("value", data.autoLdrDark);
		}
	};
	websocket.onerror = function(event) {
		debugMessage("An error occurred while connecting to the websocket.", event);
	};
}

function changeColor(color) {
	hsb[color.index][0] = color.hue;
	hsb[color.index][1] = color.saturation;
	if (color.value !== 100) {
		hsb[color.index][2] = color.value;
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
 * show the color configuration in the color picker
 */
function setColorPicker(withBackground) {
	var hsbFg = {
		h: hsb[COLOR_FOREGROUND][0],
		s: hsb[COLOR_FOREGROUND][1],
		v: hsb[COLOR_FOREGROUND][2]
	};
	var hsbBg = {
		h: hsb[COLOR_BACKGROUND][0],
		s: hsb[COLOR_BACKGROUND][1],
		v: hsb[COLOR_BACKGROUND][2]
	};
	var colors = [hsbFg];
	if (withBackground) {
		colors.push(hsbBg);
	}
	colorPicker.setColors(colors);
}

/**
 * initialize background checkbox and color picker
 */
function setColors() {
	var withBackground =
		hsb[COLOR_BACKGROUND][0] ||
		hsb[COLOR_BACKGROUND][1] ||
		hsb[COLOR_BACKGROUND][2];
	setColorPicker(withBackground);
	$("#with-background").set("checked", withBackground);
}

/**
 * toggle between no background and a default dark gray background
 */
function toggleBackground() {
	var withBackground = $("#with-background").get("checked") | 0;
	if (withBackground) {
		// set to dark gray
		hsb[COLOR_BACKGROUND][0] = 0;
		hsb[COLOR_BACKGROUND][1] = 0;
		hsb[COLOR_BACKGROUND][2] = 10;
	} else {
		// set to black
		hsb[COLOR_BACKGROUND][0] = 0;
		hsb[COLOR_BACKGROUND][1] = 0;
		hsb[COLOR_BACKGROUND][2] = 0;
	}
	setColorPicker(withBackground);
	sendColorData(command, nstr(1));
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

function setTransition() {
	if (document.getElementById("mode-wordclock").checked) {
		$("#transition-box").set({
			$display: "block"
		});
	} else {
		$("#transition-box").set({
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

function autoLdrValueUpdater() {
	if (autoLdrInterval !== null || autoLdrEnabled !== 1) {
		return;
	}
	autoLdrInterval = setInterval(function() {
		if ($("#auto-ldr-enabled").get("value") === "1") {
			sendCmd(COMMAND_REQUEST_AUTO_LDR, 1);
		}
	}, 1000);
	debugMessage(`Start timer autoLdrInterval with ID ${autoLdrInterval}`);
}

function autoLdrStop() {
	if (autoLdrInterval === null) {
		return;
	}
	debugMessage(`Stop timer autoLdrInterval with ID ${autoLdrInterval}`);
	clearInterval(autoLdrInterval);
	autoLdrInterval = null;
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

function sendBrightnessData(command, addData = "") {
	h6 = $("#brightness-6").get("value");
	h8 = $("#brightness-8").get("value");
	h12 = $("#brightness-12").get("value");
	h16 = $("#brightness-16").get("value");
	h18 = $("#brightness-18").get("value");
	h20 = $("#brightness-20").get("value");
	h22 = $("#brightness-22").get("value");
	h24 = $("#brightness-24").get("value");

	sendCmd(COMMAND_SET_BRIGHTNESS, nstr(h6) + nstr(h8) + nstr(h12) + nstr(h16) + nstr(h18) + nstr(h20) + nstr(h22) + nstr(h24) + nstr(effectBri));
	debugMessage("Brightness" + debugMessageReconfigured);
}

function sendColorData(command, addData = "") {
	sendCmd(command, nstr(hsb[COLOR_FOREGROUND][0]) +
	nstr(hsb[COLOR_FOREGROUND][1]) +
	nstr(hsb[COLOR_FOREGROUND][2]) +
	nstr(hsb[COLOR_BACKGROUND][0]) +
	nstr(hsb[COLOR_BACKGROUND][1]) +
	nstr(hsb[COLOR_BACKGROUND][2]) +
	nstr(effectBri) +
	nstr(effectSpeed));
}

$.ready(function() {

	initConfigValues();
	createColorPicker();
	setSliders();
	setTransition();
	initWebsocket();
	setColors();

	$("#with-background").on("change", toggleBackground);
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

		sendCmd(COMMAND_SET_TIME, getPaddedString(nstr(timeZoneOffset) + time, 21));
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
			setTransition();
		}
		if (navigation === "smart-home") {
			sendCmd(COMMAND_REQUEST_MQTT_VALUES);
		}
		if (navigation === "frontoptions") {
			sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
		}
		if (navigation === "settings" || navigation === "frontoptions") {
			sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
			sendCmd(COMMAND_REQUEST_AUTO_LDR);
			updateManualTimeInput();
			autoLdrValueUpdater();
		} else {
			autoLdrStop();
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

		var hasBrightness = false;
		var hasSpeed = false;
		var hasText = false;

		if (id === "mode-wordclock") {
			command = COMMAND_MODE_WORD_CLOCK;
		}
		if (id === "mode-color") {
			command = COMMAND_MODE_COLOR;
		}
		if (id === "mode-seconds") {
			command = COMMAND_MODE_SECONDS;
		}
		if (id === "mode-digital-clock") {
			command = COMMAND_MODE_DIGITAL_CLOCK;
		}
		if (id === "mode-scrollingtext") {
			hasSpeed = true;
			hasText = true;
			command = COMMAND_MODE_SCROLLINGTEXT;
		}
		if (id === "mode-rainbow") {
			hasBrightness = true;
			hasSpeed = true;
			command = COMMAND_MODE_RAINBOWCYCLE;
		}
		if (id === "mode-change") {
			hasBrightness = true;
			hasSpeed = true;
			command = COMMAND_MODE_RAINBOW;
		}
		if (id === "mode-symbol") {
			hasSpeed = true;
			command = COMMAND_MODE_SYMBOL;
		}

		setTransition();

		if (hasBrightness === true) {
			$(".brightness").set({
				$display: "block"
			});
		} else {
			$(".brightness").set({
				$display: "none"
			});
		}

		if (hasSpeed === true) {
			$(".speed").set({
				$display: "block"
			});
		} else {
			$(".speed").set({
				$display: "none"
			});
		}

		if (hasBrightness || hasSpeed) {
			$(".functions-settings").set({
				$display: "block"
			});
		} else {
			$(".functions-settings").set({
				$display: "none"
			});
		}
		if (hasText === true) {
			$(".text").set({
				$display: "block"
			});
		} else {
			$(".text").set({
				$display: "none"
			});
		}

		sendColorData(command);
		setSliders();
		setColors();
	});

	$("[id*='slider']").onChange(function(event) {
		var id = $(this).get("id");

		if (sleep === 0) {
			if (id === "slider-brightness") {
				effectBri = $("#slider-brightness").get("value");
				sendBrightnessData(COMMAND_SET_BRIGHTNESS);
			}
			if (id === "slider-speed") {
				effectSpeed = $("#slider-speed").get("value");
				sendCmd(COMMAND_SPEED, nstr(effectSpeed));
			}
			setSliders();

			sleep = 1;
			sleeptime = 20;

			setTimeout(function() {
				sleep = 0;
			}, sleeptime);
		}
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

		sendCmd(COMMAND_MODE_TRANSITION, nstr(transitionType) + nstr(transitionDuration) + nstr(transitionSpeed) + nstr(transitionColorize) + nstr(transitionDemo ? 1 : 0));
		debugMessage("Transition" + debugMessageReconfigured);
		setTransition();
		return false;
	});
	$("#initial-values-button").on("click", function() {
		sendCmd(COMMAND_SET_INITIAL_VALUES);
	});
	$("#wifi-button").on("click", function() {
		sendCmd(COMMAND_SET_WIFI_AND_RESTART);
		debugMessage("WiFi" + debugMessageReconfigured);
		return false;
	});
	$("[id*='auto-ldr']").on("change", function() {
		autoLdrEnabled = $("#auto-ldr-enabled").get("value");
		autoLdrBright = $("#auto-ldr-bright").get("value");
		autoLdrDark = $("#auto-ldr-dark").get("value");
		sendCmd(COMMAND_SET_AUTO_LDR, nstr(autoLdrEnabled) + nstr(autoLdrBright) + nstr(autoLdrDark));
		sendCmd(COMMAND_REQUEST_AUTO_LDR);	// read back values

		displayAutoLdr = Number($("#auto-ldr-enabled").get("value"));
		enableSpecific("specific-layout-brightness-man", !displayAutoLdr);
		enableSpecific("specific-layout-brightness-auto", displayAutoLdr);
		return false;
	});
	$("#_wlanscan").on("click", function() {
		sendCmd(COMMAND_REQUEST_WIFI_LIST);
		document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";
		return false;
	});
	$("#timeserver-button").on("click", function() {
		sendCmd(COMMAND_SET_TIMESERVER, getPaddedString($("#timeserver").get("value"), DATA_TIMESERVER_TEXT_LENGTH));
		debugMessage("Timeserver" + debugMessageReconfigured);
		return false;
	});
	$("#scrollingtext-button").on("click", function() {
		sendCmd(COMMAND_SET_SCROLLINGTEXT, getPaddedString($("#scrollingtext").get("value"), DATA_SCROLLINGTEXT_LENGTH));
		debugMessage("ScrollingText" + debugMessageReconfigured);
	});
	$("[id*='brightness']").on("change", function() {
		sendBrightnessData(COMMAND_SET_BRIGHTNESS);
	});
	$("#weather-button").on("click", function() {
		sendCmd(COMMAND_SET_WEATHER_DATA, $("#owm-city-id").get("value") + " " + $("#owm-api-key").get("value"));
		debugMessage("OpenWeatherMap Login" + debugMessageReconfigured);
	});
	$("#show-minutes").on("change", function() {
		sendCmd(COMMAND_SET_MINUTE, nstr($("#show-minutes").get("value")));
		debugMessage("MinuteVariant" + debugMessageReconfigured);
	});
	$("#show-seconds").on("change", function() {
		sendCmd(COMMAND_SET_SETTING_SECOND, nstr($("#show-seconds").get("value")));
		debugMessage("SecondVariant" + debugMessageReconfigured);
	});
	$("#front-layout").on("change", function() {
		sendCmd(COMMAND_SET_UHRTYPE, nstr($("#front-layout").get("value")));
		sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
		debugMessage("FrontLayout" + debugMessageReconfigured);
	});
	$("#colortype-button").on("click", function() {
		colortype = $("#colortype").get("value");

		sendCmd(COMMAND_SET_COLORTYPE, nstr(colortype));
		debugMessage("Colortype" + debugMessageReconfigured);
	});
	$("#hostname-button").on("click", function() {
		sendCmd(COMMAND_SET_HOSTNAME, getPaddedString($("#hostname").get("value"), DATA_HOST_TEXT_LENGTH));
		debugMessage("Hostname" + debugMessageReconfigured);
	});
	$("[id*='boot-show']").on("change", function() {
		bootLedBlink = $("#boot-show-led-blink").get("checked") | 0;
		bootLedSweep = $("#boot-show-led-sweep").get("checked") | 0;
		bootShowWifi = $("#boot-show-wifi").get("checked") | 0;
		bootShowIP = $("#boot-show-ip").get("checked") | 0;

		sendCmd(COMMAND_SET_BOOT, nstr(bootLedBlink) + nstr(bootLedSweep) + nstr(bootShowWifi) + nstr(bootShowIP));
		debugMessage("Bootoption" + debugMessageReconfigured);
	});
	$("#disable-button").on("click", function() {
		sendCmd(COMMAND_SET_WIFI_DISABLED);
	});
	$("#reset-button").on("click", function() {
		sendCmd(COMMAND_RESET);
	});
	$("#uhrzeit-button").on("click", function() {

		if (!$("#time")[0].validity.valid) {
			return;
		}
		const timeString = $("#time").get("value");
		const [hours, minutes] = timeString.split(":");

		sendCmd(COMMAND_SET_TIME_MANUAL, nstr(hours) + nstr(minutes));
		debugMessage(`Time manually set to ${hours}:${minutes}`);
	});
	$("#mqtt-button").on("click", function() {
		MQTTState = $("#mqtt-state").get("checked") | 0;
		MQTTPort = $("#mqtt-port").get("value");
		MQTTServer = $("#mqtt-server").get("value");
		MQTTUser = $("#mqtt-user").get("value");
		MQTTPass = $("#mqtt-pass").get("value");
		MQTTClientId = $("#mqtt-clientid").get("value");
		MQTTTopic = $("#mqtt-topic").get("value");

		sendCmd(COMMAND_SET_MQTT, nstr(MQTTState) + nstr5(MQTTPort) + getPaddedString(MQTTServer, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTUser, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTPass, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTClientId, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTTopic, DATA_MQTT_RESPONSE_TEXT_LENGTH));
		debugMessage("MQTT config" + debugMessageReconfigured);
	});
	$("#mqtt-discovery-button").on("click", function() {
		sendCmd(COMMAND_SET_MQTT_HA_DISCOVERY);
		debugMessage("MQTT Discovery" + debugMessageReconfigured);
	});
	$("[id*='dialect']").on("change", function() {
		dialect[0] = $("#dialect-0").get("value");
		dialect[1] = $("#dialect-1").get("value");
		dialect[2] = $("#dialect-2").get("value");
		dialect[3] = $("#dialect-3").get("value");
		dialect[4] = $("#dialect-4").get("checked") | 0;
		dialect[5] = $("#dialect-5").get("value");

		sendCmd(COMMAND_SET_LANGUAGE_VARIANT, nstr(dialect[0]) + nstr(dialect[1]) + nstr(dialect[2]) + nstr(dialect[3]) + nstr(dialect[4]) + nstr(dialect[5]));
		debugMessage("langVar" + debugMessageReconfigured);
	});
	$("[id*='layvar']").on("change", function() {
		layVar[0] = $("#layvar-0").get("checked") | 0;
		layVar[1] = $("#layvar-1").get("checked") | 0;
		layVar[2] = $("#layvar-2").get("checked") | 0;

		sendCmd(COMMAND_SET_LAYOUT_VARIANT, nstr(layVar[0]) + nstr(layVar[1]) + nstr(layVar[2]));
		debugMessage("layVar" + debugMessageReconfigured);
	});
	$("[id*='buildtype']").on("change", function() {
		buildtype = $("#buildtype").get("value");

		sendCmd(COMMAND_SET_BUILDTYPE, nstr(buildtype));
		debugMessage("buildtype" + debugMessageReconfigured);
	});
	$("[id*='whitetype']").on("change", function() {
		wType = $("#whitetype").get("value");

		sendCmd(COMMAND_SET_WHITETYPE, nstr(wType));
		debugMessage("whitetype" + debugMessageReconfigured);
	});
});
