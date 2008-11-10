#!BPY
"""
Name: 'SpatDIF OSC Sender'
Blender: 244
Group: 'Object, Animation'
Tooltip: 'Open Sound Control Sender (link to objects ObjectUpdate events, or Scene/World FrameChanged events) '
"""
__author__ = ("Natanael Olaiz", "CLAM Team")
__url__ = ("http://clam.iua.upf.edu/")
__email__ = ["clam@iua.upf.edu"]
__version__ = "CLAM 1.3.1 - GSOC 2008"
__bpydoc__ = """\
This script allows to send objects positions through Open Sound Control, with
 a very simple implementation of SpatDIF. 

Requires: 
 - Python OSC Sender (OSC.py) by Stefan Kersten on '../../osc/oscpython' 
    (CLAM SVN relative path) or '~/src/liblo'

Usage:
- Link the script to a scene/world FrameChanged event, and the positions of 
    the 'Audio_Sources' and 'Audio_Listeners' groups will be sent on every new frame
or
- Link the script to an ObjectUpdate event of an object of the 'AudioSources' 
    or 'Audio_Listeners' groups and the position of the object will be sent on every
    ObjectUpdate event
"""
#
# Copyright (c) 2008 Fundacio Barcelona Media Universitat Pompeu Fabra
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 

from bpy import data
from sys import path
from os import getenv
import Blender, math

#SourcesGroupName='Audio_Sources'
#ListenersGroupName='Audio_Listeners'

SourcesSubstring='source'
ListenersSubstring='listener'

def isSource (object):
	return (object.name.lower().search(SourcesSubstring)!=-1)
def getSources(scene=Blender.Scene.GetCurrent()):
	# old method: #sources=list(data.groups[SourcesGroupName].objects)
	sources=list()
	for object in scene.objects:
		if isSource(object):
			sources.append(object)
	return sources

def isListener (object):
	return (object.name.lower().search(ListenersSubstring)!=-1)
def getListeners(scene=Blender.Scene.GetCurrent()):
	# old method: #listeners=list(data.groups[ListenersGroupName].objects)
	listeners=list()
	for object in scene.objects:
		if isListener(object):
			listeners.append(object)
	return listeners

# use OSC client module for python - by Stefan Kersten
pathToOSCList=["../../osc/oscpython",getenv("HOME")+"/src/liblo",getenv("HOME")+"/acustica/realtime_blender_demo"]
configured=0
for testpath in pathToOSCList:
	if Blender.sys.exists(testpath+"/OSC.py"):
		path.append(testpath)
		from OSC import Message
		configured=1
		break
if configured==0:
	print "Can't found OSC.py. Aborting."
	
def sendGroupObjectsPositions(listeners, sources,typeName):
	for source in sources:
		sourceLocation=source.mat.translationPart()
		sourceNumber=sources.index(source)
		sendObjectValue(sourceNumber,typeName,"location",sourceLocation,7000+sourceNumber)
#		print "ANIMATION Send Source "+repr(sourceNumber)+" Port"+repr(7000+sourceNumber)+" "+repr(sourceLocation)
		for listener in listeners:
			listenerLocation=listener.mat.translationPart()
			listenerNumber=listeners.index(listener)
			roll, descention, azimuth=listener.mat.toEuler()
			elevation = -descention
			rotation = (roll,elevation,azimuth)
			sendObjectValue(listenerNumber,typeName,"location",sourceLocation,7000+sourceNumber)
			sendObjectValue(listenerNumber,typeName,"rotation",rotation,7000+sourceNumber)
#			print "ANIMATION Send Listener"+repr(listenerNumber)+" Port"+repr(7000+sourceNumber)+" "+repr(listenerLocation)
		

def sendObjectValue(objectNumber,typeName,typeValue,value,port):
	message="/SpatDIF/%s/%i/xyz/%s" % (typeName,objectNumber,typeValue)
	Message(message,value).sendlocal(port)

def main():
	if (configured==0):
		return
	if Blender.event=='FrameChanged':
		frame=Blender.Get('curframe')
		Message("/SpatDIF/sync/FrameChanged",[frame]).sendlocal(7000)
		return
#		sendGroupObjectsPositions(getListeners(),getSources())
#list(data.groups[ListenersGroupName].objects),list(data.groups[SourcesGroupName].objects),'sources')

	if Blender.bylink==True and Blender.event=='ObjectUpdate':
		object=Blender.link
		location=object.mat.translationPart()
		roll, descention, azimuth=object.mat.toEuler()
		elevation = -descention
		rotation = (roll,elevation,azimuth)
		if isSource(object):
			typename='sources'
			sources=getSources()
			objectNumber=sources.index(object)
			sendObjectValue(objectNumber,typename,"location",location,7000+objectNumber)
#			print "UPDATE L Source "+repr(objectNumber)+" Port"+repr(7000+objectNumber)+" "+repr(location)
			return
		if isListener(object):
			listeners=getListeners()
			typename='listeners'
			objectNumber=listeners.index(object)
			sendObjectValue(objectNumber,typename,"location",location,7000+objectNumber)
			sendObjectValue(objectNumber,typename,"rotation",rotation,7000+objectNumber)
#			print "UPDATE L Listener "+repr(objectNumber)+" Port"+repr(7000+sourceNumber)+" "+repr(location)
#			print "UPDATE R Listener "+repr(objectNumber)+" Port"+repr(7000+sourceNumber)+" "+repr(rotation)
			return
		if not typename:
			return

# This lets you can import the script without running it
if __name__ == '__main__':
	main()
