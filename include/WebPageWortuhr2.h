
#ifndef _INDEX_H
#define _INDEX_H


const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
	<title>Wortuhr 2</title>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=yes">
	<meta name="apple-mobile-web-app-capable" content="yes">

	<!-- scripts -->
	<script>/^u/.test(typeof define)&&function(d){var c=this.require=function(a){return d[a]};this.define=function(b,a){d[b]=d[b]||a(c)}}({}),define("minified",function(){function an(b){return b!=H?""+b:""}function am(b){return/^str/.test(typeof b)}function al(b){return b&&b.nodeType}function ak(b){return b}function aj(d,c){ae(d,function(b){b(c)})}function ai(e,d){for(var f in e){d(f,e[f])}}function ah(e,d){var f=[];return ae(e,function(b,a){d.call(e,b,a)&&f.push(b)}),f}function ag(f,e,h){var g=[];return f(e,function(b,c){ae(h.call(e,b,c),function(d){g.push(d)})}),g}function af(a,f,e){return an(a).replace(f,e||"")}function ae(e,d){if(ac(e)){for(var f=0;f<e.length;f++){d.call(e,e[f],f)}}else{e!=H&&d(e,0)}return e}function ad(b){return"function"==typeof b&&!b.item}function ac(b){return b&&b.length!=H&&!am(b)&&!al(b)&&!ad(b)&&b!==X}function ab(b){return parseFloat(af(b,/^[^\d-]+/))}function aa(b){return b.Nia=b.Nia||++R}function Z(g,f){var j,i=[],h={};return ae(g,function(b){ae(f(b),function(c){h[j=aa(c)]||(i.push(c),h[j]=!0)})}),i}function Y(f,e){var h={$position:"absolute",$visibility:"hidden",$display:"block",$height:H},g=f.get(h),h=f.set(h).get("clientHeight");return f.set(g),h*e+"px"}function W(b){P?P.push(b):setTimeout(b,0)}function U(b){return ag(ae,b,function(c){return ac(c)?U(c):(al(c)&&(c=c.cloneNode(!0),c.removeAttribute&&c.removeAttribute("id")),c)})}function S(e,d,f){return ad(e)?W(e):new J(Q(e,d,f))}function Q(b,j,h){function g(d){return ac(d)?ag(ae,d,g):d}function c(d){return ah(ag(ae,d,g),function(e){for(;e=e.parentNode;){if(e==j[0]||h){return e==j[0]}}})}return j?1!=(j=Q(j)).length?Z(j,function(a){return Q(b,a,h)}):am(b)?1!=al(j[0])?[]:h?c(j[0].querySelectorAll(b)):j[0].querySelectorAll(b):c(b):am(b)?document.querySelectorAll(b):ag(ae,b,g)}function O(b,n){function m(e,d){var f=RegExp("(^|\\s+)"+e+"(?=$|\\s)","i");return function(a){return e?f.test(a[d]):!0}}var l,k,j={},c=j;return ad(b)?b:/^num/.test(typeof b)?function(a,d){return d==b}:!b||"*"==b||am(b)&&(c=/^([\w-]*)\.?([\w-]*)$/.exec(b))?(l=m(c[1],"tagName"),k=m(c[2],"className"),function(d){return 1==al(d)&&l(d)&&k(d)}):n?function(a){return S(b,n).find(a)!=H}:(S(b).each(function(d){j[aa(d)]=!0}),function(d){return j[aa(d)]})}function M(d){var c=O(d);return function(b){return c(b)?H:!0}}function K(){function f(b,i){function d(){var a=e?b:i;ad(a)?(a=a.apply(I,h))&&a.then?a.then(function(j){c.fire(!0,[j])},function(j){c.fire(!1,[j])}):c.fire(!0,[a]):c.fire(e,h)}var c=K();return e==H?g.push(d):setTimeout(d,0),c}var e,h,g=[];return{fire:function(b,c){e==H&&(e=b,h=c,setTimeout(function(){aj(g)},0))},then:f,error:function(a){return f(0,a)}}}function J(e){for(var d=this.length=e.length,f=0;d>f;f++){this[f]=e[f]}}var I,H=null,X=window,V={},T={},R=1,P=/^[ic]/.test(document.readyState)?H:[],N={},L=0;return ai({each:function(b){return ae(this,b)},filter:function(b){return new J(ah(this,b))},collect:function(b){return new J(ag(ae,this,b))},sub:function(f,e){var h=0>f?this.length+f:f,g=e>=0?e:this.length+(e||0);return new J(ah(this,function(d,c){return c>=h&&g>c}))},find:function(g,f){for(var j,i=ad(g)?g:function(a,d){return g===a?d:void 0},h=f||0;h<this.length;h++){if((j=i.call(this,this[h],h))!=H){return j}}},remove:function(){ae(this,function(b){b.parentNode.removeChild(b)})},text:function(){return ag(ae,this,function(b){return b.textContent}).join("")},trav:function(h,g,l){var k=/^num/.test(typeof g),j=O(k?H:g),i=k?g:l;return new J(Z(this,function(a){for(var d=[];(a=a[h])&&d.length!=i;){j(a)&&d.push(a)}return d}))},next:function(d,c){return this.trav("nextSibling",d,c||1)},up:function(d,c){return this.trav("parentNode",d,c||1)},select:function(d,c){return S(d,this,c)},is:function(b){return !this.find(M(b))},only:function(b){return new J(ah(this,O(b)))},not:function(b){return new J(ah(this,M(b)))},get:function(b,n){var m,l,j,i=this,f=i[0];return f?am(b)?(m=/^(\W*)(.*)/.exec(af(b,/^%/,"@data-")),l=m[1],f=T[l]?T[l](this,m[2]):"$"==b?i.get("className"):"$$"==b?i.get("@style"):"$$slide"==b?i.get("$height"):"$$fade"==b||"$$show"==b?"hidden"==i.get("$visibility")||"none"==i.get("$display")?0:"$$fade"==b?isNaN(i.get("$opacity",!0))?1:i.get("$opacity",!0):1:"$"==l?X.getComputedStyle(f,H).getPropertyValue(af(m[2],/[A-Z]/g,function(c){return"-"+c.toLowerCase()})):"@"==l?f.getAttribute(m[2]):f[m[2]],n?ab(f):f):(j={},(ac(b)?ae:ai)(b,function(c){j[c]=i.get(c,n)}),j):void 0},set:function(b,j){var i,h,f=this;return j!==I?(i=/^(\W*)(.*)/.exec(af(af(b,/^\$float$/,"cssFloat"),/^%/,"@data-")),h=i[1],V[h]?V[h](this,i[2],j):"$$fade"==b?this.set({$visibility:j?"visible":"hidden",$opacity:j}):"$$slide"==b?f.set({$visibility:j?"visible":"hidden",$overflow:"hidden",$height:/px/.test(j)?j:function(e,c,g){return Y(S(g),j)}}):"$$show"==b?j?f.set({$visibility:j?"visible":"hidden",$display:""}).set({$display:function(c){return"none"==c?"block":c}}):f.set({$display:"none"}):"$$"==b?f.set("@style",j):ae(this,function(a,d){var c=ad(j)?j(S(a).get(b),d,a):j;"$"==h?i[2]?a.style[i[2]]=c:ae(c&&c.split(/\s+/),function(g){var m=af(g,/^[+-]/),l=a.className||"",k=af(l,RegExp("(^|\\s+)"+m+"(?=$|\\s)"));(/^\+/.test(g)||m==g&&l==k)&&(k+=" "+m),a.className=af(k,/^\s+/g)}):"$$scrollX"==b?a.scroll(c,S(a).get("$$scrollY")):"$$scrollY"==b?a.scroll(S(a).get("$$scrollX"),c):"@"==h?c==H?a.removeAttribute(i[2]):a.setAttribute(i[2],c):a[i[2]]=c})):am(b)||ad(b)?f.set("$",b):ai(b,function(d,c){f.set(d,c)}),f},show:function(){return this.set("$$show",1)},hide:function(){return this.set("$$show",0)},add:function(d,c){return this.each(function(i,h){function b(e){ac(e)?ae(e,b):ad(e)?b(e(i,h)):e!=H&&(e=al(e)?e:document.createTextNode(e),a?a.parentNode.insertBefore(e,a.nextSibling):c?c(e,i,i.parentNode):i.appendChild(e),a=e)}var a;b(h&&!ad(d)?U(d):d)})},fill:function(b){return this.each(function(c){S(c.childNodes).remove()}).add(b)},addAfter:function(b){return this.add(b,function(e,d,f){f.insertBefore(e,d.nextSibling)})},addBefore:function(b){return this.add(b,function(e,d,f){f.insertBefore(e,d)})},addFront:function(b){return this.add(b,function(d,c){c.insertBefore(d,c.firstChild)})},replace:function(b){return this.add(b,function(e,d,f){f.replaceChild(e,d)})},clone:function(){return new J(U(this))},animate:function(j,f,p){var o,n=K(),k=this,h=ag(ae,this,function(a,m){var l,i=S(a),c={};return ai(l=i.get(j),function(g,d){var b=j[g];c[g]=ad(b)?b(d,m,a):"$$slide"==g?Y(i,b):b}),i.dial(l,c,p)}),e=f||500;return n.stop=function(){return n.fire(!1),o()},o=S.loop(function(b){aj(h,b/e),b>=e&&(o(),n.fire(!0,[k]))}),n},dial:function(a,n,m){function l(d,c){return/^#/.test(d)?parseInt(6<d.length?d.substr(2*c+1,2):(d=d.charAt(c+1))+d,16):ab(d.split(",")[c])}var k=this,i=m||0,f=ad(i)?i:function(e,d,g){return g*(d-e)*(i+(1-i)*g*(3-2*g))+e};return function(b){ai(a,function(c,g){var e=n[c],d=0;k.set(c,0>=b?g:b>=1?e:/^#|rgb\(/.test(e)?"rgb("+Math.round(f(l(g,d),l(e,d++),b))+","+Math.round(f(l(g,d),l(e,d++),b))+","+Math.round(f(l(g,d),l(e,d++),b))+")":af(e,/-?[\d.]+/,an(f(ab(g),ab(e),b))))})}},toggle:function(j,i,p,o){var n,m,l=this,k=!1;return i?(l.set(j),function(a){a!==k&&(m=(k=!0===a||!1===a?a:!k)?i:j,p?(n=l.animate(m,n?n.stop():p,o)).then(function(){n=H}):l.set(m))}):l.toggle(af(j,/\b(?=\w)/g,"-"),af(j,/\b(?=\w)/g,"+"))},values:function(a){var d=a||{};return this.each(function(c){var h=c.name||c.id,g=an(c.value);if(/form/i.test(c.tagName)){for(h=0;h<c.elements.length;h++){S(c.elements[h]).values(d)}}else{!h||/ox|io/i.test(c.type)&&!c.checked||(d[h]=d[h]==H?g:ag(ae,[d[h],g],ak))}}),d},offset:function(){for(var d=this[0],c={x:0,y:0};d;){c.x+=d.offsetLeft,c.y+=d.offsetTop,d=d.offsetParent}return c},on:function(i,h,d,b,a){return ad(h)?this.on(H,i,h,d,b):am(b)?this.on(i,h,d,H,b):this.each(function(c,e){ae(i?Q(i,c):c,function(f){ae(an(h).split(/\s/),function(j){function s(m,t,p){var o,n=!a;if(p=a?p:f,a){for(o=O(a,f);p&&p!=f&&!(n=o(p));){p=p.parentNode}}return !n||g!=m||d.apply(S(p),b||[t,e])&&"?"==r||"|"==r}function l(m){s(g,m,m.target)||(m.preventDefault(),m.stopPropagation())}var g=af(j,/[?|]/g),r=af(j,/[^?|]/g),q=("blur"==g||"focus"==g)&&!!a,k=R++;f.addEventListener(g,l,q),f.M||(f.M={}),f.M[k]=s,d.M=ag(ae,[d.M,function(){f.removeEventListener(g,l,q),delete f.M[k]}],ak)})})})},onOver:function(f,e){var h=this,g=[];return ad(e)?this.on(f,"|mouseover |mouseout",function(b,i){var d=b.relatedTarget||b.toElement,c="mouseout"!=b.type;g[i]===c||!c&&d&&(d==h[i]||S(d).up(h[i]).length)||(g[i]=c,e.call(this,c,b))}):this.onOver(H,f)},onFocus:function(e,d,f){return ad(d)?this.on(e,"|blur",d,[!1],f).on(e,"|focus",d,[!0],f):this.onFocus(H,e,d)},onChange:function(e,d,f){return ad(d)?this.on(e,"|input |change |click",function(b,i){var h=this[0],g=/ox|io/i.test(h.type)?h.checked:h.value;h.NiaP!=g&&d.call(this,h.NiaP=g,i)},f):this.onChange(H,e,d)},onClick:function(f,e,h,g){return ad(e)?this.on(f,"click",e,h,g):this.onClick(H,f,e,h)},trigger:function(d,c){return this.each(function(f){for(var b=!0,a=f;a&&b;){ai(a.M,function(h,g){b=b&&g(d,c,f)}),a=a.parentNode}})}},function(d,c){J.prototype[d]=c}),ai({request:function(r,q,p,o){o=o||{};var n,j=0,h=K(),f=p&&p.constructor==o.constructor;try{h.xhr=n=new XMLHttpRequest,h.stop=function(){n.abort()},f&&(p=ag(ai,p,function(d,c){return ag(ae,c,function(e){return encodeURIComponent(d)+(e!=H?"="+encodeURIComponent(e):"")})}).join("&")),p==H||/post/i.test(r)||(q+="?"+p,p=H),n.open(r,q,!0,o.user,o.pass),f&&/post/i.test(r)&&n.setRequestHeader("Content-Type","application/x-www-form-urlencoded"),ai(o.headers,function(d,c){n.setRequestHeader(d,c)}),ai(o.xhr,function(d,c){n[d]=c}),n.onreadystatechange=function(){4!=n.readyState||j++||(200<=n.status&&300>n.status?h.fire(!0,[n.responseText,n]):h.fire(!1,[n.status,n.responseText,n]))},n.send(p)}catch(a){j||h.fire(!1,[0,H,an(a)])}return h},toJSON:JSON.stringify,parseJSON:JSON.parse,ready:W,loop:function(i){function f(b){ai(N,function(a,d){d(b)}),L&&j(f)}function n(){return N[k]&&(delete N[k],L--),l}var m,l=0,k=R++,j=X.requestAnimationFrame||function(b){setTimeout(function(){b(+new Date)},33)};return N[k]=function(a){m=m||a,i(l=a-m,n)},L++||j(f),n},off:function(b){aj(b.M),b.M=H}},function(d,c){S[d]=c}),document.addEventListener("DOMContentLoaded",function(){aj(P),P=H},!1),{$:S,$$:function(e,d,f){return Q(e,d,f)[0]},EE:function(e,d,f){return e=S(document.createElement(e)),ac(d)||!/^ob/.test(typeof d)?e.add(d):e.set(d).add(f)},M:J,getter:T,setter:V}});</script>
	<script>"use strict";

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
  [0, 0, 100],
  [0, 10, 0],
  [10, 0, 0],
  [5, 5, 5]
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
    [0, 0, 100],
    [0, 10, 0],
    [10, 0, 0],
    [5, 5, 5]
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

    sendData(301, 0, 0);
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

    debugMessage("Die Verbindung mit dem Websocket wurde geschlossen (Code " + event.code + ").", event);
  };

  websocket.onmessage = function(event) {

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
    }
    if (data.command === "set") {
      rgb[0][0] = data.rgb00;
      rgb[0][1] = data.rgb01;
      rgb[0][2] = data.rgb02;
      rgb[1][0] = data.rgb10;
      rgb[1][1] = data.rgb11;
      rgb[1][2] = data.rgb12;
      rgb[2][0] = data.rgb20;
      rgb[2][1] = data.rgb21;
      rgb[2][2] = data.rgb22;
      rgb[3][0] = data.rgb30;
      rgb[3][1] = data.rgb31;
      rgb[3][2] = data.rgb32;
      hell = data.hell;
      geschw = data.geschw;
      setSliders();
    }
  };
  websocket.onerror = function(event) {
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

  // rgb labels
  $("#slider-red-value").fill(rgb[sliderType][0]);
  $("#slider-green-value").fill(rgb[sliderType][1]);
  $("#slider-blue-value").fill(rgb[sliderType][2]);

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
    nstr(rgb[COLOR_BACKGROUND][0]) +
    nstr(rgb[COLOR_BACKGROUND][1]) +
    nstr(rgb[COLOR_BACKGROUND][2]) +
    nstr(rgb[COLOR_FOREGROUND][0]) + // 2 Removed the other colors because the were just confusing as hell
    nstr(rgb[COLOR_FOREGROUND][1]) + // 2
    nstr(rgb[COLOR_FOREGROUND][2]) + // 2
    nstr(rgb[COLOR_FOREGROUND][0]) + // 3
    nstr(rgb[COLOR_FOREGROUND][1]) + // 3
    nstr(rgb[COLOR_FOREGROUND][2]) + // 3
    nstr(hell) +
    nstr(geschw) +
    nstr(anzahl) +
    nstr(position) + "999";

  websocket.send(data);
  debugMessage("Send data: ", data);
}

$.ready(function() {

  initConfigValues();
  setSliders();
  initWebsocket();

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
  $("input[name='color-mode']").on("change", function() {

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
  $("a[class~='pure-menu-link']").on("click", function() {
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
  $("input[name='mode']").on("change", function() {
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

  $(".quick-color").on("click", function(event) {
    var hexColorString = $(this)[0].dataset.color;
    var rgbColor = hexToRgb(hexColorString);

    rgb[sliderType][0] = rgbColor.red;
    rgb[sliderType][1] = rgbColor.green;
    rgb[sliderType][2] = rgbColor.blue;

    hell = $("#slider-brightness").get("value");
    geschw = $("#slider-speed").get("value");
    anzahl = $("#slider-leds").get("value");
    position = $("#slider-position").get("value");

    setSliders();
    sendData(command, 0, 0);
    setSliders();

    return false;
  });

  $("[id*='slider']").onChange(function(event) {
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

      setTimeout(function() {
        sleep = 0;
      }, sleeptime);
    }
    return false;
  });

  $("#initial-values-button").on("click", function() {
    sendData(COMMAND_SET_INITIAL_VALUES, 0, 0);
  });

  $("#wifi-button").on("click", function() {

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
  $("#timeserver-button").on("click", function() {

    var timeserverValue = $("#timeserver").get("value");

    var data = "097000000";
    data += getPaddedString(timeserverValue, DATA_TIMESERVER_TEXT_LENGTH);
    data += "999";

    websocket.send(data);
    debugMessage("Zeitserver wurde neu konfiguriert", data);
    return false;
  });
  $("#marquee-button").on("click", function() {
    var marqueeTextValue = $("#marquee").get("value");

    var data = "096000000";
    data += getPaddedString(marqueeTextValue, DATA_MARQUEE_TEXT_LENGTH);
    data += "999";

    websocket.send(data);
    debugMessage("Lauftext wurde neu konfiguriert", data);
  });
  $("#brightness-button").on("click", function() {

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
  $("#weather-button").on("click", function() {

    var apiKey = $("#owm-api-key").get("value");
    var cityId = $("#owm-city-id").get("value");

    var data = "090000000" + cityId + " " + apiKey + "  999";

    websocket.send(data);
    debugMessage("OpenWeatherMap Zugangsdaten wurden konfiguriert", data);
  });
  $("#show-minutesbutton").on("click", function() {
    var showMinutesValue = $("#show-minutes").get("value");
    var data = "094000000" + showMinutesValue + "  999";
    websocket.send(data);
    debugMessage("Minutenanzeige wurde neu konfiguriert", data);
  });
  $("#show-seconds-button").on("click", function() {
    var showSecondsValue = $("#show-seconds").get("value");
    var data = "093000000" + showSecondsValue + "  999";

    websocket.send(data);
    debugMessage("Sekundenanzeige wurde neu konfiguriert", data);
  });
  $("#host-button").on("click", function() {
    var hostValue = $("#host").get("value");

    var data = "092000000";
    data += getPaddedString(hostValue, DATA_HOST_TEXT_LENGTH);
    data += "999";

    websocket.send(data);
    debugMessage("Hostname wurde neu konfiguriert", data);
  });
  $("#reset-button").on("click", function() {
    sendData(COMMAND_RESET, 0, 0);
  });
});
</script><style>/*!
Pure v1.0.0
Copyright 2013 Yahoo!
Licensed under the BSD License.
https://github.com/yahoo/pure/blob/master/LICENSE.md
*/
/*!
normalize.css v^3.0 | MIT License | git.io/normalize
Copyright (c) Nicolas Gallagher and Jonathan Neal
*/
/*! normalize.css v3.0.3 | MIT License | github.com/necolas/normalize.css */.pure-button:focus,a:active,a:hover{outline:0}.pure-table,table{border-collapse:collapse;border-spacing:0}html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin:0}article,aside,details,figcaption,figure,footer,header,hgroup,main,menu,nav,section,summary{display:block}audio,canvas,progress,video{display:inline-block;vertical-align:baseline}audio:not([controls]){display:none;height:0}[hidden],template{display:none}a{background-color:transparent}abbr[title]{border-bottom:1px dotted}b,optgroup,strong{font-weight:700}dfn{font-style:italic}h1{font-size:2em;margin:.67em 0}mark{background:#ff0;color:#000}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sup{top:-.5em}sub{bottom:-.25em}img{border:0}svg:not(:root){overflow:hidden}figure{margin:1em 40px}hr{box-sizing:content-box;height:0}pre,textarea{overflow:auto}code,kbd,pre,samp{font-family:monospace,monospace;font-size:1em}button,input,optgroup,select,textarea{color:inherit;font:inherit;margin:0}.pure-button,input{line-height:normal}button{overflow:visible}button,select{text-transform:none}button,html input[type=button],input[type=reset],input[type=submit]{-webkit-appearance:button;cursor:pointer}button[disabled],html input[disabled]{cursor:default}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}input[type=checkbox],input[type=radio]{box-sizing:border-box;padding:0}input[type=number]::-webkit-inner-spin-button,input[type=number]::-webkit-outer-spin-button{height:auto}input[type=search]{-webkit-appearance:textfield;box-sizing:content-box}.pure-button,.pure-form input:not([type]),.pure-menu{box-sizing:border-box}input[type=search]::-webkit-search-cancel-button,input[type=search]::-webkit-search-decoration{-webkit-appearance:none}fieldset{border:1px solid silver;margin:0 2px;padding:.35em .625em .75em}legend,td,th{padding:0}legend{border:0}.hidden,[hidden]{display:none!important}.pure-img{max-width:100%;height:auto;display:block}.pure-g{letter-spacing:-.31em;text-rendering:optimizespeed;font-family:FreeSans,Arimo,"Droid Sans",Helvetica,Arial,sans-serif;display:-webkit-box;display:-webkit-flex;display:-ms-flexbox;display:flex;-webkit-flex-flow:row wrap;-ms-flex-flow:row wrap;flex-flow:row wrap;-webkit-align-content:flex-start;-ms-flex-line-pack:start;align-content:flex-start}@media all and (-ms-high-contrast:none),(-ms-high-contrast:active){table .pure-g{display:block}}.opera-only :-o-prefocus,.pure-g{word-spacing:-.43em}.pure-u,.pure-u-1,.pure-u-1-1,.pure-u-1-12,.pure-u-1-2,.pure-u-1-24,.pure-u-1-3,.pure-u-1-4,.pure-u-1-5,.pure-u-1-6,.pure-u-1-8,.pure-u-10-24,.pure-u-11-12,.pure-u-11-24,.pure-u-12-24,.pure-u-13-24,.pure-u-14-24,.pure-u-15-24,.pure-u-16-24,.pure-u-17-24,.pure-u-18-24,.pure-u-19-24,.pure-u-2-24,.pure-u-2-3,.pure-u-2-5,.pure-u-20-24,.pure-u-21-24,.pure-u-22-24,.pure-u-23-24,.pure-u-24-24,.pure-u-3-24,.pure-u-3-4,.pure-u-3-5,.pure-u-3-8,.pure-u-4-24,.pure-u-4-5,.pure-u-5-12,.pure-u-5-24,.pure-u-5-5,.pure-u-5-6,.pure-u-5-8,.pure-u-6-24,.pure-u-7-12,.pure-u-7-24,.pure-u-7-8,.pure-u-8-24,.pure-u-9-24{letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto;display:inline-block;zoom:1}.pure-g [class*=pure-u]{font-family:sans-serif}.pure-u-1-24{width:4.1667%}.pure-u-1-12,.pure-u-2-24{width:8.3333%}.pure-u-1-8,.pure-u-3-24{width:12.5%}.pure-u-1-6,.pure-u-4-24{width:16.6667%}.pure-u-1-5{width:20%}.pure-u-5-24{width:20.8333%}.pure-u-1-4,.pure-u-6-24{width:25%}.pure-u-7-24{width:29.1667%}.pure-u-1-3,.pure-u-8-24{width:33.3333%}.pure-u-3-8,.pure-u-9-24{width:37.5%}.pure-u-2-5{width:40%}.pure-u-10-24,.pure-u-5-12{width:41.6667%}.pure-u-11-24{width:45.8333%}.pure-u-1-2,.pure-u-12-24{width:50%}.pure-u-13-24{width:54.1667%}.pure-u-14-24,.pure-u-7-12{width:58.3333%}.pure-u-3-5{width:60%}.pure-u-15-24,.pure-u-5-8{width:62.5%}.pure-u-16-24,.pure-u-2-3{width:66.6667%}.pure-u-17-24{width:70.8333%}.pure-u-18-24,.pure-u-3-4{width:75%}.pure-u-19-24{width:79.1667%}.pure-u-4-5{width:80%}.pure-u-20-24,.pure-u-5-6{width:83.3333%}.pure-u-21-24,.pure-u-7-8{width:87.5%}.pure-u-11-12,.pure-u-22-24{width:91.6667%}.pure-u-23-24{width:95.8333%}.pure-u-1,.pure-u-1-1,.pure-u-24-24,.pure-u-5-5{width:100%}.pure-button{display:inline-block;zoom:1;white-space:nowrap;vertical-align:middle;text-align:center;cursor:pointer;-webkit-user-drag:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.pure-button::-moz-focus-inner{padding:0;border:0}.pure-button-group{letter-spacing:-.31em;text-rendering:optimizespeed}.opera-only :-o-prefocus,.pure-button-group{word-spacing:-.43em}.pure-button{font-family:inherit;font-size:100%;padding:.5em 1em;color:#444;color:rgba(0,0,0,.8);border:1px solid #999;border:transparent;background-color:#E6E6E6;text-decoration:none;border-radius:2px}.pure-button-hover,.pure-button:focus,.pure-button:hover{filter:alpha(opacity=90);background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1))}.pure-button-active,.pure-button:active{box-shadow:0 0 0 1px rgba(0,0,0,.15) inset,0 0 6px rgba(0,0,0,.2) inset;border-color:#000\9}.pure-button-disabled,.pure-button-disabled:active,.pure-button-disabled:focus,.pure-button-disabled:hover,.pure-button[disabled]{border:none;background-image:none;filter:alpha(opacity=40);opacity:.4;cursor:not-allowed;box-shadow:none;pointer-events:none}.pure-button-hidden{display:none}.pure-button-primary,.pure-button-selected,a.pure-button-primary,a.pure-button-selected{background-color:#0078e7;color:#fff}.pure-button-group .pure-button{letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto;margin:0;border-radius:0;border-right:1px solid #111;border-right:1px solid rgba(0,0,0,.2)}.pure-button-group .pure-button:first-child{border-top-left-radius:2px;border-bottom-left-radius:2px}.pure-button-group .pure-button:last-child{border-top-right-radius:2px;border-bottom-right-radius:2px;border-right:none}.pure-form input[type=password],.pure-form input[type=email],.pure-form input[type=url],.pure-form input[type=date],.pure-form input[type=month],.pure-form input[type=time],.pure-form input[type=datetime],.pure-form input[type=datetime-local],.pure-form input[type=week],.pure-form input[type=tel],.pure-form input[type=color],.pure-form input[type=number],.pure-form input[type=search],.pure-form input[type=text],.pure-form select,.pure-form textarea{padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;vertical-align:middle;box-sizing:border-box}.pure-form input:not([type]){padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px}.pure-form input[type=color]{padding:.2em .5em}.pure-form input:not([type]):focus,.pure-form input[type=password]:focus,.pure-form input[type=email]:focus,.pure-form input[type=url]:focus,.pure-form input[type=date]:focus,.pure-form input[type=month]:focus,.pure-form input[type=time]:focus,.pure-form input[type=datetime]:focus,.pure-form input[type=datetime-local]:focus,.pure-form input[type=week]:focus,.pure-form input[type=tel]:focus,.pure-form input[type=color]:focus,.pure-form input[type=number]:focus,.pure-form input[type=search]:focus,.pure-form input[type=text]:focus,.pure-form select:focus,.pure-form textarea:focus{outline:0;border-color:#129FEA}.pure-form input[type=file]:focus,.pure-form input[type=checkbox]:focus,.pure-form input[type=radio]:focus{outline:#129FEA auto 1px}.pure-form .pure-checkbox,.pure-form .pure-radio{margin:.5em 0;display:block}.pure-form input:not([type])[disabled],.pure-form input[type=password][disabled],.pure-form input[type=email][disabled],.pure-form input[type=url][disabled],.pure-form input[type=date][disabled],.pure-form input[type=month][disabled],.pure-form input[type=time][disabled],.pure-form input[type=datetime][disabled],.pure-form input[type=datetime-local][disabled],.pure-form input[type=week][disabled],.pure-form input[type=tel][disabled],.pure-form input[type=color][disabled],.pure-form input[type=number][disabled],.pure-form input[type=search][disabled],.pure-form input[type=text][disabled],.pure-form select[disabled],.pure-form textarea[disabled]{cursor:not-allowed;background-color:#eaeded;color:#cad2d3}.pure-form input[readonly],.pure-form select[readonly],.pure-form textarea[readonly]{background-color:#eee;color:#777;border-color:#ccc}.pure-form input:focus:invalid,.pure-form select:focus:invalid,.pure-form textarea:focus:invalid{color:#b94a48;border-color:#e9322d}.pure-form input[type=file]:focus:invalid:focus,.pure-form input[type=checkbox]:focus:invalid:focus,.pure-form input[type=radio]:focus:invalid:focus{outline-color:#e9322d}.pure-form select{height:2.25em;border:1px solid #ccc;background-color:#fff}.pure-form select[multiple]{height:auto}.pure-form label{margin:.5em 0 .2em}.pure-form fieldset{margin:0;padding:.35em 0 .75em;border:0}.pure-form legend{display:block;width:100%;padding:.3em 0;margin-bottom:.3em;color:#333;border-bottom:1px solid #e5e5e5}.pure-form-stacked input:not([type]),.pure-form-stacked input[type=password],.pure-form-stacked input[type=email],.pure-form-stacked input[type=url],.pure-form-stacked input[type=date],.pure-form-stacked input[type=month],.pure-form-stacked input[type=time],.pure-form-stacked input[type=datetime],.pure-form-stacked input[type=datetime-local],.pure-form-stacked input[type=week],.pure-form-stacked input[type=tel],.pure-form-stacked input[type=color],.pure-form-stacked input[type=file],.pure-form-stacked input[type=number],.pure-form-stacked input[type=search],.pure-form-stacked input[type=text],.pure-form-stacked label,.pure-form-stacked select,.pure-form-stacked textarea{display:block;margin:.25em 0}.pure-form-aligned .pure-help-inline,.pure-form-aligned input,.pure-form-aligned select,.pure-form-aligned textarea,.pure-form-message-inline{display:inline-block;vertical-align:middle}.pure-form-aligned textarea{vertical-align:top}.pure-form-aligned .pure-control-group{margin-bottom:.5em}.pure-form-aligned .pure-control-group label{text-align:right;display:inline-block;vertical-align:middle;width:10em;margin:0 1em 0 0}.pure-form-aligned .pure-controls{margin:1.5em 0 0 11em}.pure-form .pure-input-rounded,.pure-form input.pure-input-rounded{border-radius:2em;padding:.5em 1em}.pure-form .pure-group fieldset{margin-bottom:10px}.pure-form .pure-group input,.pure-form .pure-group textarea{display:block;padding:10px;margin:0 0 -1px;border-radius:0;position:relative;top:-1px}.pure-form .pure-group input:focus,.pure-form .pure-group textarea:focus{z-index:3}.pure-form .pure-group input:first-child,.pure-form .pure-group textarea:first-child{top:1px;border-radius:4px 4px 0 0;margin:0}.pure-form .pure-group input:first-child:last-child,.pure-form .pure-group textarea:first-child:last-child{top:1px;border-radius:4px;margin:0}.pure-form .pure-group input:last-child,.pure-form .pure-group textarea:last-child{top:-2px;border-radius:0 0 4px 4px;margin:0}.pure-form .pure-group button{margin:.35em 0}.pure-form .pure-input-1{width:100%}.pure-form .pure-input-3-4{width:75%}.pure-form .pure-input-2-3{width:66%}.pure-form .pure-input-1-2{width:50%}.pure-form .pure-input-1-3{width:33%}.pure-form .pure-input-1-4{width:25%}.pure-form .pure-help-inline,.pure-form-message-inline{display:inline-block;padding-left:.3em;color:#666;vertical-align:middle;font-size:.875em}.pure-form-message{display:block;color:#666;font-size:.875em}@media only screen and (max-width :480px){.pure-form button[type=submit]{margin:.7em 0 0}.pure-form input:not([type]),.pure-form input[type=password],.pure-form input[type=email],.pure-form input[type=url],.pure-form input[type=date],.pure-form input[type=month],.pure-form input[type=time],.pure-form input[type=datetime],.pure-form input[type=datetime-local],.pure-form input[type=week],.pure-form input[type=tel],.pure-form input[type=color],.pure-form input[type=number],.pure-form input[type=search],.pure-form input[type=text],.pure-form label{margin-bottom:.3em;display:block}.pure-group input:not([type]),.pure-group input[type=password],.pure-group input[type=email],.pure-group input[type=url],.pure-group input[type=date],.pure-group input[type=month],.pure-group input[type=time],.pure-group input[type=datetime],.pure-group input[type=datetime-local],.pure-group input[type=week],.pure-group input[type=tel],.pure-group input[type=color],.pure-group input[type=number],.pure-group input[type=search],.pure-group input[type=text]{margin-bottom:0}.pure-form-aligned .pure-control-group label{margin-bottom:.3em;text-align:left;display:block;width:100%}.pure-form-aligned .pure-controls{margin:1.5em 0 0}.pure-form .pure-help-inline,.pure-form-message,.pure-form-message-inline{display:block;font-size:.75em;padding:.2em 0 .8em}}.pure-menu-fixed{position:fixed;left:0;top:0;z-index:3}.pure-menu-item,.pure-menu-list{position:relative}.pure-menu-list{list-style:none;margin:0;padding:0}.pure-menu-item{padding:0;margin:0;height:100%}.pure-menu-heading,.pure-menu-link{display:block;text-decoration:none;white-space:nowrap}.pure-menu-horizontal{width:100%;white-space:nowrap}.pure-menu-horizontal .pure-menu-list{display:inline-block}.pure-menu-horizontal .pure-menu-heading,.pure-menu-horizontal .pure-menu-item,.pure-menu-horizontal .pure-menu-separator{display:inline-block;zoom:1;vertical-align:middle}.pure-menu-item .pure-menu-item{display:block}.pure-menu-children{display:none;position:absolute;left:100%;top:0;margin:0;padding:0;z-index:3}.pure-menu-horizontal .pure-menu-children{left:0;top:auto;width:inherit}.pure-menu-active>.pure-menu-children,.pure-menu-allow-hover:hover>.pure-menu-children{display:block;position:absolute}.pure-menu-has-children>.pure-menu-link:after{padding-left:.5em;content:"\25B8";font-size:small}.pure-menu-horizontal .pure-menu-has-children>.pure-menu-link:after{content:"\25BE"}.pure-menu-scrollable{overflow-y:scroll;overflow-x:hidden}.pure-menu-scrollable .pure-menu-list{display:block}.pure-menu-horizontal.pure-menu-scrollable .pure-menu-list{display:inline-block}.pure-menu-horizontal.pure-menu-scrollable{white-space:nowrap;overflow-y:hidden;overflow-x:auto;-ms-overflow-style:none;-webkit-overflow-scrolling:touch;padding:.5em 0}.pure-menu-horizontal.pure-menu-scrollable::-webkit-scrollbar{display:none}.pure-menu-horizontal .pure-menu-children .pure-menu-separator,.pure-menu-separator{background-color:#ccc;height:1px;margin:.3em 0}.pure-menu-horizontal .pure-menu-separator{width:1px;height:1.3em;margin:0 .3em}.pure-menu-horizontal .pure-menu-children .pure-menu-separator{display:block;width:auto}.pure-menu-heading{text-transform:uppercase;color:#565d64}.pure-menu-link{color:#777}.pure-menu-children{background-color:#fff}.pure-menu-disabled,.pure-menu-heading,.pure-menu-link{padding:.5em 1em}.pure-menu-disabled{opacity:.5}.pure-menu-disabled .pure-menu-link:hover{background-color:transparent}.pure-menu-active>.pure-menu-link,.pure-menu-link:focus,.pure-menu-link:hover{background-color:#eee}.pure-menu-selected .pure-menu-link,.pure-menu-selected .pure-menu-link:visited{color:#000}.pure-table{empty-cells:show;border:1px solid #cbcbcb}.pure-table caption{color:#000;font:italic 85%/1 arial,sans-serif;padding:1em 0;text-align:center}.pure-table td,.pure-table th{border-left:1px solid #cbcbcb;border-width:0 0 0 1px;font-size:inherit;margin:0;overflow:visible;padding:.5em 1em}.pure-table td:first-child,.pure-table th:first-child{border-left-width:0}.pure-table thead{background-color:#e0e0e0;color:#000;text-align:left;vertical-align:bottom}.pure-table td{background-color:transparent}.pure-table-odd td,.pure-table-striped tr:nth-child(2n-1) td{background-color:#f2f2f2}.pure-table-bordered td{border-bottom:1px solid #cbcbcb}.pure-table-bordered tbody>tr:last-child>td{border-bottom-width:0}.pure-table-horizontal td,.pure-table-horizontal th{border-width:0 0 1px;border-bottom:1px solid #cbcbcb}.pure-table-horizontal tbody>tr:last-child>td{border-bottom-width:0}
/*!
Pure v1.0.0
Copyright 2013 Yahoo!
Licensed under the BSD License.
https://github.com/yahoo/pure/blob/master/LICENSE.md
*/
@media screen and (min-width:35.5em){.pure-u-sm-1,.pure-u-sm-1-1,.pure-u-sm-1-12,.pure-u-sm-1-2,.pure-u-sm-1-24,.pure-u-sm-1-3,.pure-u-sm-1-4,.pure-u-sm-1-5,.pure-u-sm-1-6,.pure-u-sm-1-8,.pure-u-sm-10-24,.pure-u-sm-11-12,.pure-u-sm-11-24,.pure-u-sm-12-24,.pure-u-sm-13-24,.pure-u-sm-14-24,.pure-u-sm-15-24,.pure-u-sm-16-24,.pure-u-sm-17-24,.pure-u-sm-18-24,.pure-u-sm-19-24,.pure-u-sm-2-24,.pure-u-sm-2-3,.pure-u-sm-2-5,.pure-u-sm-20-24,.pure-u-sm-21-24,.pure-u-sm-22-24,.pure-u-sm-23-24,.pure-u-sm-24-24,.pure-u-sm-3-24,.pure-u-sm-3-4,.pure-u-sm-3-5,.pure-u-sm-3-8,.pure-u-sm-4-24,.pure-u-sm-4-5,.pure-u-sm-5-12,.pure-u-sm-5-24,.pure-u-sm-5-5,.pure-u-sm-5-6,.pure-u-sm-5-8,.pure-u-sm-6-24,.pure-u-sm-7-12,.pure-u-sm-7-24,.pure-u-sm-7-8,.pure-u-sm-8-24,.pure-u-sm-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-sm-1-24{width:4.1667%}.pure-u-sm-1-12,.pure-u-sm-2-24{width:8.3333%}.pure-u-sm-1-8,.pure-u-sm-3-24{width:12.5%}.pure-u-sm-1-6,.pure-u-sm-4-24{width:16.6667%}.pure-u-sm-1-5{width:20%}.pure-u-sm-5-24{width:20.8333%}.pure-u-sm-1-4,.pure-u-sm-6-24{width:25%}.pure-u-sm-7-24{width:29.1667%}.pure-u-sm-1-3,.pure-u-sm-8-24{width:33.3333%}.pure-u-sm-3-8,.pure-u-sm-9-24{width:37.5%}.pure-u-sm-2-5{width:40%}.pure-u-sm-10-24,.pure-u-sm-5-12{width:41.6667%}.pure-u-sm-11-24{width:45.8333%}.pure-u-sm-1-2,.pure-u-sm-12-24{width:50%}.pure-u-sm-13-24{width:54.1667%}.pure-u-sm-14-24,.pure-u-sm-7-12{width:58.3333%}.pure-u-sm-3-5{width:60%}.pure-u-sm-15-24,.pure-u-sm-5-8{width:62.5%}.pure-u-sm-16-24,.pure-u-sm-2-3{width:66.6667%}.pure-u-sm-17-24{width:70.8333%}.pure-u-sm-18-24,.pure-u-sm-3-4{width:75%}.pure-u-sm-19-24{width:79.1667%}.pure-u-sm-4-5{width:80%}.pure-u-sm-20-24,.pure-u-sm-5-6{width:83.3333%}.pure-u-sm-21-24,.pure-u-sm-7-8{width:87.5%}.pure-u-sm-11-12,.pure-u-sm-22-24{width:91.6667%}.pure-u-sm-23-24{width:95.8333%}.pure-u-sm-1,.pure-u-sm-1-1,.pure-u-sm-24-24,.pure-u-sm-5-5{width:100%}}@media screen and (min-width:48em){.pure-u-md-1,.pure-u-md-1-1,.pure-u-md-1-12,.pure-u-md-1-2,.pure-u-md-1-24,.pure-u-md-1-3,.pure-u-md-1-4,.pure-u-md-1-5,.pure-u-md-1-6,.pure-u-md-1-8,.pure-u-md-10-24,.pure-u-md-11-12,.pure-u-md-11-24,.pure-u-md-12-24,.pure-u-md-13-24,.pure-u-md-14-24,.pure-u-md-15-24,.pure-u-md-16-24,.pure-u-md-17-24,.pure-u-md-18-24,.pure-u-md-19-24,.pure-u-md-2-24,.pure-u-md-2-3,.pure-u-md-2-5,.pure-u-md-20-24,.pure-u-md-21-24,.pure-u-md-22-24,.pure-u-md-23-24,.pure-u-md-24-24,.pure-u-md-3-24,.pure-u-md-3-4,.pure-u-md-3-5,.pure-u-md-3-8,.pure-u-md-4-24,.pure-u-md-4-5,.pure-u-md-5-12,.pure-u-md-5-24,.pure-u-md-5-5,.pure-u-md-5-6,.pure-u-md-5-8,.pure-u-md-6-24,.pure-u-md-7-12,.pure-u-md-7-24,.pure-u-md-7-8,.pure-u-md-8-24,.pure-u-md-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-md-1-24{width:4.1667%}.pure-u-md-1-12,.pure-u-md-2-24{width:8.3333%}.pure-u-md-1-8,.pure-u-md-3-24{width:12.5%}.pure-u-md-1-6,.pure-u-md-4-24{width:16.6667%}.pure-u-md-1-5{width:20%}.pure-u-md-5-24{width:20.8333%}.pure-u-md-1-4,.pure-u-md-6-24{width:25%}.pure-u-md-7-24{width:29.1667%}.pure-u-md-1-3,.pure-u-md-8-24{width:33.3333%}.pure-u-md-3-8,.pure-u-md-9-24{width:37.5%}.pure-u-md-2-5{width:40%}.pure-u-md-10-24,.pure-u-md-5-12{width:41.6667%}.pure-u-md-11-24{width:45.8333%}.pure-u-md-1-2,.pure-u-md-12-24{width:50%}.pure-u-md-13-24{width:54.1667%}.pure-u-md-14-24,.pure-u-md-7-12{width:58.3333%}.pure-u-md-3-5{width:60%}.pure-u-md-15-24,.pure-u-md-5-8{width:62.5%}.pure-u-md-16-24,.pure-u-md-2-3{width:66.6667%}.pure-u-md-17-24{width:70.8333%}.pure-u-md-18-24,.pure-u-md-3-4{width:75%}.pure-u-md-19-24{width:79.1667%}.pure-u-md-4-5{width:80%}.pure-u-md-20-24,.pure-u-md-5-6{width:83.3333%}.pure-u-md-21-24,.pure-u-md-7-8{width:87.5%}.pure-u-md-11-12,.pure-u-md-22-24{width:91.6667%}.pure-u-md-23-24{width:95.8333%}.pure-u-md-1,.pure-u-md-1-1,.pure-u-md-24-24,.pure-u-md-5-5{width:100%}}@media screen and (min-width:64em){.pure-u-lg-1,.pure-u-lg-1-1,.pure-u-lg-1-12,.pure-u-lg-1-2,.pure-u-lg-1-24,.pure-u-lg-1-3,.pure-u-lg-1-4,.pure-u-lg-1-5,.pure-u-lg-1-6,.pure-u-lg-1-8,.pure-u-lg-10-24,.pure-u-lg-11-12,.pure-u-lg-11-24,.pure-u-lg-12-24,.pure-u-lg-13-24,.pure-u-lg-14-24,.pure-u-lg-15-24,.pure-u-lg-16-24,.pure-u-lg-17-24,.pure-u-lg-18-24,.pure-u-lg-19-24,.pure-u-lg-2-24,.pure-u-lg-2-3,.pure-u-lg-2-5,.pure-u-lg-20-24,.pure-u-lg-21-24,.pure-u-lg-22-24,.pure-u-lg-23-24,.pure-u-lg-24-24,.pure-u-lg-3-24,.pure-u-lg-3-4,.pure-u-lg-3-5,.pure-u-lg-3-8,.pure-u-lg-4-24,.pure-u-lg-4-5,.pure-u-lg-5-12,.pure-u-lg-5-24,.pure-u-lg-5-5,.pure-u-lg-5-6,.pure-u-lg-5-8,.pure-u-lg-6-24,.pure-u-lg-7-12,.pure-u-lg-7-24,.pure-u-lg-7-8,.pure-u-lg-8-24,.pure-u-lg-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-lg-1-24{width:4.1667%}.pure-u-lg-1-12,.pure-u-lg-2-24{width:8.3333%}.pure-u-lg-1-8,.pure-u-lg-3-24{width:12.5%}.pure-u-lg-1-6,.pure-u-lg-4-24{width:16.6667%}.pure-u-lg-1-5{width:20%}.pure-u-lg-5-24{width:20.8333%}.pure-u-lg-1-4,.pure-u-lg-6-24{width:25%}.pure-u-lg-7-24{width:29.1667%}.pure-u-lg-1-3,.pure-u-lg-8-24{width:33.3333%}.pure-u-lg-3-8,.pure-u-lg-9-24{width:37.5%}.pure-u-lg-2-5{width:40%}.pure-u-lg-10-24,.pure-u-lg-5-12{width:41.6667%}.pure-u-lg-11-24{width:45.8333%}.pure-u-lg-1-2,.pure-u-lg-12-24{width:50%}.pure-u-lg-13-24{width:54.1667%}.pure-u-lg-14-24,.pure-u-lg-7-12{width:58.3333%}.pure-u-lg-3-5{width:60%}.pure-u-lg-15-24,.pure-u-lg-5-8{width:62.5%}.pure-u-lg-16-24,.pure-u-lg-2-3{width:66.6667%}.pure-u-lg-17-24{width:70.8333%}.pure-u-lg-18-24,.pure-u-lg-3-4{width:75%}.pure-u-lg-19-24{width:79.1667%}.pure-u-lg-4-5{width:80%}.pure-u-lg-20-24,.pure-u-lg-5-6{width:83.3333%}.pure-u-lg-21-24,.pure-u-lg-7-8{width:87.5%}.pure-u-lg-11-12,.pure-u-lg-22-24{width:91.6667%}.pure-u-lg-23-24{width:95.8333%}.pure-u-lg-1,.pure-u-lg-1-1,.pure-u-lg-24-24,.pure-u-lg-5-5{width:100%}}@media screen and (min-width:80em){.pure-u-xl-1,.pure-u-xl-1-1,.pure-u-xl-1-12,.pure-u-xl-1-2,.pure-u-xl-1-24,.pure-u-xl-1-3,.pure-u-xl-1-4,.pure-u-xl-1-5,.pure-u-xl-1-6,.pure-u-xl-1-8,.pure-u-xl-10-24,.pure-u-xl-11-12,.pure-u-xl-11-24,.pure-u-xl-12-24,.pure-u-xl-13-24,.pure-u-xl-14-24,.pure-u-xl-15-24,.pure-u-xl-16-24,.pure-u-xl-17-24,.pure-u-xl-18-24,.pure-u-xl-19-24,.pure-u-xl-2-24,.pure-u-xl-2-3,.pure-u-xl-2-5,.pure-u-xl-20-24,.pure-u-xl-21-24,.pure-u-xl-22-24,.pure-u-xl-23-24,.pure-u-xl-24-24,.pure-u-xl-3-24,.pure-u-xl-3-4,.pure-u-xl-3-5,.pure-u-xl-3-8,.pure-u-xl-4-24,.pure-u-xl-4-5,.pure-u-xl-5-12,.pure-u-xl-5-24,.pure-u-xl-5-5,.pure-u-xl-5-6,.pure-u-xl-5-8,.pure-u-xl-6-24,.pure-u-xl-7-12,.pure-u-xl-7-24,.pure-u-xl-7-8,.pure-u-xl-8-24,.pure-u-xl-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-xl-1-24{width:4.1667%}.pure-u-xl-1-12,.pure-u-xl-2-24{width:8.3333%}.pure-u-xl-1-8,.pure-u-xl-3-24{width:12.5%}.pure-u-xl-1-6,.pure-u-xl-4-24{width:16.6667%}.pure-u-xl-1-5{width:20%}.pure-u-xl-5-24{width:20.8333%}.pure-u-xl-1-4,.pure-u-xl-6-24{width:25%}.pure-u-xl-7-24{width:29.1667%}.pure-u-xl-1-3,.pure-u-xl-8-24{width:33.3333%}.pure-u-xl-3-8,.pure-u-xl-9-24{width:37.5%}.pure-u-xl-2-5{width:40%}.pure-u-xl-10-24,.pure-u-xl-5-12{width:41.6667%}.pure-u-xl-11-24{width:45.8333%}.pure-u-xl-1-2,.pure-u-xl-12-24{width:50%}.pure-u-xl-13-24{width:54.1667%}.pure-u-xl-14-24,.pure-u-xl-7-12{width:58.3333%}.pure-u-xl-3-5{width:60%}.pure-u-xl-15-24,.pure-u-xl-5-8{width:62.5%}.pure-u-xl-16-24,.pure-u-xl-2-3{width:66.6667%}.pure-u-xl-17-24{width:70.8333%}.pure-u-xl-18-24,.pure-u-xl-3-4{width:75%}.pure-u-xl-19-24{width:79.1667%}.pure-u-xl-4-5{width:80%}.pure-u-xl-20-24,.pure-u-xl-5-6{width:83.3333%}.pure-u-xl-21-24,.pure-u-xl-7-8{width:87.5%}.pure-u-xl-11-12,.pure-u-xl-22-24{width:91.6667%}.pure-u-xl-23-24{width:95.8333%}.pure-u-xl-1,.pure-u-xl-1-1,.pure-u-xl-24-24,.pure-u-xl-5-5{width:100%}}</style><style>body{background:#f7f7f7;font-size:16px}.content{padding:20px;max-width:1200px;margin:auto}.content>.pure-g{margin-left:-10px;margin-right:-10px}p{margin-bottom:15px;margin-top:0}a{text-decoration:none}h1,h2,h3,h4,h5,h6{color:rgba(0,0,0,.7)}h1{font-size:1.4em;margin-top:0;margin-bottom:8px}h2{font-size:1.2em;color:rgba(0,0,0,.6)}.main-menu{background:#5c5c5c;box-shadow:inset 0 -10px 7px -10px rgba(0,0,0,.8);overflow-x:scroll;overflow-y:hidden}.main-menu .content{padding:0}.pure-menu-item{height:auto}.main-menu .pure-menu-link{padding:1.2em;padding-bottom:1em;padding-top:1em;color:#fff}.pure-menu-link:hover{background:#898989;color:#fff}.pure-menu-link.active{background:#f7f7f7;color:#5c5c5c}.color-area{height:40px;width:100%;background:gray;margin-bottom:15px;margin-top:10px;box-shadow:inset 0 0 0 1px rgba(0,0,0,.3)}.button-xl{font-size:150%}.pure-g.colors>div{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.pure-g.colors>div{padding:.3em}.pure-g.colors button{width:100%;height:40px;box-shadow:inset 0 0 0 1px rgba(0,0,0,.3)}label.pure-radio{padding:5px;display:inline-block}label.pure-radio input[type=radio]{margin-top:-14px;display:inline-block}input[type=range],input[type=range]:-moz-focusring,input[type=range]::-moz-focus-inner,input[type=range]::-moz-focus-outer,input[type=range]:active,input[type=range]:focus{border:0;outline:0}input[type=range]{width:100%;-webkit-appearance:none;-moz-appearance:none;margin-top:8px;margin-bottom:8px;height:20px}input[type=range]::-webkit-slider-runnable-track{height:6px;cursor:pointer;background:#ddd;border:none;border-radius:100px;box-shadow:inset 0 1px 1px 0 rgba(0,0,0,.3);-webkit-appearance:none;-moz-appearance:none}input[type=range]::-webkit-slider-thumb{height:28px;width:28px;margin-top:-11px;border-radius:200px;background:linear-gradient(to bottom,rgba(255,255,255,1) 0,rgba(235,235,235,1) 100%);box-shadow:0 1px 1px rgba(0,0,0,.3),inset 0 0 0 1px rgba(255,255,255,.9);border:1px solid rgba(0,0,0,.3);cursor:pointer;-webkit-appearance:none;-moz-appearance:none}input[type=range]::-moz-range-track{height:6px;background:#ddd;border:none;border-radius:100px;box-shadow:inset 0 1px 1px 0 rgba(0,0,0,.3);-webkit-appearance:none;-moz-appearance:none}input[type=range]::-moz-range-thumb{height:28px;width:28px;border-radius:200px;background:linear-gradient(to bottom,rgba(255,255,255,1) 0,rgba(235,235,235,1) 100%);box-shadow:0 1px 1px rgba(0,0,0,.3),inset 0 0 0 1px rgba(255,255,255,.9);border:1px solid rgba(0,0,0,.3);cursor:pointer;-webkit-appearance:none;-moz-appearance:none}.slider-label{margin-top:11px;margin-bottom:4px;display:inline-block;width:100%}.slider-label .value{float:right;margin-right:15px}.section{display:none}.section-main{display:block}.box-debug{overflow:hidden;overflow-wrap:break-word}.box-debug>div{font-family:monospace}.brightness,.functions-settings,.leds,.position,.speed{display:none}.section-settings .box,.section-settings .pure-u-1,.section-settings>div{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.section-settings .pure-u-1{padding-bottom:20px}.section-settings .box{height:100%}.status.online{color:green}.status.offline{color:red}.box{padding:20px;margin:10px;box-shadow:0 1px 1px 0 rgba(60,64,67,.08),0 1px 3px 1px rgba(60,64,67,.16);background:#fff;border-radius:3px;color:#5f6368}.box h2{margin-top:-5px;border-bottom:1px solid #f0f0f0;padding-bottom:10px;margin-left:-20px;margin-right:-20px;padding-right:15px;padding-left:15px;font-weight:400;font-size:1.1em}</style>
</head>
<body>

	<div class="pure-g">
		<div class="pure-u-24-24">
			<div class="pure-menu pure-menu-horizontal main-menu">
				<div class="content">
					<a href="#" class="pure-menu-heading pure-menu-link active" data-navigation="main">Wortuhr 2</a><ul class="pure-menu-list"><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="functions">Funktionen</a></li><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="settings">Einstellungen</a></li><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="about">Ueber</a></li>
					</ul>
				</div>
			</div>
		</div>
	</div>

	<div class="content">
		<div class="pure-g">
			<div class="pure-u-24-24 section-connection-lost" id="section-connection-lost">
				<div class="box">
					<button class="pure-button status-button">Verbinden</button>
					<p>Die Verbindung zur Wortuhr wurde unterbrochen.</p>
				</div>
			</div>

			<div class="pure-u-24-24 section section-main">
				<div class="box">
					<h2>Farbart</h2>
					<label for="fg" class="pure-radio">
						<input id="fg" type="radio" name="color-mode" value="foreground" checked> Vordergrund
					</label>
					<label for="bg" class="pure-radio">
						<input id="bg" type="radio" name="color-mode" value="background"> Hintergrund
					</label>
					<!--<label for="bo" class="pure-radio">
						<input id="bo" type="radio" name="color-mode" value="border"> Rahmen
					</label>
					<label for="un" class="pure-radio">
						<input id="un" type="radio" name="color-mode" value="unknown"> Effekt
					</label>-->
				</div>

				<div class="box">
					<h2>Farbmischer</h2>
					<div class="color-area" id="color-area"></div>

					<div class="pure-g color-r">
						<div class="pure-u-1 pure-u-md-4-24"><label class="slider-label" for="slider-red">Red <span class="value" id="slider-red-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-5-6"><input id="slider-red" type="range" min="0" max="255" value="0"></div>
					</div>
					<div class="pure-g color-g">
						<div class="pure-u-1 pure-u-md-4-24"><label class="slider-label" for="slider-green">Green <span class="value" id="slider-green-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-5-6"><input id="slider-green" type="range" min="0" max="255" value="0"></div>
					</div>
					<div class="pure-g color-b">
						<div class="pure-u-1 pure-u-md-4-24"><label class="slider-label" for="slider-blue">Blue <span class="value" id="slider-blue-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-5-6"><input id="slider-blue" type="range" min="0" max="255" value="0"></div>
					</div>
				</div>

				<div class="box">
					<h2>Schnellfarben</h2>
					<div class="pure-g colors">
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#F44336" style="background: #F44336"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FF0000" style="background: #FF0000"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#E91E63" style="background: #E91E63"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FF00FF" style="background: #FF00FF"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#9C27B0" style="background: #9C27B0"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#673AB7" style="background: #673AB7"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#3F51B5" style="background: #3F51B5"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#0000FF" style="background: #0000FF"></button>
						</div>

						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#2196F3" style="background: #2196F3"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#03A9F4" style="background: #03A9F4"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#00FFFF" style="background: #00FFFF"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#00BCD4" style="background: #00BCD4"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#009688" style="background: #009688"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#4CAF50" style="background: #4CAF50"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#00FF00" style="background: #00FF00"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#8BC34A" style="background: #8BC34A"></button>
						</div>

						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#CDDC39" style="background: #CDDC39"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FFEB3B" style="background: #FFEB3B"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FFFF00" style="background: #FFFF00"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FFC107" style="background: #FFC107"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FF9800" style="background: #FF9800"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FF5722" style="background: #FF5722"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#FFFFFF" style="background: #FFFFFF"></button>
						</div>
						<div class="pure-u-1-3 pure-u-sm-1-8">
							<button class="quick-color pure-button" data-color="#000000" style="background: #000000"></button>
						</div>
					</div>
				</div>

			</div>
			<div class="pure-u-24-24 section section-functions">

				<div class="box">
					<h2>Funktion</h2>

					<label for="mode-wordclock" class="pure-radio">
						<input id="mode-wordclock" type="radio" name="mode" value="color" checked> Wort Uhr
					</label>
					<label for="mode-color" class="pure-radio">
						<input id="mode-color" type="radio" name="mode" value="color"> Farbe
					</label>
					<label for="mode-seconds" class="pure-radio">
						<input id="mode-seconds" type="radio" name="mode" value="seconds"> Sekunden
					</label>
					<label for="mode-marquee" class="pure-radio">
						<input id="mode-marquee" type="radio" name="mode" value="marquee"> Laufschrift
					</label>
					<label for="mode-rainbow" class="pure-radio">
						<input id="mode-rainbow" type="radio" name="mode" value="rainbow"> Regenbogen
					</label>
					<label for="mode-change" class="pure-radio">
						<input id="mode-change" type="radio" name="mode" value="change"> Farbwechsel
					</label>
				</div>

				<div class="box functions-settings">
					<h2>Einstellung</h2>

					<div class="pure-g brightness">
						<div class="pure-u-1 pure-u-md-1-4"><label class="slider-label" for="slider-brightness">Helligkeit <span class="value" id="slider-brightness-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-3-4"><input id="slider-brightness" type="range" min="1" max="10" value="1"></div>
					</div>
					<div class="pure-g speed">
						<div class="pure-u-1 pure-u-md-1-4"><label class="slider-label" for="slider-speed">Geschwindigkeit <span class="value" id="slider-speed-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-3-4"><input id="slider-speed" type="range" min="0" max="40" value="0"></div>
					</div>
					<div class="pure-g leds">
						<div class="pure-u-1 pure-u-md-1-4"><label class="slider-label" for="slider-leds">Anzahl LEDs <span class="value" id="slider-leds-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-3-4"><input id="slider-leds" type="range" min="5" max="100" value="100"></div>
					</div>
					<div class="pure-g position">
						<div class="pure-u-1 pure-u-md-1-4"><label class="slider-label" for="slider-position">Positon <span class="value" id="slider-position-value">0</span></label></div>
						<div class="pure-u-1 pure-u-md-3-4"><input id="slider-position" type="range" min="1" max="100" value="100"></div>
					</div>
				</div>

			</div>
			<div class="pure-u-24-24 section section-settings">
				<div class="pure-g">

					<div class="pure-u-1 pure-u-md-1-2">
						<div class="box">
							<h2>Status</h2>
							<form class="pure-form pure-form-aligned">
								<fieldset>
									<div class="pure-control-group">
										<label for="status">Verbindungsstatus</label><input id="status" class="status" value="Loading ...">
									</div>
									<div class="pure-controls">
										<button class="pure-button">Verbinden</button>
									</div>
								</fieldset>
							</form>
						</div>
					</div>

					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Startwerte</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-controls">
									<button id="initial-values-button" class="pure-button">Startwerte speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>WLAN</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="ssid">SSID</label><input id="ssid" type="text" placeholder="SSID">
								</div>
								<div class="pure-control-group">
									<label for="password">Passwort</label><input id="password" type="password" placeholder="Passwort">
								</div>
								<div class="pure-controls">
									<button id="wifi-button" class="pure-button">WLAN speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Zeitserver</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="timeserver">Zeitserver</label><input id="timeserver" type="text" placeholder="Zeitserver">
								</div>
								<div class="pure-controls">
									<button id="timeserver-button" class="pure-button">Zeitserver speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Lauftext</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="marquee">Lauftext</label><input id="marquee" type="text" placeholder="Lauftext">
								</div>
								<div class="pure-controls">
									<button id="marquee-button" class="pure-button">Lauftext speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Helligkeit</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="brightness-6">6 Uhr</label><select name="brightness-6" id="brightness-6" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-8">8 Uhr</label><select name="brightness-8" id="brightness-8" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-12">12 Uhr</label><select name="brightness-12" id="brightness-12" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-16">16 Uhr</label><select name="brightness-16" id="brightness-16" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-18">18 Uhr</label><select name="brightness-18" id="brightness-18" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-20">20 Uhr</label><select name="brightness-20" id="brightness-20" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-22">22 Uhr</label><select name="brightness-22" id="brightness-22" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-control-group">
									<label for="brightness-24">24 Uhr</label><select name="brightness-24" id="brightness-24" size="1">
										<option value="100">100</option>
										<option value="80">80</option>
										<option value="60">60</option>
										<option value="40">40</option>
										<option value="20">20</option>
										<option value="0">Aus</option>
									</select>
								</div>
								<div class="pure-controls">
									<button id="brightness-button" class="pure-button">Helligkeit speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Hostname</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="hostname">Hostname</label><input id="hostname" type="text" placeholder="Hostname">
								</div>
								<div class="pure-controls">
									<button id="hostname-button" class="pure-button">Hostname speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Wetter</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="owm-api-key">OpenWeatherMap API-Schluessel</label><input id="owm-api-key" type="text" minlength="32" maxlength="32" placeholder="API-Schlüssel">
								</div>
								<div class="pure-control-group">
									<label for="owm-city-id">OpenWeatherMap City ID</label><input id="owm-city-id" type="text" minlength="7" maxlength="7" placeholder="City ID">
								</div>
								<div class="pure-controls">
									<button id="weather-button" class="pure-button">OpenWeatherMap Daten speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Minuten-Anzeige</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="show-minutes">Minuten anzeigen?</label><select name="show-minutes" id="show-minutes" size="1">
										<option value="0" selected>Nein</option>
										<option value="1">als Zeile</option>
										<option value="2">in den Ecken</option>
									</select>
								</div>
								<div class="pure-controls">
									<button id="show-minutes-button" class="pure-button">Minuten-Anzeige speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Sekunden-Anzeige</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="show-seconds">Sekunden anzeigen?</label><select name="show-seconds" id="show-seconds" size="1">
										<option value="0" selected>Nein</option>
										<option value="1">Ja</option>
									</select>
								</div>
								<div class="pure-controls">
									<button id="show-seconds-button" class="pure-button">Sekunden-Anzeige speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
						<h2>Zuruecksetzen</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-controls">
									<button id="reset-button" class="pure-button">Zuruecksetzen</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
				</div>

			</div>

			<div class="pure-u-24-24 section section-about">
				<div class="box">
					<h2>Ueber</h2>
					<p>Die Wortuhr 2 basiert auf der Wortuhr von <a href="https://www.ulrichradig.de/home/index.php/projekte/wort-uhr-neu">Ulrich Radig</a> und wird entwickelt von <a href="https://github.com/Eisbaeeer">Lars Weimar</a>, <a href="https://github.com/Wandmalfarbe">Pascal Wagler</a>, <a href="https://github.com/dbambus">dbambus</a>, <a href="https://github.com/Flo455">Flo455</a>, <a href="https://github.com/Elektron79">Elektron79</a> und <a href="https://github.com/masju1">masju1</a>.</p>
					<p>This project is open source licensed under the BSD 3-Clause License. Please see the LICENSE file for more information.</p>
				</div>

				<div class="box">
					<h2>Enthaltene Bibliotheken und Software</h2>

					<div class="pure-g">
						<div class="pure-u-1 pure-u-md-1-3"><p>Ulrich Radig Wort Uhr (Version 1)</p></div>
						<div class="pure-u-1 pure-u-md-2-3"><p><a href="https://www.ulrichradig.de/home/index.php/projekte/wort-uhr-neu">https://www.ulrichradig.de/home/index.php/projekte/wort-uhr-neu</a></p></div>
						<div class="pure-u-1 pure-u-md-1-3"><p>Pure.css</p></div>
						<div class="pure-u-1 pure-u-md-2-3"><p><a href="https://purecss.io">https://purecss.io</a></p></div>
						<div class="pure-u-1 pure-u-md-1-3"><p>Minified.js</p></div>
						<div class="pure-u-1 pure-u-md-2-3"><p><a href="http://minifiedjs.com">http://minifiedjs.com</a></p></div>
					</div>
				</div>

				<div class="box box-debug">
					<h2>Debug-Ausgabe</h2>
					<div id="output"></div>
				</div>
			</div>
		</div> <!--/pure-g-->
	</div> <!--/content-->
</body>
</html>
)=====";


#endif // #define _INDEX_H
