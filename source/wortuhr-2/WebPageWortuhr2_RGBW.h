
#ifndef _INDEX_H
#define _INDEX_H

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Uhr</title>
<meta charset=utf-8>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=0">
<meta name="apple-mobile-web-app-capable" content="yes" />
<meta http-equiv="Cache-control" content="private", x-gzip-ok="", max-age=31536000>

<!-- BEGINN Variablenuebergabe -->
<script>/^u/.test(typeof define)&&function(d){var c=this.require=function(a){return d[a]};
this.define=function(b,a){d[b]=d[b]||a(c)}}({}),define("minified",function(){function an(b){return b!=H?""+b:""}function am(b){return/^str/.test(typeof b)}function al(b){return b&&b.nodeType}function ak(b){return b}function aj(d,c){ae(d,function(b){b(c)})}function ai(e,d){for(var f in e){d(f,e[f])}}function ah(e,d){var f=[];
return ae(e,function(b,a){d.call(e,b,a)&&f.push(b)}),f}function ag(f,e,h){var g=[];
return f(e,function(b,c){ae(h.call(e,b,c),function(d){g.push(d)})}),g}function af(a,f,e){return an(a).replace(f,e||"")}function ae(e,d){if(ac(e)){for(var f=0;f<e.length;f++){d.call(e,e[f],f)}}else{e!=H&&d(e,0)}return e}function ad(b){return"function"==typeof b&&!b.item}function ac(b){return b&&b.length!=H&&!am(b)&&!al(b)&&!ad(b)&&b!==X}function ab(b){return parseFloat(af(b,/^[^\d-]+/))}function aa(b){return b.Nia=b.Nia||++R}function Z(g,f){var j,i=[],h={};
return ae(g,function(b){ae(f(b),function(c){h[j=aa(c)]||(i.push(c),h[j]=!0)})}),i}function Y(f,e){var h={$position:"absolute",$visibility:"hidden",$display:"block",$height:H},g=f.get(h),h=f.set(h).get("clientHeight");
return f.set(g),h*e+"px"}function W(b){P?P.push(b):setTimeout(b,0)}function U(b){return ag(ae,b,function(c){return ac(c)?U(c):(al(c)&&(c=c.cloneNode(!0),c.removeAttribute&&c.removeAttribute("id")),c)})}function S(e,d,f){return ad(e)?W(e):new J(Q(e,d,f))}function Q(b,j,h){function g(d){return ac(d)?ag(ae,d,g):d}function c(d){return ah(ag(ae,d,g),function(e){for(;e=e.parentNode;){if(e==j[0]||h){return e==j[0]}}})}return j?1!=(j=Q(j)).length?Z(j,function(a){return Q(b,a,h)}):am(b)?1!=al(j[0])?[]:h?c(j[0].querySelectorAll(b)):j[0].querySelectorAll(b):c(b):am(b)?document.querySelectorAll(b):ag(ae,b,g)}function O(b,n){function m(e,d){var f=RegExp("(^|\\s+)"+e+"(?=$|\\s)","i");
return function(a){return e?f.test(a[d]):!0}}var l,k,j={},c=j;
return ad(b)?b:/^num/.test(typeof b)?function(a,d){return d==b}:!b||"*"==b||am(b)&&(c=/^([\w-]*)\.?([\w-]*)$/.exec(b))?(l=m(c[1],"tagName"),k=m(c[2],"className"),function(d){return 1==al(d)&&l(d)&&k(d)}):n?function(a){return S(b,n).find(a)!=H}:(S(b).each(function(d){j[aa(d)]=!0}),function(d){return j[aa(d)]})}function M(d){var c=O(d);
return function(b){return c(b)?H:!0}}function K(){function f(b,i){function d(){var a=e?b:i;ad(a)?(a=a.apply(I,h))&&a.then?a.then(function(j){c.fire(!0,[j])},function(j){c.fire(!1,[j])}):c.fire(!0,[a]):c.fire(e,h)}var c=K();
return e==H?g.push(d):setTimeout(d,0),c}var e,h,g=[];
return{fire:function(b,c){e==H&&(e=b,h=c,setTimeout(function(){aj(g)},0))},then:f,error:function(a){return f(0,a)}}}function J(e){for(var d=this.length=e.length,f=0;d>f;f++){this[f]=e[f]}}var I,H=null,X=window,V={},T={},R=1,P=/^[ic]/.test(document.readyState)?H:[],N={},L=0;
return ai({each:function(b){return ae(this,b)},filter:function(b){return new J(ah(this,b))},collect:function(b){return new J(ag(ae,this,b))},sub:function(f,e){var h=0>f?this.length+f:f,g=e>=0?e:this.length+(e||0);
return new J(ah(this,function(d,c){return c>=h&&g>c}))},find:function(g,f){for(var j,i=ad(g)?g:function(a,d){return g===a?d:void 0},h=f||0;h<this.length;h++){if((j=i.call(this,this[h],h))!=H){return j}}},remove:function(){ae(this,function(b){b.parentNode.removeChild(b)})},text:function(){return ag(ae,this,function(b){return b.textContent}).join("")},trav:function(h,g,l){var k=/^num/.test(typeof g),j=O(k?H:g),i=k?g:l;
return new J(Z(this,function(a){for(var d=[];(a=a[h])&&d.length!=i;){j(a)&&d.push(a)}return d}))},next:function(d,c){return this.trav("nextSibling",d,c||1)},up:function(d,c){return this.trav("parentNode",d,c||1)},select:function(d,c){return S(d,this,c)},is:function(b){return !this.find(M(b))},only:function(b){return new J(ah(this,O(b)))},not:function(b){return new J(ah(this,M(b)))},get:function(b,n){var m,l,j,i=this,f=i[0];
return f?am(b)?(m=/^(\W*)(.*)/.exec(af(b,/^%/,"@data-")),l=m[1],f=T[l]?T[l](this,m[2]):"$"==b?i.get("className"):"$$"==b?i.get("@style"):"$$slide"==b?i.get("$height"):"$$fade"==b||"$$show"==b?"hidden"==i.get("$visibility")||"none"==i.get("$display")?0:"$$fade"==b?isNaN(i.get("$opacity",!0))?1:i.get("$opacity",!0):1:"$"==l?X.getComputedStyle(f,H).getPropertyValue(af(m[2],/[A-Z]/g,function(c){return"-"+c.toLowerCase()})):"@"==l?f.getAttribute(m[2]):f[m[2]],n?ab(f):f):(j={},(ac(b)?ae:ai)(b,function(c){j[c]=i.get(c,n)}),j):void 0},set:function(b,j){var i,h,f=this;
return j!==I?(i=/^(\W*)(.*)/.exec(af(af(b,/^\$float$/,"cssFloat"),/^%/,"@data-")),h=i[1],V[h]?V[h](this,i[2],j):"$$fade"==b?this.set({$visibility:j?"visible":"hidden",$opacity:j}):"$$slide"==b?f.set({$visibility:j?"visible":"hidden",$overflow:"hidden",$height:/px/.test(j)?j:function(e,c,g){return Y(S(g),j)}}):"$$show"==b?j?f.set({$visibility:j?"visible":"hidden",$display:""}).set({$display:function(c){return"none"==c?"block":c}}):f.set({$display:"none"}):"$$"==b?f.set("@style",j):ae(this,function(a,d){var c=ad(j)?j(S(a).get(b),d,a):j;"$"==h?i[2]?a.style[i[2]]=c:ae(c&&c.split(/\s+/),function(g){var m=af(g,/^[+-]/),l=a.className||"",k=af(l,RegExp("(^|\\s+)"+m+"(?=$|\\s)"));(/^\+/.test(g)||m==g&&l==k)&&(k+=" "+m),a.className=af(k,/^\s+/g)}):"$$scrollX"==b?a.scroll(c,S(a).get("$$scrollY")):"$$scrollY"==b?a.scroll(S(a).get("$$scrollX"),c):"@"==h?c==H?a.removeAttribute(i[2]):a.setAttribute(i[2],c):a[i[2]]=c})):am(b)||ad(b)?f.set("$",b):ai(b,function(d,c){f.set(d,c)}),f},show:function(){return this.set("$$show",1)},hide:function(){return this.set("$$show",0)},add:function(d,c){return this.each(function(i,h){function b(e){ac(e)?ae(e,b):ad(e)?b(e(i,h)):e!=H&&(e=al(e)?e:document.createTextNode(e),a?a.parentNode.insertBefore(e,a.nextSibling):c?c(e,i,i.parentNode):i.appendChild(e),a=e)}var a;b(h&&!ad(d)?U(d):d)})},fill:function(b){return this.each(function(c){S(c.childNodes).remove()}).add(b)},addAfter:function(b){return this.add(b,function(e,d,f){f.insertBefore(e,d.nextSibling)})},addBefore:function(b){return this.add(b,function(e,d,f){f.insertBefore(e,d)})},addFront:function(b){return this.add(b,function(d,c){c.insertBefore(d,c.firstChild)})},replace:function(b){return this.add(b,function(e,d,f){f.replaceChild(e,d)})},clone:function(){return new J(U(this))},animate:function(j,f,p){var o,n=K(),k=this,h=ag(ae,this,function(a,m){var l,i=S(a),c={};
return ai(l=i.get(j),function(g,d){var b=j[g];c[g]=ad(b)?b(d,m,a):"$$slide"==g?Y(i,b):b}),i.dial(l,c,p)}),e=f||500;
return n.stop=function(){return n.fire(!1),o()},o=S.loop(function(b){aj(h,b/e),b>=e&&(o(),n.fire(!0,[k]))}),n},dial:function(a,n,m){function l(d,c){return/^#/.test(d)?parseInt(6<d.length?d.substr(2*c+1,2):(d=d.charAt(c+1))+d,16):ab(d.split(",")[c])}var k=this,i=m||0,f=ad(i)?i:function(e,d,g){return g*(d-e)*(i+(1-i)*g*(3-2*g))+e};
return function(b){ai(a,function(c,g){var e=n[c],d=0;k.set(c,0>=b?g:b>=1?e:/^#|rgb\(/.test(e)?"rgb("+Math.round(f(l(g,d),l(e,d++),b))+","+Math.round(f(l(g,d),l(e,d++),b))+","+Math.round(f(l(g,d),l(e,d++),b))+")":af(e,/-?[\d.]+/,an(f(ab(g),ab(e),b))))})}},toggle:function(j,i,p,o){var n,m,l=this,k=!1;
return i?(l.set(j),function(a){a!==k&&(m=(k=!0===a||!1===a?a:!k)?i:j,p?(n=l.animate(m,n?n.stop():p,o)).then(function(){n=H}):l.set(m))}):l.toggle(af(j,/\b(?=\w)/g,"-"),af(j,/\b(?=\w)/g,"+"))},values:function(a){var d=a||{};
return this.each(function(c){var h=c.name||c.id,g=an(c.value);if(/form/i.test(c.tagName)){for(h=0;h<c.elements.length;h++){S(c.elements[h]).values(d)}}else{!h||/ox|io/i.test(c.type)&&!c.checked||(d[h]=d[h]==H?g:ag(ae,[d[h],g],ak))}}),d},offset:function(){for(var d=this[0],c={x:0,y:0};d;){c.x+=d.offsetLeft,c.y+=d.offsetTop,d=d.offsetParent}return c},on:function(i,h,d,b,a){return ad(h)?this.on(H,i,h,d,b):am(b)?this.on(i,h,d,H,b):this.each(function(c,e){ae(i?Q(i,c):c,function(f){ae(an(h).split(/\s/),function(j){function s(m,t,p){var o,n=!a;if(p=a?p:f,a){for(o=O(a,f);p&&p!=f&&!(n=o(p));){p=p.parentNode}}return !n||g!=m||d.apply(S(p),b||[t,e])&&"?"==r||"|"==r}function l(m){s(g,m,m.target)||(m.preventDefault(),m.stopPropagation())}var g=af(j,/[?|]/g),r=af(j,/[^?|]/g),q=("blur"==g||"focus"==g)&&!!a,k=R++;f.addEventListener(g,l,q),f.M||(f.M={}),f.M[k]=s,d.M=ag(ae,[d.M,function(){f.removeEventListener(g,l,q),delete f.M[k]}],ak)})})})},onOver:function(f,e){var h=this,g=[];
return ad(e)?this.on(f,"|mouseover |mouseout",function(b,i){var d=b.relatedTarget||b.toElement,c="mouseout"!=b.type;g[i]===c||!c&&d&&(d==h[i]||S(d).up(h[i]).length)||(g[i]=c,e.call(this,c,b))}):this.onOver(H,f)},onFocus:function(e,d,f){return ad(d)?this.on(e,"|blur",d,[!1],f).on(e,"|focus",d,[!0],f):this.onFocus(H,e,d)},onChange:function(e,d,f){return ad(d)?this.on(e,"|input |change |click",function(b,i){var h=this[0],g=/ox|io/i.test(h.type)?h.checked:h.value;h.NiaP!=g&&d.call(this,h.NiaP=g,i)},f):this.onChange(H,e,d)},onClick:function(f,e,h,g){return ad(e)?this.on(f,"click",e,h,g):this.onClick(H,f,e,h)},trigger:function(d,c){return this.each(function(f){for(var b=!0,a=f;a&&b;){ai(a.M,function(h,g){b=b&&g(d,c,f)}),a=a.parentNode}})}},function(d,c){J.prototype[d]=c}),ai({request:function(r,q,p,o){o=o||{};var n,j=0,h=K(),f=p&&p.constructor==o.constructor;try{h.xhr=n=new XMLHttpRequest,h.stop=function(){n.abort()},f&&(p=ag(ai,p,function(d,c){return ag(ae,c,function(e){return encodeURIComponent(d)+(e!=H?"="+encodeURIComponent(e):"")})}).join("&")),p==H||/post/i.test(r)||(q+="?"+p,p=H),n.open(r,q,!0,o.user,o.pass),f&&/post/i.test(r)&&n.setRequestHeader("Content-Type","application/x-www-form-urlencoded"),ai(o.headers,function(d,c){n.setRequestHeader(d,c)}),ai(o.xhr,function(d,c){n[d]=c}),n.onreadystatechange=function(){4!=n.readyState||j++||(200<=n.status&&300>n.status?h.fire(!0,[n.responseText,n]):h.fire(!1,[n.status,n.responseText,n]))},n.send(p)}catch(a){j||h.fire(!1,[0,H,an(a)])}return h},toJSON:JSON.stringify,parseJSON:JSON.parse,ready:W,loop:function(i){function f(b){ai(N,function(a,d){d(b)}),L&&j(f)}function n(){return N[k]&&(delete N[k],L--),l}var m,l=0,k=R++,j=X.requestAnimationFrame||function(b){setTimeout(function(){b(+new Date)},33)};
return N[k]=function(a){m=m||a,i(l=a-m,n)},L++||j(f),n},off:function(b){aj(b.M),b.M=H}},function(d,c){S[d]=c}),document.addEventListener("DOMContentLoaded",function(){aj(P),P=H},!1),{$:S,$$:function(e,d,f){return Q(e,d,f)[0]},EE:function(e,d,f){return e=S(document.createElement(e)),ac(d)||!/^ob/.test(typeof d)?e.add(d):e.set(d).add(f)},M:J,getter:T,setter:V}});</script>
<script>var MINI=require("minified");var _=MINI._,$=MINI.$,$$=MINI.$$,EE=MINI.EE,HTML=MINI.HTML;var ip_esp="ws://192.168.4.1";

var debug=false; <!-- DEBUG AUSGABE -->
var command=1;
var rgb=[[0,0,100, 0],[0,10,0, 0],[10,0,0, 0],[5,5,5, 0]];
var sl_typ=0;
var hell=2;
var geschw=10;
var anzahl=100;
var sleep=0;
var sleeptime=1;
var position=100;
var anzahl=100;
var color=0;
var h8=100;
var h12=100;
var h16=100;
var h18=100;
var h20=100;
var h22=100;
var h24=100;
var zeige_sek=0;
var zeige_min=0;
var ldr=0;
var ldrCal=0;
var apikey=0;
var cityid=0;

function init_websocket(){var a=ip_esp;websocket=new WebSocket(a);

websocket.onopen=function(b){$("#_websocket").fill("Online ");
$("#_websocket").set("$backgroundColor","#4DED4D");
$("#_websocket").set("$color","#000000");
$("#_websocket").set("@value","1");
send_data(301,0,0)};

websocket.onclose=function(b){$("#_websocket").fill("Offline ");
$("#_websocket").set("$backgroundColor","#ED4D4D");
$("#_websocket").set("$color","#ffffff");
$("#_websocket").set("@value","0")};

websocket.onmessage=function(c){if(debug==true){$("#output").fill("RESPONSE: "+c.data)}var b=JSON.parse(c.data);
if(b.command=="config"){
$("#_ssid").set("value",b.ssid);
$("#_mm").set("value",b.minute);
$("#_ss").set("value",b.sekunde);
$("#_zs").set("value",b.zeitserver);
$("#_hn").set("value",b.hostname);
$("#_lt").set("value",b.ltext);
$("#_h6").set("value",b.h6);
$("#_h8").set("value",b.h8);
$("#_h12").set("value",b.h12);
$("#_h16").set("value",b.h16);
$("#_h18").set("value",b.h18);
$("#_h20").set("value",b.h20);
$("#_h22").set("value",b.h22);
$("#_h24").set("value",b.h24);
$("#_rg6").set("value",b.hell);
$("#_rg7").set("value",b.geschw);
$("#_zeige_sek").set("value",b.zeige_sek);
$("#_zeige_min").set("value",b.zeige_min);
$("#_ldr").set("value",b.ldr);
$("#_ldrCal").set("value",b.ldrCal);
$("#_cityid").set("value",b.cityid);
$("#_apikey").set("value",b.apikey)}

if(b.command=="set"){
rgb[0][0]=b.rgb00;
rgb[0][1]=b.rgb01;
rgb[0][2]=b.rgb02;
rgb[0][3]=b.rgb03;
rgb[1][0]=b.rgb10;
rgb[1][1]=b.rgb11;
rgb[1][2]=b.rgb12;
rgb[1][3]=b.rgb13;
rgb[2][0]=b.rgb20;
rgb[2][1]=b.rgb21;
rgb[2][2]=b.rgb22;
rgb[2][3]=b.rgb23;
rgb[3][0]=b.rgb30;
rgb[3][1]=b.rgb31;
rgb[3][2]=b.rgb32;
rgb[3][3]=b.rgb33;
hell=b.hell;
geschw=b.geschw;
set_slider()}
                                
if(b.command=="wlan"){document.getElementById("wlanlist").innerHTML = b.list}};

websocket.onerror=function(b){if(debug==true){$("#output").fill("ERROR: "+b.data)}}}function clock(){var a=new Date();
var c=a.getHours();
var f=a.getMinutes();
var b=a.getSeconds();
var e=a.getTimezoneOffset();
if(c<10){c="0"+c}if(f<10){f="0"+f}if(b<10){b="0"+b}var d=c+":"+f+":"+b;
$("#_clock").fill(d);setTimeout("clock()",1000)}clock();
function toArray(a){return Array.prototype.slice.call(a)}setTimeout(function(){window.scrollTo(0,1)},1);
function wait(a){var c=new Date().getTime();
for(var b=0;b<10000000;b++){if((new Date().getTime()-c)>a){break}}}function get_slider(){rgb[sl_typ][0]=$("#_rg1").get("value");
rgb[sl_typ][1]=$("#_rg2").get("value");
rgb[sl_typ][2]=$("#_rg3").get("value");
rgb[sl_typ][3]=$("#_rg4").get("value");
hell=$("#_rg6").get("value");
geschw=$("#_rg7").get("value");
anzahl=$("#_rg9").get("value");
position=$("#_rg91").get("value")}function set_slider(){$("#_rg1").set("value",rgb[sl_typ][0]);
$("#_rg2").set("value",rgb[sl_typ][1]);
$("#_rg3").set("value",rgb[sl_typ][2]);
$("#_rg4").set("value",rgb[sl_typ][3]);
$("#_rg6").set("value",hell);
$("#_rg7").set("value",geschw);
$("#_rg9").set("value",anzahl);
$("#_rg91").set("value",position);
$("#rg1wert").fill(rgb[sl_typ][0]);
$("#rg2wert").fill(rgb[sl_typ][1]);
$("#rg3wert").fill(rgb[sl_typ][2]);
$("#rg4wert").fill(rgb[sl_typ][3])}function nstr(a){if(a<10){a="00"+a}else{if(a<100){a="0"+a}}return a}function send_data(d,c,b){var a=nstr(d)+nstr(c)+nstr(b)+nstr(rgb[0][0])+nstr(rgb[0][1])+nstr(rgb[0][2])+nstr(rgb[0][3])+nstr(rgb[1][0])+nstr(rgb[1][1])+nstr(rgb[1][2])+nstr(rgb[1][3])+nstr(rgb[2][0])+nstr(rgb[2][1])+nstr(rgb[2][2])+nstr(rgb[2][3])+nstr(rgb[3][0])+nstr(rgb[3][1])+nstr(rgb[3][2])+nstr(rgb[3][3])+nstr(hell)+nstr(geschw)+nstr(anzahl)+nstr(position)+"999";
websocket.send(a);if(debug==true){$("#output").fill(a)}}function wlanlists(a){$("#_ssid").set("value", a.innerText||a.textContent);
document.getElementById("_passwd").focus();}$.ready(function(){var a=location.host;
if(a!=0){ip_esp="ws://"+a}set_slider();
init_websocket();
$("#_websocket").on("click",function(){var b=$(this).get("value");
if(b=="1"){b=0;$("#_websocket").set("$backgroundColor","#EDED19");
$("#_websocket").set("$color","#000000");
$("#_websocket").fill("..working..");
$("#_websocket").set("value",b);
websocket.close()}else{b=1;
$("#_websocket").set("$backgroundColor","#EDED19");
$("#_websocket").set("$color","#000000");
$("#_websocket").fill("..working..");
$("#_websocket").set("value",b);
init_websocket()}return false});
$("#_clock").on("click",function(){var b=new Date();
var e=b.getHours();
var j=b.getMinutes();
var d=b.getSeconds();
var h=b.getTimezoneOffset();
var g=b.getTime()/1000;
h=h/60*-1;
var f="030000000"+nstr(h)+g;
for(var c=0;c<30;c++){if(f.length<30){f+=" "}}f+="999";
if(debug==true){$("#output").fill(f)}websocket.send(f)});
$("#_speichern").on("click",function(){send_data(20,0,0);
return false});
$("#_fb").on("click",function(){sl_typ=$(this).get("value");
if(sl_typ=="2"){sl_typ=0;$("#_fb").set("$backgroundColor","#e9e9e9");
$("#_fb").fill("Vordergrund");
$("#_fb").set("value",sl_typ)}else{if(sl_typ=="0"){sl_typ=1;
$("#_fb").set("$backgroundColor","#4B97D6");
$("#_fb").fill("Hintergrund");
$("#_fb").set("value",sl_typ)}else{sl_typ=2;
$("#_fb").set("$backgroundColor","#cd5c5c");
$("#_fb").fill("Rahmenfarbe");
$("#_fb").set("value",sl_typ)}}set_slider();
return false});
$(('[id*="_pbutton"]')).on("click",function(){var c=$(this).get("id");
$(('[id*="_pbutton_"]')).set("$backgroundColor","#e9e9e9");
var b=new RegExp("^_pbutton_");
if(b.test(c)){$("#"+c).set("$backgroundColor","#4B97D6")}$(('[id*="cl_"]')).set({$display:"none"});
if(c=="_pbutton0"){$(".cl_start").set({$display:"block"});
$(".cl_rgb1").set({$display:"block"});
$(".cl_rgb2").set({$display:"block"});
sl_typ=0;
$("#_fb").set("$backgroundColor","#e9e9e9");
$("#_fb").fill("Vordergrund");
$("#_fb").set("value",sl_typ);
command=1;
send_data(command,0,0);
set_slider()}if(c=="_pbutton1"){$(".cl_effekte").set({$display:"block"});
command=1}if(c=="_pbutton2"){$(".cl_setup").set({$display:"block"});
command=999;
send_data(300,0,0)}if(c=="_pbutton_3"){$(".cl_effekte").set({$display:"block"});
$(".cl_rgb2").set({$display:"block"});
sl_typ=3;
command=204;
send_data(command,0,0);
set_slider()}if(c=="_pbutton_4"){$(".cl_effekte").set({$display:"block"});
$(".cl_rgb2").set({$display:"block"});
sl_typ=3;
command=200;
send_data(command,0,0);
set_slider()}if(c=="_pbutton_5"){$(".cl_effekte").set({$display:"block"});
$(".cl_rgb2").set({$display:"block"});
$(".cl_geschw").set({$display:"block"});
sl_typ=3;
command=201;
send_data(command,0,0);
set_slider()}if(c=="_pbutton_6"){$(".cl_effekte").set({$display:"block"});
$(".cl_hell").set({$display:"block"});
$(".cl_geschw").set({$display:"block"});
command=202;
send_data(command,0,0);
set_slider()}if(c=="_pbutton_7"){$(".cl_effekte").set({$display:"block"});
$(".cl_hell").set({$display:"block"});
$(".cl_geschw").set({$display:"block"});
command=203;
send_data(command,0,0);
set_slider()}});
$('[id*="_fbutton"]').on("click",function(b){var c=$(this).get("id");
if(c=="_fbutton1"){rgb[sl_typ][0]=255,rgb[sl_typ][1]=0;rgb[sl_typ][2]=0}
if(c=="_fbutton2"){rgb[sl_typ][0]=0,rgb[sl_typ][1]=255;rgb[sl_typ][2]=0}
if(c=="_fbutton3"){rgb[sl_typ][0]=0,rgb[sl_typ][1]=0;rgb[sl_typ][2]=255}
if(c=="_fbutton4"){rgb[sl_typ][0]=255,rgb[sl_typ][1]=255;rgb[sl_typ][2]=0}
if(c=="_fbutton5"){rgb[sl_typ][0]=0,rgb[sl_typ][1]=255;rgb[sl_typ][2]=255}
if(c=="_fbutton6"){rgb[sl_typ][0]=255,rgb[sl_typ][1]=255;rgb[sl_typ][2]=255}
if(c=="_fbutton7"){rgb[sl_typ][0]=0,rgb[sl_typ][1]=0;rgb[sl_typ][2]=0}
hell=$("#_rg6").get("value");
geschw=$("#_rg7").get("value");
anzahl=$("#_rg9").get("value");
position=$("#_rg91").get("value");
set_slider();
send_data(command,0,0);
set_slider();
return false});
$('[id*="_rg"]').onChange(function(c){var d=$(this).get("id");
if(sleep==0){get_slider();
if(d=="_rg1"){send_data(command,1,0)}if(d=="_rg2"){send_data(command,1,0)}if(d=="_rg3"){send_data(command,1,0)}if(d=="_rg4"){send_data(command,1,0)}if(d=="_rg6"){send_data(251,0,0)}if(d=="_rg7"){send_data(252,0,0)}if(d=="_rg9"){send_data(253,0,0)}if(d=="_rg91"){send_data(254,0,0)}set_slider();
sleep=1;
sleeptime=20;
var b=setTimeout(function(){sleep=0},sleeptime)}return false});
                                                                           
$("#_wlan").on("click",function(){var e=$(_ssid).get("value");
var b=$(_passwd).get("value");
var d="099000000"+e;
for(var c=0;c<25;c++){if(d.length<34){d+=" "}}d+=b;
for(var c=0;c<25;c++){if(d.length<59){d+=" "}}d+="999";
websocket.send(d);
$("#output").fill("WLAN wurde neu konfiguriert");
if(debug==true){$("#output").fill(d)}return false});

$("#_wlanscan").on("click",function(){send_data(302,0,0);
document.getElementById("wlanlist").innerHTML = "<div>WLAN Netzwerke werden gesucht</div>";return false});

$("#_zeitserver").on("click",function(){var d=$(_zs).get("value");
var c="097000000"+d;
for(var b=0;b<25;b++){if(c.length<25){c+=" "}}c+="999";
websocket.send(c);
$("#output").fill("Zeitserver wurde neu konfiguriert");
if(debug==true){$("#output").fill(c)}return false});

$("#_ltext").on("click",function(){var b=$(_lt).get("value");
var d="096000000"+b;
for(var c=0;c<39;c++){if(d.length<39){d+=" "}}d+="999";
websocket.send(d);
$("#output").fill("Lauftext wurde neu konfiguriert");
if(debug==true){$("#output").fill(d)}});

$("#_helligkeit").on("click",function(){h6=$(_h6).get("value");
h8=$(_h8).get("value");
h12=$(_h12).get("value");
h16=$(_h16).get("value");
h18=$(_h18).get("value");
h20=$(_h20).get("value");
h22=$(_h22).get("value");
h24=$(_h24).get("value");
var b="095000000"+nstr(h6)+nstr(h8)+nstr(h12)+nstr(h16)+nstr(h18)+nstr(h20)+nstr(h22)+nstr(h24)+"999";
websocket.send(b);$("#output").fill("Helligkeit wurde neu konfiguriert");
if(debug==true){$("#output").fill(b)}});

$("#_minute").on("click",function(){zeige_min=$(_zeige_min).get("value");
var b="094000000"+zeige_min+"  999";
websocket.send(b);
$("#output").fill("Anzeige Minute wurde neu konfiguriert");
if(debug==true){$("#output").fill(b)}return false});

$("#_ldrsave").on("click",function(){ldr=$(_ldr).get("value");
ldrCal=$(_ldrCal).get("value");
var b="091000000"+ldr+"  "+ldrCal+"  999";
websocket.send(b);
$("#output").fill("LDR Einstellung wurde neu konfiguriert");
if(debug==true){$("#output").fill(b)}return false});

$("#_weathersave").on("click",function(){var c=$(_cityid).get("value");
var d=$(_apikey).get("value");
var b="090000000"+c+" "+d+"  999";
websocket.send(b);
$("#output").fill("API Key wurde neu konfiguriert");
if(debug==true){$("#output").fill(b)}return false});

$("#_sekunde").on("click",function(){zeige_sek=$(_zeige_sek).get("value");
var b="093000000"+zeige_sek+"  999";
websocket.send(b);
$("#output").fill("Anzeige Sekunde wurde neu konfiguriert");
if(debug==true){$("#output").fill(b)}return false});

$("#_host").on("click",function(){var c=$(_hn).get("value");
var d="092000000"+c;
for(var b=0;b<25;b++){if(d.length<25){d+=" "}}d+="999";
websocket.send(d);
$("#output").fill("Hostname wurde neu konfiguriert");
if(debug==true){$("#output").fill(d)}return false});

$("#_update").on("click",function(){var text = window.location.origin + ":81/update"; parent.open(text);});
$("#_reset").on("click",function(){send_data(100,0,0)})});

</script>
<!-- ENDE Variablenuebergabe -->


<style type="text/css">.button{// color:#fff;display:inline-block;// font-size:2em;margin:1px;padding:1px;width:12%;min-width:30px;height:50px;border:1px solid #AAA;text-decoration:none}.button2{color:#000;background-color:#e9e9e9;display:inline-block;font-size:1em;margin:1px;padding:1px;width:30%;min-width:90px;height:40px;border:1px solid #aaa;text-decoration:none}.button3{color:#000;background-color:#b9b9b9;display:inline-block;font-size:1em;margin:1px;padding:1px;width:30%;min-width:90px;height:40px;border:1px solid #000;text-decoration:none}.config{background-color:#fff;border:1px solid #000}.rglabel{-webkit-appearance:none!important;width:5%;align:right;display:inline-block}.rglinfo{-webkit-appearance:none!important;width:5%;align:left;vertical-align:50%;display:inline-block}input[type=range]{-webkit-appearance:none;width:75%;margin:10px 0}input[type=range]:focus{outline:0}input[type=range]::-webkit-slider-runnable-track{width:100%;height:32px;cursor:pointer;box-shadow:.1px .1px 0 #000000,0px 0 .1px #0d0d0d;border-radius:0;border:0 solid #010101}input[type=range]::-webkit-slider-thumb{box-shadow:0 0 0 rgba(0,0,0,0),0px 0 0 rgba(13,13,13,0);border:2px solid #000;height:35px;width:35px;border-radius:5px;cursor:pointer;-webkit-appearance:none;margin-top:-1.5px}input[type=range]:focus::-webkit-slider-runnable-track{background:#ff8080}input[type=range]::-moz-range-track{width:100%;height:32px;cursor:pointer;box-shadow:.1px .1px 0 #000000,0px 0 .1px #0d0d0d;border-radius:0;border:0 solid #010101}input[type=range]::-moz-range-thumb{box-shadow:0 0 0 rgba(0,0,0,0),0px 0 0 rgba(13,13,13,0);border:2px solid #000;height:35px;width:35px;border-radius:5px;background:#fff;cursor:pointer}input[type=range]::-ms-track{width:100%;height:32px;cursor:pointer;background:transparent;border-color:transparent;color:transparent}input[type=range]::-ms-fill-lower{border:0 solid #010101;border-radius:0;box-shadow:.1px .1px 0 #000000,0px 0 .1px #0d0d0d}input[type=range]::-ms-fill-upper{border:0 solid #010101;border-radius:0;box-shadow:.1px .1px 0 #000000,0px 0 .1px #0d0d0d}input[type=range]::-ms-thumb{box-shadow:0 0 0 rgba(0,0,0,0),0px 0 0 rgba(13,13,13,0);border:2px solid #000;height:35px;width:35px;border-radius:5px;background:#fff;cursor:pointer;height:32px}#_rg1::-webkit-slider-runnable-track{background:#f00}#_rg1::-moz-range-track{background:#f00}#_rg1::-ms-fill-lower{background:#f00}#_rg1::-ms-fill-upper{background:#f00}#_rg2::-webkit-slider-runnable-track{background:#0f0}#_rg2::-moz-range-track{background:#0f0}#_rg2::-ms-fill-lower{background:#0f0}#_rg2::-ms-fill-upper{background:#0f0}#_rg3::-webkit-slider-runnable-track{background:#00f}#_rg3::-moz-range-track{background:#00f}#_rg3::-ms-fill-lower{background:#00f}#_rg3::-ms-fill-upper{background:#00f}#_rg4::-webkit-slider-runnable-track{background:#eee}#_rg4::-moz-range-track{background:#eee}#_rg4::-ms-fill-lower{background:#eee}#_rg4::-ms-fill-upper{background:#eee}.cl_start{display:block}.cl_effekte{display:none}.cl_rgb1{display:block}.cl_rgb2{display:block}.cl_hell{display:none}.cl_geschw{display:none}.cl_anzahl{display:none}.cl_setup{display:none}.cl_schweif{display:none}.cl_lauflicht{display:none}.wlanlistr{float: right;width: 64px;text-align: right;}.wlanlistl{background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==") no-repeat left center;background-size: 1em;}</style>
</head>
<body>
<div align=center>
<div align=center>
<button class="button3" style="font-weight:bold" id="_pbutton0">WortUhr</button>
<button class="button3" style="font-weight:bold" id="_clock">--:--:--</button>
<button class="button3" style="background-color:#ed4d4d;color:#fff" id="_websocket" value=0>Offline</button>
</div>
</div>
<div id="cl_start" class="cl_start">
<div align=center>
<button class="button2" id="_fb" type="button" value=0>Vordergrund</button>
<button class="button2" id="_pbutton1">Effekte</button>
<button class="button2" id="_pbutton2">Konfig</button>
</div>
</div>
<div id="cl_effekte" class="cl_effekte">
<div align=center>
<button class="button2" id="_pbutton_3">Farbe</button>
<button class="button2" id="_pbutton_4" value=0>Sekunden</button>
<button class="button2" id="_pbutton_5" value=0>Laufschrift</button>
</div>
<div align=center>
<button class="button2" id="_pbutton_6">Regenbogen</button>
<button class="button2" id="_pbutton_7">Farbwechsel</button>
<button class="button2" id="_pbutton2">Konfig</button>
</div>
</div>
<div id="cl_rgb1" class="cl_rgb1">
<br>
<div align=center>
<button class="button" id="_fbutton1" style="background-color:#ff0000">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton2" style="background-color:#00ff00">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton3" style="background-color:#0000ff">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton4" style="background-color:#ffff00">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton5" style="background-color:#00ffff">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton6" style="background-color:#ffffff">&nbsp;&nbsp;&nbsp;</button>
<button class="button" id="_fbutton7" style="background-color:#000000">&nbsp;&nbsp;&nbsp;</button>
</div>
</div>
<div id="cl_rgb2" class="cl_rgb2">
<br>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg1" type="range" min="0" max="255" value="0">
<span class="rginfo" id="rg1wert">0</span>
</label>
</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg2" type="range" min="0" max="255" value="0">
<span class="rginfo" id="rg2wert">0</span>
</label>
</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg3" type="range" min="0" max="255" value="0">
<span class="rginfo" id="rg3wert">0</span>
</label>
</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg4" type="range" min="0" max="255" value="0">
<span class="rginfo" id="rg4wert">0</span>
</label>
</div>
</div>
<div id="cl_hell" class="cl_hell">
<div align=center>Helligkeit</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg6" type="range" min="1" max="10" value="0">
<span class="rginfo" id="rg6wert"></span>
</label>
</div>
</div>
<div id="cl_geschw" class="cl_geschw">
<div align=center>Geschwindigkeit</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg7" type="range" min="0" max="40" value="0">
<span class="rginfo" id="rg7wert"></span>
</label>
</div>
</div>
<div id="cl_anzahl" class="cl_anzahl">
<div align=center>Anzahl LEDs</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg9" type="range" min="5" max="100" value="0">
<span class="rginfo" id="rg9wert"></span>
</label>
</div>
<div align=center>Position</div>
<div align=center>
<label>
<span class="rglabel"></span>
<input id="_rg91" type="range" min="1" max="100" value="0">
<span class="rginfo" id="rg91wert"></span>
</label>
</div>
</div>
<div id="cl_schweif" class="cl_schweif">
<BR>
<div align=center>
<button class="button2" id="_pbschweif_updown">Up</button>
</div>
</div>
<div id="cl_setup" class="cl_setup">
<div align=center>
<button class="button2" id="_pbutton0">Uhr</button>
<button class="button2" id="_pbutton1">Effekte</button>
<button class="button2" id="_pbutton2">Konfig</button>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_speichern">Startwerte speichern</button>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_wlan">WLAN speichern</button>
<table>
<tr>
<td>SSID</td>
<td><input id="_ssid" type="text"></input></td>
</tr><tr>
<td>Passwort</td>
<td><input id="_passwd" type="text"></input></td>
</tr>
</table>
<BR>
<button class="button2" style="font-weight:bold;width:60%" id="_wlanscan">WLAN suchen</button>
<BR>
<div id="wlanlist" style="text-align:left;display:inline-block;min-width:100px;"></div>
<BR>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_zeitserver">Zeitserver speichern</button>
<table>
<tr>
<td>Zeitserver</td>
<td><input id="_zs" type="text"></input></td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_ltext">Lauftext speichern</button>
<table>
<tr>
<td>Lauftext</td>
<td><input id="_lt" type="text"></input></td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_helligkeit">Helligkeit</button>
<table>
<tr>
<td>6 Uhr</td>
<td>8 Uhr</td>
<td>12 Uhr</td>
<td>16 Uhr</td>
</tr>
<tr>
<td>
<select name="_h6" id="_h6"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h8" id="_h8"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h12" id="_h12"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h16" id="_h16"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
</tr>
<BR>
<tr>
<td>18 Uhr</td>
<td>20 Uhr</td>
<td>22 Uhr</td>
<td>24 Uhr</td>
</tr>
<tr>
<td>
<select name="_h18" id="_h18"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h20" id="_h20"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h22" id="_h22"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
<td>
<select name="_h24" id="_h24"size="1">
<option value="100">100</option>
<option value="80">80</option>
<option value="60">60</option>
<option value="40">40</option>
<option value="20">20</option>
<option value="0">Aus</option>
</select>
</td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_ldrsave">LDR Einstellungen speichern</button>
<table>
<tr>
<td>LDR für Helligkeitsregelung </td>
<td>
<select name="ldr" id="_ldr"size="1">
<option value="0" selected>Nein</option>
<option value="1">Ja</option>
</select>
</td>
</tr>
</table>
<table>
<tr>
<td>LDR Kalibrierung</td>
<td><input id="_ldrCal" type="text"></input></td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_host">Host-Name speichern</button>
<table>
<tr>
<td>Hostname</td>
<td><input id="_hn" type="text"></input></td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_minute">Minuten anzeigen</button>
<table>
<tr>
<td>
<select name="_zeige_min" id="_zeige_min"size="1">
<option value="0" selected>Nein</option>
<option value="1">Zeile</option>
<option value="2">Ecken</option>
</select>
</td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_sekunde">Sekunden anzeigen</button>
<table>
<tr>
<td>
<select name="_zeige_sek" id="_zeige_sek"size="1">
<option value="0" selected>Nein</option>
<option value="1">Ja</option>
</select>
</td>
</tr>
</table>
</div>
<br>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_weathersave">Open Weathermap speichern</button>
<table>
<tr>
<td>Open Weather Map API Key (32 Zeichen)</td>
<td><input id="_apikey" minlength="32" maxlength="32" type="text"></input></td>
</tr>
</table>
<table>
<tr>
<td>Open Weather Map City ID (7 Zeichen)</td>
<td><input id="_cityid" minlength="7" maxlength="7" type="text"></input></td>
</tr>
</table>
</div>
<BR>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_update">Over The Air Update</button>
</div>
<br>
<div class="config" align=center>
<button class="button2" style="font-weight:bold;width:60%" id="_reset">Reset</button>
</div>
</div>
<br>
<div align=center>
<div id="output"></div>
</div>
</body>
</html>
)=====";




#endif // #define _INDEX_H
