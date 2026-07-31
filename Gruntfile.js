/* global process */

module.exports = function(grunt) {

	grunt.initConfig({

		settings: {
			tempDirectory: ".pio/build/webpage",
			srcDirectory: "webpage",
			target: "include/WebPageContent.gen.inc"
		},

		clean: {
			temp: ["<%= settings.tempDirectory %>"]
		},

		eslint: {
			options: {
				overrideConfigFile: "eslintrc.json"
			},
			files: [
				"Gruntfile.js",
				"<%= settings.srcDirectory %>/script.js",
				"<%= settings.srcDirectory %>/i18n.js",
				"<%= settings.srcDirectory %>/language/*.js"
			]
		},

		stylelint: {
			all: ["<%= settings.srcDirectory %>/style.css"]
		},

		htmllint: {
			files: ["<%= settings.srcDirectory %>/index.html"],
			options: {
				"id-class-style": "dash",
				"attr-bans": ["align", "background", "bgcolor", "border", "frameborder", "longdesc", "marginwidth", "marginheight", "scrolling", "width"],
				"line-end-style": false,
				"attr-name-ignore-regex": "[viewBox|gradientUnits]",
				"tag-name-lowercase": false
			}
		},

		htmlmin: {
			dev: {
				options: {
					removeComments: true,
					collapseWhitespace: true,
					removeRedundantAttributes: true,
					removeScriptTypeAttributes: true,
					removeStyleLinkTypeAttributes: true,
					minifyJS: true,
					minifyCSS: true
				},
				files: {
					"<%= settings.tempDirectory %>/index.html": "<%= settings.tempDirectory %>/index.html"
				}
			}
		},

		cssmin: {
			dev: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>",
					src: ["*.css", "!*.min.css"],
					dest: "<%= settings.tempDirectory %>"
				}]
			}
		},

		terser: {
			dev: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>",
					src: ["*.js", "!*.min.js"],
					dest: "<%= settings.tempDirectory %>"
				},
				{
					expand: true,
					cwd: "<%= settings.srcDirectory %>/language/",
					src: ["**/*"],
					dest: "<%= settings.tempDirectory %>/language/"
				}]
			}
		},

		version: {
			index: {
				options: {
					prefix: "<span id=\"version\" class=\"version\">"
				},
				src: "<%= settings.tempDirectory %>/index.html"
			}
		},

		replace: {
			// The <option> list of the front layout select is generated from
			// CLOCK_TYPES_LIST in include/WordClockState.h, so that a new clock
			// type only has to be registered in that single place.
			frontlayout: {
				options: {
					patterns: [{
						match: /<!-- FRONTLAYOUTPLACEHOLDER -->/,
						replacement: function() {
							const fs = require("fs");
							// Join the macro line continuations first, otherwise an
							// entry wrapped by clang-format would be missed.
							const header = fs.readFileSync("include/WordClockState.h", "utf8").replace(/\\\s*\n\s*/g, " ");
							const config = fs.readFileSync("include/Config.h", "utf8");

							const defaultLayout = (config.match(/^#define\s+DEFAULT_LAYOUT\s+(\w+)/m) || [])[1];
							const entry = /X\(\s*(\w+)\s*,\s*(\d+)\s*,\s*\w+\s*,\s*"([\w-]+)"\s*\)/g;

							let options = [];
							let match;
							while ((match = entry.exec(header)) !== null) {
								const [, name, id, key] = match;
								const selected = name === defaultLayout ? " selected" : "";
								options.push("<option value=\"" + id + "\" data-i18next=\"view.front." + key + "\"" + selected + "></option>");
							}

							if (options.length === 0) {
								throw new Error("No clock types found in include/WordClockState.h");
							}
							grunt.log.writeln("Generated " + options.length + " front layout options");
							return options.join("\n");
						}
					}]
				},
				files: [{
					expand: true,
					overwrite: true,
					src: "<%= settings.tempDirectory %>/index.html"
				}]
			},
			pioenv: {
				options: {
					patterns: [{
						match: /PIOENVIRONMENTPLACEHOLDER/g,
						replacement: process.env.PIO_ENV_NAME || "unknown"
					}]
				},
				files: [{
					expand: true,
					overwrite: true,
					src: "<%= settings.tempDirectory %>/index.html"
				}]
			}
		},

		copy: {
			index: {
				src: "<%= settings.srcDirectory %>/index.html",
				dest: "<%= settings.tempDirectory %>/index.html"
			},
			icons: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>/icons/",
					src: ["*.svg"],
					dest: "<%= settings.tempDirectory %>/icons/"
				}]
			},
			minified_js_files: {
				files: [{
					expand: true,
					cwd: "node_modules/minified/",
					src: ["minified-web.js"],
					dest: "<%= settings.tempDirectory %>"
				}, {
					expand: true,
					cwd: "node_modules/@jaames/iro/dist/",
					src: ["iro.min.js"],
					dest: "<%= settings.tempDirectory %>"
				}, {
					expand: true,
					cwd: "node_modules/i18next",
					src: ["i18next.min.js"],
					dest: "<%= settings.tempDirectory %>"
				}, {
					expand: true,
					cwd: "node_modules/i18next-browser-languagedetector",
					src: ["i18nextBrowserLanguageDetector.min.js"],
					dest: "<%= settings.tempDirectory %>"
				}]
			},
			minified_css_files: {
				files: [{
					expand: true,
					cwd: "node_modules/purecss/build",
					src: ["*-min.css"],
					dest: "<%= settings.tempDirectory %>"
				}]
			},
			html_to_h: {
				options: {
					process: function(content, srcpath) {
						const fs = require("fs");
						const buffer = fs.readFileSync(srcpath);
						let hexValues = [];
						for (let i = 0; i < buffer.length; i++) {
							hexValues.push("0x" + buffer[i].toString(16).padStart(2, "0"));
						}

						let gen = "// generated file -- do not modify\n";
						gen += "#include <stdint.h>\n";
						gen += "#include <pgmspace.h>\n\n";
						gen += "const uint8_t html_code[] PROGMEM = {\n  ";
						gen += hexValues.join(", ");
						gen += "\n};\n";
						gen += "const uint32_t html_size = " + buffer.length + ";\n";
						return gen;
					},
					noProcess: ["**/*.gz"]
				},
				src: "<%= settings.tempDirectory %>/index.html.gz",
				dest: "<%= settings.target %>"
			}
		},

		assets_inline: {
			dev: {
				options: { assetsDir: "<%= settings.tempDirectory %>/" },
				files: { "<%= settings.tempDirectory %>/index.html": "<%= settings.tempDirectory %>/index.html" }
			}
		},

		compress: {
			main: {
				options: { mode: "gzip", level: 9 },
				files: [{
					expand: true,
					cwd: "<%= settings.tempDirectory %>",
					src: ["index.html"],
					dest: "<%= settings.tempDirectory %>",
					ext: ".html.gz"
				}]
			}
		}
	});

	// Linters
	grunt.loadNpmTasks("grunt-stylelint");
	grunt.loadNpmTasks("grunt-eslint");
	grunt.loadNpmTasks("grunt-htmllint");
	grunt.loadNpmTasks("grunt-contrib-htmlmin");
	grunt.loadNpmTasks("grunt-contrib-cssmin");
	grunt.loadNpmTasks("grunt-terser");
	grunt.loadNpmTasks("grunt-assets-inline");
	grunt.loadNpmTasks("grunt-contrib-copy");
	grunt.loadNpmTasks("grunt-contrib-clean");
	grunt.loadNpmTasks("grunt-version");
	grunt.loadNpmTasks("grunt-replace");
	grunt.loadNpmTasks("grunt-contrib-compress");

	grunt.registerTask("lint", ["eslint", "stylelint", "htmllint"]);

	grunt.registerTask("build", [
		"lint",
		"clean:temp",
		"cssmin",
		"copy:minified_css_files",
		"terser",
		"copy:minified_js_files",
		"copy:index",
		"copy:icons",
		"version:index",
		"replace:frontlayout",
		"replace:pioenv",
		"assets_inline",
		"htmlmin",
		"compress",
		"copy:html_to_h"
	]);

	grunt.registerTask("default", ["build"]);
};
