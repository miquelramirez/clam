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
home=getenv("HOME")
path.append(home+"/src/liblo")

# use OSC client module for python - by Stefan Kersten
from OSC import Message

def existsInGroup(objectToFind, group):
	objectNumber=1
	for object in group.objects:
		if object==objectToFind:
			return objectNumber
		objectNumber+=1
	return False

def sendGroupObjects(group,typeName):
	objectNumber=1
	for object in group.objects:
		location=object.getLocation()
		print object.name
		message="/SpatDIF/%s/%i/xyz" % (typeName,objectNumber)
		Message(message,location).sendlocal(7000)
		objectNumber+=1

import Blender

def main():
	
	scene=data.scenes.active
	print '-'*30
	print 'OSCSender Called! - Event: '+Blender.event

	if Blender.event=='FrameChanged':
		frame=Blender.Get('curframe')
		print 'frame: ' +str(frame)
		Message("/SpatDIF/sync/FrameChanged",[frame]).sendlocal(7000)
		sendGroupObjects(data.groups['AudioSources'],'sources')
		sendGroupObjects(data.groups['AudioSinks'],'sinks')
			
# This lets you can import the script without running it
if __name__ == '__main__':
	main()

