#pragma once

const char index_html_body_first[] PROGMEM= R"=====(<body>

	<div class="pure-g">
		<div class="pure-u-24-24">
			<div class="pure-menu pure-menu-horizontal main-menu">
				<div class="content">
					<a href="#" class="pure-menu-heading pure-menu-link active" data-navigation="main">Wortuhr 3</a><ul class="pure-menu-list"><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="functions">Funktionen</a></li><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="settings">Einstellungen</a></li><li class="pure-menu-item"><a href="#" class="pure-menu-link" data-navigation="about">Ueber</a></li>
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
)=====";

const char slider_RGB[] PROGMEM= R"=====(
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
)=====";

const char slider_RGBW[] PROGMEM = R"=====(
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
          <div class="pure-g color-w">
            <div class="pure-u-1 pure-u-md-4-24"><label class="slider-label" for="slider-white">White <span class="value" id="slider-white-value">0</span></label></div>
            <div class="pure-u-1 pure-u-md-5-6"><input id="slider-white" type="range" min="0" max="255" value="0"></div>
          </div>
        </div>
)=====";

const char index_html_body_rest[] PROGMEM = R"=====(
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
									<label for="show-minutes">Exklusive Einstellung für die Uhr des Types UHR_169. Sollen bei dieser Uhr die Minuten angezeigt werden ?</label><select name="show-minutes" id="show-minutes" size="1">
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
									<label for="show-seconds">Exklusive Einstellung für die Uhr des Types UHR_169. Sollen bei dieser Uhr die Sekunden im Rahmen angezeigt werden ?</label><select name="show-seconds" id="show-seconds" size="1">
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
						<h2>Uhrentyp ?</h2>
						<form class="pure-form pure-form-aligned">
							<fieldset>
								<div class="pure-control-group">
									<label for="uhrtype">Änderung des Uhrentypes.</label><select name="uhrtype" id="uhrtype" size="1">
										<option value="1" selected>Uhr_114</option>
										<option value="2">Uhr_114_Alternative</option>
										<option value="3">Uhr_125</option>
										<option value="4">Uhr_169</option>
										<option value="5">Uhr_242</option>
									</select>
								</div>
								<div class="pure-controls">
									<button id="uhrtyp-button" class="pure-button">Uhrentyp speichern</button>
								</div>
							</fieldset>
						</form>
						</div>

					</div>
					<div class="pure-u-1 pure-u-md-1-2">

						<div class="box">
							<h2>WLAN Ausschalten</h2>
							<form class="pure-form pure-form-aligned">
								<fieldset>
									<div class="pure-controls">
										<button id="disable-button" class="pure-button">WLAN Ausschalten</button>
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

const char* const html_sliders[] PROGMEM = {slider_RGB, slider_RGBW};

/* The following CODE is created by the Python Script Convert.py in the Source Folder */

const char index_html_head[] PROGMEM= R"=====(<!DOCTYPE html>
<html lang="de">
<head>
	<title>Wortuhr 2.4.0</title>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=yes">
	<meta name="apple-mobile-web-app-capable" content="yes">

	<!-- scripts -->
	<script>/^u/.test(typeof define)&&function(t){var n=this.require=function(n){return t[n]};this.define=function(e,i){t[e]=t[e]||i(n)}}({}),define("minified",function(){function t(t){return t!=M?""+t:""}function n(t){return/^str/.test(typeof t)}function e(t){return t&&t.nodeType}function i(t){return t}function r(t,n){f(t,function(t){t(n)})}function o(t,n){for(var e in t)n(e,t[e])}function u(t,n){var e=[];return f(t,function(i,r){n.call(t,i,r)&&e.push(i)}),e}function c(t,n,e){var i=[];return t(n,function(t,r){f(e.call(n,t,r),function(t){i.push(t)})}),i}function s(n,e,i){return t(n).replace(e,i||"")}function f(t,n){if(l(t))for(var e=0;e<t.length;e++)n.call(t,t[e],e);else t!=M&&n(t,0);return t}function a(t){return"function"==typeof t&&!t.item}function l(t){return t&&t.length!=M&&!n(t)&&!e(t)&&!a(t)&&t!==S}function h(t){return parseFloat(s(t,/^[^\d-]+/))}function d(t){return t.Nia=t.Nia||++q}function p(t,n){var e,i=[],r={};return f(t,function(t){f(n(t),function(t){r[e=d(t)]||(i.push(t),r[e]=!0)})}),i}function v(t,n){var e={$position:"absolute",$visibility:"hidden",$display:"block",$height:M},i=t.get(e);e=t.set(e).get("clientHeight");return t.set(i),e*n+"px"}function g(t){E?E.push(t):setTimeout(t,0)}function $(t){return c(f,t,function(t){return l(t)?$(t):(e(t)&&((t=t.cloneNode(!0)).removeAttribute&&t.removeAttribute("id")),t)})}function y(t,n,e){return a(t)?g(t):new x(m(t,n,e))}function m(t,i,r){function o(t){return l(t)?c(f,t,o):t}function s(t){return u(c(f,t,o),function(t){for(;t=t.parentNode;)if(t==i[0]||r)return t==i[0]})}return i?1!=(i=m(i)).length?p(i,function(n){return m(t,n,r)}):n(t)?1!=e(i[0])?[]:r?s(i[0].querySelectorAll(t)):i[0].querySelectorAll(t):s(t):n(t)?document.querySelectorAll(t):c(f,t,o)}function b(t,i){function r(t,n){var e=RegExp("(^|\\s+)"+t+"(?=$|\\s)","i");return function(i){return!t||e.test(i[n])}}var o,u,c={},s=c;return a(t)?t:/^num/.test(typeof t)?function(n,e){return e==t}:!t||"*"==t||n(t)&&(s=/^([\w-]*)\.?([\w-]*)$/.exec(t))?(o=r(s[1],"tagName"),u=r(s[2],"className"),function(t){return 1==e(t)&&o(t)&&u(t)}):i?function(n){return y(t,i).find(n)!=M}:(y(t).each(function(t){c[d(t)]=!0}),function(t){return c[d(t)]})}function w(t){var n=b(t);return function(t){return!n(t)||M}}function N(){function t(t,r){function o(){var i=n?t:r;a(i)?(i=i.apply(C,e))&&i.then?i.then(function(t){u.fire(!0,[t])},function(t){u.fire(!1,[t])}):u.fire(!0,[i]):u.fire(n,e)}var u=N();return n==M?i.push(o):setTimeout(o,0),u}var n,e,i=[];return{fire:function(t,o){n==M&&(n=t,e=o,setTimeout(function(){r(i)},0))},then:t,error:function(n){return t(0,n)}}}function x(t){for(var n=this.length=t.length,e=0;n>e;e++)this[e]=t[e]}var C,M=null,S=window,A={},T={},q=1,E=/^[ic]/.test(document.readyState)?M:[],k={},L=0;return o({each:function(t){return f(this,t)},filter:function(t){return new x(u(this,t))},collect:function(t){return new x(c(f,this,t))},sub:function(t,n){var e=0>t?this.length+t:t,i=n>=0?n:this.length+(n||0);return new x(u(this,function(t,n){return n>=e&&i>n}))},find:function(t,n){for(var e,i=a(t)?t:function(n,e){return t===n?e:void 0},r=n||0;r<this.length;r++)if((e=i.call(this,this[r],r))!=M)return e},remove:function(){f(this,function(t){t.parentNode.removeChild(t)})},text:function(){return c(f,this,function(t){return t.textContent}).join("")},trav:function(t,n,e){var i=/^num/.test(typeof n),r=b(i?M:n),o=i?n:e;return new x(p(this,function(n){for(var e=[];(n=n[t])&&e.length!=o;)r(n)&&e.push(n);return e}))},next:function(t,n){return this.trav("nextSibling",t,n||1)},up:function(t,n){return this.trav("parentNode",t,n||1)},select:function(t,n){return y(t,this,n)},is:function(t){return!this.find(w(t))},only:function(t){return new x(u(this,b(t)))},not:function(t){return new x(u(this,w(t)))},get:function(t,e){var i,r,u,c=this,a=c[0];return a?n(t)?(r=(i=/^(\W*)(.*)/.exec(s(t,/^%/,"@data-")))[1],a=T[r]?T[r](this,i[2]):"$"==t?c.get("className"):"$$"==t?c.get("@style"):"$$slide"==t?c.get("$height"):"$$fade"==t||"$$show"==t?"hidden"==c.get("$visibility")||"none"==c.get("$display")?0:"$$fade"==t?isNaN(c.get("$opacity",!0))?1:c.get("$opacity",!0):1:"$"==r?S.getComputedStyle(a,M).getPropertyValue(s(i[2],/[A-Z]/g,function(t){return"-"+t.toLowerCase()})):"@"==r?a.getAttribute(i[2]):a[i[2]],e?h(a):a):(u={},(l(t)?f:o)(t,function(t){u[t]=c.get(t,e)}),u):void 0},set:function(t,e){var i,r,u=this;return e!==C?(i=/^(\W*)(.*)/.exec(s(s(t,/^\$float$/,"cssFloat"),/^%/,"@data-")),r=i[1],A[r]?A[r](this,i[2],e):"$$fade"==t?this.set({$visibility:e?"visible":"hidden",$opacity:e}):"$$slide"==t?u.set({$visibility:e?"visible":"hidden",$overflow:"hidden",$height:/px/.test(e)?e:function(t,n,i){return v(y(i),e)}}):"$$show"==t?e?u.set({$visibility:e?"visible":"hidden",$display:""}).set({$display:function(t){return"none"==t?"block":t}}):u.set({$display:"none"}):"$$"==t?u.set("@style",e):f(this,function(n,o){var u=a(e)?e(y(n).get(t),o,n):e;"$"==r?i[2]?n.style[i[2]]=u:f(u&&u.split(/\s+/),function(t){var e=s(t,/^[+-]/),i=n.className||"",r=s(i,RegExp("(^|\\s+)"+e+"(?=$|\\s)"));(/^\+/.test(t)||e==t&&i==r)&&(r+=" "+e),n.className=s(r,/^\s+/g)}):"$$scrollX"==t?n.scroll(u,y(n).get("$$scrollY")):"$$scrollY"==t?n.scroll(y(n).get("$$scrollX"),u):"@"==r?u==M?n.removeAttribute(i[2]):n.setAttribute(i[2],u):n[i[2]]=u})):n(t)||a(t)?u.set("$",t):o(t,function(t,n){u.set(t,n)}),u},show:function(){return this.set("$$show",1)},hide:function(){return this.set("$$show",0)},add:function(t,n){return this.each(function(i,r){var o;!function t(u){l(u)?f(u,t):a(u)?t(u(i,r)):u!=M&&(u=e(u)?u:document.createTextNode(u),o?o.parentNode.insertBefore(u,o.nextSibling):n?n(u,i,i.parentNode):i.appendChild(u),o=u)}(r&&!a(t)?$(t):t)})},fill:function(t){return this.each(function(t){y(t.childNodes).remove()}).add(t)},addAfter:function(t){return this.add(t,function(t,n,e){e.insertBefore(t,n.nextSibling)})},addBefore:function(t){return this.add(t,function(t,n,e){e.insertBefore(t,n)})},addFront:function(t){return this.add(t,function(t,n){n.insertBefore(t,n.firstChild)})},replace:function(t){return this.add(t,function(t,n,e){e.replaceChild(t,n)})},clone:function(){return new x($(this))},animate:function(t,n,e){var i,u=N(),s=this,l=c(f,this,function(n,i){var r,u=y(n),c={};return o(r=u.get(t),function(e,r){var o=t[e];c[e]=a(o)?o(r,i,n):"$$slide"==e?v(u,o):o}),u.dial(r,c,e)}),h=n||500;return u.stop=function(){return u.fire(!1),i()},i=y.loop(function(t){r(l,t/h),t>=h&&(i(),u.fire(!0,[s]))}),u},dial:function(n,e,i){function r(t,n){return/^#/.test(t)?parseInt(6<t.length?t.substr(2*n+1,2):(t=t.charAt(n+1))+t,16):h(t.split(",")[n])}var u=this,c=i||0,f=a(c)?c:function(t,n,e){return e*(n-t)*(c+(1-c)*e*(3-2*e))+t};return function(i){o(n,function(n,o){var c=e[n],a=0;u.set(n,0>=i?o:i>=1?c:/^#|rgb\(/.test(c)?"rgb("+Math.round(f(r(o,a),r(c,a++),i))+","+Math.round(f(r(o,a),r(c,a++),i))+","+Math.round(f(r(o,a),r(c,a++),i))+")":s(c,/-?[\d.]+/,t(f(h(o),h(c),i))))})}},toggle:function(t,n,e,i){var r,o,u=this,c=!1;return n?(u.set(t),function(s){s!==c&&(o=(c=!0===s||!1===s?s:!c)?n:t,e?(r=u.animate(o,r?r.stop():e,i)).then(function(){r=M}):u.set(o))}):u.toggle(s(t,/\b(?=\w)/g,"-"),s(t,/\b(?=\w)/g,"+"))},values:function(n){var e=n||{};return this.each(function(n){var r=n.name||n.id,o=t(n.value);if(/form/i.test(n.tagName))for(r=0;r<n.elements.length;r++)y(n.elements[r]).values(e);else!r||/ox|io/i.test(n.type)&&!n.checked||(e[r]=e[r]==M?o:c(f,[e[r],o],i))}),e},offset:function(){for(var t=this[0],n={x:0,y:0};t;)n.x+=t.offsetLeft,n.y+=t.offsetTop,t=t.offsetParent;return n},on:function(e,r,o,u,l){return a(r)?this.on(M,e,r,o,u):n(u)?this.on(e,r,o,M,u):this.each(function(n,a){f(e?m(e,n):n,function(n){f(t(r).split(/\s/),function(t){function e(t,e,i){var r,c=!l;if(i=l?i:n,l)for(r=b(l,n);i&&i!=n&&!(c=r(i));)i=i.parentNode;return!c||h!=t||o.apply(y(i),u||[e,a])&&"?"==d||"|"==d}function r(t){e(h,t,t.target)||(t.preventDefault(),t.stopPropagation())}var h=s(t,/[?|]/g),d=s(t,/[^?|]/g),p=("blur"==h||"focus"==h)&&!!l,v=q++;n.addEventListener(h,r,p),n.M||(n.M={}),n.M[v]=e,o.M=c(f,[o.M,function(){n.removeEventListener(h,r,p),delete n.M[v]}],i)})})})},onOver:function(t,n){var e=this,i=[];return a(n)?this.on(t,"|mouseover |mouseout",function(t,r){var o=t.relatedTarget||t.toElement,u="mouseout"!=t.type;i[r]===u||!u&&o&&(o==e[r]||y(o).up(e[r]).length)||(i[r]=u,n.call(this,u,t))}):this.onOver(M,t)},onFocus:function(t,n,e){return a(n)?this.on(t,"|blur",n,[!1],e).on(t,"|focus",n,[!0],e):this.onFocus(M,t,n)},onChange:function(t,n,e){return a(n)?this.on(t,"|input |change |click",function(t,e){var i=this[0],r=/ox|io/i.test(i.type)?i.checked:i.value;i.NiaP!=r&&n.call(this,i.NiaP=r,e)},e):this.onChange(M,t,n)},onClick:function(t,n,e,i){return a(n)?this.on(t,"click",n,e,i):this.onClick(M,t,n,e)},trigger:function(t,n){return this.each(function(e){for(var i=!0,r=e;r&&i;)o(r.M,function(r,o){i=i&&o(t,n,e)}),r=r.parentNode})}},function(t,n){x.prototype[t]=n}),o({request:function(n,e,i,r){r=r||{};var u,s=0,a=N(),l=i&&i.constructor==r.constructor;try{a.xhr=u=new XMLHttpRequest,a.stop=function(){u.abort()},l&&(i=c(o,i,function(t,n){return c(f,n,function(n){return encodeURIComponent(t)+(n!=M?"="+encodeURIComponent(n):"")})}).join("&")),i==M||/post/i.test(n)||(e+="?"+i,i=M),u.open(n,e,!0,r.user,r.pass),l&&/post/i.test(n)&&u.setRequestHeader("Content-Type","application/x-www-form-urlencoded"),o(r.headers,function(t,n){u.setRequestHeader(t,n)}),o(r.xhr,function(t,n){u[t]=n}),u.onreadystatechange=function(){4!=u.readyState||s++||(200<=u.status&&300>u.status?a.fire(!0,[u.responseText,u]):a.fire(!1,[u.status,u.responseText,u]))},u.send(i)}catch(n){s||a.fire(!1,[0,M,t(n)])}return a},toJSON:JSON.stringify,parseJSON:JSON.parse,ready:g,loop:function(t){function n(){return k[r]&&(delete k[r],L--),i}var e,i=0,r=q++,u=S.requestAnimationFrame||function(t){setTimeout(function(){t(+new Date)},33)};return k[r]=function(r){t(i=r-(e=e||r),n)},L++||u(function t(n){o(k,function(t,e){e(n)}),L&&u(t)}),n},off:function(t){r(t.M),t.M=M}},function(t,n){y[t]=n}),document.addEventListener("DOMContentLoaded",function(){r(E),E=M},!1),{$:y,$$:function(t,n,e){return m(t,n,e)[0]},EE:function(t,n,e){return t=y(document.createElement(t)),l(n)||!/^ob/.test(typeof n)?t.add(n):t.set(n).add(e)},M:x,getter:T,setter:A}});</script>
	<script>"use strict";var websocket,MINI=require("minified"),_=MINI._,$=MINI.$,$$=MINI.$$,EE=MINI.EE,HTML=MINI.HTML,ipEsp="ws://192.168.4.1",debug=!0,command=1,rgb=[[0,0,100,0],[0,10,0,0],[10,0,0,0],[5,5,5,0]],sliderType=0,hell=2,geschw=10,anzahl=100,sleep=0,sleeptime=1,position=100,color=0,h6=100,h8=100,h12=100,h16=100,h18=100,h20=100,h22=100,h24=100,showSeconds=0,showMinutes=0,uhrtype=0,COMMAND_SET_INITIAL_VALUES=20,COMMAND_SET_TIME=30,COMMAND_SET_WEATHER_DATA=90,COMMAND_SET_LDR=91,COMMAND_SET_HOSTNAME=92,COMMAND_SET_SETTING_SECOND=93,COMMAND_SET_MINUTE=94,COMMAND_SET_BRIGHTNESS=95,COMMAND_SET_MARQUEE_TEXT=96,COMMAND_SET_TIMESERVER=97,COMMAND_SET_WIFI_DISABLED=98,COMMAND_SET_WIFI_AND_RESTART=99,COMMAND_RESET=100,COMMAND_REQUEST_CONFIG_VALUES=300,COMMAND_REQUEST_COLOR_VALUES=301,COMMAND_REQUEST_WIFI_LIST=302,COMMAND_MODE_WORD_CLOCK=1,COMMAND_MODE_SECONDS=200,COMMAND_MODE_MARQUEE=201,COMMAND_MODE_RAINBOW=202,COMMAND_MODE_CHANGE=203,COMMAND_MODE_COLOR=204,COMMAND_BRIGHTNESS=251,COMMAND_SPEED=252,COMMAND_LEDS=253,COMMAND_POSITION=254,COLOR_FOREGROUND=0,COLOR_BACKGROUND=1,COLOR_BORDER=2,COLOR_EFFECT=3,DATA_MARQUEE_TEXT_LENGTH=30,DATA_SSID_TEXT_LENGTH=25,DATA_PASSWORT_TEXT_LENGTH=25,DATA_TIMESERVER_TEXT_LENGTH=16,DATA_HOST_TEXT_LENGTH=16;function initConfigValues(){var e=location.host;ipEsp=""!==e?"ws://"+e:"ws://192.168.178.44/",debug=!0,command=1,rgb=[[0,0,100,0],[0,10,0,0],[10,0,0,0],[5,5,5,0]],sliderType=0,hell=2,geschw=10,anzahl=100,sleep=0,sleeptime=1,position=100,anzahl=100,color=0,h6=100,h8=100,h12=100,h16=100,h18=100,h20=100,h22=100,h24=100,showSeconds=0,showMinutes=0,uhrtype=0}function hexToRgb(e){var s=/^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(e);return s?{red:parseInt(s[1],16),green:parseInt(s[2],16),blue:parseInt(s[3],16)}:null}function debugMessage(e,s){!0===debug&&(void 0!==console&&(console.log(e),s&&console.log(s)),$("#output").fill(e))}function initWebsocket(){(websocket=new WebSocket(ipEsp)).onopen=function(e){$("#status").set("+online"),$("#status").set("-offline"),$("#status").set("@value","Online"),$(".status-button").fill("Verbindung trennen"),$(".status-button").set("@value","1"),$("#section-connection-lost").set({$display:"none"}),debugMessage("Die Verbindung mit dem Websocket wurde aufgebaut.",e),sendData(301,0,0)},websocket.onclose=function(e){$("#status").set("-online"),$("#status").set("+offline"),$("#status").set("@value","Offline"),$(".status-button").fill("Verbinden"),$(".status-button").set("@value","0"),$("#section-connection-lost").set({$display:"block"}),debugMessage("Die Verbindung mit dem Websocket wurde geschlossen (Code "+e.code+").",e)},websocket.onmessage=function(e){var s=JSON.parse(e.data);debugMessage("Webservice response arrived (Command "+s.command+").",s),"config"===s.command&&($("#ssid").set("value",s.ssid),$("#timeserver").set("value",s.zeitserver),$("#hostname").set("value",s.hostname),$("#marquee").set("value",s.ltext),$("#brightness-6").set("value",s.h6),$("#brightness-8").set("value",s.h8),$("#brightness-12").set("value",s.h12),$("#brightness-16").set("value",s.h16),$("#brightness-18").set("value",s.h18),$("#brightness-20").set("value",s.h20),$("#brightness-22").set("value",s.h22),$("#brightness-24").set("value",s.h24),$("#slider-brightness").set("value",s.hell),$("#slider-speed").set("value",s.geschw),$("#uhrtype").set("value",uhrtype),$("#showSeconds").set("value",s.zeige_sek),$("#showMinutes").set("value",s.zeige_min),$("#owm-api-key").set("value",s.apiKey),$("#owm-city-id").set("value",s.cityid)),"set"===s.command&&(rgb[0][0]=s.rgb00,rgb[0][1]=s.rgb01,rgb[0][2]=s.rgb02,rgb[0][3]=s.rgb03,rgb[1][0]=s.rgb10,rgb[1][1]=s.rgb11,rgb[1][2]=s.rgb12,rgb[1][3]=s.rgb13,rgb[2][0]=s.rgb20,rgb[2][1]=s.rgb21,rgb[2][2]=s.rgb22,rgb[2][3]=s.rgb23,rgb[3][0]=s.rgb30,rgb[3][1]=s.rgb31,rgb[3][2]=s.rgb32,rgb[3][3]=s.rgb33,hell=s.hell,geschw=s.geschw,setSliders())},websocket.onerror=function(e){debugMessage("Bei der Verbindung mit dem Websocket ist ein Fehler aufgetreten.",e)}}function getSliders(){rgb[sliderType][0]=$("#slider-red").get("value"),rgb[sliderType][1]=$("#slider-green").get("value"),rgb[sliderType][2]=$("#slider-blue").get("value"),rgb[sliderType][3]=$("#slider-white").get("value"),hell=$("#slider-brightness").get("value"),geschw=$("#slider-speed").get("value"),anzahl=$("#slider-leds").get("value"),position=$("#slider-position").get("value")}function setSliders(){$("#slider-red").set("value",rgb[sliderType][0]),$("#slider-green").set("value",rgb[sliderType][1]),$("#slider-blue").set("value",rgb[sliderType][2]),$("#slider-white").set("value",rgb[sliderType][3]),$("#slider-red-value").fill(rgb[sliderType][0]),$("#slider-green-value").fill(rgb[sliderType][1]),$("#slider-blue-value").fill(rgb[sliderType][2]),$("#slider-white-value").fill(rgb[sliderType][3]),$("#slider-brightness").set("value",hell),$("#slider-speed").set("value",geschw),$("#slider-leds").set("value",anzahl),$("#slider-position").set("value",position),$("#slider-brightness-value").fill(hell),$("#slider-speed-value").fill(geschw),$("#slider-leds-value").fill(anzahl),$("#slider-position-value").fill(position),$("#color-area")[0].style.backgroundColor="rgb("+rgb[sliderType][0]+","+rgb[sliderType][1]+","+rgb[sliderType][2]+")"}function nstr(e){return e<10?e="00"+e:e<100&&(e="0"+e),e}function getPaddedString(e,s){for(;e.length<s;)e+=" ";return e}function sendData(e,s,t){var n=nstr(e)+nstr(s)+nstr(t)+nstr(rgb[COLOR_FOREGROUND][0])+nstr(rgb[COLOR_FOREGROUND][1])+nstr(rgb[COLOR_FOREGROUND][2])+nstr(rgb[COLOR_FOREGROUND][3])+nstr(rgb[COLOR_BACKGROUND][0])+nstr(rgb[COLOR_BACKGROUND][1])+nstr(rgb[COLOR_BACKGROUND][2])+nstr(rgb[COLOR_BACKGROUND][3])+nstr(rgb[COLOR_FOREGROUND][0])+nstr(rgb[COLOR_FOREGROUND][1])+nstr(rgb[COLOR_FOREGROUND][2])+nstr(rgb[COLOR_FOREGROUND][3])+nstr(rgb[COLOR_FOREGROUND][0])+nstr(rgb[COLOR_FOREGROUND][1])+nstr(rgb[COLOR_FOREGROUND][2])+nstr(rgb[COLOR_FOREGROUND][3])+nstr(hell)+nstr(geschw)+nstr(anzahl)+nstr(position)+"999";websocket.send(n),debugMessage("Send data: ",n)}$.ready(function(){initConfigValues(),setSliders(),initWebsocket(),$(".status-button").on("click",function(){var e=$(this).get("value");return"1"===e?(e=0,$("#status").fill("Verbinden ..."),$(".status-button").set("value",e),websocket.close()):(e=1,$("#status").fill("Verbinden ..."),$(".status-button").set("value",e),initWebsocket()),!1}),$("#_clock").on("click",function(){var e=new Date,s=e.getTimezoneOffset();s=s/60*-1;var t=e.getTime()/1e3,n="030000000";n+=getPaddedString(nstr(s)+t,21),debugMessage("Clock data: ",n+="999"),websocket.send(n)}),$("input[name='color-mode']").on("change",function(){switch($(this).get("value")){case"foreground":sliderType=COLOR_FOREGROUND;break;case"background":sliderType=COLOR_BACKGROUND}setSliders()}),$("a[class~='pure-menu-link']").on("click",function(){var e=$(this)[0].dataset.navigation;$(".pure-menu-link").set("-active"),$(this).set("+active"),"settings"===e&&sendData(COMMAND_REQUEST_CONFIG_VALUES,0,0),$(".section").set({$display:"none"}),$(".section-"+e).set({$display:"block"})}),$("input[name='mode']").on("change",function(){var e=$(this).get("id"),s=!1,t=!1;"mode-wordclock"===e&&(sliderType=COLOR_FOREGROUND,command=COMMAND_MODE_WORD_CLOCK),"mode-color"===e&&(sliderType=COLOR_FOREGROUND,command=COMMAND_MODE_COLOR),"mode-seconds"===e&&(sliderType=COLOR_FOREGROUND,command=COMMAND_MODE_SECONDS),"mode-marquee"===e&&(t=!0,sliderType=COLOR_FOREGROUND,command=COMMAND_MODE_MARQUEE),"mode-rainbow"===e&&(s=!0,t=!0,command=COMMAND_MODE_RAINBOW),"mode-change"===e&&(s=!0,t=!0,command=COMMAND_MODE_CHANGE),!0===s?$(".brightness").set({$display:"block"}):$(".brightness").set({$display:"none"}),!0===t?$(".speed").set({$display:"block"}):$(".speed").set({$display:"none"}),s||t?$(".functions-settings").set({$display:"block"}):$(".functions-settings").set({$display:"none"}),sendData(command,0,0),setSliders()}),$(".quick-color").on("click",function(e){var s=hexToRgb($(this)[0].dataset.color);return rgb[sliderType][0]=s.red,rgb[sliderType][1]=s.green,rgb[sliderType][2]=s.blue,rgb[sliderType][3]=s.white,hell=$("#slider-brightness").get("value"),geschw=$("#slider-speed").get("value"),anzahl=$("#slider-leds").get("value"),position=$("#slider-position").get("value"),setSliders(),sendData(command,0,0),setSliders(),!1}),$("[id*='slider']").onChange(function(e){var s=$(this).get("id");return 0===sleep&&(getSliders(),"slider-red"===s&&sendData(command,1,0),"slider-green"===s&&sendData(command,1,0),"slider-blue"===s&&sendData(command,1,0),"slider-white"===s&&sendData(command,1,0),"slider-brightness"===s&&sendData(COMMAND_BRIGHTNESS,0,0),"slider-speed"===s&&sendData(COMMAND_SPEED,0,0),"slider-leds"===s&&sendData(COMMAND_LEDS,0,0),"slider-position"===s&&sendData(COMMAND_POSITION,0,0),setSliders(),sleep=1,sleeptime=20,setTimeout(function(){sleep=0},sleeptime)),!1}),$("#initial-values-button").on("click",function(){sendData(COMMAND_SET_INITIAL_VALUES,0,0)}),$("#wifi-button").on("click",function(){var e=$("#ssid").get("value"),s=$("#password").get("value"),t="099000000";return t+=getPaddedString(e,DATA_SSID_TEXT_LENGTH),t+=getPaddedString(s,DATA_PASSWORT_TEXT_LENGTH),t+="999",websocket.send(t),debugMessage("WLAN wurde neu konfiguriert",t),!1}),$("#timeserver-button").on("click",function(){var e="097000000";return e+=getPaddedString($("#timeserver").get("value"),DATA_TIMESERVER_TEXT_LENGTH),e+="999",websocket.send(e),debugMessage("Zeitserver wurde neu konfiguriert",e),!1}),$("#marquee-button").on("click",function(){var e="096000000";e+=getPaddedString($("#marquee").get("value"),DATA_MARQUEE_TEXT_LENGTH),e+="999",websocket.send(e),debugMessage("Lauftext wurde neu konfiguriert",e)}),$("#brightness-button").on("click",function(){h6=$("#brightness-6").get("value"),h8=$("#brightness-8").get("value"),h12=$("#brightness-12").get("value"),h16=$("#brightness-16").get("value"),h18=$("#brightness-18").get("value"),h20=$("#brightness-20").get("value"),h22=$("#brightness-22").get("value"),h24=$("#brightness-24").get("value");var e="095000000"+nstr(h6)+nstr(h8)+nstr(h12)+nstr(h16)+nstr(h18)+nstr(h20)+nstr(h22)+nstr(h24)+"999";websocket.send(e),debugMessage("Helligkeit wurde neu konfiguriert",e)}),$("#weather-button").on("click",function(){var e=$("#owm-api-key").get("value"),s="090000000"+$("#owm-city-id").get("value")+" "+e+"  999";websocket.send(s),debugMessage("OpenWeatherMap Zugangsdaten wurden konfiguriert",s)}),$("#show-minutesbutton").on("click",function(){var e="094000000"+$("#show-minutes").get("value")+"  999";websocket.send(e),debugMessage("Minutenanzeige wurde neu konfiguriert",e)}),$("#show-seconds-button").on("click",function(){var e="093000000"+$("#show-seconds").get("value")+"  999";websocket.send(e),debugMessage("Sekundenanzeige wurde neu konfiguriert",e)}),$("#uhrtype-button").on("click",function(){var e="089000000"+$("#uhrtype").get("value")+"  999";websocket.send(e),debugMessage("Uhrtyp wurde neu konfiguriert",e)}),$("#host-button").on("click",function(){var e="092000000";e+=getPaddedString($("#host").get("value"),DATA_HOST_TEXT_LENGTH),e+="999",websocket.send(e),debugMessage("Hostname wurde neu konfiguriert",e)}),$("#disable-button").on("click",function(){sendData(COMMAND_SET_WIFI_DISABLED,0,0)}),$("#reset-button").on("click",function(){sendData(COMMAND_RESET,0,0)})});</script>

	<!-- stylesheets -->
<style>/*!
Pure v1.0.0
Copyright 2013 Yahoo!
Licensed under the BSD License.
https://github.com/yahoo/pure/blob/master/LICENSE.md
*//*!
normalize.css v^3.0 | MIT License | git.io/normalize
Copyright (c) Nicolas Gallagher and Jonathan Neal
*//*! normalize.css v3.0.3 | MIT License | github.com/necolas/normalize.css */.pure-button:focus,a:active,a:hover{outline:0}.pure-table,table{border-collapse:collapse;border-spacing:0}html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin:0}article,aside,details,figcaption,figure,footer,header,hgroup,main,menu,nav,section,summary{display:block}audio,canvas,progress,video{display:inline-block;vertical-align:baseline}audio:not([controls]){display:none;height:0}[hidden],template{display:none}a{background-color:transparent}abbr[title]{border-bottom:1px dotted}b,optgroup,strong{font-weight:700}dfn{font-style:italic}h1{font-size:2em;margin:.67em 0}mark{background:#ff0;color:#000}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sup{top:-.5em}sub{bottom:-.25em}img{border:0}svg:not(:root){overflow:hidden}figure{margin:1em 40px}hr{box-sizing:content-box;height:0}pre,textarea{overflow:auto}code,kbd,pre,samp{font-family:monospace,monospace;font-size:1em}button,input,optgroup,select,textarea{color:inherit;font:inherit;margin:0}.pure-button,input{line-height:normal}button{overflow:visible}button,select{text-transform:none}button,html input[type=button],input[type=reset],input[type=submit]{-webkit-appearance:button;cursor:pointer}button[disabled],html input[disabled]{cursor:default}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}input[type=checkbox],input[type=radio]{box-sizing:border-box;padding:0}input[type=number]::-webkit-inner-spin-button,input[type=number]::-webkit-outer-spin-button{height:auto}input[type=search]{-webkit-appearance:textfield;box-sizing:content-box}.pure-button,.pure-form input:not([type]),.pure-menu{box-sizing:border-box}input[type=search]::-webkit-search-cancel-button,input[type=search]::-webkit-search-decoration{-webkit-appearance:none}fieldset{border:1px solid silver;margin:0 2px;padding:.35em .625em .75em}legend,td,th{padding:0}legend{border:0}.hidden,[hidden]{display:none!important}.pure-img{max-width:100%;height:auto;display:block}.pure-g{letter-spacing:-.31em;text-rendering:optimizespeed;font-family:FreeSans,Arimo,"Droid Sans",Helvetica,Arial,sans-serif;display:-webkit-box;display:-webkit-flex;display:-ms-flexbox;display:flex;-webkit-flex-flow:row wrap;-ms-flex-flow:row wrap;flex-flow:row wrap;-webkit-align-content:flex-start;-ms-flex-line-pack:start;align-content:flex-start}@media all and (-ms-high-contrast:none),(-ms-high-contrast:active){table .pure-g{display:block}}.opera-only :-o-prefocus,.pure-g{word-spacing:-.43em}.pure-u,.pure-u-1,.pure-u-1-1,.pure-u-1-12,.pure-u-1-2,.pure-u-1-24,.pure-u-1-3,.pure-u-1-4,.pure-u-1-5,.pure-u-1-6,.pure-u-1-8,.pure-u-10-24,.pure-u-11-12,.pure-u-11-24,.pure-u-12-24,.pure-u-13-24,.pure-u-14-24,.pure-u-15-24,.pure-u-16-24,.pure-u-17-24,.pure-u-18-24,.pure-u-19-24,.pure-u-2-24,.pure-u-2-3,.pure-u-2-5,.pure-u-20-24,.pure-u-21-24,.pure-u-22-24,.pure-u-23-24,.pure-u-24-24,.pure-u-3-24,.pure-u-3-4,.pure-u-3-5,.pure-u-3-8,.pure-u-4-24,.pure-u-4-5,.pure-u-5-12,.pure-u-5-24,.pure-u-5-5,.pure-u-5-6,.pure-u-5-8,.pure-u-6-24,.pure-u-7-12,.pure-u-7-24,.pure-u-7-8,.pure-u-8-24,.pure-u-9-24{letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto;display:inline-block;zoom:1}.pure-g [class*=pure-u]{font-family:sans-serif}.pure-u-1-24{width:4.1667%}.pure-u-1-12,.pure-u-2-24{width:8.3333%}.pure-u-1-8,.pure-u-3-24{width:12.5%}.pure-u-1-6,.pure-u-4-24{width:16.6667%}.pure-u-1-5{width:20%}.pure-u-5-24{width:20.8333%}.pure-u-1-4,.pure-u-6-24{width:25%}.pure-u-7-24{width:29.1667%}.pure-u-1-3,.pure-u-8-24{width:33.3333%}.pure-u-3-8,.pure-u-9-24{width:37.5%}.pure-u-2-5{width:40%}.pure-u-10-24,.pure-u-5-12{width:41.6667%}.pure-u-11-24{width:45.8333%}.pure-u-1-2,.pure-u-12-24{width:50%}.pure-u-13-24{width:54.1667%}.pure-u-14-24,.pure-u-7-12{width:58.3333%}.pure-u-3-5{width:60%}.pure-u-15-24,.pure-u-5-8{width:62.5%}.pure-u-16-24,.pure-u-2-3{width:66.6667%}.pure-u-17-24{width:70.8333%}.pure-u-18-24,.pure-u-3-4{width:75%}.pure-u-19-24{width:79.1667%}.pure-u-4-5{width:80%}.pure-u-20-24,.pure-u-5-6{width:83.3333%}.pure-u-21-24,.pure-u-7-8{width:87.5%}.pure-u-11-12,.pure-u-22-24{width:91.6667%}.pure-u-23-24{width:95.8333%}.pure-u-1,.pure-u-1-1,.pure-u-24-24,.pure-u-5-5{width:100%}.pure-button{display:inline-block;zoom:1;white-space:nowrap;vertical-align:middle;text-align:center;cursor:pointer;-webkit-user-drag:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.pure-button::-moz-focus-inner{padding:0;border:0}.pure-button-group{letter-spacing:-.31em;text-rendering:optimizespeed}.opera-only :-o-prefocus,.pure-button-group{word-spacing:-.43em}.pure-button{font-family:inherit;font-size:100%;padding:.5em 1em;color:#444;color:rgba(0,0,0,.8);border:1px solid #999;border:transparent;background-color:#e6e6e6;text-decoration:none;border-radius:2px}.pure-button-hover,.pure-button:focus,.pure-button:hover{background-image:-webkit-linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1));background-image:linear-gradient(transparent,rgba(0,0,0,.05) 40%,rgba(0,0,0,.1))}.pure-button-active,.pure-button:active{box-shadow:0 0 0 1px rgba(0,0,0,.15) inset,0 0 6px rgba(0,0,0,.2) inset}.pure-button-disabled,.pure-button-disabled:active,.pure-button-disabled:focus,.pure-button-disabled:hover,.pure-button[disabled]{border:none;background-image:none;opacity:.4;cursor:not-allowed;box-shadow:none;pointer-events:none}.pure-button-hidden{display:none}.pure-button-primary,.pure-button-selected,a.pure-button-primary,a.pure-button-selected{background-color:#0078e7;color:#fff}.pure-button-group .pure-button{letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto;margin:0;border-radius:0;border-right:1px solid #111;border-right:1px solid rgba(0,0,0,.2)}.pure-button-group .pure-button:first-child{border-top-left-radius:2px;border-bottom-left-radius:2px}.pure-button-group .pure-button:last-child{border-top-right-radius:2px;border-bottom-right-radius:2px;border-right:none}.pure-form input[type=color],.pure-form input[type=date],.pure-form input[type=datetime-local],.pure-form input[type=datetime],.pure-form input[type=email],.pure-form input[type=month],.pure-form input[type=number],.pure-form input[type=password],.pure-form input[type=search],.pure-form input[type=tel],.pure-form input[type=text],.pure-form input[type=time],.pure-form input[type=url],.pure-form input[type=week],.pure-form select,.pure-form textarea{padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px;vertical-align:middle;box-sizing:border-box}.pure-form input:not([type]){padding:.5em .6em;display:inline-block;border:1px solid #ccc;box-shadow:inset 0 1px 3px #ddd;border-radius:4px}.pure-form input[type=color]{padding:.2em .5em}.pure-form input:not([type]):focus,.pure-form input[type=color]:focus,.pure-form input[type=date]:focus,.pure-form input[type=datetime-local]:focus,.pure-form input[type=datetime]:focus,.pure-form input[type=email]:focus,.pure-form input[type=month]:focus,.pure-form input[type=number]:focus,.pure-form input[type=password]:focus,.pure-form input[type=search]:focus,.pure-form input[type=tel]:focus,.pure-form input[type=text]:focus,.pure-form input[type=time]:focus,.pure-form input[type=url]:focus,.pure-form input[type=week]:focus,.pure-form select:focus,.pure-form textarea:focus{outline:0;border-color:#129fea}.pure-form input[type=checkbox]:focus,.pure-form input[type=file]:focus,.pure-form input[type=radio]:focus{outline:#129fea auto 1px}.pure-form .pure-checkbox,.pure-form .pure-radio{margin:.5em 0;display:block}.pure-form input:not([type])[disabled],.pure-form input[type=color][disabled],.pure-form input[type=date][disabled],.pure-form input[type=datetime-local][disabled],.pure-form input[type=datetime][disabled],.pure-form input[type=email][disabled],.pure-form input[type=month][disabled],.pure-form input[type=number][disabled],.pure-form input[type=password][disabled],.pure-form input[type=search][disabled],.pure-form input[type=tel][disabled],.pure-form input[type=text][disabled],.pure-form input[type=time][disabled],.pure-form input[type=url][disabled],.pure-form input[type=week][disabled],.pure-form select[disabled],.pure-form textarea[disabled]{cursor:not-allowed;background-color:#eaeded;color:#cad2d3}.pure-form input[readonly],.pure-form select[readonly],.pure-form textarea[readonly]{background-color:#eee;color:#777;border-color:#ccc}.pure-form input:focus:invalid,.pure-form select:focus:invalid,.pure-form textarea:focus:invalid{color:#b94a48;border-color:#e9322d}.pure-form input[type=checkbox]:focus:invalid:focus,.pure-form input[type=file]:focus:invalid:focus,.pure-form input[type=radio]:focus:invalid:focus{outline-color:#e9322d}.pure-form select{height:2.25em;border:1px solid #ccc;background-color:#fff}.pure-form select[multiple]{height:auto}.pure-form label{margin:.5em 0 .2em}.pure-form fieldset{margin:0;padding:.35em 0 .75em;border:0}.pure-form legend{display:block;width:100%;padding:.3em 0;margin-bottom:.3em;color:#333;border-bottom:1px solid #e5e5e5}.pure-form-stacked input:not([type]),.pure-form-stacked input[type=color],.pure-form-stacked input[type=date],.pure-form-stacked input[type=datetime-local],.pure-form-stacked input[type=datetime],.pure-form-stacked input[type=email],.pure-form-stacked input[type=file],.pure-form-stacked input[type=month],.pure-form-stacked input[type=number],.pure-form-stacked input[type=password],.pure-form-stacked input[type=search],.pure-form-stacked input[type=tel],.pure-form-stacked input[type=text],.pure-form-stacked input[type=time],.pure-form-stacked input[type=url],.pure-form-stacked input[type=week],.pure-form-stacked label,.pure-form-stacked select,.pure-form-stacked textarea{display:block;margin:.25em 0}.pure-form-aligned .pure-help-inline,.pure-form-aligned input,.pure-form-aligned select,.pure-form-aligned textarea,.pure-form-message-inline{display:inline-block;vertical-align:middle}.pure-form-aligned textarea{vertical-align:top}.pure-form-aligned .pure-control-group{margin-bottom:.5em}.pure-form-aligned .pure-control-group label{text-align:right;display:inline-block;vertical-align:middle;width:10em;margin:0 1em 0 0}.pure-form-aligned .pure-controls{margin:1.5em 0 0 11em}.pure-form .pure-input-rounded,.pure-form input.pure-input-rounded{border-radius:2em;padding:.5em 1em}.pure-form .pure-group fieldset{margin-bottom:10px}.pure-form .pure-group input,.pure-form .pure-group textarea{display:block;padding:10px;margin:0 0 -1px;border-radius:0;position:relative;top:-1px}.pure-form .pure-group input:focus,.pure-form .pure-group textarea:focus{z-index:3}.pure-form .pure-group input:first-child,.pure-form .pure-group textarea:first-child{top:1px;border-radius:4px 4px 0 0;margin:0}.pure-form .pure-group input:first-child:last-child,.pure-form .pure-group textarea:first-child:last-child{top:1px;border-radius:4px;margin:0}.pure-form .pure-group input:last-child,.pure-form .pure-group textarea:last-child{top:-2px;border-radius:0 0 4px 4px;margin:0}.pure-form .pure-group button{margin:.35em 0}.pure-form .pure-input-1{width:100%}.pure-form .pure-input-3-4{width:75%}.pure-form .pure-input-2-3{width:66%}.pure-form .pure-input-1-2{width:50%}.pure-form .pure-input-1-3{width:33%}.pure-form .pure-input-1-4{width:25%}.pure-form .pure-help-inline,.pure-form-message-inline{display:inline-block;padding-left:.3em;color:#666;vertical-align:middle;font-size:.875em}.pure-form-message{display:block;color:#666;font-size:.875em}@media only screen and (max-width :480px){.pure-form button[type=submit]{margin:.7em 0 0}.pure-form input:not([type]),.pure-form input[type=color],.pure-form input[type=date],.pure-form input[type=datetime-local],.pure-form input[type=datetime],.pure-form input[type=email],.pure-form input[type=month],.pure-form input[type=number],.pure-form input[type=password],.pure-form input[type=search],.pure-form input[type=tel],.pure-form input[type=text],.pure-form input[type=time],.pure-form input[type=url],.pure-form input[type=week],.pure-form label{margin-bottom:.3em;display:block}.pure-group input:not([type]),.pure-group input[type=color],.pure-group input[type=date],.pure-group input[type=datetime-local],.pure-group input[type=datetime],.pure-group input[type=email],.pure-group input[type=month],.pure-group input[type=number],.pure-group input[type=password],.pure-group input[type=search],.pure-group input[type=tel],.pure-group input[type=text],.pure-group input[type=time],.pure-group input[type=url],.pure-group input[type=week]{margin-bottom:0}.pure-form-aligned .pure-control-group label{margin-bottom:.3em;text-align:left;display:block;width:100%}.pure-form-aligned .pure-controls{margin:1.5em 0 0}.pure-form .pure-help-inline,.pure-form-message,.pure-form-message-inline{display:block;font-size:.75em;padding:.2em 0 .8em}}.pure-menu-fixed{position:fixed;left:0;top:0;z-index:3}.pure-menu-item,.pure-menu-list{position:relative}.pure-menu-list{list-style:none;margin:0;padding:0}.pure-menu-item{padding:0;margin:0;height:100%}.pure-menu-heading,.pure-menu-link{display:block;text-decoration:none;white-space:nowrap}.pure-menu-horizontal{width:100%;white-space:nowrap}.pure-menu-horizontal .pure-menu-list{display:inline-block}.pure-menu-horizontal .pure-menu-heading,.pure-menu-horizontal .pure-menu-item,.pure-menu-horizontal .pure-menu-separator{display:inline-block;zoom:1;vertical-align:middle}.pure-menu-item .pure-menu-item{display:block}.pure-menu-children{display:none;position:absolute;left:100%;top:0;margin:0;padding:0;z-index:3}.pure-menu-horizontal .pure-menu-children{left:0;top:auto;width:inherit}.pure-menu-active>.pure-menu-children,.pure-menu-allow-hover:hover>.pure-menu-children{display:block;position:absolute}.pure-menu-has-children>.pure-menu-link:after{padding-left:.5em;content:"\25B8";font-size:small}.pure-menu-horizontal .pure-menu-has-children>.pure-menu-link:after{content:"\25BE"}.pure-menu-scrollable{overflow-y:scroll;overflow-x:hidden}.pure-menu-scrollable .pure-menu-list{display:block}.pure-menu-horizontal.pure-menu-scrollable .pure-menu-list{display:inline-block}.pure-menu-horizontal.pure-menu-scrollable{white-space:nowrap;overflow-y:hidden;overflow-x:auto;-ms-overflow-style:none;-webkit-overflow-scrolling:touch;padding:.5em 0}.pure-menu-horizontal.pure-menu-scrollable::-webkit-scrollbar{display:none}.pure-menu-horizontal .pure-menu-children .pure-menu-separator,.pure-menu-separator{background-color:#ccc;height:1px;margin:.3em 0}.pure-menu-horizontal .pure-menu-separator{width:1px;height:1.3em;margin:0 .3em}.pure-menu-horizontal .pure-menu-children .pure-menu-separator{display:block;width:auto}.pure-menu-heading{text-transform:uppercase;color:#565d64}.pure-menu-link{color:#777}.pure-menu-children{background-color:#fff}.pure-menu-disabled,.pure-menu-heading,.pure-menu-link{padding:.5em 1em}.pure-menu-disabled{opacity:.5}.pure-menu-disabled .pure-menu-link:hover{background-color:transparent}.pure-menu-active>.pure-menu-link,.pure-menu-link:focus,.pure-menu-link:hover{background-color:#eee}.pure-menu-selected .pure-menu-link,.pure-menu-selected .pure-menu-link:visited{color:#000}.pure-table{empty-cells:show;border:1px solid #cbcbcb}.pure-table caption{color:#000;font:italic 85%/1 arial,sans-serif;padding:1em 0;text-align:center}.pure-table td,.pure-table th{border-left:1px solid #cbcbcb;border-width:0 0 0 1px;font-size:inherit;margin:0;overflow:visible;padding:.5em 1em}.pure-table td:first-child,.pure-table th:first-child{border-left-width:0}.pure-table thead{background-color:#e0e0e0;color:#000;text-align:left;vertical-align:bottom}.pure-table td{background-color:transparent}.pure-table-odd td,.pure-table-striped tr:nth-child(2n-1) td{background-color:#f2f2f2}.pure-table-bordered td{border-bottom:1px solid #cbcbcb}.pure-table-bordered tbody>tr:last-child>td{border-bottom-width:0}.pure-table-horizontal td,.pure-table-horizontal th{border-width:0 0 1px;border-bottom:1px solid #cbcbcb}.pure-table-horizontal tbody>tr:last-child>td{border-bottom-width:0}/*!
Pure v1.0.0
Copyright 2013 Yahoo!
Licensed under the BSD License.
https://github.com/yahoo/pure/blob/master/LICENSE.md
*/@media screen and (min-width:35.5em){.pure-u-sm-1,.pure-u-sm-1-1,.pure-u-sm-1-12,.pure-u-sm-1-2,.pure-u-sm-1-24,.pure-u-sm-1-3,.pure-u-sm-1-4,.pure-u-sm-1-5,.pure-u-sm-1-6,.pure-u-sm-1-8,.pure-u-sm-10-24,.pure-u-sm-11-12,.pure-u-sm-11-24,.pure-u-sm-12-24,.pure-u-sm-13-24,.pure-u-sm-14-24,.pure-u-sm-15-24,.pure-u-sm-16-24,.pure-u-sm-17-24,.pure-u-sm-18-24,.pure-u-sm-19-24,.pure-u-sm-2-24,.pure-u-sm-2-3,.pure-u-sm-2-5,.pure-u-sm-20-24,.pure-u-sm-21-24,.pure-u-sm-22-24,.pure-u-sm-23-24,.pure-u-sm-24-24,.pure-u-sm-3-24,.pure-u-sm-3-4,.pure-u-sm-3-5,.pure-u-sm-3-8,.pure-u-sm-4-24,.pure-u-sm-4-5,.pure-u-sm-5-12,.pure-u-sm-5-24,.pure-u-sm-5-5,.pure-u-sm-5-6,.pure-u-sm-5-8,.pure-u-sm-6-24,.pure-u-sm-7-12,.pure-u-sm-7-24,.pure-u-sm-7-8,.pure-u-sm-8-24,.pure-u-sm-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-sm-1-24{width:4.1667%}.pure-u-sm-1-12,.pure-u-sm-2-24{width:8.3333%}.pure-u-sm-1-8,.pure-u-sm-3-24{width:12.5%}.pure-u-sm-1-6,.pure-u-sm-4-24{width:16.6667%}.pure-u-sm-1-5{width:20%}.pure-u-sm-5-24{width:20.8333%}.pure-u-sm-1-4,.pure-u-sm-6-24{width:25%}.pure-u-sm-7-24{width:29.1667%}.pure-u-sm-1-3,.pure-u-sm-8-24{width:33.3333%}.pure-u-sm-3-8,.pure-u-sm-9-24{width:37.5%}.pure-u-sm-2-5{width:40%}.pure-u-sm-10-24,.pure-u-sm-5-12{width:41.6667%}.pure-u-sm-11-24{width:45.8333%}.pure-u-sm-1-2,.pure-u-sm-12-24{width:50%}.pure-u-sm-13-24{width:54.1667%}.pure-u-sm-14-24,.pure-u-sm-7-12{width:58.3333%}.pure-u-sm-3-5{width:60%}.pure-u-sm-15-24,.pure-u-sm-5-8{width:62.5%}.pure-u-sm-16-24,.pure-u-sm-2-3{width:66.6667%}.pure-u-sm-17-24{width:70.8333%}.pure-u-sm-18-24,.pure-u-sm-3-4{width:75%}.pure-u-sm-19-24{width:79.1667%}.pure-u-sm-4-5{width:80%}.pure-u-sm-20-24,.pure-u-sm-5-6{width:83.3333%}.pure-u-sm-21-24,.pure-u-sm-7-8{width:87.5%}.pure-u-sm-11-12,.pure-u-sm-22-24{width:91.6667%}.pure-u-sm-23-24{width:95.8333%}.pure-u-sm-1,.pure-u-sm-1-1,.pure-u-sm-24-24,.pure-u-sm-5-5{width:100%}}@media screen and (min-width:48em){.pure-u-md-1,.pure-u-md-1-1,.pure-u-md-1-12,.pure-u-md-1-2,.pure-u-md-1-24,.pure-u-md-1-3,.pure-u-md-1-4,.pure-u-md-1-5,.pure-u-md-1-6,.pure-u-md-1-8,.pure-u-md-10-24,.pure-u-md-11-12,.pure-u-md-11-24,.pure-u-md-12-24,.pure-u-md-13-24,.pure-u-md-14-24,.pure-u-md-15-24,.pure-u-md-16-24,.pure-u-md-17-24,.pure-u-md-18-24,.pure-u-md-19-24,.pure-u-md-2-24,.pure-u-md-2-3,.pure-u-md-2-5,.pure-u-md-20-24,.pure-u-md-21-24,.pure-u-md-22-24,.pure-u-md-23-24,.pure-u-md-24-24,.pure-u-md-3-24,.pure-u-md-3-4,.pure-u-md-3-5,.pure-u-md-3-8,.pure-u-md-4-24,.pure-u-md-4-5,.pure-u-md-5-12,.pure-u-md-5-24,.pure-u-md-5-5,.pure-u-md-5-6,.pure-u-md-5-8,.pure-u-md-6-24,.pure-u-md-7-12,.pure-u-md-7-24,.pure-u-md-7-8,.pure-u-md-8-24,.pure-u-md-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-md-1-24{width:4.1667%}.pure-u-md-1-12,.pure-u-md-2-24{width:8.3333%}.pure-u-md-1-8,.pure-u-md-3-24{width:12.5%}.pure-u-md-1-6,.pure-u-md-4-24{width:16.6667%}.pure-u-md-1-5{width:20%}.pure-u-md-5-24{width:20.8333%}.pure-u-md-1-4,.pure-u-md-6-24{width:25%}.pure-u-md-7-24{width:29.1667%}.pure-u-md-1-3,.pure-u-md-8-24{width:33.3333%}.pure-u-md-3-8,.pure-u-md-9-24{width:37.5%}.pure-u-md-2-5{width:40%}.pure-u-md-10-24,.pure-u-md-5-12{width:41.6667%}.pure-u-md-11-24{width:45.8333%}.pure-u-md-1-2,.pure-u-md-12-24{width:50%}.pure-u-md-13-24{width:54.1667%}.pure-u-md-14-24,.pure-u-md-7-12{width:58.3333%}.pure-u-md-3-5{width:60%}.pure-u-md-15-24,.pure-u-md-5-8{width:62.5%}.pure-u-md-16-24,.pure-u-md-2-3{width:66.6667%}.pure-u-md-17-24{width:70.8333%}.pure-u-md-18-24,.pure-u-md-3-4{width:75%}.pure-u-md-19-24{width:79.1667%}.pure-u-md-4-5{width:80%}.pure-u-md-20-24,.pure-u-md-5-6{width:83.3333%}.pure-u-md-21-24,.pure-u-md-7-8{width:87.5%}.pure-u-md-11-12,.pure-u-md-22-24{width:91.6667%}.pure-u-md-23-24{width:95.8333%}.pure-u-md-1,.pure-u-md-1-1,.pure-u-md-24-24,.pure-u-md-5-5{width:100%}}@media screen and (min-width:64em){.pure-u-lg-1,.pure-u-lg-1-1,.pure-u-lg-1-12,.pure-u-lg-1-2,.pure-u-lg-1-24,.pure-u-lg-1-3,.pure-u-lg-1-4,.pure-u-lg-1-5,.pure-u-lg-1-6,.pure-u-lg-1-8,.pure-u-lg-10-24,.pure-u-lg-11-12,.pure-u-lg-11-24,.pure-u-lg-12-24,.pure-u-lg-13-24,.pure-u-lg-14-24,.pure-u-lg-15-24,.pure-u-lg-16-24,.pure-u-lg-17-24,.pure-u-lg-18-24,.pure-u-lg-19-24,.pure-u-lg-2-24,.pure-u-lg-2-3,.pure-u-lg-2-5,.pure-u-lg-20-24,.pure-u-lg-21-24,.pure-u-lg-22-24,.pure-u-lg-23-24,.pure-u-lg-24-24,.pure-u-lg-3-24,.pure-u-lg-3-4,.pure-u-lg-3-5,.pure-u-lg-3-8,.pure-u-lg-4-24,.pure-u-lg-4-5,.pure-u-lg-5-12,.pure-u-lg-5-24,.pure-u-lg-5-5,.pure-u-lg-5-6,.pure-u-lg-5-8,.pure-u-lg-6-24,.pure-u-lg-7-12,.pure-u-lg-7-24,.pure-u-lg-7-8,.pure-u-lg-8-24,.pure-u-lg-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-lg-1-24{width:4.1667%}.pure-u-lg-1-12,.pure-u-lg-2-24{width:8.3333%}.pure-u-lg-1-8,.pure-u-lg-3-24{width:12.5%}.pure-u-lg-1-6,.pure-u-lg-4-24{width:16.6667%}.pure-u-lg-1-5{width:20%}.pure-u-lg-5-24{width:20.8333%}.pure-u-lg-1-4,.pure-u-lg-6-24{width:25%}.pure-u-lg-7-24{width:29.1667%}.pure-u-lg-1-3,.pure-u-lg-8-24{width:33.3333%}.pure-u-lg-3-8,.pure-u-lg-9-24{width:37.5%}.pure-u-lg-2-5{width:40%}.pure-u-lg-10-24,.pure-u-lg-5-12{width:41.6667%}.pure-u-lg-11-24{width:45.8333%}.pure-u-lg-1-2,.pure-u-lg-12-24{width:50%}.pure-u-lg-13-24{width:54.1667%}.pure-u-lg-14-24,.pure-u-lg-7-12{width:58.3333%}.pure-u-lg-3-5{width:60%}.pure-u-lg-15-24,.pure-u-lg-5-8{width:62.5%}.pure-u-lg-16-24,.pure-u-lg-2-3{width:66.6667%}.pure-u-lg-17-24{width:70.8333%}.pure-u-lg-18-24,.pure-u-lg-3-4{width:75%}.pure-u-lg-19-24{width:79.1667%}.pure-u-lg-4-5{width:80%}.pure-u-lg-20-24,.pure-u-lg-5-6{width:83.3333%}.pure-u-lg-21-24,.pure-u-lg-7-8{width:87.5%}.pure-u-lg-11-12,.pure-u-lg-22-24{width:91.6667%}.pure-u-lg-23-24{width:95.8333%}.pure-u-lg-1,.pure-u-lg-1-1,.pure-u-lg-24-24,.pure-u-lg-5-5{width:100%}}@media screen and (min-width:80em){.pure-u-xl-1,.pure-u-xl-1-1,.pure-u-xl-1-12,.pure-u-xl-1-2,.pure-u-xl-1-24,.pure-u-xl-1-3,.pure-u-xl-1-4,.pure-u-xl-1-5,.pure-u-xl-1-6,.pure-u-xl-1-8,.pure-u-xl-10-24,.pure-u-xl-11-12,.pure-u-xl-11-24,.pure-u-xl-12-24,.pure-u-xl-13-24,.pure-u-xl-14-24,.pure-u-xl-15-24,.pure-u-xl-16-24,.pure-u-xl-17-24,.pure-u-xl-18-24,.pure-u-xl-19-24,.pure-u-xl-2-24,.pure-u-xl-2-3,.pure-u-xl-2-5,.pure-u-xl-20-24,.pure-u-xl-21-24,.pure-u-xl-22-24,.pure-u-xl-23-24,.pure-u-xl-24-24,.pure-u-xl-3-24,.pure-u-xl-3-4,.pure-u-xl-3-5,.pure-u-xl-3-8,.pure-u-xl-4-24,.pure-u-xl-4-5,.pure-u-xl-5-12,.pure-u-xl-5-24,.pure-u-xl-5-5,.pure-u-xl-5-6,.pure-u-xl-5-8,.pure-u-xl-6-24,.pure-u-xl-7-12,.pure-u-xl-7-24,.pure-u-xl-7-8,.pure-u-xl-8-24,.pure-u-xl-9-24{display:inline-block;zoom:1;letter-spacing:normal;word-spacing:normal;vertical-align:top;text-rendering:auto}.pure-u-xl-1-24{width:4.1667%}.pure-u-xl-1-12,.pure-u-xl-2-24{width:8.3333%}.pure-u-xl-1-8,.pure-u-xl-3-24{width:12.5%}.pure-u-xl-1-6,.pure-u-xl-4-24{width:16.6667%}.pure-u-xl-1-5{width:20%}.pure-u-xl-5-24{width:20.8333%}.pure-u-xl-1-4,.pure-u-xl-6-24{width:25%}.pure-u-xl-7-24{width:29.1667%}.pure-u-xl-1-3,.pure-u-xl-8-24{width:33.3333%}.pure-u-xl-3-8,.pure-u-xl-9-24{width:37.5%}.pure-u-xl-2-5{width:40%}.pure-u-xl-10-24,.pure-u-xl-5-12{width:41.6667%}.pure-u-xl-11-24{width:45.8333%}.pure-u-xl-1-2,.pure-u-xl-12-24{width:50%}.pure-u-xl-13-24{width:54.1667%}.pure-u-xl-14-24,.pure-u-xl-7-12{width:58.3333%}.pure-u-xl-3-5{width:60%}.pure-u-xl-15-24,.pure-u-xl-5-8{width:62.5%}.pure-u-xl-16-24,.pure-u-xl-2-3{width:66.6667%}.pure-u-xl-17-24{width:70.8333%}.pure-u-xl-18-24,.pure-u-xl-3-4{width:75%}.pure-u-xl-19-24{width:79.1667%}.pure-u-xl-4-5{width:80%}.pure-u-xl-20-24,.pure-u-xl-5-6{width:83.3333%}.pure-u-xl-21-24,.pure-u-xl-7-8{width:87.5%}.pure-u-xl-11-12,.pure-u-xl-22-24{width:91.6667%}.pure-u-xl-23-24{width:95.8333%}.pure-u-xl-1,.pure-u-xl-1-1,.pure-u-xl-24-24,.pure-u-xl-5-5{width:100%}}</style>	<style>body{background:#f7f7f7;font-size:16px}.content{padding:20px;max-width:1200px;margin:auto}.content>.pure-g{margin-left:-10px;margin-right:-10px}p{margin-bottom:15px;margin-top:0}a{text-decoration:none}h1,h2,h3,h4,h5,h6{color:rgba(0,0,0,.7)}h1{font-size:1.4em;margin-top:0;margin-bottom:8px}h2{font-size:1.2em;color:rgba(0,0,0,.6)}.main-menu{background:#5c5c5c;box-shadow:inset 0 -10px 7px -10px rgba(0,0,0,.8);overflow-x:scroll;overflow-y:hidden}.main-menu .content{padding:0}.pure-menu-item{height:auto}.main-menu .pure-menu-link{padding:1.2em;padding-bottom:1em;padding-top:1em;color:#fff}.pure-menu-link:hover{background:#898989;color:#fff}.pure-menu-link.active{background:#f7f7f7;color:#5c5c5c}.color-area{height:40px;width:100%;background:gray;margin-bottom:15px;margin-top:10px;box-shadow:inset 0 0 0 1px rgba(0,0,0,.3)}.button-xl{font-size:150%}.pure-g.colors>div{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.pure-g.colors>div{padding:.3em}.pure-g.colors button{width:100%;height:40px;box-shadow:inset 0 0 0 1px rgba(0,0,0,.3)}label.pure-radio{padding:5px;display:inline-block}label.pure-radio input[type=radio]{margin-top:-14px;display:inline-block}input[type=range],input[type=range]:-moz-focusring,input[type=range]::-moz-focus-inner,input[type=range]::-moz-focus-outer,input[type=range]:active,input[type=range]:focus{border:0;outline:0}input[type=range]{width:100%;-webkit-appearance:none;-moz-appearance:none;margin-top:8px;margin-bottom:8px;height:20px}input[type=range]::-webkit-slider-runnable-track{height:6px;cursor:pointer;background:#ddd;border:none;border-radius:100px;box-shadow:inset 0 1px 1px 0 rgba(0,0,0,.3);-webkit-appearance:none;-moz-appearance:none}input[type=range]::-webkit-slider-thumb{height:28px;width:28px;margin-top:-11px;border-radius:200px;background:linear-gradient(to bottom,#fff 0,#ebebeb 100%);box-shadow:0 1px 1px rgba(0,0,0,.3),inset 0 0 0 1px rgba(255,255,255,.9);border:1px solid rgba(0,0,0,.3);cursor:pointer;-webkit-appearance:none;-moz-appearance:none}input[type=range]::-moz-range-track{height:6px;background:#ddd;border:none;border-radius:100px;box-shadow:inset 0 1px 1px 0 rgba(0,0,0,.3);-webkit-appearance:none;-moz-appearance:none}input[type=range]::-moz-range-thumb{height:28px;width:28px;border-radius:200px;background:linear-gradient(to bottom,#fff 0,#ebebeb 100%);box-shadow:0 1px 1px rgba(0,0,0,.3),inset 0 0 0 1px rgba(255,255,255,.9);border:1px solid rgba(0,0,0,.3);cursor:pointer;-webkit-appearance:none;-moz-appearance:none}.slider-label{margin-top:11px;margin-bottom:4px;display:inline-block;width:100%}.slider-label .value{float:right;margin-right:15px}.section,.section-connection-lost{display:none}.section-connection-lost .box{background:#ffe9e9;color:#a94442;border:1px solid #e0b2b2;box-shadow:none}.section-connection-lost .status-button{float:right;color:#fff;background-color:#d9534f;border-color:#d9534f;margin-left:20px}.section-main{display:block}.box-debug{overflow:hidden;overflow-wrap:break-word}.box-debug>div{font-family:monospace}.brightness,.functions-settings,.leds,.position,.speed{display:none}.section-settings .box,.section-settings .pure-u-1,.section-settings>div{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}.section-settings .pure-u-1{padding-bottom:20px}.section-settings .box{height:100%}.status.online{color:green}.status.offline{color:red}.box{padding:20px;margin:10px;box-shadow:0 1px 1px 0 rgba(60,64,67,.08),0 1px 3px 1px rgba(60,64,67,.16);background:#fff;border-radius:3px;color:#5f6368}.box h2{margin-top:-5px;border-bottom:1px solid #f0f0f0;padding-bottom:10px;margin-left:-20px;margin-right:-20px;padding-right:15px;padding-left:15px;font-weight:400;font-size:1.1em}</style></head>)=====";

/* End for CODE generation by Script */