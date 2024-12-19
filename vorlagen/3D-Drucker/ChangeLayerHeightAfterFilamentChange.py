# -*- coding: utf-8 -*-
"""
Created on Fri Nov 24 07:26:01 2023

@author: Sebastian
"""

# Reading a text file to a list
file_path = 'Wortuhr_8x8_v08-Case_DE15min_0.2mm_PLA_MK3_2h17m.gcode'
deltaH = 0.2

with open(file_path) as file:
    lines = file.readlines()

bLayerChange = False

for i in range(len(lines)):
    if "M600" in lines[i]:
        bLayerChange = True
    if bLayerChange == True and not lines[i].startswith(";") and " Z" in lines[i]:
        attr = lines[i].split(" ")
        for j in range(len(attr)):
            if attr[j].startswith("Z"):
                h = float(attr[j].replace("Z", ""))
                h -= deltaH
                attr[j] = "Z" + str(h)
        lines[i] = " ".join(attr)
                
with open(file_path.replace(".gcode", "_edit.gcode"), 'w') as f:
    f.writelines(lines)
