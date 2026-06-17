
# -*- coding: utf-8 -*-

import re
import os
import subprocess
import random 
import xml.etree.ElementTree as ET 

# --- Function for parsing letter_grid from the .hpp file ---
def parse_letter_grid_from_hpp(filepath):
    """
    Read the given .hpp file and extract the letter layout from its comment
    block.
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
        print(f"Error: File '{filepath}' was not found.")
        return None
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        return None
    
    return letter_grid

# --- Function for generating a random, subtle color ---
def generate_random_subtle_color():
    """
    Generate a random hex color that is subtle and not too bright.
    RGB values are kept in the 50-150 range to get darker, non-black colors.
    """
    r = random.randint(50, 150)
    g = random.randint(50, 150)
    b = random.randint(50, 150)
    
    # Convert RGB to hex format.
    return f'#{r:02x}{g:02x}{b:02x}'

# --- Function for generating SVG files ---
def generate_svg_files(hpp_filepath, letter_grid):
    """
    Generate SVG files (text and paths) for a given letter_grid.
    """
    # --- Configuration ---
    spacing_x = 63
    spacing_y = 69.9213
    textSize = 40
    text_id_counter = 65

    # --- Generate output filenames ---
    dir_name, base_name = os.path.split(hpp_filepath)
    file_name_without_extension, _ = os.path.splitext(base_name)

    svg_output_filename_text = f'{file_name_without_extension}.svg'
    svg_output_filepath_text = os.path.join(dir_name, svg_output_filename_text)

    svg_output_filename_paths = f'{file_name_without_extension}_paths.svg'
    svg_output_filepath_paths = os.path.join(dir_name, svg_output_filename_paths)

    print(f"\nProcessing: {hpp_filepath}")

    # Generate a random background color for this file.
    background_color = generate_random_subtle_color()
    print(f"  Using background color: {background_color}") 

    # --- SVG dimensions and viewBox with origin in the center ---
    svg_width = 1133.8582
    svg_height = 1133.8583
    
    # Set the first two viewBox values to -width/2 and -height/2.
    # This moves the origin (0,0) to the center of the viewport.
    viewBox_x_offset = -svg_width / 2
    viewBox_y_offset = -svg_height / 2
    
    # This is the new viewBox string.
    viewBox_str = f"{viewBox_x_offset} {viewBox_y_offset} {svg_width} {svg_height}"
    
    # Number of columns and rows in the layout.
    num_cols = len(re.split(r'\s+', letter_grid[0].strip())) if letter_grid else 0
    num_rows = len(letter_grid) 
    
    if num_cols == 0:
        print(f"Warning: No columns found in letter_grid for '{hpp_filepath}'. Skipping.")
        return

    # New start coordinates for placing the full layout in the centered SVG.
    adjusted_startX = -(num_cols * spacing_x / 2) + (spacing_x / 2)
    adjusted_startY = -(num_rows * spacing_y / 2) + (spacing_y / 2)

    # Open the first SVG file for writing text as text objects.
    with open(svg_output_filepath_text, 'w', encoding='utf-8') as file_object:
        # --- Write SVG header with adjusted viewBox and without translate on the g tag ---
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

        # --- Generate letter grid ---
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

        # --- Write SVG footer ---
        file_object.write("""  </g>\n</svg>""")

    print(f"  -> '{svg_output_filepath_text}' (text as text objects) was created successfully.")

    # --- Convert text to paths with rsvg-convert ---
    # rsvg-convert receives an SVG without the 'path2' background.
    command = ['rsvg-convert', '-f', 'svg', '-o', svg_output_filepath_paths, svg_output_filepath_text]

    try:
        result = subprocess.run(command, check=True, capture_output=True, text=True, encoding='utf-8')
        print(f"  -> '{svg_output_filepath_paths}' (text as paths) was created successfully.")
        
        # --- Post-process _paths.svg for correct centering and no background ---
        try:
            tree = ET.parse(svg_output_filepath_paths)
            root = tree.getroot()

            # Register all relevant namespaces.
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
                # Set the SVG element viewBox to the same value as before.
                svg_element.set('viewBox', viewBox_str)
                
                # Remove the background color from _paths.svg.
                if 'style' in svg_element.attrib:
                    del svg_element.attrib['style'] # Remove background style.

                # Remove the transform attribute if a <g> element exists.
                if g_element is not None and 'transform' in g_element.attrib:
                    del g_element.attrib['transform']
                
                # --- Remove all background rectangles and path2 ---
                # Remove all <rect> elements.
                # rsvg-convert could add a default background as a <rect>.
                elements_to_remove = []
                for child in list(target_element): # list() avoids errors while iterating and modifying.
                    if child.tag == '{http://www.w3.org/2000/svg}rect' or \
                       (child.tag == '{http://www.w3.org/2000/svg}path' and child.get('id') == 'path2'):
                        elements_to_remove.append(child)
                
                for element in elements_to_remove:
                    target_element.remove(element)

                # Optional: ensure there is no path2 even if a converter creates one.
                # path2_element = root.find(".//*[@id='path2']") 
                # if path2_element is not None:
                #    path2_element.getparent().remove(path2_element) # Remove path2 from its parent.


                # Save the modified file.
                tree.write(svg_output_filepath_paths, encoding='utf-8', xml_declaration=True)
                print(f"  -> '{svg_output_filepath_paths}' (paths, no background) was post-processed and centered.")
            else:
                print(f"  Warning: Neither '<g>' nor '<svg>' in '{svg_output_filepath_paths}' could be processed for centering.")

        except Exception as e:
            print(f"  ERROR while post-processing '{svg_output_filepath_paths}': {e}")
            print("  Please check the generated SVG file manually.")

    except FileNotFoundError:
        print("\n  ERROR: 'rsvg-convert' was not found.")
        print("  Please make sure 'librsvg2-bin' (Linux) or 'librsvg' (macOS/Homebrew) is installed and available in PATH.")
        print("  Installation on Windows is more complex; see the instructions from the previous answer.")
    except subprocess.CalledProcessError as e:
        print(f"\n  ERROR: 'rsvg-convert' could not convert the file: {e.cmd}")
        print(f"  Return Code: {e.returncode}")
        if e.stdout:
            print(f"  Stdout: {e.stdout}")
        if e.stderr:
            print(f"  Stderr: {e.stderr}")
    except Exception as e:
        print(f"\n  An unexpected error occurred: {e}")
# --- Main script logic ---
if __name__ == "__main__":
    # <<<<<<< ADJUST HERE >>>>>>>
    # Base directory containing your .hpp files.
    # Windows example: base_directory = r'C:\Users\YourUsername\Documents\ClockLayouts'
    # Linux/macOS example: base_directory = '/home/YourUsername/Documents/ClockLayouts'
    # Current directory: base_directory = '.'
    base_directory = '/Users/davidp/Wortuhr/include/WordClockTypes' # Set to '.' if the .hpp files are in the same folder as the script.

    # List of .hpp filenames without path.
    hpp_filenames_to_process = [
        'DE10x11.hpp',
    ]
    # <<<<<<< END ADJUSTMENT >>>>>>>


    for filename in hpp_filenames_to_process:
        # Full path to the current .hpp file.
        hpp_full_filepath = os.path.join(base_directory, filename)

        if not os.path.exists(hpp_full_filepath):
            print(f"Skipping: File '{hpp_full_filepath}' does not exist.")
            continue # Continue with the next file in the list.

        letter_grid = parse_letter_grid_from_hpp(hpp_full_filepath)

        if letter_grid is None or not letter_grid:
            print(f"Skipping: letter_grid for '{hpp_full_filepath}' could not be parsed or is empty.")
            continue # Continue with the next file.

        generate_svg_files(hpp_full_filepath, letter_grid)

    print("\nAll requested files were processed.")
