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
					collapseWhitespace: true
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
					process: function(content) {
						let gen = "";
						gen += "// generated file -- do not modify\n";
						gen += "// change *.html/*.css/*.js files instead\n\n";
						gen += "const char html_code[] PROGMEM = R\"=====(\n";
						gen += content + "\n";
						gen += ")=====\";\n";
						gen += "const uint32_t html_size = sizeof(html_code);\n";
						return gen;
					}
				},
				src: "<%= settings.tempDirectory %>/index.html",
				dest: "<%= settings.target %>"
			}
		},

		assets_inline: {
			dev: {
				options: {
					assetsDir: "<%= settings.tempDirectory %>/"
				},
				files: {
					"<%= settings.tempDirectory %>/index.html": "<%= settings.tempDirectory %>/index.html"
				}
			}
		}

	});

	// Linters
	grunt.loadNpmTasks("grunt-stylelint");
	grunt.loadNpmTasks("grunt-eslint");
	grunt.loadNpmTasks("grunt-htmllint");

	// Minifiers
	grunt.loadNpmTasks("grunt-contrib-htmlmin");
	grunt.loadNpmTasks("grunt-contrib-cssmin");
	grunt.loadNpmTasks("grunt-terser");

	// Other
	grunt.loadNpmTasks("grunt-assets-inline");
	grunt.loadNpmTasks("grunt-contrib-copy");
	grunt.loadNpmTasks("grunt-contrib-clean");
	grunt.loadNpmTasks("grunt-version");
	grunt.loadNpmTasks("grunt-replace");

	grunt.registerTask("lint", [
		"eslint",
		"stylelint",
		"htmllint"
	]);

	// tasks
	grunt.registerTask("build", [
		"lint",
		"clean:temp",
		"cssmin",
		"copy:minified_css_files",
		"terser",
		"copy:minified_js_files",
		"copy",
		"copy:index",
		"version:index",
		"replace:pioenv",
		"assets_inline",
		"htmlmin",
		"copy:html_to_h"
	]);

	grunt.registerTask("default", [
		"build"
	]);
};
