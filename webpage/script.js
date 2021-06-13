"use strict";
const iro = window.iro; // require("@jaames/iro");
(function(window, document) {

	var layout = document.getElementById("layout");
	var menu = document.getElementById("menu");
	var menuLink = document.getElementById("menu-link");

	function toggleClass(element, className) {
		var classes = element.className.split(/\s+/);
		var length = classes.length;
		var i = 0;

		for (; i < length; i++) {
			if (classes[i] === className) {
				classes.splice(i, 1);
				break;
			}
		}
		// The className is not found
		if (length === classes.length) {
			classes.push(className);
		}

		element.className = classes.join(" ");
	}

	function toggleAll(element) {
		var active = "active";

		element.preventDefault();
		toggleClass(layout, active);
		toggleClass(menu, active);
		toggleClass(menuLink, active);
	}

	function handleEvent(element) {
		if (element.target.id === menuLink.id) {
			return toggleAll(element);
		}

		if (menu.className.indexOf("active") !== -1) {
			return toggleAll(element);
		}
	}

	document.addEventListener("click", handleEvent);

}(this, this.document));

var MINI = require("minified");
// eslint-disable-next-line one-var
var _ = MINI._,
	$ = MINI.$,
	$$ = MINI.$$,
	EE = MINI.EE,
	HTML = MINI.HTML;

var websocket;
var ipEsp = "ws://192.168.4.1";
var debug = true;
var command = 1;
var rgb = [
	[0, 0, 100, 0],
	[0, 10, 0, 0],
	[10, 0, 0, 0],
	[5, 5, 5, 0]
];
var hell = 2;
var geschw = 10;
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
var dialect = [0, 0, 0, 0];
var ldr = 0;
var ldrCal = 0;
var showSeconds = 0;
var showMinutes = 0;
var UhrtypeDef = 0;
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
var autoLdrBright = " ";
var autoLdrDark = " ";
var animType = 0;
var animTypes = ["keine"];
var animDuration = 1;
var animSpeed = 30;
var animColorize = 1;
var animDemo = false;

// operation modes
var COMMAND_MODE_WORD_CLOCK = 1;
var COMMAND_MODE_SECONDS = 2;
var COMMAND_MODE_MARQUEE = 3;
var COMMAND_MODE_RAINBOW = 4;
var COMMAND_MODE_CHANGE = 5;
var COMMAND_MODE_COLOR = 6;
var COMMAND_MODE_ANIMATION = 10;

// other commands
var COMMAND_SET_INITIAL_VALUES = 20;
var COMMAND_SET_TIME = 30;
var COMMAND_SET_LANGUAGE_VARIANT = 84;
var COMMAND_SET_MQTT = 85;
var COMMAND_SET_TIME_MANUAL = 86;
var COMMAND_SET_WPS_MODE = 87;
var COMMAND_SET_COLORTYPE = 88;
var COMMAND_SET_UHRTYPE = 89;
var COMMAND_SET_WEATHER_DATA = 90;
var COMMAND_SET_LDR = 91;
var COMMAND_SET_HOSTNAME = 92;
var COMMAND_SET_SETTING_SECOND = 93;
var COMMAND_SET_MINUTE = 94;
var COMMAND_SET_BRIGHTNESS = 95;
var COMMAND_SET_MARQUEE_TEXT = 96;
var COMMAND_SET_TIMESERVER = 97;
var COMMAND_SET_WIFI_DISABLED = 98;
var COMMAND_SET_WIFI_AND_RESTART = 99;
var COMMAND_RESET = 100;
var COMMAND_SET_BOOT = 101;
var COMMAND_SET_AUTO_LDR = 102;

var COMMAND_BRIGHTNESS = 151;
var COMMAND_SPEED = 152;
var COMMAND_LEDS = 153;

var COMMAND_REQUEST_CONFIG_VALUES = 200;
var COMMAND_REQUEST_COLOR_VALUES = 201;
var COMMAND_REQUEST_WIFI_LIST = 202;
var COMMAND_REQUEST_AUTO_LDR = 203;
var COMMAND_REQUEST_ANIMATION = 204;
var COMMAND_REQUEST_MQTT_VALUES = 205;

// colors
var COLOR_FOREGROUND = 0;
var COLOR_BACKGROUND = 1;

// data that gets send back to the esp
var DATA_MARQUEE_TEXT_LENGTH = 30;
var DATA_TIMESERVER_TEXT_LENGTH = 30;
var DATA_MQTT_RESPONSE_TEXT_LENGTH = 30;
var DATA_HOST_TEXT_LENGTH = 30;

// color pickers
var colorPicker;

function initConfigValues() {
	var locationHost = location.host;
	if (locationHost !== "") {
		ipEsp = "ws://" + locationHost;
	} else {
		ipEsp = "ws://192.168.178.44/";
	}

	debug = true;
	command = 1;
	rgb = [
		[0, 0, 100, 0],
		[0, 10, 0, 0],
		[10, 0, 0, 0],
		[5, 5, 5, 0]
	];
	hell = 2;
	geschw = 10;
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
	ldr = 0;
	ldrCal = 10;
	dialect = [0, 0, 0, 0];
	showSeconds = 0;
	showMinutes = 0;
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
	autoLdrEnabled = 0;
	autoLdrBright = " ";
	autoLdrDark = " ";
	animType = 0;
	animTypes = ["keine"];
	animDuration = 1;
	animSpeed = 30;
	animColorize = 1;
	animDemo = false;
}

/* eslint-disable no-console */
function debugMessage(debugMessage, someObject) {
	if (debug === true) {

		if (console !== undefined) {
			console.log(debugMessage);

			if (someObject) {
				console.log(someObject);
			}
		}

		$("#output").fill(debugMessage);
	}
}

/// only shows elements of class `cls` if `enbl` is true.
function enableSpecific(cls, enbl) {
	let items = document.getElementsByClassName(cls);
	for (var item of items) {
		item.style.display = enbl ? null : "none";
	}
}

function initWebsocket() {
	websocket = new WebSocket(ipEsp);

	var ldrSelect = document.getElementById("ldr-cal");
	var ldrIndex;
	var ldrOption;
	for (ldrIndex = 0; ldrIndex < 11; ldrIndex++) {
		ldrOption = document.createElement("option");
		ldrOption.value = ldrIndex;
		ldrOption.text = ldrIndex;
		ldrSelect.remove(ldrIndex);
		ldrSelect.add(ldrOption);
	}

	var hourSelect = document.getElementById("stunde");
	var hourIndex;
	var hourOption;
	for (hourIndex = 0; hourIndex < 24; hourIndex++) {
		hourOption = document.createElement("option");
		hourOption.value = hourIndex;
		hourOption.text = hourIndex;
		hourSelect.remove(hourIndex);
		hourSelect.add(hourOption);
	}

	var minuteSelect = document.getElementById("minute");
	var minuteIndex;
	var minuteOption;
	for (minuteIndex = 0; minuteIndex < 60; minuteIndex++) {
		minuteOption = document.createElement("option");
		minuteOption.value = minuteIndex;
		minuteOption.text = minuteIndex;
		minuteSelect.remove(minuteIndex);
		minuteSelect.add(minuteOption);
	}

	var brightnessSelectIndex;
	for (brightnessSelectIndex = 0; brightnessSelectIndex < 8; brightnessSelectIndex++) {
		var brightnessSelect = document.getElementById("brightness-" + String(brightnessSelectIndex));
		var brightnessIndex;
		var brightnessOption;
		for (brightnessIndex = 0; brightnessIndex < 6; brightnessIndex++) {
			brightnessOption = document.createElement("option");
			brightnessOption.value = brightnessIndex * 20;
			brightnessOption.text = String(brightnessIndex * 20) + " %";
			brightnessSelect.remove(brightnessIndex);
			brightnessSelect.add(brightnessOption);
		}
	}

	websocket.onopen = function(event) {

		$("#status").set("+online");
		$("#status").set("-offline");
		$("#status").set("@value", "Online");
		$(".status-button").fill("Verbindung trennen");
		$(".status-button").set("@value", "1");
		$("#section-connection-lost").set({
			$display: "none"
		});

		debugMessage("Die Verbindung mit dem Websocket wurde aufgebaut.", event);

		sendCmd(COMMAND_REQUEST_COLOR_VALUES);
		sendCmd(COMMAND_REQUEST_ANIMATION);
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

		debugMessage("Die Verbindung mit dem Websocket wurde geschlossen (Code " + event.code + ").", event);
	};

	websocket.onmessage = function(event) {

		var data = JSON.parse(event.data);

		debugMessage("Webservice response arrived (Command " + data.command + ").", data);

		if (data.command === "mqtt") {
			$("#mqtt-port").set("value", data.MQTT_Port);
			$("#mqtt-server").set("value", data.MQTT_Server);
			$("#mqtt-state").set("value", data.MQTT_State);
			$("#mqtt-user").set("value", data.MQTT_User);
			$("#mqtt-pass").set("value", data.MQTT_Pass);
			$("#mqtt-clientid").set("value", data.MQTT_ClientId);
			$("#mqtt-topic").set("value", data.MQTT_Topic);
		}

		if (data.command === "config") {

			$("#ssid").set("value", data.ssid);

			$("#timeserver").set("value", data.zeitserver);
			$("#hostname").set("value", data.hostname);
			$("#marquee").set("value", data.ltext);

			$("#brightness-0").set("value", data.h6);
			$("#brightness-1").set("value", data.h8);
			$("#brightness-2").set("value", data.h12);
			$("#brightness-3").set("value", data.h16);
			$("#brightness-4").set("value", data.h18);
			$("#brightness-5").set("value", data.h20);
			$("#brightness-6").set("value", data.h22);
			$("#brightness-7").set("value", data.h24);

			$("#dialect-0").set("value", data.spv0);
			$("#dialect-1").set("value", data.spv1);
			$("#dialect-2").set("value", data.spv2);
			$("#dialect-3").set("value", data.spv3);

			$("#ldr").set("value", data.ldr);
			$("#ldr-cal").set("value", data.ldrCal);
			$("#slider-brightness").set("value", data.hell);
			$("#slider-speed").set("value", data.geschw); // TODO: there is no property geschw!
			$("#show-seconds").set("value", data.zeige_sek);
			$("#show-minutes").set("value", data.zeige_min);

			$("#owm-api-key").set("value", data.apiKey);
			$("#owm-city-id").set("value", data.cityid);

			$("#front-layout").set("value", data.UhrtypeDef);
			$("#colortype").set("value", data.colortype);

			$("#boot-led-blink").set("checked", data.bootLedBlink | 0);
			$("#boot-led-sweep").set("checked", data.bootLedSweep | 0);
			$("#boot-show-wifi").set("checked", data.bootShowWifi | 0);
			$("#boot-show-ip").set("checked", data.bootShowIP | 0);

			enableSpecific("specific-layout-2", data.hasDreiviertel);
			enableSpecific("specific-layout-3", data.hasZwanzig);
			enableSpecific("specific-layout-4", data.hasSecondsFrame);
			enableSpecific("specific-layout-5", data.hasWeatherLayout);
			enableSpecific("specific-colortype-4", data.colortype === 4);
		}
		if (data.command === "set") {
			rgb[0][0] = data.rgb00;
			rgb[0][1] = data.rgb01;
			rgb[0][2] = data.rgb02;
			rgb[0][3] = data.rgb03;
			rgb[1][0] = data.rgb10;
			rgb[1][1] = data.rgb11;
			rgb[1][2] = data.rgb12;
			rgb[1][3] = data.rgb13;
			rgb[2][0] = data.rgb20;
			rgb[2][1] = data.rgb21;
			rgb[2][2] = data.rgb22;
			rgb[2][3] = data.rgb23;
			rgb[3][0] = data.rgb30;
			rgb[3][1] = data.rgb31;
			rgb[3][2] = data.rgb32;
			rgb[3][3] = data.rgb33;
			hell = data.hell;
			geschw = data.geschw;
			colortype = data.colortype;
			var map = [0, 0, 2, 3, 4, 5, 1];	// see COMMAND_MODE_XX
			var prog = data.prog;
			command = prog === 0 ? COMMAND_MODE_WORD_CLOCK : prog;	// 0 == COMMAND_IDLE
			document.prog.mode[map[prog]].checked = true;
			setSliders();
			setColors();
			enableSpecific("specific-colortype-4", data.colortype === 4);
		}
		if (data.command === "wlan") {
			document.getElementById("wlanlist").innerHTML = data.list;
		}
		if (data.command === "animation") {
			animType = data.animType;
			animTypes = data.animTypes;
			animDuration = data.animDuration;
			animSpeed = data.animSpeed;
			animColorize = data.animColorize;
			animDemo = data.animDemo;
			var index;
			var animSelect = document.getElementById("animation-types");
			var option;
			for (index = 0; index < animTypes.length; index++) {
				option = document.createElement("option");
				option.value = index;
				option.text = animTypes[index];
				animSelect.remove(index);
				animSelect.add(option);
			}
			setAnimation();
		}
		if (data.command === "autoLdr") {
			$("#auto-ldr-enabled").set("value", data.autoLdrEnabled);
			$("#auto-ldr-value").set("value", data.autoLdrValue);
			$("#auto-ldr-bright").set("value", data.autoLdrBright);
			$("#auto-ldr-dark").set("value", data.autoLdrDark);
		}
	};
	websocket.onerror = function(event) {
		debugMessage("Bei der Verbindung mit dem Websocket ist ein Fehler aufgetreten.", event);
	};
}

function changeColor(color) {
	rgb[color.index][0] = color.red;
	rgb[color.index][1] = color.green;
	rgb[color.index][2] = color.blue;
	rgb[color.index][3] = Math.round(255 * (1.0 - color.alpha));
	sendData(command, nstr(1));
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
			options: { sliderType: "alpha" }
		}]
	});
	colorPicker.on("input:change", changeColor);
	// attach extra attribute to the alpha slider to only show it for RGBW LEDs
	const alphaSlider = colorPicker.el.lastElementChild.lastElementChild;
	alphaSlider.classList.add("specific-colortype-4");
}

/**
 * Gets all the values from the sliders and puts them in the config variables.
 */
function getSliders() {
	hell = $("#slider-brightness").get("value");
	geschw = $("#slider-speed").get("value");
}

/**
 * show the color configuration in the color picker
 */
function setColorPicker(withBackground) {
	var rgbaFg = {
		r: rgb[COLOR_FOREGROUND][0],
		g: rgb[COLOR_FOREGROUND][1],
		b: rgb[COLOR_FOREGROUND][2],
		a: 1.0 - rgb[COLOR_FOREGROUND][3] / 255.0
	};
	var rgbaBg = {
		r: rgb[COLOR_BACKGROUND][0],
		g: rgb[COLOR_BACKGROUND][1],
		b: rgb[COLOR_BACKGROUND][2],
		a: 1.0 - rgb[COLOR_BACKGROUND][3] / 255.0
	};
	var colors = [rgbaFg];
	if (withBackground) {
		colors.push(rgbaBg);
	}
	colorPicker.setColors(colors);
}

/**
 * initialize background checkbox and color picker
 */
function setColors() {
	var withBackground =
		rgb[COLOR_BACKGROUND][0] ||
		rgb[COLOR_BACKGROUND][1] ||
		rgb[COLOR_BACKGROUND][2] ||
		rgb[COLOR_BACKGROUND][3];
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
		rgb[COLOR_BACKGROUND][0] = 5;
		rgb[COLOR_BACKGROUND][1] = 5;
		rgb[COLOR_BACKGROUND][2] = 5;
		rgb[COLOR_BACKGROUND][3] = 5;
	} else {
		// set to black
		rgb[COLOR_BACKGROUND][0] = 0;
		rgb[COLOR_BACKGROUND][1] = 0;
		rgb[COLOR_BACKGROUND][2] = 0;
		rgb[COLOR_BACKGROUND][3] = 0;
	}
	setColorPicker(withBackground);
	sendData(command, nstr(1));
}

/**
 * Sets all sliders (the values) and their corresponsding labels to
 * the currently stored config values.
 *
 * This function also updated the color area with the current rgb values.
 */
function setSliders() {
	// sliders
	$("#slider-brightness").set("value", hell);
	$("#slider-speed").set("value", geschw);

	// labels
	$("#slider-brightness-value").fill(hell);
	$("#slider-speed-value").fill(geschw);
}

function setAnimation() {
	if (document.getElementById("mode-wordclock").checked) {
		$("#animation-box").set({
			$display: "block"
		});
	} else {
		$("#animation-box").set({
			$display: "none"
		});
	}
	$("#animation-types").set("value", animType);
	$("#animation-duration").set("value", animDuration);
	$("#animation-speed-value").fill(animSpeed);
	$("#animation-speed").set("value", animSpeed);
	$("#animation-colorize").set("value", animColorize);
	document.getElementById("animation-demo").checked = animDemo;
}

function autoLdrValueUpdater() {
	if (autoLdrInterval == null) {
		autoLdrInterval = setInterval(function() {
			// jede Sekunde ausfuehren
			if ($("#auto-ldr-enabled").get("value") === "1") {
				sendCmd(COMMAND_REQUEST_AUTO_LDR, 1);
			}
		}, 1000);
	}
}

function autoLdrStop() {
	if (autoLdrInterval != null) {
		clearInterval(autoLdrInterval);
		autoLdrInterval = null;
		debugMessage("LDR Requests stopped", null);
	}
}

function nstr5(number) {
	if (number < 10) {
		number = "00" + number;
	} else {
		if (number < 100) {
			number = "0" + number;
		} else {
			if (number < 1000) {
				number = "0" + number;
			} else {
				if (number < 10000) {
					number = "0" + number;
				}
			}
		}
	}
	return number;
}

function nstr(number) {
	if (number < 10) {
		number = "00" + number;
	} else {
		if (number < 100) {
			number = "0" + number;
		}
	}
	return number;
}

/**
 * Returns the padding for the string that is send to the esp.
 * The string is padded until it has a length of exactly maxStringLength.
 *
 * @param  {string} string - The string that is padded with spaces.
 * @param  {int}    maxStringLength - The resulting length of the padded string.
 * @return {string} The padded string.
 */
function getPaddedString(string, maxStringLength) {

	while (string.length < maxStringLength) {
		string += " ";
	}
	return string;
}

/**
 * Sends data to the esp via a websocket connection.
 *
 * @param  {int} The command that specifies what to do on the esp.
 */
function sendData(command, addData = "") {
	var data = nstr(command) +
        nstr(rgb[COLOR_FOREGROUND][0]) +
        nstr(rgb[COLOR_FOREGROUND][1]) +
        nstr(rgb[COLOR_FOREGROUND][2]) +
        nstr(rgb[COLOR_FOREGROUND][3]) +
        nstr(rgb[COLOR_BACKGROUND][0]) +
        nstr(rgb[COLOR_BACKGROUND][1]) +
        nstr(rgb[COLOR_BACKGROUND][2]) +
        nstr(rgb[COLOR_BACKGROUND][3]) +
        nstr(hell) +
        nstr(geschw) + addData + "999";

	websocket.send(data);
	debugMessage("Send data: ", data);
}

function sendCmd(command, addData = "") {
	var data = nstr(command) + addData + "999";
	websocket.send(data);
	debugMessage("Send data: ", data);
}

$.ready(function() {

	initConfigValues();
	createColorPicker();
	setSliders();
	setAnimation();
	initWebsocket();
	setColors();

	$("#with-background").on("change", toggleBackground);
	$(".status-button").on("click", function() {
		var value = $(this).get("value");
		if (value === "1") {
			value = 0;
			$("#status").fill("Verbinden ...");
			$(".status-button").set("value", value);
			websocket.close();
		} else {
			value = 1;
			$("#status").fill("Verbinden ...");
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
	$("a[class~='pure-menu-link']").on("click", function() {
		var navigation = $(this)[0].dataset.navigation;

		// add/remove active class
		$(".pure-menu-link").set("-active");
		$(this).set("+active");

		if (navigation === "functions") {
			setAnimation();
		}
		if (navigation === "smart-home") {
			sendCmd(COMMAND_REQUEST_MQTT_VALUES);
		}
		if (navigation === "settings") {
			sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
			sendCmd(COMMAND_REQUEST_AUTO_LDR);
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

		if (id === "mode-wordclock") {
			command = COMMAND_MODE_WORD_CLOCK;
		}
		if (id === "mode-color") {
			command = COMMAND_MODE_COLOR;
		}
		if (id === "mode-seconds") {
			command = COMMAND_MODE_SECONDS;
		}
		if (id === "mode-marquee") {
			hasSpeed = true;
			command = COMMAND_MODE_MARQUEE;
		}
		if (id === "mode-rainbow") {
			hasBrightness = true;
			hasSpeed = true;
			command = COMMAND_MODE_RAINBOW;
		}
		if (id === "mode-change") {
			hasBrightness = true;
			hasSpeed = true;
			command = COMMAND_MODE_CHANGE;
		}

		setAnimation();

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

		sendData(command);
		setSliders();
		setColors();
	});

	$("[id*='slider']").onChange(function(event) {
		var id = $(this).get("id");

		if (sleep === 0) {
			getSliders();
			if (id === "slider-brightness") {
				sendData(COMMAND_BRIGHTNESS);
			}
			if (id === "slider-speed") {
				sendData(COMMAND_SPEED);
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

	$("#animation-speed").onChange(function(event) {
		animSpeed = $("#animation-speed").get("value");
		$("#animation-speed-value").fill(animSpeed);
	});

	$("[id*='animation']").on("change", function(event) {
		animType = $("#animation-types").get("value");
		animDuration = $("#animation-duration").get("value");
		animSpeed = $("#animation-speed").get("value");
		animColorize = $("#animation-colorize").get("value");
		animDemo = document.getElementById("animation-demo").checked;

		sendCmd(COMMAND_MODE_ANIMATION, nstr(animType) + nstr(animDuration) + nstr(animSpeed) + nstr(animColorize) + nstr(animDemo ? 1 : 0));
		debugMessage("Animation wurde neu konfiguriert");
		setAnimation();
		return false;
	});
	$("#initial-values-button").on("click", function() {
		sendCmd(COMMAND_SET_INITIAL_VALUES);
	});
	$("#wifi-button").on("click", function() {
		sendCmd(COMMAND_SET_WIFI_AND_RESTART);
		debugMessage("WLAN wird neu konfiguriert");
		return false;
	});
	$("#_wlanscan").on("click", function() {
		sendCmd(COMMAND_REQUEST_WIFI_LIST);
		document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";
		return false;
	});
	$("#timeserver-button").on("click", function() {

		var timeserverValue = $("#timeserver").get("value");

		sendCmd(COMMAND_SET_TIMESERVER, getPaddedString(timeserverValue, DATA_TIMESERVER_TEXT_LENGTH));
		debugMessage("Zeitserver wurde neu konfiguriert");
		return false;
	});
	$("#marquee-button").on("click", function() {
		var marqueeTextValue = $("#marquee").get("value");

		sendCmd(COMMAND_SET_MARQUEE_TEXT, getPaddedString(marqueeTextValue, DATA_MARQUEE_TEXT_LENGTH));
		debugMessage("Lauftext wurde neu konfiguriert");
	});
	$("#auto-ldr-button").on("click", function() {
		autoLdrEnabled = $("#auto-ldr-enabled").get("value");
		autoLdrBright = $("#auto-ldr-bright").get("value");
		autoLdrDark = $("#auto-ldr-dark").get("value");
		sendCmd(COMMAND_SET_AUTO_LDR, nstr(autoLdrEnabled) + nstr(autoLdrBright) + nstr(autoLdrDark));
		sendCmd(COMMAND_REQUEST_AUTO_LDR);	// read back values
	});
	$("#brightness-button").on("click", function() {

		h6 = $("#brightness-0").get("value");
		h8 = $("#brightness-1").get("value");
		h12 = $("#brightness-2").get("value");
		h16 = $("#brightness-3").get("value");
		h18 = $("#brightness-4").get("value");
		h20 = $("#brightness-5").get("value");
		h22 = $("#brightness-6").get("value");
		h24 = $("#brightness-7").get("value");

		sendCmd(COMMAND_SET_BRIGHTNESS, nstr(h6) + nstr(h8) + nstr(h12) + nstr(h16) + nstr(h18) + nstr(h20) + nstr(h22) + nstr(h24));
		debugMessage("Helligkeit wurde neu konfiguriert");
	});
	$("#weather-button").on("click", function() {

		var apiKey = $("#owm-api-key").get("value");
		var cityId = $("#owm-city-id").get("value");

		sendCmd(COMMAND_SET_WEATHER_DATA, cityId + " " + apiKey);
		debugMessage("OpenWeatherMap Zugangsdaten wurden konfiguriert");
	});
	$("#show-minutesbutton").on("click", function() {
		var showMinutesValue = $("#show-minutes").get("value");

		sendCmd(COMMAND_SET_MINUTE, nstr(showMinutesValue));
		debugMessage("Minutenanzeige wurde neu konfiguriert");
	});
	$("#show-seconds-button").on("click", function() {
		var showSecondsValue = $("#show-seconds").get("value");

		sendCmd(COMMAND_SET_SETTING_SECOND, nstr(showSecondsValue));
		debugMessage("Sekundenanzeige wurde neu konfiguriert");
	});
	$("#front-layout-button").on("click", function() {
		var frontLayout = $("#front-layout").get("value");

		sendCmd(COMMAND_SET_UHRTYPE, nstr(frontLayout));
		debugMessage("frontLayout wurde neu konfiguriert");
	});
	$("#colortype-button").on("click", function() {
		colortype = $("#colortype").get("value");

		sendCmd(COMMAND_SET_COLORTYPE, nstr(colortype));
		debugMessage("Colortype wurde neu konfiguriert");
	});
	$("#ldr-button").on("click", function() {
		ldr = $("#ldr").get("value");

		sendCmd(COMMAND_SET_LDR, nstr(ldr) + nstr(ldrCal));
		debugMessage("LDR Steuerung wurde konfiguriert");
	});
	$("#hostname-button").on("click", function() {
		var hostValue = $("#hostname").get("value");

		sendCmd(COMMAND_SET_HOSTNAME, getPaddedString(hostValue, DATA_HOST_TEXT_LENGTH));
		debugMessage("Hostname wurde neu konfiguriert");
	});
	$("#boot-button").on("click", function() {

		bootLedBlink = $("#boot-led-blink").get("checked") | 0;
		bootLedSweep = $("#boot-led-sweep").get("checked") | 0;
		bootShowWifi = $("#boot-show-wifi").get("checked") | 0;
		bootShowIP = $("#boot-show-ip").get("checked") | 0;

		sendCmd(COMMAND_SET_BOOT, nstr(bootLedBlink) + nstr(bootLedSweep) + nstr(bootShowWifi) + nstr(bootShowIP));
		debugMessage("Bootoptionen wurden neu konfiguriert");
	});
	$("#disable-button").on("click", function() {
		sendCmd(COMMAND_SET_WIFI_DISABLED);
	});
	$("#wps-button").on("click", function() {
		sendCmd(COMMAND_SET_WPS_MODE);
	});
	$("#reset-button").on("click", function() {
		sendCmd(COMMAND_RESET);
	});
	$("#uhrzeit-button").on("click", function() {
		var stunde = $("#stunde").get("value");
		var minute = $("#minute").get("value");

		sendCmd(COMMAND_SET_TIME_MANUAL, nstr(stunde) + nstr(minute));
		debugMessage("Uhrzeit wurde manuell konfiguriert");
	});
	$("#mqtt-button").on("click", function() {
		MQTTState = $("#mqtt-state").get("value");
		MQTTPort = $("#mqtt-port").get("value");
		MQTTServer = $("#mqtt-server").get("value");
		MQTTUser = $("#mqtt-user").get("value");
		MQTTPass = $("#mqtt-pass").get("value");
		MQTTClientId = $("#mqtt-clientid").get("value");
		MQTTTopic = $("#mqtt-topic").get("value");

		sendCmd(COMMAND_SET_MQTT, nstr(MQTTState) + nstr5(MQTTPort) + getPaddedString(MQTTServer, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTUser, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTPass, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTClientId, DATA_MQTT_RESPONSE_TEXT_LENGTH) + getPaddedString(MQTTTopic, DATA_MQTT_RESPONSE_TEXT_LENGTH));
		debugMessage("MQTT Server wurde konfiguriert");
	});
	$("#dialect-button").on("click", function() {
		dialect[0] = $("#dialect-0").get("value");
		dialect[1] = $("#dialect-1").get("value");
		dialect[2] = $("#dialect-2").get("value");
		dialect[3] = $("#dialect-3").get("value");

		sendCmd(COMMAND_SET_LANGUAGE_VARIANT, nstr(dialect[0]) + nstr(dialect[1]) + nstr(dialect[2]) + nstr(dialect[3]));
		debugMessage("dialect wurde konfiguriert");
	});
});
