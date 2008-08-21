
import Blender
from bpy import data
from sys import path

if Blender.sys.exists("network_scene_exporter.py")==1:
	path.append(Blender.sys.dirname("network_scene_exporter.py"))
	import network_scene_exporter as SceneExporter

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
	sink=sinks[0]
	source=sources[0]
	scene = Blender.Scene.GetCurrent()
	f=open(choreoFilename,'w')
	for frame in range(Blender.Get('staframe'),Blender.Get('endframe')):
		Blender.Set('curframe',frame)
		targetAzimuth=sink.RotY # use y for azimuth
		targetElevation=sink.RotX # and x for elevation
		targetX=sink.LocX
		targetY=sink.LocY
		targetZ=sink.LocZ
		sourceX=source.LocX
		sourceY=source.LocY
		sourceZ=source.LocZ
		f.write(row % vars())
	f.close()
	print "Exported choreo file: %s" % choreoFilename
	WriteChoreoNetwork(choreoFilename)

# generate CLAM network with the choreo sequencer and monitors
def WriteChoreoNetwork(choreoFilename):
	networkFilename=choreoFilename.replace(".choreo",".clamnetwork")
	networkId="ChoreoSequencer_Blender_exported_network"
	audioSourceName="AudioSync"
	audioSourceOutName="AudioOut"
	choreoSyncInName="sync"
	processings=""
	connections=""
	fps=Blender.Scene.GetCurrent().getRenderingContext().framesPerSec()
	choreoSequencerId="ChoreoSequencer"
	processings+=SceneExporter.makeAudioSource(audioSourceName,(-115,59))
	processings+=SceneExporter.makeChoreoSequencer(choreoSequencerId,choreoFilename,fps,(82,59))
	connections+= SceneExporter.makePortConnection(audioSourceName,audioSourceOutName,choreoSequencerId,choreoSyncInName)

	printersParameters=[\
		("target xyz",(37,184),3,(203,165)), \
		("target azimuth - zenith",(37,353),2,(203,130)), \
		("source xyz",(235,184),3,(203,165)), \
		("source azimuth - zenith",(235,353),2,(203,130)) \
	]
	choreoOuts = [\
		"target X","target Y","target Z", \
		"target azimuth","target elevation", \
		"source X","source Y","source Z", \
		"source azimuth","source elevation" \
	]
	outputNumber=0
	for parameters in printersParameters:
		printerName=parameters[0]
		printerPos=parameters[1]
		printerInputs=parameters[2]
		printerSize=parameters[3]
		processings+=SceneExporter.makeControlPrinter(printerName,printerPos,printerInputs,printerSize)
		for outIndex,outConnector in enumerate(choreoOuts[outputNumber:outputNumber+printerInputs]):
			connections+=SceneExporter.makeControlConnection(choreoSequencerId,outConnector,printerName,"ControlPrinter_"+str(outIndex))
			outputNumber+=1
	f=open(networkFilename,'w')
	body=processings+connections
	f.write(SceneExporter.Network % vars())
	f.close()
	print "Exported CLAM network: %s" % networkFilename

def main():
	Blender.Window.FileSelector(WriteSceneAsChoreo, "Export choreo sequencer file", Blender.sys.makename(ext=".choreo"))
		 
if __name__=='__main__':
	main()
