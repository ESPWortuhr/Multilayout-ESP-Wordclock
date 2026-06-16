
# -*- coding: utf-8 -*-

import re
import os
import subprocess
import random 
import xml.etree.ElementTree as ET 

# --- Funktion zum Parsen des Letter_grid aus der .hpp-Datei ---
def parse_letter_grid_from_hpp(filepath):
    """
    Liest die angegebene .hpp-Datei und extrahiert das Buchstaben-Layout
    aus dem Kommentarbereich.
    """
    letter_grid = []
    in_grid_section = False
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                if 'Layout Front' in line:
                    in_grid_section = True
                    continue
                
                if in_grid_section:
                    if '*/' in line or 'class' in line:
                        in_grid_section = False
                        break
                    
                    match = re.match(r'^\s*\*\s*\d+\s*\|\s*(.*)', line)
                    if match:
                        grid_line = ' '.join(match.group(1).strip().split())
                        letter_grid.append(grid_line)
            
    except FileNotFoundError:
        print(f"Fehler: Die Datei '{filepath}' wurde nicht gefunden.")
        return None
    except Exception as e:
        print(f"Ein Fehler ist beim Lesen der Datei aufgetreten: {e}")
        return None
    
    return letter_grid

# --- Funktion zum Generieren einer zufälligen, dezenten Farbe ---
def generate_random_subtle_color():
    """
    Generiert eine zufällige Hexadezimal-Farbe, die dezent und nicht zu hell ist.
    Die RGB-Werte liegen im Bereich 50-150, um dunklere, aber nicht schwarze Farben zu erhalten.
    """
    r = random.randint(50, 150)
    g = random.randint(50, 150)
    b = random.randint(50, 150)
    
    # Konvertiere RGB in Hex-Format
    return f'#{r:02x}{g:02x}{b:02x}'

# --- Funktion zum Generieren der SVG-Dateien ---
def generate_svg_files(hpp_filepath, letter_grid):
    """
    Generiert die SVG-Dateien (Text und Pfade) für ein gegebenes letter_grid.
    """
    # --- Konfiguration ---
    spacing_x = 63
    spacing_y = 69.9213
    textSize = 40
    text_id_counter = 65

    # --- Ausgabedateinamen generieren ---
    dir_name, base_name = os.path.split(hpp_filepath)
    file_name_without_extension, _ = os.path.splitext(base_name)

    svg_output_filename_text = f'{file_name_without_extension}.svg'
    svg_output_filepath_text = os.path.join(dir_name, svg_output_filename_text)

    svg_output_filename_paths = f'{file_name_without_extension}_paths.svg'
    svg_output_filepath_paths = os.path.join(dir_name, svg_output_filename_paths)

    print(f"\nVerarbeite: {hpp_filepath}")

    # Generiere eine zufällige Hintergrundfarbe für diese Datei
    background_color = generate_random_subtle_color()
    print(f"  Verwende Hintergrundfarbe: {background_color}") 

    # --- SVG Dimensionen und ViewBox für Ursprung in der Mitte ---
    svg_width = 1133.8582
    svg_height = 1133.8583
    
    # Die ersten beiden Werte des viewBox werden auf -Breite/2 und -Höhe/2 gesetzt
    # Dies verschiebt den Ursprung (0,0) in die Mitte des Viewports
    viewBox_x_offset = -svg_width / 2
    viewBox_y_offset = -svg_height / 2
    
    # Dies ist der neue viewBox-String
    viewBox_str = f"{viewBox_x_offset} {viewBox_y_offset} {svg_width} {svg_height}"
    
    # Anzahl der Spalten und Reihen im Layout
    num_cols = len(re.split(r'\s+', letter_grid[0].strip())) if letter_grid else 0
    num_rows = len(letter_grid) 
    
    if num_cols == 0:
        print(f"Warnung: Keine Spalten im Letter_grid für '{hpp_filepath}' gefunden. Überspringe.")
        return

    # Neue Startkoordinaten, um das gesamte Layout im zentrierten SVG zu platzieren
    adjusted_startX = -(num_cols * spacing_x / 2) + (spacing_x / 2)
    adjusted_startY = -(num_rows * spacing_y / 2) + (spacing_y / 2)

    # Öffnet die erste SVG-Datei zum Schreiben (Text als Text)
    with open(svg_output_filepath_text, 'w', encoding='utf-8') as file_object:
        # --- SVG-Header schreiben mit angepasstem viewBox und ohne translate im g-Tag ---
        file_object.write(f'''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg
   xmlns:dc="http://purl.org/dc/elements/1.1/"
   xmlns:cc="http://creativecommons.org/ns#"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:svg="http://www.w3.org/2000/svg"
   xmlns="http://www.w3.org/2000/svg"
   xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd"
   xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
   enable-background="new 0 0 1726.3 861.732"
   height="300mm"
   viewBox="{viewBox_str}"
   width="300mm"
   version="1.1"
   id="svg918"
   inkscape:version="0.92.4 (5da689c313, 2019-01-14)"
   style="background-color:{background_color};">
  <metadata
     id="metadata924">
    <rdf:RDF>
      <cc:Work
         rdf:about="">
        <dc:format>image/svg+xml</dc:format>
        <dc:type
           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />
        <dc:title></dc:title>
      </cc:Work>
    </rdf:RDF>
  </metadata>
  <defs
     id="defs922" />
  <sodipodi:namedview
     pagecolor="#ffffff"
     bordercolor="#666666"
     borderopacity="1"
     objecttolerance="10"
     gridtolerance="10"
     guidetolerance="10"
     inkscape:pageopacity="0"
     inkscape:pageshadow="2"
     inkscape:window-width="1920"
     inkscape:window-height="1109"
     id="namedview920"
     showgrid="true"
     inkscape:zoom="0.31115"
     inkscape:cx="-90.764306"
     inkscape:cy="1012.9005"
     inkscape:window-x="1920"
     inkscape:window-y="0"
     inkscape:window-maximized="1"
     inkscape:current-layer="g916"
     inkscape:document-units="mm" />
  <g id="g916">
    ''')

        # --- Buchstaben-Gitter generieren ---
        for row_index, row_text in enumerate(letter_grid):
            letters = re.split(r'\s+', row_text.strip()) 
            
            for col_index, letter in enumerate(letters):
                center_x = adjusted_startX + (col_index * spacing_x)
                center_y = adjusted_startY + (row_index * spacing_y)
                
                text_id_counter += 1 
                
                file_object.write(f'''    <text
                xml:space="preserve"
                style="font-style:normal;font-weight:normal;font-size:{textSize}px;line-height:1.25;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;text-anchor:middle;dominant-baseline:middle;"
                x="{center_x}"
                y="{center_y}"
                id="text{text_id_counter}"><tspan
                  sodipodi:role="line"
                  id="tspan{text_id_counter}"
                  x="{center_x}"
                  y="{center_y}">{letter}</tspan></text>
''')

        # --- SVG-Footer schreiben ---
        file_object.write("""  </g>\n</svg>""")

    print(f"  -> '{svg_output_filepath_text}' (Text als Textobjekte) wurde erfolgreich erstellt.")

    # --- Konvertiere Text in Pfade mit rsvg-convert ---
    # rsvg-convert erhält jetzt eine SVG OHNE den 'path2'-Hintergrund.
    command = ['rsvg-convert', '-f', 'svg', '-o', svg_output_filepath_paths, svg_output_filepath_text]

    try:
        result = subprocess.run(command, check=True, capture_output=True, text=True, encoding='utf-8')
        print(f"  -> '{svg_output_filepath_paths}' (Text als Pfade) wurde erfolgreich erstellt.")
        
        # --- NACHBEARBEITUNG der _paths.svg für korrekte Zentrierung und KEINEN Hintergrund ---
        try:
            tree = ET.parse(svg_output_filepath_paths)
            root = tree.getroot()

            # Registriere alle relevanten Namensräume
            ET.register_namespace('', "http://www.w3.org/2000/svg")
            ET.register_namespace('sodipodi', "http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd")
            ET.register_namespace('inkscape', "http://www.inkscape.org/namespaces/inkscape")
            ET.register_namespace('dc', "http://purl.org/dc/elements/1.1/")
            ET.register_namespace('cc', "http://creativecommons.org/ns#")
            ET.register_namespace('rdf', "http://www.w3.org/1999/02/22-rdf-syntax-ns#")

            svg_element = root
            g_element = svg_element.find('{http://www.w3.org/2000/svg}g')
            target_element = g_element if g_element is not None else svg_element

            if target_element is not None:
                # Setzen Sie den viewBox des SVG-Elements auf den gleichen wie zuvor
                svg_element.set('viewBox', viewBox_str)
                
                # KEINE HINTERGRUNDFARBE mehr für _paths.svg
                if 'style' in svg_element.attrib:
                    del svg_element.attrib['style'] # Hintergrundstil entfernen

                # Wenn wir ein <g>-Element haben, löschen wir sein transform-Attribut.
                if g_element is not None and 'transform' in g_element.attrib:
                    del g_element.attrib['transform']
                
                # --- Alle Hintergrund-Rechtecke und path2 entfernen ---
                # Wir suchen nach allen <rect> Elementen und entfernen sie.
                # rsvg-convert könnte einen Standard-Hintergrund als <rect> hinzufügen.
                elements_to_remove = []
                for child in list(target_element): # list() um Fehler bei der Iteration und Modifikation zu vermeiden
                    if child.tag == '{http://www.w3.org/2000/svg}rect' or \
                       (child.tag == '{http://www.w3.org/2000/svg}path' and child.get('id') == 'path2'):
                        elements_to_remove.append(child)
                
                for element in elements_to_remove:
                    target_element.remove(element)

                # Optional: Wenn du sicher sein möchtest, dass es keinen path2 gibt,
                # aber er war ja sowieso nicht in der temporären SVG.
                # Wenn Inkscape doch einen neuen path2 erzeugt, würde das ihn entfernen.
                # path2_element = root.find(".//*[@id='path2']") 
                # if path2_element is not None:
                #    path2_element.getparent().remove(path2_element) # Entferne path2 von seinem Elternteil


                # Speichern Sie die modifizierte Datei
                tree.write(svg_output_filepath_paths, encoding='utf-8', xml_declaration=True)
                print(f"  -> '{svg_output_filepath_paths}' (Pfade, ohne Hintergrund) wurde nachbearbeitet und zentriert.")
            else:
                print(f"  Warnung: Weder '<g>' noch '<svg>' Element in '{svg_output_filepath_paths}' konnte für die Zentrierung verarbeitet werden.")

        except Exception as e:
            print(f"  FEHLER bei der Nachbearbeitung von '{svg_output_filepath_paths}': {e}")
            print(f"  Bitte überprüfen Sie die generierte SVG-Datei manuell.")

    except FileNotFoundError:
        print("\n  FEHLER: 'rsvg-convert' wurde nicht gefunden.")
        print("  Bitte stelle sicher, dass 'librsvg2-bin' (Linux) oder 'librsvg' (macOS/Homebrew) installiert und im PATH verfügbar ist.")
        print("  Auf Windows ist die Installation komplexer; siehe Anweisungen in der vorherigen Antwort.")
    except subprocess.CalledProcessError as e:
        print(f"\n  FEHLER: 'rsvg-convert' konnte die Datei nicht konvertieren: {e.cmd}")
        print(f"  Return Code: {e.returncode}")
        if e.stdout:
            print(f"  Stdout: {e.stdout}")
        if e.stderr:
            print(f"  Stderr: {e.stderr}")
    except Exception as e:
        print(f"\n  Ein unerwarteter Fehler ist aufgetreten: {e}")
# --- Haupt-Logik des Skripts ---
if __name__ == "__main__":
    # <<<<<<< HIER ANPASSEN >>>>>>>
    # Basisverzeichnis, in dem deine .hpp-Dateien liegen
    # Beispiel Windows: base_directory = r'C:\Users\DeinBenutzername\Dokumente\ClockLayouts'
    # Beispiel Linux/macOS: base_directory = '/home/DeinBenutzername/Dokumente/ClockLayouts'
    # Aktuelles Verzeichnis: base_directory = '.'
    base_directory = '/Users/davidp/Wortuhr/include/WordClockTypes' # Setze '.' wenn die .hpp-Dateien im selben Ordner wie das Skript sind

    # Liste der Dateinamen der .hpp-Dateien (ohne Pfad)
    hpp_filenames_to_process = [
        'DE10x11.hpp',
    ]
    # <<<<<<< ENDE DER ANPASSUNG >>>>>>>


    for filename in hpp_filenames_to_process:
        # Voller Pfad zur aktuellen .hpp-Datei
        hpp_full_filepath = os.path.join(base_directory, filename)

        if not os.path.exists(hpp_full_filepath):
            print(f"Überspringe: Die Datei '{hpp_full_filepath}' existiert nicht.")
            continue # Springt zur nächsten Datei in der Liste

        letter_grid = parse_letter_grid_from_hpp(hpp_full_filepath)

        if letter_grid is None or not letter_grid:
            print(f"Überspringe: Das Letter_grid für '{hpp_full_filepath}' konnte nicht geparst werden oder ist leer.")
            continue # Springt zur nächsten Datei

        generate_svg_files(hpp_full_filepath, letter_grid)

    print("\nAlle angegebenen Dateien wurden verarbeitet.")