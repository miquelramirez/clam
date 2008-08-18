#!BPY
"""
Name: 'Blender Scene OSC CLAM exporter'
Blender: 244
Group: 'Object'
"""

# use bpy module of Blender:
from bpy import data
import Blender

def main():
	Blender.Window.FileSelector(write, "Export to CLAM network","exported_scene.clamnetwork")
	
def write(filename):
	liblos=""
	connections=""
	printers=""
	argumentNumbers=3
	i=1
	xPosition=50
	yPosition=150
	path="/SpatDIF/sources/%(objectNumber)s/xyz"
	for source in data.groups['AudioSources'].objects:
		libloName="source_"+source.name
		printerName="printer_source_"+source.name
		objectNumber=str(i)
		liblos+=makeLibloSource(libloName,path % vars(),xPosition,yPosition,3,7000)
		printers+=makeControlPrinter(printerName,xPosition,yPosition+150,3)

		for o in range(3):
			sufix="_"+str(o)
			connections+=makeConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		i+=1
		xPosition+=300

	i=1
	path="/SpatDIF/sinks/%(objectNumber)s/xyz"
	for sink in data.groups['AudioSinks'].objects:
		libloName="sink_"+sink.name
		printerName="printer_sink_"+sink.name
		objectNumber=str(i)
		liblos+=makeLibloSource(libloName,path % vars(),xPosition,yPosition,3,7000)
		printers+=makeControlPrinter(printerName,xPosition,yPosition+150,3)
		for o in range(3):
			sufix="_"+str(o)
			connections+=makeConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		i+=1
		xPosition+=300

	path="/SpatDIF/sync/FrameChanged"
	libloName="sync_Framechanged"
	printerName="printer_sync_FrameChanged"
	liblos+=makeLibloSource(libloName,path,50,0,1,7000)
	printers+=makeControlPrinter(printerName,300,0,1,291,70)
	connections+=makeConnection(libloName,path.replace("/","_")+"_0",printerName,"In Control")

	f=open(filename,'w')
	f.write(header+liblos+printers+connections+endHeader)
	f.close()
	print "OSC receivers CLAM Network exported as %(filename)s" % vars()

# This lets you can import the script without running it
if __name__ == '__main__':
	main()

def makeLibloSource(libloName,path,xPosition,yPosition,argumentsNumber,port=7000):
	return libloSource % vars()
def makeControlPrinter(printerName,xPosition,yPosition,argumentsNumber,width=300,height=285):
	return controlPrinter % vars()

def makeConnection(sourceName,outName,targetName,inName):
	outDescriptor="%(sourceName)s.%(outName)s" % vars()
	inDescriptor="%(targetName)s.%(inName)s" % vars()
	return connection % vars()

header="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="Blender_exported_scene">"""
endHeader="""
</network>"""

controlPrinter="""
 <processing id="%(printerName)s" position="%(xPosition)i,%(yPosition)i" size="%(width)i,%(height)i" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>%(argumentsNumber)s</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
 </processing>"""
libloSource="""
 <processing id="%(libloName)s" position="%(xPosition)i,%(yPosition)i" size="130,65" type="MultiLibloSource">
    <OscPath>%(path)s</OscPath>
    <ServerPort>%(port)s</ServerPort>
    <NumberOfArguments>%(argumentsNumber)s</NumberOfArguments>
 </processing>"""
connection="""
 <control_connection>
   <out>%(outDescriptor)s</out>
   <in>%(inDescriptor)s</in>
 </control_connection>"""
