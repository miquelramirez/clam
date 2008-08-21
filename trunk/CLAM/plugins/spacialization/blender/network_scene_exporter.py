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
	Blender.Window.FileSelector(GenerateNetworkOSCReceiver, "Generate CLAM network OSC monitor",Blender.sys.makename(ext='.clamnetwork'))
	
def GenerateNetworkOSCReceiver(filename):
	liblos=""
	connections=""
	printers=""
	argumentNumbers=3
	xPosition=50
	yPosition=150
	path="/SpatDIF/sources/%(objectNumber)s/xyz"
	port=7000
#create sources receivers and its monitors
	objectsList=list(data.groups['AudioSources'].objects)
	for source in data.groups['AudioSources'].objects:
		libloName="source_"+source.name
		printerName="printer_source_"+source.name
		objectNumber=str(objectsList.index(source))
		liblos+=makeLibloSource(libloName,path % vars(),(xPosition,yPosition),3,port)
		printers+=makeControlPrinter(printerName,(xPosition,yPosition+150),3)
		for o in range(3):
			sufix="_"+str(o)
			connections+=makeControlConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		xPosition+=300
	xPosition=50
	yPosition=600
#create sinks receivers and its monitors
	objectsList=list(data.groups['AudioSinks'].objects)
	path="/SpatDIF/sinks/%(objectNumber)s/xyz"
	for sink in data.groups['AudioSinks'].objects:
		libloName="sink_"+sink.name
		printerName="printer_sink_"+sink.name
		objectNumber=str(objectsList.index(sink))
		liblos+=makeLibloSource(libloName,path % vars(),(xPosition,yPosition),3,port)
		printers+=makeControlPrinter(printerName,(xPosition,yPosition+150),3)
		for o in range(3):
			sufix="_"+str(o)
			connections+=makeControlConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		xPosition+=300
#create sync receivers and it monitors
	path="/SpatDIF/sync/FrameChanged"
	libloName="sync_Framechanged"
	printerName="printer_sync_FrameChanged"
	liblos+=makeLibloSource(libloName,path,(50,0),1,port)
	printers+=makeControlPrinter(printerName,(300,0),1,(291,70))
	connections+=makeControlConnection(libloName,path.replace("/","_")+"_0",printerName,"In Control")
	f=open(filename,'w')
	networkId="Exported_Blender_scene_receiver_network"
	header=Header % vars()
	f.write(header+liblos+printers+connections+Tail)
	f.close()
	print "OSC receivers CLAM Network exported as %(filename)s" % vars()

# This lets you can import the script without running it
if __name__ == '__main__':
	main()

def makeAudioSource(processingName,position,size=(130,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])	
	return AudioSource % vars()

def makeChoreoSequencer(processingName,choreoFilename,fps,position,size=(194,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	return ChoreoSequencer % vars()

def makeLibloSource(processingName,path,position,argumentsNumber,port=7000,size=(130,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	return LibloSource % vars()
	
def makeControlPrinter(processingName,position,argumentsNumber,size=(300,285)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	printerId="ControlPrinter"
	return ControlPrinter % vars()

def makeControlConnection(sourceName,outName,targetName,inName):
	return ControlConnection % vars()

def makePortConnection(sourceName,outName,targetName,inName):
	return PortConnection % vars()

Header="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="%(networkId)s">"""
Tail="""
</network>
"""
ControlPrinter="""
  <processing id="%(processingName)s" position="%(processingPosition)s" size="%(processingSize)s" type="ControlPrinter">
    <Identifier>%(printerId)s</Identifier>
    <NumberOfInputs>%(argumentsNumber)s</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>
  """
LibloSource="""
  <processing id="%(processingName)s" position="%(processingPosition)s" size="%(processingSize)s" type="MultiLibloSource">
    <OscPath>%(path)s</OscPath>
    <ServerPort>%(port)s</ServerPort>
    <NumberOfArguments>%(argumentsNumber)s</NumberOfArguments>
  </processing>
  """
ControlConnection="""
  <control_connection>
    <out>%(sourceName)s.%(outName)s</out>
    <in>%(targetName)s.%(inName)s</in>
  </control_connection>
  """
PortConnection="""
  <port_connection>
    <out>%(sourceName)s.%(outName)s</out>
    <in>%(targetName)s.%(inName)s</in>
  </port_connection>
  """
ChoreoSequencer="""
  <processing id="%(processingName)s" position="%(processingPosition)s" size="%(processingSize)s" type="ChoreoSequencer">
    <Filename>%(choreoFilename)s</Filename>
    <SourceIndex>0</SourceIndex>
    <FrameSize>512</FrameSize>
    <SampleRate>48000</SampleRate>
    <ControlsPerSecond>%(fps)i</ControlsPerSecond>
    <SizeX>1</SizeX>
    <SizeY>1</SizeY>
    <SizeZ>1</SizeZ>
  </processing>
  """
AudioSource="""
  <processing id="%(processingName)s" position="%(processingPosition)s" size="%(processingSize)s" type="AudioSource"/>
  """