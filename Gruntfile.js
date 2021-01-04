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
				configFile: "eslintrc.json"
			},
			files: ["Gruntfile.js", "<%= settings.srcDirectory %>/script.js"]
		},

		csslint: {
			files: ["<%= settings.srcDirectory %>/style.css"],
			options: {
				"ids": false,
				"order-alphabetical": false,
				"fallback-colors": false,
				"box-sizing": false,
				"adjoining-classes": false,
				"qualified-headings": false,
				"unique-headings": false,
				"box-model": false
			}
		},

		htmllint: {
			files: ["<%= settings.srcDirectory %>/index.html"],
			options: {
				"id-class-style": "dash",
				"attr-bans": ["align", "background", "bgcolor", "border", "frameborder", "longdesc", "marginwidth", "marginheight", "scrolling", "width"]
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
				}]
			}
		},

		version: {
			index: {
				options: {
					prefix: "<span class=\"version\">"
				},
				src: "<%= settings.tempDirectory %>/index.html"
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
						var gen = "";
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
	grunt.loadNpmTasks("grunt-contrib-csslint");
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

	grunt.registerTask("lint", [
		"eslint",
		"csslint",
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
		"copy:index",
		"version:index",
		"assets_inline",
		"htmlmin",
		"copy:html_to_h"
	]);

	grunt.registerTask("default", [
		"build"
	]);
};
