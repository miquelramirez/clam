#!BPY

networkTemplate="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="ChoreoSequencer_Blender_exported_network">

  <processing id="AudioSource" position="-114,82" size="128,108" type="AudioSource"/>

  <processing id="ChoreoSequencer" position="82,59" size="194,65" type="ChoreoSequencer">
    <Filename>%(choreoFilename)s</Filename>
    <SourceIndex>0</SourceIndex>
    <FrameSize>512</FrameSize>
    <SampleRate>48000</SampleRate>
    <ControlsPerSecond>%(fps)i</ControlsPerSecond>
    <SizeX>1</SizeX>
    <SizeY>1</SizeY>
    <SizeZ>1</SizeZ>
  </processing>

  <processing id="source azimuth - zenith" position="313,431" size="203,161" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>2</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>

  <processing id="source xyz" position="302,255" size="203,161" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>3</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>

  <processing id="target azimuth - zenith" position="86,433" size="203,161" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>2</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>

  <processing id="target xyz" position="78,250" size="203,161" type="ControlPrinter">
    <Identifier>ControlPrinter</Identifier>
    <NumberOfInputs>3</NumberOfInputs>
    <GuiOnly>1</GuiOnly>
  </processing>

  <port_connection>
    <out>AudioSource.AudioOut</out>
    <in>ChoreoSequencer.sync</in>
  </port_connection>

  <control_connection>
    <out>ChoreoSequencer.target X</out>
    <in>target xyz.ControlPrinter_0</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.target Y</out>
    <in>target xyz.ControlPrinter_1</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.target Z</out>
    <in>target xyz.ControlPrinter_2</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.target azimuth</out>
    <in>target azimuth - zenith.ControlPrinter_0</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.target elevation</out>
    <in>target azimuth - zenith.ControlPrinter_1</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source X</out>
    <in>source xyz.ControlPrinter_0</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source Y</out>
    <in>source xyz.ControlPrinter_1</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source Z</out>
    <in>source xyz.ControlPrinter_2</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source azimuth</out>
    <in>source azimuth - zenith.ControlPrinter_0</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source elevation</out>
    <in>source azimuth - zenith.ControlPrinter_1</in>
  </control_connection>

</network>
"""
import Blender
from bpy import data
#import math

row="""%(frame)i 0 %(targetAzimuth)f %(targetElevation)f %(targetX)f %(targetY)f %(targetZ)f %(sourceX)f %(sourceY)f %(sourceZ)f
"""

def GetGroupObjects(objects,groupName):
	returnList=list()
	for object in data.groups[groupName].objects:
		if (list(objects).count(object)>0):
			returnList.append(object)
	return returnList

def WriteSceneAsChoreo (choreoFilename):
#	Window.WaitCursor(1)
	SelectedObjects = Blender.Object.GetSelected()
	sinks=GetGroupObjects(SelectedObjects,'AudioSinks')
	sources=GetGroupObjects(SelectedObjects,'AudioSources')
	if (len(sinks)!=1) or (len(sources)!=1):
		Blender.Draw.PupMenu('You have to select one source and one sink objects!')
		return

	scene = Blender.Scene.GetCurrent()
	f=open(choreoFilename,'w')
	for frame in range(Blender.Get('staframe'),Blender.Get('endframe')):
		Blender.Set('curframe',frame)
		targetAzimuth=0
		targetElevation=0
		targetX=sinks[0].getLocation()[0]
		targetY=sinks[0].getLocation()[1]
		targetZ=sinks[0].getLocation()[2]
		sourceX=sources[0].getLocation()[0]
		sourceY=sources[0].getLocation()[1]
		sourceZ=sources[0].getLocation()[2]
		f.write(row % vars())
	f.close()
	f=open(choreoFilename.replace(".choreo",".clamnetwork"),'w')
	fps=Blender.Scene.GetCurrent().getRenderingContext().framesPerSec()
	f.write(networkTemplate % vars())
	f.close()

def main():
	Blender.Window.FileSelector(WriteSceneAsChoreo, "Export choreo sequencer file", Blender.sys.makename(ext='.choreo'))

if __name__=='__main__':
	main()