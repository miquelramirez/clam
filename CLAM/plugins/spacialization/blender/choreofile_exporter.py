#!BPY

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

def WriteSceneAsChoreo (File):
	SelectedObjects = Blender.Object.GetSelected()
	sinks=GetGroupObjects(SelectedObjects,'AudioSinks')
	sources=GetGroupObjects(SelectedObjects,'AudioSources')
	if (len(sinks)!=1) or (len(sources)!=1):
		Blender.Draw.PupMenu('You have to select one source and one sink objects!')
		return

	scene = Blender.Scene.GetCurrent()
#	context = Scene.getRenderingContext()
#		FrameRate = float(context.framesPerSec())

	f=open(File,'w')
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

def main():
	Blender.Window.FileSelector(WriteSceneAsChoreo, "Export choreo sequencer file", Blender.sys.makename(ext='.choreo'))

if __name__=='__main__':
	main()
