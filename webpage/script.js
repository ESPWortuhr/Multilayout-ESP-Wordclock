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

var debugMessageReconfigured = " was reconfigured.";
var websocket;
var ipEsp = "ws://192.168.4.1";
var debug = true;
var command = 1;
var rgbw = [
	[0, 0, 100, 0],
	[0, 10, 0, 0],
	[10, 0, 0, 0],
	[5, 5, 5, 0]
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
var dialect = [0, 0, 0, 0, 0, 0, 0];
var layVar = [0];
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
var displayAutoLdr = 0;
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
var COMMAND_MODE_SCROLLINGTEXT = 3;
var COMMAND_MODE_RAINBOWCYCLE = 4;
var COMMAND_MODE_RAINBOW = 5;
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
var COMMAND_SET_LAYOUT_VARIANT = 103;

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
	rgbw = [
		[0, 0, 100, 0],
		[0, 10, 0, 0],
		[10, 0, 0, 0],
		[5, 5, 5, 0]
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
	ldr = 0;
	ldrCal = 10;
	dialect = [0, 0, 0, 0, 0, 0, 0];
	layVar = [0];
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
	displayAutoLdr = 0;
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

		debugMessage("The connection with the websocket was closed (code " + event.code + ").", event);
	};

	websocket.onmessage = function(event) {

		var data = JSON.parse(event.data);

		debugMessage("Webservice response arrived (Command " + data.command + ").", data);

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
			$("#marquee").set("value", data.scrollingText);

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
			document.getElementById("dialect-6").checked = data.langVar6;

			document.getElementById("layvar-0").checked = data.layVar0;

			document.getElementById("ldr").checked = data.ldr;
			$("ldr-cal").set("value", data.ldrCal);
			$("#slider-brightness").set("value", data.effectBri);
			$("#slider-speed").set("value", data.effectSpeed); // TODO: there is no property effectSpeed!
			document.getElementById("show-seconds").checked = data.secondVariant;
			$("#show-minutes").set("value", data.minuteVariant);

			$("#owm-api-key").set("value", data.apiKey);
			$("#owm-city-id").set("value", data.cityid);

			$("#front-layout").set("value", data.UhrtypeDef);
			$("#colortype").set("value", data.colortype);

			document.getElementById("boot-show-led-blink").checked = data.bootLedBlink;
			document.getElementById("boot-show-led-sweep").checked = data.bootLedSweep;
			document.getElementById("boot-show-wifi").checked = data.bootShowWifi;
			document.getElementById("boot-show-ip").checked = data.bootShowIP;

			enableSpecific("specific-layout-2", data.hasDreiviertel);
			enableSpecific("specific-layout-3", data.hasZwanzig);
			enableSpecific("specific-layout-4", data.hasSecondsFrame);
			enableSpecific("specific-layout-5", data.hasWeatherLayout);
			enableSpecific("specific-layout-6", data.UhrtypeDef === 10); // EN10x11
			enableSpecific("specific-colortype-4", data.colortype === 4);
			removeSpecificOption("show-minutes", "2", data.UhrtypeDef !== 4); // MinuteVariant "Row"
			removeSpecificOption("show-minutes", "3", data.UhrtypeDef !== 9); // MinuteVariant "In Words"

			autoLdrEnabled = data.autoLdrEnabled;
			$("#auto-ldr-enabled").set("value", autoLdrEnabled);
			enableSpecific("specific-layout-brightness-man", autoLdrEnabled === 0);
			enableSpecific("specific-layout-brightness-auto", autoLdrEnabled === 1);
		}
		if (data.command === "set") {
			rgbw[0][0] = data.rgbw00;
			rgbw[0][1] = data.rgbw01;
			rgbw[0][2] = data.rgbw02;
			rgbw[0][3] = data.rgbw03;
			rgbw[1][0] = data.rgbw10;
			rgbw[1][1] = data.rgbw11;
			rgbw[1][2] = data.rgbw12;
			rgbw[1][3] = data.rgbw13;
			rgbw[2][0] = data.rgbw20;
			rgbw[2][1] = data.rgbw21;
			rgbw[2][2] = data.rgbw22;
			rgbw[2][3] = data.rgbw23;
			rgbw[3][0] = data.rgbw30;
			rgbw[3][1] = data.rgbw31;
			rgbw[3][2] = data.rgbw32;
			rgbw[3][3] = data.rgbw33;
			effectBri = data.effectBri;
			effectSpeed = data.effectSpeed;
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
		debugMessage("An error occurred while connecting to the websocket.", event);
	};
}

function changeColor(color) {
	rgbw[color.index][0] = color.red;
	rgbw[color.index][1] = color.green;
	rgbw[color.index][2] = color.blue;
	rgbw[color.index][3] = Math.round(255 * (1.0 - color.alpha));
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
			options: { sliderType: "alpha" }
		}]
	});
	colorPicker.on("input:change", changeColor);
	// attach extra attribute to the alpha slider to only show it for RGBW LEDs
	const alphaSlider = colorPicker.el.lastElementChild.lastElementChild;
	alphaSlider.classList.add("specific-colortype-4");
}

/**
 * show the color configuration in the color picker
 */
function setColorPicker(withBackground) {
	var rgbaFg = {
		r: rgbw[COLOR_FOREGROUND][0],
		g: rgbw[COLOR_FOREGROUND][1],
		b: rgbw[COLOR_FOREGROUND][2],
		a: 1.0 - rgbw[COLOR_FOREGROUND][3] / 255.0
	};
	var rgbaBg = {
		r: rgbw[COLOR_BACKGROUND][0],
		g: rgbw[COLOR_BACKGROUND][1],
		b: rgbw[COLOR_BACKGROUND][2],
		a: 1.0 - rgbw[COLOR_BACKGROUND][3] / 255.0
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
		rgbw[COLOR_BACKGROUND][0] ||
		rgbw[COLOR_BACKGROUND][1] ||
		rgbw[COLOR_BACKGROUND][2] ||
		rgbw[COLOR_BACKGROUND][3];
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
		rgbw[COLOR_BACKGROUND][0] = 5;
		rgbw[COLOR_BACKGROUND][1] = 5;
		rgbw[COLOR_BACKGROUND][2] = 5;
		rgbw[COLOR_BACKGROUND][3] = 5;
	} else {
		// set to black
		rgbw[COLOR_BACKGROUND][0] = 0;
		rgbw[COLOR_BACKGROUND][1] = 0;
		rgbw[COLOR_BACKGROUND][2] = 0;
		rgbw[COLOR_BACKGROUND][3] = 0;
	}
	setColorPicker(withBackground);
	sendColorData(command, nstr(1));
}

/**
 * Sets all sliders (the values) and their corresponsding labels to
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
	if (autoLdrInterval == null && autoLdrEnabled === 1) {
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
		debugMessage("LDR requests stopped", null);
	}
}

function nstr5(number) {
	return number.toString().padStart(5, "0");
}

function nstr(number) {
	return number.toString().padStart(3, "0");
}

function getPaddedString(string, maxStringLength) {
	return string.padEnd(maxStringLength, " ");
}

function sendCmd(command, addData = "") {
	var data = nstr(command) + addData + "999";
	websocket.send(data);
	debugMessage("Send data: ", data);
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
	sendCmd(command, nstr(rgbw[COLOR_FOREGROUND][0]) +
	nstr(rgbw[COLOR_FOREGROUND][1]) +
	nstr(rgbw[COLOR_FOREGROUND][2]) +
	nstr(rgbw[COLOR_FOREGROUND][3]) +
	nstr(rgbw[COLOR_BACKGROUND][0]) +
	nstr(rgbw[COLOR_BACKGROUND][1]) +
	nstr(rgbw[COLOR_BACKGROUND][2]) +
	nstr(rgbw[COLOR_BACKGROUND][3]) +
	nstr(effectBri) +
	nstr(effectSpeed));
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
		if (navigation === "frontoptions") {
			sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
		}
		if (navigation === "settings" || navigation === "frontoptions") {
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
		if (id === "mode-marquee") {
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
		debugMessage("Animation" + debugMessageReconfigured);
		setAnimation();
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

		var timeserverValue = $("#timeserver").get("value");

		sendCmd(COMMAND_SET_TIMESERVER, getPaddedString(timeserverValue, DATA_TIMESERVER_TEXT_LENGTH));
		debugMessage("Timeserver" + debugMessageReconfigured);
		return false;
	});
	$("#marquee-button").on("click", function() {
		var marqueeTextValue = $("#marquee").get("value");

		sendCmd(COMMAND_SET_MARQUEE_TEXT, getPaddedString(marqueeTextValue, DATA_MARQUEE_TEXT_LENGTH));
		debugMessage("ScrollingText" + debugMessageReconfigured);
	});
	$("[id*='brightness']").on("change", function() {
		sendBrightnessData(COMMAND_SET_BRIGHTNESS);
	});
	$("#weather-button").on("click", function() {

		var apiKey = $("#owm-api-key").get("value");
		var cityId = $("#owm-city-id").get("value");

		sendCmd(COMMAND_SET_WEATHER_DATA, cityId + " " + apiKey);
		debugMessage("OpenWeatherMap Login" + debugMessageReconfigured);
	});
	$("#show-minutes").on("change", function() {
		var showMinutesValue = $("#show-minutes").get("value");

		sendCmd(COMMAND_SET_MINUTE, nstr(showMinutesValue));
		debugMessage("MinuteVariant" + debugMessageReconfigured);
	});
	$("#show-seconds").on("change", function() {
		var showSecondsValue = $("#show-seconds").get("checked") | 0;

		sendCmd(COMMAND_SET_SETTING_SECOND, nstr(showSecondsValue));
		debugMessage("SecondVariant" + debugMessageReconfigured);
	});
	$("#front-layout").on("change", function() {
		var frontLayout = $("#front-layout").get("value");

		sendCmd(COMMAND_SET_UHRTYPE, nstr(frontLayout));
		sendCmd(COMMAND_REQUEST_CONFIG_VALUES);
		debugMessage("FrontLayout" + debugMessageReconfigured);
	});
	$("#colortype-button").on("click", function() {
		colortype = $("#colortype").get("value");

		sendCmd(COMMAND_SET_COLORTYPE, nstr(colortype));
		debugMessage("Colortype" + debugMessageReconfigured);
	});
	$("[id*='ldr']").on("change", function() {
		ldr = $("#ldr").get("checked") | 0;
		ldrCal = $("#ldr-cal").get("value");

		sendCmd(COMMAND_SET_LDR, nstr(ldr) + nstr(ldrCal));
		debugMessage("LDR mode" + debugMessageReconfigured);
	});
	$("#hostname-button").on("click", function() {
		var hostname = $("#hostname").get("value");

		sendCmd(COMMAND_SET_HOSTNAME, getPaddedString(hostname, DATA_HOST_TEXT_LENGTH));
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
	$("#wps-button").on("click", function() {
		sendCmd(COMMAND_SET_WPS_MODE);
	});
	$("#reset-button").on("click", function() {
		sendCmd(COMMAND_RESET);
	});
	$("#uhrzeit-button").on("click", function() {
		var hour = $("#hour").get("value");
		var minute = $("#minute").get("value");

		sendCmd(COMMAND_SET_TIME_MANUAL, nstr(hour) + nstr(minute));
		debugMessage("Time manually set");
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
	$("[id*='dialect']").on("change", function() {
		dialect[0] = $("#dialect-0").get("value");
		dialect[1] = $("#dialect-1").get("value");
		dialect[2] = $("#dialect-2").get("value");
		dialect[3] = $("#dialect-3").get("value");
		dialect[4] = $("#dialect-4").get("checked") | 0;
		dialect[5] = $("#dialect-5").get("value");
		dialect[6] = $("#dialect-6").get("checked") | 0;

		sendCmd(COMMAND_SET_LANGUAGE_VARIANT, nstr(dialect[0]) + nstr(dialect[1]) + nstr(dialect[2]) + nstr(dialect[3]) + nstr(dialect[4]) + nstr(dialect[5]) + nstr(dialect[6]));
		debugMessage("langVar" + debugMessageReconfigured);
	});
	$("[id*='layvar']").on("change", function() {
		layVar[0] = $("#layvar-0").get("checked") | 0;

		sendCmd(COMMAND_SET_LAYOUT_VARIANT, nstr(layVar[0]));
		debugMessage("layVar" + debugMessageReconfigured);
	});
});
