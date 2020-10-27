"use strict";

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
var sliderType = 0; // 0: foreground, 1 background
var hell = 2;
var geschw = 10;
var anzahl = 100;
var sleep = 0;
var sleeptime = 1;
var position = 100;
var color = 0;
var h6 = 100;
var h8 = 100;
var h12 = 100;
var h16 = 100;
var h18 = 100;
var h20 = 100;
var h22 = 100;
var h24 = 100;
var showSeconds = 0;
var showMinutes = 0;
var UhrtypeDef = 0;
var colortype = 0;

// other commands
var COMMAND_SET_INITIAL_VALUES = 20;
var COMMAND_SET_TIME = 30;
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
var COMMAND_REQUEST_CONFIG_VALUES = 300;
var COMMAND_REQUEST_COLOR_VALUES = 301;
var COMMAND_REQUEST_WIFI_LIST = 302;

// operation modes
var COMMAND_MODE_WORD_CLOCK = 1;
var COMMAND_MODE_SECONDS = 200;
var COMMAND_MODE_MARQUEE = 201;
var COMMAND_MODE_RAINBOW = 202;
var COMMAND_MODE_CHANGE = 203;
var COMMAND_MODE_COLOR = 204;

var COMMAND_BRIGHTNESS = 251;
var COMMAND_SPEED = 252;
var COMMAND_LEDS = 253;
var COMMAND_POSITION = 254;

// colors
var COLOR_FOREGROUND = 0;
var COLOR_BACKGROUND = 1;
var COLOR_BORDER = 2;
var COLOR_EFFECT = 3;

// data that gets send back to the esp
var DATA_MARQUEE_TEXT_LENGTH = 30;
var DATA_SSID_TEXT_LENGTH = 25;
var DATA_PASSWORT_TEXT_LENGTH = 25;
var DATA_TIMESERVER_TEXT_LENGTH = 16;
var DATA_HOST_TEXT_LENGTH = 16;

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
    sliderType = 0;
    hell = 2;
    geschw = 10;
    anzahl = 100;
    sleep = 0;
    sleeptime = 1;
    position = 100;
    anzahl = 100;
    color = 0;
    h6 = 100;
    h8 = 100;
    h12 = 100;
    h16 = 100;
    h18 = 100;
    h20 = 100;
    h22 = 100;
    h24 = 100;
    showSeconds = 0;
    showMinutes = 0;
    UhrtypeDef = 0;
    colortype = 0;
}

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        red: parseInt(result[1], 16),
        green: parseInt(result[2], 16),
        blue: parseInt(result[3], 16)
    } : null;
}

/**
 * Displays a debug message, if the global debug flag is set to true. Optionally
 * an object can be provided that will be printed on the console (when a console
 * is available).
 */

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

function initWebsocket() {

    websocket = new WebSocket(ipEsp);

    websocket.onopen = function (event) {

        $("#status").set("+online");
        $("#status").set("-offline");
        $("#status").set("@value", "Online");
        $(".status-button").fill("Verbindung trennen");
        $(".status-button").set("@value", "1");
        $("#section-connection-lost").set({
            $display: "none"
        });

        debugMessage("Die Verbindung mit dem Websocket wurde aufgebaut.", event);

        sendData(301, 0, 0);
    };

    websocket.onclose = function (event) {

        $("#status").set("-online");
        $("#status").set("+offline");
        $("#status").set("@value", "Offline");
        $(".status-button").fill("Verbinden");
        $(".status-button").set("@value", "0");
        $("#section-connection-lost").set({
            $display: "block"
        });

        debugMessage("Die Verbindung mit dem Websocket wurde geschlossen (Code " + event.code + ").", event);
    };

    websocket.onmessage = function (event) {

        var data = JSON.parse(event.data);

        debugMessage("Webservice response arrived (Command " + data.command + ").", data);

        if (data.command === "config") {

            $("#ssid").set("value", data.ssid);

            $("#timeserver").set("value", data.zeitserver);
            $("#hostname").set("value", data.hostname);
            $("#marquee").set("value", data.ltext);

            $("#brightness-6").set("value", data.h6);
            $("#brightness-8").set("value", data.h8);
            $("#brightness-12").set("value", data.h12);
            $("#brightness-16").set("value", data.h16);
            $("#brightness-18").set("value", data.h18);
            $("#brightness-20").set("value", data.h20);
            $("#brightness-22").set("value", data.h22);
            $("#brightness-24").set("value", data.h24);

            $("#slider-brightness").set("value", data.hell);
            $("#slider-speed").set("value", data.geschw); // TODO: there is no property geschw!
            $("#showSeconds").set("value", data.zeige_sek);
            $("#showMinutes").set("value", data.zeige_min);

            $("#owm-api-key").set("value", data.apiKey);
            $("#owm-city-id").set("value", data.cityid);

            $("#UhrtypeDef").set("value", data.UhrtypeDef);
            $("#colortype").set("value", data.colortype);
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
            setSliders();
        }
        if (data.command === "wlan"){
            document.getElementById("wlanlist").innerHTML = data.list}
    };
    websocket.onerror = function (event) {
        debugMessage("Bei der Verbindung mit dem Websocket ist ein Fehler aufgetreten.", event);
    };
}

/**
 * Gets all the values from the sliders and puts them in the config variables.
 */
function getSliders() {

    // rgb sliders
    rgb[sliderType][0] = $("#slider-red").get("value");
    rgb[sliderType][1] = $("#slider-green").get("value");
    rgb[sliderType][2] = $("#slider-blue").get("value");
    if (colortype === "4"){
        rgb[sliderType][3] = $("#slider-white").get("value");
    }

    // other sliders
    hell = $("#slider-brightness").get("value");
    geschw = $("#slider-speed").get("value");
    anzahl = $("#slider-leds").get("value");
    position = $("#slider-position").get("value");
}

/**
 * Sets all sliders (the values) and their corresponsding labels to
 * the currently stored config values.
 *
 * This function also updated the color area with the current rgb values.
 */
function setSliders() {

    // rgb sliders
    $("#slider-red").set("value", rgb[sliderType][0]);
    $("#slider-green").set("value", rgb[sliderType][1]);
    $("#slider-blue").set("value", rgb[sliderType][2]);
    $("#slider-white").set("value", rgb[sliderType][3]);

    // rgb labels
    $("#slider-red-value").fill(rgb[sliderType][0]);
    $("#slider-green-value").fill(rgb[sliderType][1]);
    $("#slider-blue-value").fill(rgb[sliderType][2]);
    $("#slider-white-value").fill(rgb[sliderType][3]);

    // various
    $("#slider-brightness").set("value", hell);
    $("#slider-speed").set("value", geschw);
    $("#slider-leds").set("value", anzahl);
    $("#slider-position").set("value", position);

    // various labels
    $("#slider-brightness-value").fill(hell);
    $("#slider-speed-value").fill(geschw);
    $("#slider-leds-value").fill(anzahl);
    $("#slider-position-value").fill(position);

    // Update the current color in the color area
    var colorArea = $("#color-area");
    colorArea[0].style.backgroundColor = "rgb(" + rgb[sliderType][0] + "," + rgb[sliderType][1] + "," + rgb[sliderType][2] + ")";
}

/**
 * Add '0' as a padding in front of the number to make it
 * a 3 character string.
 *
 * @param  {int} number - The number to be padded.
 * @return {string} The padded number.
 */
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
 * @param  {int} An unknown parameter.
 * @param  {int} An unknown parameter.
 */
function sendData(command, unknown2, unknown3) {
    var data = nstr(command) +
        nstr(unknown2) +
        nstr(unknown3) +
        nstr(rgb[COLOR_FOREGROUND][0]) +
        nstr(rgb[COLOR_FOREGROUND][1]) +
        nstr(rgb[COLOR_FOREGROUND][2]) +
        nstr(rgb[COLOR_FOREGROUND][3]) +
        nstr(rgb[COLOR_BACKGROUND][0]) +
        nstr(rgb[COLOR_BACKGROUND][1]) +
        nstr(rgb[COLOR_BACKGROUND][2]) +
        nstr(rgb[COLOR_BACKGROUND][3]) +
        nstr(rgb[COLOR_FOREGROUND][0]) + // 2 Removed the other colors because the were just confusing as hell
        nstr(rgb[COLOR_FOREGROUND][1]) + // 2
        nstr(rgb[COLOR_FOREGROUND][2]) + // 2
        nstr(rgb[COLOR_FOREGROUND][3]) + // 2
        nstr(rgb[COLOR_FOREGROUND][0]) + // 3
        nstr(rgb[COLOR_FOREGROUND][1]) + // 3
        nstr(rgb[COLOR_FOREGROUND][2]) + // 3
        nstr(rgb[COLOR_FOREGROUND][3]) + // 3
        nstr(hell) +
        nstr(geschw) +
        nstr(anzahl) +
        nstr(position) + "999";

    websocket.send(data);
    debugMessage("Send data: ", data);
}

$.ready(function () {

    initConfigValues();
    setSliders();
    initWebsocket();

    $(".status-button").on("click", function () {
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

    $("#_clock").on("click", function () {

        var date = new Date();
        var timeZoneOffset = date.getTimezoneOffset();
        timeZoneOffset = timeZoneOffset / 60 * -1;
        var time = date.getTime() / 1000;

        var data = "030000000";
        data += getPaddedString(nstr(timeZoneOffset) + time, 21);
        data += "999";

        debugMessage("Clock data: ", data);
        websocket.send(data);
    });

    /**
     * The color mode has been changed.
     *
     * There are a total of four different color modes that can
     * be changed (foreground, background, border and effect).
     * I disabled the last two because they were just confusing.
     */
    $("input[name='color-mode']").on("change", function () {

        switch ($(this).get("value")) {
            case "foreground":
                sliderType = COLOR_FOREGROUND;
                break;
            case "background":
                sliderType = COLOR_BACKGROUND;
                break;
        }

        setSliders();
    });

    /**
     * A menu item has been clicked.
     */
    $("a[class~='pure-menu-link']").on("click", function () {
        var navigation = $(this)[0].dataset.navigation;

        // add/remove active class
        $(".pure-menu-link").set("-active");
        $(this).set("+active");

        if (navigation === "settings") {
            sendData(COMMAND_REQUEST_CONFIG_VALUES, 0, 0);
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
    $("input[name='mode']").on("change", function () {
        var id = $(this).get("id");

        var hasBrightness = false;
        var hasSpeed = false;

        if (id === "mode-wordclock") {
            sliderType = COLOR_FOREGROUND;
            command = COMMAND_MODE_WORD_CLOCK;
        }
        if (id === "mode-color") {
            sliderType = COLOR_FOREGROUND;
            command = COMMAND_MODE_COLOR;
        }
        if (id === "mode-seconds") {
            sliderType = COLOR_FOREGROUND;
            command = COMMAND_MODE_SECONDS;
        }
        if (id === "mode-marquee") {
            hasSpeed = true;
            sliderType = COLOR_FOREGROUND;
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

        sendData(command, 0, 0);
        setSliders();
    });

    $(".quick-color").on("click", function (event) {
        var hexColorString = $(this)[0].dataset.color;
        var rgbColor = hexToRgb(hexColorString);

        rgb[sliderType][0] = rgbColor.red;
        rgb[sliderType][1] = rgbColor.green;
        rgb[sliderType][2] = rgbColor.blue;
        rgb[sliderType][3] = 0;

        hell = $("#slider-brightness").get("value");
        geschw = $("#slider-speed").get("value");
        anzahl = $("#slider-leds").get("value");
        position = $("#slider-position").get("value");

        setSliders();
        sendData(command, 0, 0);
        setSliders();

        return false;
    });

    $("[id*='slider']").onChange(function (event) {
        var id = $(this).get("id");

        if (sleep === 0) {
            getSliders();
            if (id === "slider-red") {
                sendData(command, 1, 0);
            }
            if (id === "slider-green") {
                sendData(command, 1, 0);
            }
            if (id === "slider-blue") {
                sendData(command, 1, 0);
            }
            if (id === "slider-white") {
                sendData(command, 1, 0);
            }
            if (id === "slider-brightness") {
                sendData(COMMAND_BRIGHTNESS, 0, 0);
            }
            if (id === "slider-speed") {
                sendData(COMMAND_SPEED, 0, 0);
            }
            if (id === "slider-leds") {
                sendData(COMMAND_LEDS, 0, 0);
            }
            if (id === "slider-position") {
                sendData(COMMAND_POSITION, 0, 0);
            }
            setSliders();

            sleep = 1;
            sleeptime = 20;

            setTimeout(function () {
                sleep = 0;
            }, sleeptime);
        }
        return false;
    });

    $("#initial-values-button").on("click", function () {
        sendData(COMMAND_SET_INITIAL_VALUES, 0, 0);
    });

    $("#wifi-button").on("click", function () {

        var ssidValue = $("#ssid").get("value");
        var passwordValue = $("#password").get("value");

        // append ssid
        var data = "099000000";
        data += getPaddedString(ssidValue, DATA_SSID_TEXT_LENGTH);

        // append password
        data += getPaddedString(passwordValue, DATA_PASSWORT_TEXT_LENGTH);
        data += "999";

        websocket.send(data);
        debugMessage("WLAN wurde neu konfiguriert", data);
        return false;
    });


    $("#_wlanscan").on("click",function(){
        var data = "302000000";
        websocket.send(data);
        document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";
        return false;
    });

    $("#timeserver-button").on("click", function () {

        var timeserverValue = $("#timeserver").get("value");

        var data = "097000000";
        data += getPaddedString(timeserverValue, DATA_TIMESERVER_TEXT_LENGTH);
        data += "999";

        websocket.send(data);
        debugMessage("Zeitserver wurde neu konfiguriert", data);
        return false;
    });
    $("#marquee-button").on("click", function () {
        var marqueeTextValue = $("#marquee").get("value");

        var data = "096000000";
        data += getPaddedString(marqueeTextValue, DATA_MARQUEE_TEXT_LENGTH);
        data += "999";

        websocket.send(data);
        debugMessage("Lauftext wurde neu konfiguriert", data);
    });
    $("#brightness-button").on("click", function () {

        h6 = $("#brightness-6").get("value");
        h8 = $("#brightness-8").get("value");
        h12 = $("#brightness-12").get("value");
        h16 = $("#brightness-16").get("value");
        h18 = $("#brightness-18").get("value");
        h20 = $("#brightness-20").get("value");
        h22 = $("#brightness-22").get("value");
        h24 = $("#brightness-24").get("value");

        var data = "095000000" + nstr(h6) + nstr(h8) + nstr(h12) + nstr(h16) + nstr(h18) + nstr(h20) + nstr(h22) + nstr(h24) + "999";

        websocket.send(data);
        debugMessage("Helligkeit wurde neu konfiguriert", data);
    });
    $("#weather-button").on("click", function () {

        var apiKey = $("#owm-api-key").get("value");
        var cityId = $("#owm-city-id").get("value");

        var data = "090000000" + cityId + " " + apiKey + "  999";

        websocket.send(data);
        debugMessage("OpenWeatherMap Zugangsdaten wurden konfiguriert", data);
    });
    $("#show-minutesbutton").on("click", function () {
        var showMinutesValue = $("#show-minutes").get("value");
        var data = "094000000" + showMinutesValue + "  999";
        websocket.send(data);
        debugMessage("Minutenanzeige wurde neu konfiguriert", data);
    });
    $("#show-seconds-button").on("click", function () {
        var showSecondsValue = $("#show-seconds").get("value");
        var data = "093000000" + showSecondsValue + "  999";

        websocket.send(data);
        debugMessage("Sekundenanzeige wurde neu konfiguriert", data);
    });
    $("#UhrtypeDef-button").on("click", function() {
        var UhrtypeDef = $("#UhrtypeDef").get("value");
        var data = "089000000" + UhrtypeDef + "  999";
        websocket.send(data);
        debugMessage("UhrtypeDef wurde neu konfiguriert", data);
    });
    $("#colortype-button").on("click", function() {
        colortype = $("#colortype").get("value");
        var data = "088000000" + colortype + "  999";
        websocket.send(data);
        debugMessage("Colortype wurde neu konfiguriert", data);
    });
    $("#host-button").on("click", function () {
        var hostValue = $("#host").get("value");

        var data = "092000000";
        data += getPaddedString(hostValue, DATA_HOST_TEXT_LENGTH);
        data += "999";

        websocket.send(data);
        debugMessage("Hostname wurde neu konfiguriert", data);
    });
    $("#disable-button").on("click", function () {
        sendData(COMMAND_SET_WIFI_DISABLED, 0, 0);
    });
    $("#reset-button").on("click", function () {
        sendData(COMMAND_RESET, 0, 0);
    });
});