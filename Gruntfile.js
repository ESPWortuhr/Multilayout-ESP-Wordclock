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
		"version:index",
		"replace:pioenv",
		"assets_inline",
		"htmlmin",
		"compress",
		"copy:html_to_h"
	]);

	grunt.registerTask("default", ["build"]);
};
