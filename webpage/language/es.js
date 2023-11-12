let TRANSLATION_ES = {
	"title": "Reloj de Palabras",

	"menu": {
		"aria-show-menu": "Muestrame el menu",
		"aria-hide-menu": "Ocultar el menú",
		"language": {
			"label": "Idioma",
			"de": "🇩🇪 Alemán",
			"en": "🇺🇸 Inglés",
			"nl": "🇳🇱 Holandés",
			"es": "🇪🇸 Español",
			"it": "🇮🇹 Italiano"
		}
	},

	"connection-lost": {
		"button": "Conectar",
		"text": "Se perdió la conexión con el reloj."
	},

	"colors": {
		"h1": "Colores",
		"with-background": "Con color de fondo"
	},

	"functions": {
		"h1": "Características",
		"mode": {
			"h2": "Modo",
			"wordclock": "Reloj de Palabras",
			"color": "Color",
			"seconds": "Segundos",
			"digital-clock": "Reloj Digital",
			"scrollingtext": "Texto en Ejecución",
			"rainbow": "Arcoíris",
			"color-change": "Cambio de color",
			"symbol": "Símbolo"
		},
		"animation": {
			"h2": "Animación",
			"type": {
				"label": "Tipo"
			},
			"duration": {
				"label": "Período de Tiempo",
				"short": "corto",
				"medium": "medio",
				"long": "largo"
			},
			"color": {
				"label": "Animación en Colores",
				"off": "Apagado",
				"words": "Palabras Coloridas",
				"letters": "Letras Coloridas"
			},
			"speed": {
				"label": "Velocidad"
			},
			"demo": {
				"label": "Demo"
			}
		},
		"settings": {
			"h2": "Ajustes",
			"brightness": "Brillo ",
			"scrollingtext": "Texto en Ejecución",
			"save": "Ahorrar",
			"speed": "Velocidad ",
			"leds": "Número de LEDs ",
			"position": "Posición "
		}
	},

	"view": {
		"h1": "Opciones de Pantalla",
		"front": {
			"h2": "Front",
			"text": "Los cambios en el tamaño de la matriz solo se aplican cuando se reinicia el reloj.",
			"variant": "Elección de variante de reloj",
			"de-10-11": "Alemán 10 × 11",
			"de-10-11-alt": "Alemán 10 × 11 Alternativ",
			"de-10-11-alt-frame": "Alemán 10 × 11 Alternativer Rahmen",
			"de-10-11-vertical": "Alemán 10 × 11 Vertikal",
			"de-10-11-clock": "Alemán 10 × 11 Uhr",
			"de-10-11-nero": "Alemán 10 × 11 Nero",
			"de-11-11": "Alemán 11 × 11",
			"de-11-11-v2": "Alemán 11 × 11 Version 2",
			"de-11-11-v3": "Alemán 11 × 11 (panbachi)",
			"de-22-11-weather": "Alemán 10 × 11 Wetter",
			"de-16-8": "Alemán 16 × 8",
			"de-16-18": "Alemán 16 × 18",
			"en-10-11": "Inglés 10 × 11",
			"nl-10-11": "Holandés 10 × 11",
			"es-10-11": "Español 10 × 11",
			"it-10-11": "Italiano 10 × 11",
			"invert-min-dir": "Invertir la dirección de conteo de minutos",
			"mirror-vertically": "Voltear el reloj verticalmente",
			"mirror-horizontally": "Voltear el reloj horizontalmente",
			"buildtype": "Tipo de Diseño",
			"normal": "Cada LED corresponde a una letra",
			"doubleResM1": "Cada segundo LED corresponde a una letra",
			"doubleRes": "Dos LED por letra"
		},
		"language": {
			"h2": "Idioma",
			"hide-it-is": "„Son las“ esconderse",
			"at-nine-fifteen": "Son las 9:15 ...",
			"quarter-past-nine": "las nueve y cuarto",
			"quarter-nine": "un cuarto de diez",
			"at-nine-twenty": "Son las 9:20 ...",
			"twenty-past-nine": "nueve y veinte",
			"at-nine-fourty": "Son las 9:40 ...",
			"twenty-before-ten": "de veinte a diez",
			"at-nine-fourtyfive-german": "Son las 9:45 ...",
			"quarter-to-ten": "un cuarto para las diez",
			"three-quarter-ten": "las diez menos cuarto",
			"quarter-to-ten-english": "Quarter to ten",
			"a-quarter-to-ten-english": "A quarter to ten"
		},
		"minutes": {
			"h2": "minutos",
			"mode": "Modo de visualización de minutos",
			"off": "Apagado",
			"normal-4-leds": "Normal (4 LEDs)",
			"normal-7-leds": "Normal (7 LEDs)",
			"edges": "Esquinas",
			"in-words": "En palabras"
		},
		"seconds": {
			"h2": "Segundos",
			"in-border": "Los segundos se muestran en el marco.",
			"off": "Apagado",
			"point": "Punto",
			"sector": "Sector",
			"revolving-sector": "Sector circulante"
		},
		"boot": {
			"h2": "Boot",
			"help": "¿Qué debería pasar cuando reinicies??",
			"blink-leds": "Deje que los LED parpadeen brevemente",
			"iterate-leds": "Deje que todos los LED pasen una vez",
			"wifi-symbol": "Mostrar el icono de WiFi durante la búsqueda de red",
			"ip-address": "Mostrar dirección IP"
		}
	},

	"settings": {
		"h1": "Ajustes",
		"status": {
			"h2": "Estado",
			"label": "Estado de Conexión",
			"connect": "Conectar"
		},
		"led-type": {
			"h2": "Tipo de LED",
			"help": "Aquí puede configurar el tipo de tira de LED utilizada (WS2812 o SK6812).\n" +
				"Dependiendo del fabricante de las tiras SK2812 utilizadas, existe una versión diferente que muestra los colores en un orden diferente.\n" +
				"En consecuencia, aquí puede configurar la combinación de colores correcta de la tira.\n" +
				"G-Verde / B-Azul / R-Rojo / W-Blanco",
			"label": "Cambio de tipo de color",
			"ws2812-brg": "WS2812 BRG",
			"ws2812-grb": "WS2812 GRB",
			"ws2812-rgb": "WS2812 RGB",
			"ws2812-rbg": "WS2812 RBG",
			"sk6812-brgw": "SK6812 RGBW",
			"save": "Guardar ajustes"
		},
		"whitetype": {
			"h2": "Tipo de LED blanco",
			"in-border": "Configuración del tipo de LED blanco de la tira RGBW",
			"ww": "Blanco cálido",
			"nw": "Blanco neutro",
			"cw": "Blanco frío"
		},
		"time-server": {
			"h2": "Servidor de tiempo",
			"label": "Servidor de tiempo",
			"save": "Guardar ajustes"
		},
		"manual-time": {
			"h2": "Establecer la hora manualmente",
			"time": "Tiempo",
			"save": "Guardar ajustes"
		},
		"brightness": {
			"h2": "Brillo",
			"mode": "Modo",
			"automatic": "Automáticamente",
			"manual": "Manualmente",
			"p100": "100 %",
			"p80": "80 %",
			"p60": "60 %",
			"p40": "40 %",
			"p20": "20 %",
			"off": "Aus",
			"zero-to-six": "0:00 – 5:59",
			"six-to-eight": "6:00 – 7:59",
			"eight-to-twelve": "8:00 – 11:59",
			"twelve-to-sixteen": "12:00 – 15:59",
			"sixteen-to-eighteen": "16:00 – 17:59",
			"eighteen-to-twenty": "18:00 – 19:59",
			"twenty-to-twenty-two": "20:00 – 21:59",
			"twenty-two-to-zero": "22:00 – 23:59",
			"ldr-value": "Brillo actual del fotorresistor (LDR)",
			"value-bright": "Valor „Brillante“ (0 – 255)",
			"value-dark": "Valor „Oscuro“ (0 – 255)"
		},
		"hostname": {
			"h2": "Hostname",
			"label": "Hostname",
			"save": "Guardar ajustes"
		},
		"weather": {
			"h2": "Clima",
			"api-key": "OpenWeatherMap API-Schlüssel",
			"city-id": "OpenWeatherMap City-ID",
			"save": "Guardar ajustes"
		},
		"wifi": {
			"h2": "Wi-Fi / WLAN",
			"help": "La reconfiguración crea su propia WLAN.\n" +
				"Conéctese a esto y luego se podrá seleccionar la red de destino.",
			"ssid": "WLAN-Name (SSID)",
			"other-wifi": "Otros WiFi",
			"configure": "Configurar",
			"until-restart": "Hasta el reinicio",
			"deactivate": "Apagar"
		},
		"restart": {
			"h2": "Reiniciar",
			"reset": "Empezar de nuevo",
			"restart": "Restablecer los valores predeterminados"
		}
	},

	"smart-home": {
		"h1": "Smart Home",
		"mqtt": {
			"h2": "MQTT",
			"text": "El software Word Clock ofrece la posibilidad de control a través de la interfaz MQTT. Puede integrar el reloj en la configuración de su asistente doméstico existente para poder controlar funciones rudimentarias. Esta función aún es experimental y se amplía constantemente.",
			"activate": "Habilitar MQTT",
			"server": "Dirección del servidor",
			"port": "Port",
			"username": "Nombre de usuario",
			"password": "Contraseña",
			"client-id": "Client-ID",
			"topic": "Tema",
			"save": "Guardar ajustes"
		}
	},

	"about": {
		"h1": "Acerca de",
		"text-general": "El software de este reloj se basa en el reloj de <a href='https://web.archive.org/web/20180422160812/http://www.ulrichradig.de/home/index.php/projekte/wort-uhr-neu'>Ulrich Radig (Stand 2019)</a> y lo haré ahora <a href='https://github.com/ESPWortuhr/Wortuhr/graphs/contributors'>de la comunidad</a> desarrollado adicionalmente <a href='https://github.com/ESPWortuhr/Wortuhr'>GitHub</a>.",
		"text-update": "Es posible realizar una actualización del software, hágalo. <a href='update' onmouseover='javascript:event.target.port=81'>Ir a la página de actualización</a>.",
		"text-license": "Este proyecto es de código abierto con licencia bajo el <span lang='en'>3-Clause BSD License</span>.",
		"software": "Bibliotecas y software",
		"debug": "Salida de Debug"
	},
	"footer": {
		"version": "Versión"
	}
};
