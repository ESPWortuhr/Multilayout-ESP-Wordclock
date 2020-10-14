#!/usr/bin/python
#	 pip install requests
import requests
import os

input_dir = "HTML"              # Sub folder of webfiles
output_dir = "HTML_minified"

f_output = open("output_dir", "w")
URL_minify_js   = 'https://javascript-minifier.com/raw' # Website to minify javascript
URL_minify_html = 'https://html-minifier.com/raw'        # Website to minify html
URL_minify_css  = 'https://cssminifier.com/raw'         # Website to minify css

def write_to_file(file, data, dir=""):
    filename, file_extension = os.path.splitext(file)       # Split filename and file extension
    file_extension = file_extension.replace(".","")         # Remove puncuation in file extension

    dir = dir.replace(input_dir,"")                         # Remove the first directory(input_dir)
    dir = dir.replace("\\","/")                             # Chang to /
    f_output.write("// " + dir + "\n")                      # Print comment
    f_output.write("const char* data_" + filename + "_" + file_extension + "_path PROGMEM = \""+str(dir)+"\";\n")    # print path
    f_output.write("const char data_"+filename+"_"+file_extension+"[] PROGMEM = {"+data.upper()+"};\n\n")            # print binary data

    # f_output.write("#define data_" + filename + "_len " + str(data.count('0x')) +"\n")

def aschii2Hex(text):
    output_str = ""
    x = 1
    strLen = len(text)
    for character in text:
        output_str += hex(ord(character))

        if (x != strLen):
            output_str += ","
        x += 1
    return output_str

def minify_js(input_file):
    url = URL_minify_js
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    return response.text

def addMinifiedMinJsTo(html_head):
    count = html_head.find("src=minified.min.js")
    lenght = len("src=minified.min.js")
    html_head = html_head[0:count -1] +">" + minified_min_js + html_head[count+lenght+1:len(html_head)]
    return html_head

def addScriptJsTo(html_head):
    count = html_head.find("src=script.js")
    lenght = len("src=script.js")
    html_head = html_head[0:count -1] +">" + script_js + html_head[count+lenght+1:len(html_head)]
    return html_head

def addPureMinCssTo(html_head):
    count = html_head.find("<link rel=stylesheet href=pure.min.css>")
    lenght = len("<link rel=stylesheet href=pure.min.css>")
    html_head = html_head[0:count -1] +"<style>" + pure_min_css + "</style>" + html_head[count+lenght+1:len(html_head)]
    return html_head

def addStyleCssTo(html_head):
    count = html_head.find("<link rel=stylesheet href=style.css>")
    lenght = len("<link rel=stylesheet href=style.css>")
    html_head = html_head[0:count] + "<style>" + style_css + "</style>" + html_head[count + lenght + 1:len(html_head)]
    return html_head

def minify_html(input_file):
    url = URL_minify_html
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    count = response.text.find("<body")
    return response.text[0:count-1]

def minify_html_orginal(input_file):
    url = URL_minify_html
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    count = response.text.find("<body")
    return response.text[count-1:len(response.text)]

def minify_css(input_file):
    url = URL_minify_css
    data = {'input': open(input_file, 'rb').read()}
    response = requests.post(url, data=data)
    return response.text


for root, dirs, files in os.walk(input_dir, topdown=False):
    for name in files:   # for files

        if name.endswith("index.html"):
            print(os.path.join(root, name))
            html_head = minify_html(os.path.join(root, name))        # minify html
            html_head = addMinifiedMinJsTo(html_head)
            html_head = addScriptJsTo(html_head)
            html_head = addPureMinCssTo(html_head)
            html_head = addStyleCssTo(html_head)
            html_body = minify_html_orginal(os.path.join(root, name))        # minify html
            #hexified = aschii2Hex(minified)                         # convert to hex
            write_to_file(name, html_head, os.path.join(root, name)) # write to file
            write_to_file(name, html_body, os.path.join(root, name))  # write to file

        elif name.endswith("minified_min.js"):
            text = os.path.join(root, name)
            print(os.path.join(root, name))
            minified_min_js = minify_js(os.path.join(root, name))  # minify javascript
            #hexified = aschii2Hex(minified)  # convert to hex

        elif name.endswith("script.js"):
            text = os.path.join(root, name)
            print(os.path.join(root, name))
            script_js = minify_js(os.path.join(root, name))  # minify javascript
            #hexified = aschii2Hex(minified)  # convert to hex

        elif name.endswith("style.css"):
            print(os.path.join(root, name))
            style_css = minify_css(os.path.join(root, name))         # minify css
            #hexified = aschii2Hex(minified)                         # convet to hex

        elif name.endswith("pure_min.css"):
            print(os.path.join(root, name))
            pure_min_css = minify_css(os.path.join(root, name))  # minify css
            #hexified = aschii2Hex(minified)  # convet to hex


f_output.close()