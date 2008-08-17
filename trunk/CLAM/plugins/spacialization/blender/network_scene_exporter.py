#!BPY
"""
Name: 'Blender Scene OSC CLAM exporter'
Blender: 244
Group: 'Object'
"""
header="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="Blender_exported_scene">"""

controlPrinter="""
 <processing id="%(printerName)s" position="%(XLPosition)f,%(YPPosition)f" size="291,285" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>3</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
 </processing>"""
libloSource="""
 <processing id="%(libloName)s" position="%(XLPosition)f,%(YLPosition)f" size="130,65" type="MultiLibloSource">
    <OscPath>/SpatDIF/%(objectType)s/%(objectNumber)s/xyz</OscPath>
    <ServerPort>7000</ServerPort>
    <NumberOfArguments>3</NumberOfArguments>
 </processing>"""
connection="""
 <control_connection>
   <out>%(libloName)s._SpatDIF_%(objectType)s_%(objectNumber)s_xyz_0</out>
   <in>printer_%(libloName)s.ControlPrinter_0</in>
 </control_connection>

 <control_connection>
   <out>%(libloName)s._SpatDIF_%(objectType)s_%(objectNumber)s_xyz_1</out>
   <in>printer_%(libloName)s.ControlPrinter_1</in>
 </control_connection>

 <control_connection>
   <out>%(libloName)s._SpatDIF_%(objectType)s_%(objectNumber)s_xyz_2</out>
   <in>printer_%(libloName)s.ControlPrinter_2</in>
 </control_connection>"""
endHeader="""
</network>"""


# use bpy module of Blender:
from bpy import data
import Blender

def main():
	Blender.Window.FileSelector(write, "Export to CLAM network","exported_scene.clamnetwork")
	
def write(filename):
	liblos=""
	connections=""
	printers=""
	#print header
	i=1
	XLPosition=100
	YLPosition=100
	YPPosition=300
	objectType='sources'
	for source in data.groups['AudioSources'].objects:
		libloName="source_"+source.name
		printerName="printer_source_"+source.name
		objectNumber=str(i)
		liblos+=libloSource % vars()
		printers+=controlPrinter % vars()
		connections+=connection % vars()
		i+=1
		XLPosition+=300

	objectType='sinks'
	i=1
	for sink in data.groups['AudioSinks'].objects:
		libloName="sink_"+sink.name
		printerName="printer_sink_"+sink.name
		objectNumber=str(i)
		liblos+=libloSource % vars()
		printers+=controlPrinter % vars()
		connections+=connection % vars()
		i+=1
		XLPosition+=300

	f=open(filename,'w')
	f.write(header+liblos+printers+connections+endHeader)
	f.close()

# This lets you can import the script without running it
if __name__ == '__main__':
	main()

