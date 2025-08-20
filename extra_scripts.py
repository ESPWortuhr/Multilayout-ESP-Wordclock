Import("env")

package_json = env.File("package.json")

#
# automatically generate `include/version.h`
#
def build_version_h(target, source, env):
    import json

    version = None
    with open(str(source[0]), "r") as f:
        version = json.load(f)["version"]

    with open(str(target[0]), "w") as h:
        h.write('// Automatically generated -- do not modify\n')
        h.write('// Modify `package.json` instead.\n\n')
        h.write('#define VERSION "' + version + '"\n')

    return None

#
# automatically generate `include/uhrtype.h`
#
def build_uhrtype_h(target, source, env):
    import glob

    with open(str(target[0]), "w") as h:
        h.write('// Automatically generated -- do not modify\n\n')
        for file in glob.glob("include/Uhrtypes/*.hpp"):
            h.write('#include "' + file.replace('include/','') + '"\n')

    return None

env.Command(
    target="include/uhrtype.gen.h",
    source=package_json,
    action=build_uhrtype_h
)

env.Command(
    target="include/version.gen.h",
    source=package_json,
    action=build_version_h
)

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
    action="PIO_ENV_NAME=$PIOENV npx --no-install grunt build"
)
env.Depends(grunt_build, npm_ci)
env.Depends(grunt_build, package_json)
env.Depends(grunt_build, env.Glob("webpage/*"))
