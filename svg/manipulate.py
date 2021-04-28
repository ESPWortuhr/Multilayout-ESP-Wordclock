# Open a file with access mode 'a'
file_object = open('UHR114_Front_old.svg', 'a')

startX = 63.0
startY = 0
spacing_x = 16.6667
spacing_y = 18.500
textSize = 40

col = 0
row = 0

textId = 65
tspanId = 67

Text = '''* E S K I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * T G N A C H V O R J M
 * H A L B Q Z W Ö L F P
 * Z W E I N S I E B E N
 * K D R E I R H F Ü N F
 * E L F N E U N V I E R
 * W A C H T Z E H N B X
 * B S E C H S F M U H R
 * M O R G E N X F R Ü H
 * A B E N D M I T T A G
 * N A C H T S C H N E E
 * K L A R W A R N U N G
 * R E G E N W O L K E N
 * O G E W I T T E R B N
 * U N T E R Z Ü B E R K
 * Y M I N U S A N U L L
 * H N Z W A N Z I G J T
 * D R E I ẞ I G O ° C X'''

# Append 'hello' at the end of file
for char_text in range(len(Text)):
    temp = Text[char_text]
    if Text[char_text] != Text[0]:
        if Text[char_text] != Text[1]:
            variable_x_spacing = str(startX + col * spacing_x)
            if Text[char_text] != Text[8]:
                variable_x_spacing = str(startX + col * spacing_x - 2.1)
            file_object.write('''    <text
       xml:space="preserve"
       style="font-style:normal;font-weight:normal;font-size:''' + str(textSize) + '''px;line-height:1.25;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none"
       x="''' + variable_x_spacing + '''mm"
       y="''' + str(startY + row * spacing_y) + '''mm"
       id="text''' + str(textId + col * 2) + '''"><tspan
         sodipodi:role="line"
         id="tspan''' + str(tspanId + col * 2) + '''"
         x="150mm"
         y="5.5902829">''' + Text[char_text] + '''</tspan></text>
''')
            col = col + 1
            if col == 12:
                row = row + 1
                col = 0

file_object.write("""\n</g>\n</svg>""")

# Close the file
file_object.close()
