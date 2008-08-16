#!BPY
"""
Name: 'Blender Scene OSC CLAM exporter'
Blender: 244
Group: 'Object'
"""

header="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="Blender_exported_scene">
"""

controlPrinter=""" <processing id="NameOfControlPrinter" position="XPosition,YPosition" size="291,285" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>3</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>
"""

libloSource="""<processing id="NameOfMultiLibloSource" position="XPosition,YPosition" size="130,65" type="MultiLibloSource">
    <OscPath>/SpatDIF/SUBPATH/xyz</OscPath>
    <ServerPort>7000</ServerPort>
    <NumberOfArguments>3</NumberOfArguments>
  </processing>
"""
connection="""
  <control_connection>
    <out>NameOfMultiLibloSource_xyz_0</out>
    <in>NameOfControlPrinter.ControlPrinter_0</in>
  </control_connection>

  <control_connection>
    <out>NameOfMultiLibloSource_xyz_1</out>
    <in>NameOfControlPrinter.ControlPrinter_1</in>
  </control_connection>

  <control_connection>
    <out>NameOfMultiLibloSource_xyz_2</out>
    <in>NameOfControlPrinter.ControlPrinter_2</in>
  </control_connection>
"""
endHeader="""</network>"""


# use bpy module of Blender:
from bpy import data
#import Blender

def main():

	liblos=""
	connections=""
	printers=""
	#print header
	i=1
	XPosition=100
	YPosition=100
	for source in data.groups['AudioSources'].objects:
		#print source.name
		libloName="source_"+source.name
		printerName="printer_source_"+source.name
		liblos+=libloSource.replace("NameOfMultiLibloSource",libloName).replace("SUBPATH","sources/"+str(i)).replace("XPosition",str(XPosition)).replace("YPosition",str(YPosition))
		printers+=controlPrinter.replace("NameOfControlPrinter",printerName).replace("XPosition",str(XPosition)).replace("YPosition",str(YPosition+200))
		connections+=connection.replace("NameOfMultiLibloSource",libloName+"._SpatDIF_sources_"+str(i)).replace("NameOfControlPrinter","printer_"+libloName)
		i+=1
		XPosition+=300
	i=1
	for sink in data.groups['AudioSinks'].objects:
		libloName="sink_"+sink.name
		printerName="printer_sink_"+sink.name
		liblos+=libloSource.replace("NameOfMultiLibloSource",libloName).replace("SUBPATH","sinks/"+str(i)).replace("XPosition",str(XPosition)).replace("YPosition",str(YPosition))
		printers+=controlPrinter.replace("NameOfControlPrinter",printerName).replace("XPosition",str(XPosition)).replace("YPosition",str(YPosition+200))
		connections+=connection.replace("NameOfMultiLibloSource",libloName+"._SpatDIF_sinks_"+str(i)).replace("NameOfControlPrinter","printer_"+libloName)		
		i+=1
		XPosition+=300
	f=open('exported_scene.clamnetwork','w')
	f.write(header+liblos+printers+connections+endHeader)
	f.close()

# This lets you can import the script without running it
if __name__ == '__main__':
	main()

