#!/usr/bin/python
#	 pip install requests
import requests
import os

input_dir = "WebPageSource"              # Sub folder of webfiles
output_dir = "WebPageWortuhr.h"          # Source C++ Header

URL_minify_js   = 'https://javascript-minifier.com/raw' # Website to minify javascript
URL_minify_css  = 'https://cssminifier.com/raw'         # Website to minify css

def write_to_file():
    source_file = open(output_dir, 'rb').read()
    source_data = source_file.decode("UTF-8")
    count = source_data.find("/* The following CODE is created by the Python Script Convert.py in the Source Folder */")
    lenght = len("/* The following CODE is created by the Python Script Convert.py in the Source Folder */")

    f_output = open(output_dir, "w")
    f_output.write(source_data[0:count+lenght+1] + "\nconst char index_html_head[] PROGMEM= R\"=====("+html_head+")=====\";\n\n/* End for CODE generation by Script */")            # print binary data
    f_output.close()

def minify_js(input_file):
    url = URL_minify_js
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    return response.text

def addMinifiedMinJsTo(html_head):
    count = html_head.find("src=\"minified.min.js\"")
    lenght = len("src=\"minified.min.js\"")
    html_head = html_head[0:count -1] +">" + minified_min_js + html_head[count+lenght+1:len(html_head)]
    return html_head

def addScriptJsTo(html_head):
    count = html_head.find("src=\"script.js\"")
    lenght = len("src=\"script.js\"")
    html_head = html_head[0:count -1] +">" + script_js + html_head[count+lenght+1:len(html_head)]
    return html_head

def addPureMinCssTo(html_head):
    count = html_head.find("<link rel=\"stylesheet\" href=\"pure.min.css\">")
    lenght = len("<link rel=\"stylesheet\" href=\"pure.min.css\">")
    html_head = html_head[0:count -1] +"<style>" + pure_min_css + "</style>" + html_head[count+lenght+1:len(html_head)]
    return html_head

def addStyleCssTo(html_head):
    count = html_head.find("<link rel=\"stylesheet\" href=\"style.css\">")
    lenght = len("<link rel=\"stylesheet\" href=\"style.css\">")
    html_head = html_head[0:count] + "<style>" + style_css + "</style>" + html_head[count + lenght + 1:len(html_head)]
    return html_head

def minify_html(input_file):
    data = open(input_file, 'rb').read()
    data = data.decode("utf-8")
    count = data.find("<body")
    return data[0:count-1]

def minify_css(input_file):
    url = URL_minify_css
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    return response.text


for root, dirs, files in os.walk(input_dir, topdown=False):
    for name in files:   # for files

        if name.endswith("index.html"):
            print(os.path.join(root, name))
            html_head = minify_html(os.path.join(root, name))
            html_head = addMinifiedMinJsTo(html_head)
            html_head = addScriptJsTo(html_head)
            html_head = addPureMinCssTo(html_head)
            html_head = addStyleCssTo(html_head)
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

        elif name.endswith("pure.min.css"):
            print(os.path.join(root, name))
            pure_min_css = minify_css(os.path.join(root, name))  # minify css

