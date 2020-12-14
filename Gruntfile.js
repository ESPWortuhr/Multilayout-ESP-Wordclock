module.exports = function(grunt) {

	grunt.initConfig({

		settings: {
			distDirectory: "dist",
			tempDirectory: "temp",
			srcDirectory: "Wortuhr/WebPageSource/",
			target: "Wortuhr/WebPageWortuhr.h"
		},

		clean: {
			temp: ["<%= settings.tempDirectory %>"],
			dist: ["<%= settings.distDirectory %>"]
		},

		eslint: {
			options: {
				configFile: "eslintrc.json"
			},
			files: ["Gruntfile.js", "<%= settings.srcDirectory %>/JavaScript/script.js"]
		},

		csslint: {
			files: ["<%= settings.srcDirectory %>/CSS/style.css"],
			options: {
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
					cwd: "<%= settings.srcDirectory %>/CSS",
					src: ["*.css", "!*.min.css"],
					dest: "<%= settings.tempDirectory %>/CSS"
				}]
			}
		},

		terser: {
			dev: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>/JavaScript",
					src: ["*.js", "!*.min.js"],
					dest: "<%= settings.tempDirectory %>/JavaScript/"
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
			from_temp_to_dist: {
				src: "<%= settings.tempDirectory %>/index.html",
				dest: "<%= settings.distDirectory %>/index.html"
			},
			index: {
				src: "<%= settings.srcDirectory %>/index.html",
				dest: "<%= settings.tempDirectory %>/index.html"
			},
			minified_js_files: {
				files: [{
					expand: true,
					cwd: "node_modules/minified/",
					src: ["minified-web.js"],
					dest: "<%= settings.tempDirectory %>/JavaScript/"
				}]
			},
			minified_css_files: {
				files: [{
					expand: true,
					cwd: "node_modules/purecss/build",
					src: ["*-min.css"],
					dest: "<%= settings.tempDirectory %>/CSS/"
				}]
			},
			html_to_h: {
				options: {
					process: function(content, srcpath) {
						return    "// generated file -- do not modify\n"
							+ "// change *.html/*.css/*.js instead\n\n"
							+ "const char html_code[] PROGMEM = R\"=====(\n"
							+ content + "\n"
							+ ")=====\";\n"
							+ "const uint32_t html_size = sizeof(html_code);\n";
					}
				},
				src: "<%= settings.distDirectory %>/index.html",
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
		//"lint",
		"clean:dist",
		"cssmin",
		"copy:minified_css_files",
		"terser",
		"copy:minified_js_files",
		"copy:index",
		"version:index",
		"assets_inline",
		"htmlmin",
		"copy:from_temp_to_dist",
		"copy:html_to_h",
		"clean:temp"
	]);

	grunt.registerTask("default", [
		"build"
	]);
};
