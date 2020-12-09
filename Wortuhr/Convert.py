#!/usr/bin/python
#	 pip install requests
import requests
import os
from io import open

input_dir = "WebPageSource"              # Sub folder of webfiles
output_dir = "WebPageWortuhr.h"          # Source C++ Header
output_data = []

URL_minify_js   = 'https://javascript-minifier.com/raw' # Website to minify javascript
URL_minify_css  = 'https://cssminifier.com/raw'         # Website to minify css
URL_minify_html = 'https://html-minifier.com/raw'        # Website to minify html

# Get Version String from Uhr.h
source_fileVersion = open("Uhr.h", 'rb').read()
source_fileVersion = source_fileVersion.decode("UTF-8")
count = source_fileVersion.find("const char *VER = ")
lenght = len("const char *VER = ")
source_fileVersion = source_fileVersion[count+lenght+1:count+lenght+6]

# Replace Version in README
source_README= open("../README.md", 'rb').read()
source_README = source_README.decode("UTF-8")
count = source_README.find("<!-- VER_placeholder1 -->")
lenght = len("<!-- VER_placeholder1 -->")
source_README = source_README[0:count+lenght] + source_fileVersion + source_README[count+lenght + 5:len(source_README)]

count = source_README.find("<!-- VER_placeholder2 -->")
lenght = len("<!-- VER_placeholder2 -->")
source_README = source_README[0:count+lenght] + source_fileVersion + source_README[count+lenght + 5:len(source_README)]

# Save Version to README
f_output = open("../README.md", "w")
f_output.write(source_README)            # print binary data
f_output.close()


def write_to_file():
    source_data = open(output_dir, 'r', encoding="utf-8").read()
    count = source_data.find("/* The following CODE is created by the Python Script Convert.py in the Source Folder */")
    lenght = len("/* The following CODE is created by the Python Script Convert.py in the Source Folder */")
    buffer = ""
    n = 0

    for i in output_data:
        buffer = buffer + "\nconst char HTML_Code" + str(n) + "[] PROGMEM= R\"=====(" + i + ")=====\";\n"
        n += 1

    buffer = buffer + "\nconst char* const html_code[] PROGMEM = {"

    SizeOfArrays = "\n\nconst uint32_t HTML_Size[] = {"

    for i in range(0, n):
        if i == n-1:
            buffer = buffer + "HTML_Code" + str(i) + "};"
            SizeOfArrays = SizeOfArrays + "sizeof(HTML_Code" + str(i) + ")};"
        else:
            buffer = buffer + "HTML_Code" + str(i) + ","
            SizeOfArrays = SizeOfArrays + "sizeof(HTML_Code" + str(i) + "),"

    f_output = open(output_dir, "w", encoding="utf-8")
    f_output.write(source_data[0:count+lenght+1] + buffer + SizeOfArrays + "\n\n/* End for CODE generation by Script */")            # print binary data

    f_output.close()

# Minify JavaScript
def minify_js(input_file):
    url = URL_minify_js
    data = {'input': open(input_file, 'r', encoding="utf-8").read()}
    response = requests.post(url, data=data)
    return response.text

# Add Minified Javascript Version to HTML code
def addMinifiedMinJsTo(html):
    count = html.find("src=\"JavaScript/minified.min.js\"")
    lenght = len("src=\"JavaScript/minified.min.js\"")
    html = html[0:count -1] +">" + minified_min_js + html[count+lenght+1:len(html)]
    return html

def addScriptJsTo(html):
    count = html.find("src=\"JavaScript/script.js\"")
    lenght = len("src=\"JavaScript/script.js\"")
    html = html[0:count -1] +">" + script_js + html[count+lenght+1:len(html)]
    return html

def addPureMinCssTo(html):
    count = html.find("<link rel=\"stylesheet\" href=\"CSS/pure-min.css\">")
    lenght = len("<link rel=\"stylesheet\" href=\"CSS/pure-min.css\">")
    html = html[0:count -1] +"<style>" + pure_min_css + "</style>" + html[count+lenght+1:len(html)]
    return html

def addGridsResponsiveMinCssTo(html):
    count = html.find("<link rel=\"stylesheet\" href=\"CSS/grids-responsive-min.css\">")
    lenght = len("<link rel=\"stylesheet\" href=\"CSS/grids-responsive-min.css\">")
    html = html[0:count -1] +"<style>" + grids_responsive_min_css + "</style>" + html[count+lenght+1:len(html)]
    return html

def addStyleCssTo(html):
    count = html.find("<link rel=\"stylesheet\" href=\"CSS/style.css\">")
    lenght = len("<link rel=\"stylesheet\" href=\"CSS/style.css\">")
    html = html[0:count] + "<style>" + style_css + "</style>" + html[count + lenght + 1:len(html)]
    return html

def loadHtmlAndReplaceVersion(input_file):
    data = open(input_file, 'r', encoding="utf-8").read()
    data = data.replace("**VER_placeholder**", source_fileVersion)        # Replace Version in HTML_Code
    return data

def minify_css(input_file):
    url = URL_minify_css
    data = {'input': open(input_file, 'r', encoding="utf-8").read()}
    response = requests.post(url, data=data)
    return response.text

def cutSpecificRGBW(data):
    count_start = data.find("<!-- Specific RGBW Start-->")
    lenght_start = len("<!-- Specific RGBW Start-->")
    count_end = data.find("<!-- Specific RGBW End-->")
    lenght_end = len("<!-- Specific RGBW End-->")

    #Generate First element
    if data[0:count_start] != "":
        output_data.append(data[0:count_start])

    #Cut out RGBW Specific Data
    output_data.append(data[count_start+lenght_start:count_end])

    return data[count_end+lenght_end:len(data)]

def cutSpecificUHR242(data):
    count_start = data.find("<!-- Specific UHR242 Start-->")
    lenght_start = len("<!-- Specific UHR242 Start-->")
    count_end = data.find("<!-- Specific UHR242 End-->")
    lenght_end = len("<!-- Specific UHR242 End-->")

    #Generate First element
    if data[0:count_start] != "":
        output_data.append(data[0:count_start])

    #Cut out UHR242 Specific Data
    output_data.append(data[count_start+lenght_start:count_end])

    return data[count_end+lenght_end:len(data)]

def cutSpecificUHR169(data):
    count_start = data.find("<!-- Specific UHR169 Start-->")
    lenght_start = len("<!-- Specific UHR169 Start-->")
    count_end = data.find("<!-- Specific UHR169 End-->")
    lenght_end = len("<!-- Specific UHR169 End-->")

    #Generate First element
    if data[0:count_start] != "":
        output_data.append(data[0:count_start])

    #Cut out UHR169 Specific Data
    output_data.append(data[count_start+lenght_start:count_end])

    return data[count_end+lenght_end:len(data)]

def cutSpecificUHR114_Alternative(data):
    count_start = data.find("<!-- Specific UHR114_Alternative Start-->")
    lenght_start = len("<!-- Specific UHR114_Alternative Start-->")
    count_end = data.find("<!-- Specific UHR114_Alternative End-->")
    lenght_end = len("<!-- Specific UHR114_Alternative End-->")

    #Generate First element
    if data[0:count_start] != "":
        output_data.append(data[0:count_start])

    #Cut out UHR114_Alternative Specific Data
    output_data.append(data[count_start+lenght_start:count_end])

    return data[count_end+lenght_end:len(data)]

def appendRestOfHTML(data):
    output_data.append(data[0:len(data)])

for root, dirs, files in os.walk(input_dir, topdown=False):
    for name in files:   # for files

        if name.endswith("index.html"):
            print(os.path.join(root, name))
            html = loadHtmlAndReplaceVersion(os.path.join(root, name))
            html = addMinifiedMinJsTo(html)
            html = addScriptJsTo(html)
            html = addPureMinCssTo(html)
            html = addGridsResponsiveMinCssTo(html)
            html = addStyleCssTo(html)
            html = cutSpecificRGBW(html)
            html = cutSpecificUHR242(html)
            html = cutSpecificUHR169(html)
            html = cutSpecificUHR114_Alternative(html)
            appendRestOfHTML(html)
            write_to_file()

        elif name.endswith("minified.min.js"):
            text = os.path.join(root, name)
            print(os.path.join(root, name))
            minified_min_js = minify_js(os.path.join(root, name))  # minify javascript

        elif name.endswith("script.js"):
            text = os.path.join(root, name)
            print(os.path.join(root, name))
            script_js = minify_js(os.path.join(root, name))  # minify javascript

        elif name.endswith("style.css"):
            print(os.path.join(root, name))
            style_css = minify_css(os.path.join(root, name))         # minify css

        elif name.endswith("pure-min.css"):
            print(os.path.join(root, name))
            pure_min_css = minify_css(os.path.join(root, name))  # minify css

        elif name.endswith("grids-responsive-min.css"):
            print(os.path.join(root, name))
            grids_responsive_min_css = minify_css(os.path.join(root, name))  # minify css

