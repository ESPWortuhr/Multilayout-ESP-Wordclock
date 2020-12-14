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
    target="include/WebPageContent.gen.inc",
    source="Gruntfile.js",
    action="node_modules/grunt/bin/grunt build"
)
env.Depends(grunt_build, npm_ci)
env.Depends(grunt_build, Glob("webpage/*"))
