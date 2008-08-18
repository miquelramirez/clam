#!BPY
"""
Name: 'SpatDIF OSC Sender'
Blender: 244
Group: 'Object'
"""
# use bpy module of Blender:
from bpy import data
from sys import path
from os import getenv
import Blender

# use OSC client module for python - by Stefan Kersten
pathToOSCList=["../../osc/oscpython",getenv("HOME")+"/src/liblo"]
configured=0
for testpath in pathToOSCList:
	if Blender.sys.exists(testpath+"/OSC.py"):
		path.append(testpath)
		from OSC import Message
		configured=1
		break
if configured==0:
	print "Can't found OSC.py. Aborting."
	
def sendGroupObjectsPositions(group,typeName):
	objectsList=list(group.objects)
	for object in group.objects:
		location=object.getLocation()
		objectNumber=objectsList.index(object)
		sendObjectLocation(objectNumber,typeName,location)

def sendObjectLocation(objectNumber,typeName,location):
	message="/SpatDIF/%s/%i/xyz" % (typeName,objectNumber)
	Message(message,location).sendlocal(7000)

import Blender

def main():
	scene=data.scenes.active
#	print '-'*30
#	print 'OSCSender Called! - Event: '+Blender.event

	if (configured==0):
		return
	if Blender.event=='FrameChanged':
		frame=Blender.Get('curframe')
		Message("/SpatDIF/sync/FrameChanged",[frame]).sendlocal(7000)
		sendGroupObjectsPositions(data.groups['AudioSources'],'sources')
		sendGroupObjectsPositions(data.groups['AudioSinks'],'sinks')
	if Blender.bylink==True and Blender.event=='ObjectUpdate':
		object=Blender.link
		location=object.getLocation()
		sources=list(data.groups['AudioSources'].objects)
		sinks=list(data.groups['AudioSinks'].objects)
		if sources.count(object)!=0:
			typename='sources'
			objectNumber=sources.index(object)
		if sinks.count(object)!=0:
			typename='sinks'
			objectNumber=sinks.index(object)
		if not typename:
			return
		sendObjectLocation(objectNumber,typename,location)
		
# This lets you can import the script without running it
if __name__ == '__main__':
	main()
