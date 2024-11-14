# Druckdatei erstellen
Die Beschreibungen beziehen sich alle auf den PrusaSlicer, können aber mit Sicherheit auch auf andere Slicer übertragen werden.

## Einfache Version
Um den Diffusor direkt ins Gehäuse zu drucken kann entweder die Datei "Wortuhr_8x8_CaseWithoutLayerChange.3mf" direkt gesliced werden und in Höhe des Diffusors ein Filamentwechsel eingefügt. So hat man jedoch einen weißen Streifen aus am Gehäuse.

## Schwirigere Version
In der Date "Wortuhr_8x8_Case_DE15min_Prusa.3mf" ist in Höhe des Diffusors ein Schlitz im Gehäuse, sodass der Diffusor in der Luft hängt. Dies kann so gesliced werden und im Anschluss mit dem Python-Script "ChangeLayerHeightAfterFilamentChange.py" nach unten gesetzt werden. Somit wird der Diffusor in das Gehäuse eingedruckt (siehe "Wortuhr_8x8_Case_DE15min_Prusa_0.2mm_PLA,PLA_MK3_2h24m.gcode"). Es soll wohl auch möglich sein, direkt im Slicer durch das definieren eines zweiten Extruders den Diffusor in das Gehäuse zu drucken, allerdings habe ich das bisher noch nicht getestet.

## Druckeinstellungen
- Höhenmodifizierer im Bereich des Diffusors zu 0.1mm (Zwei Schichten sind notwendig, damit homogen)
- Druckeinstellungen -> Erweitert -> Infill/Perimeter Überlappung: 35% (Sonst bleiben lücken zwischen Buchstaben und Infill)
- PLA mit möglichst niedriger Temperatur (205-210°C) um Fäden zwischen den Buchstaben zu verhindern.

# FreeCAD
Zum erstellen habe ich die Version FreeCAD 1.0 RC1 (https://github.com/FreeCAD/FreeCAD/releases/tag/1.0rc1) verwendet. Das Model ist über die Variablen in VarSet parametrierbar. Es wird empfohlen, nur manuell zu aktualisieren (Klick Rechte Maustaste auf Wortuhr_8x8_Vorlage -> Neuberechnungen überspringen). Die Vorlage ist für eine maximale Größe von 16x16 LEDs ausgelegt.

Folgende Parameter sind anpassbar:
- Anzahl der LEDs in X und Y Richtung
- Abstand der LEDs in X und Y Richtung
- Höhe und Breite der Platine der LED Matrix
- Buchstaben auf der Wortuhr
- Schriftart & -größe
- Wandstärke
- Spielraum

Falls ihr Fragen oder eigene Idee zu Layout's oder Änderungen habt könnt ihr euch gerne bei mir melden.
