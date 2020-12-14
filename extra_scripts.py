Import("env")

#
# automatically build web page
#

npm_ci = env.Command(
    target="node_modules/.package-lock.json",
    source="package-lock.json",
    action="npm ci --silent"
)

grunt_build = env.Command(
    target="Wortuhr/WebPageWortuhr.h",
    source="Wortuhr/WebPageSource/index.html",
    action="npm exec grunt build"
)
env.Depends(grunt_build, npm_ci)
env.Depends(grunt_build, "Gruntfile.js")
env.Depends(grunt_build, "Wortuhr/WebPageSource/CSS/style.css")
env.Depends(grunt_build, "Wortuhr/WebPageSource/JavaScript/script.js")

env.Depends("$BUILD_DIR/${PROGNAME}.elf", grunt_build)
