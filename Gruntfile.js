module.exports = function(grunt) {

	grunt.initConfig({

		settings: {
			distDirectory: "dist",
			tempDirectory: "temp",
			srcDirectory: "source/webinterface/"
		},

		clean: {
			temp: ["<%= settings.tempDirectory %>"],
			dist: ["<%= settings.distDirectory %>"]
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
					dest: "<%= settings.tempDirectory %>/"
				}]
			}
		},

		uglify: {
			options: {
				beautify: false,
				mangle: true, // sadly has no effect https://github.com/gruntjs/grunt-contrib-uglify/issues/65
				compress: true
			},
			dev: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>",
					src: ["*.js", "!*.min.js"],
					dest: "<%= settings.tempDirectory %>/"
				}]
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
					cwd: "<%= settings.srcDirectory %>",
					src: ["*.min.js"],
					dest: "<%= settings.tempDirectory %>/"
				}]
			},
			minified_css_files: {
				files: [{
					expand: true,
					cwd: "<%= settings.srcDirectory %>",
					src: ["*.min.css"],
					dest: "<%= settings.tempDirectory %>/"
				}]
			}
		},

		assets_inline: {
			dev: {
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
	grunt.loadNpmTasks("grunt-contrib-uglify");

	// Other
	grunt.loadNpmTasks("grunt-assets-inline");
	grunt.loadNpmTasks("grunt-contrib-copy");
	grunt.loadNpmTasks("grunt-contrib-clean");

	grunt.registerTask("lint", [
		"eslint",
		"csslint",
		"htmllint"
	]);

	// tasks
	grunt.registerTask("build", [
		"lint",
		"clean:dist",
		"cssmin",
		"copy:minified_css_files",
		"uglify",
		"copy:minified_js_files",
		"copy:index",
		"assets_inline",
		"htmlmin",
		"copy:from_temp_to_dist",
		"clean:temp"
	]);

	grunt.registerTask("default", [
		"build"
	]);
};
