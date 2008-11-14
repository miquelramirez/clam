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
- Link the script to an ObjectUpdate event of an object named with substring "source" 
    or "listener" and the position will be sent on every ObjectUpdate event
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
import re

SourcesSubstring='source'
ListenersSubstring='listener'

def isSource (object):
	return (object.name.lower().find(SourcesSubstring)!=-1)
def getSources(scene=Blender.Scene.GetCurrent()):
	sources=list()
	for object in scene.objects:
		if isSource(object):
			sources.append(object)
	return sources

def isListener (object):
	return (object.name.lower().find(ListenersSubstring)!=-1)
def getListeners(scene=Blender.Scene.GetCurrent()):
	listeners=list()
	for object in scene.objects:
		if isListener(object):
			listeners.append(object)
	return listeners

# use OSC client module for python - by Stefan Kersten
home=getenv("HOME")
pathToOSCList=["../../osc/oscpython","%s/src/liblo" % home, "%s/acustica/realtime_blender_demo" % home, "%s/clam/CLAM/plugins/osc/oscpython"%home]
configured=0
for testpath in pathToOSCList:
	if Blender.sys.exists(testpath+"/OSC.py"):
		path.append(testpath)
		from OSC import Message
		configured=1
		break
if configured==0:
	print "Can't found OSC.py. Aborting."
	

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

	if Blender.bylink==True and Blender.event=='ObjectUpdate':
		object=Blender.link
		location=object.mat.translationPart()
		roll, descention, azimuth=object.mat.toEuler()
		elevation = -descention
		rotation = (roll,elevation,azimuth)
		ports=[7000]
		# try to get the ports on object name:
		if re.search('_p([0-9_]+)$',object.name)!=None:
			ports=[]
			portsInName=re.search("_p([0-9_]+)$",object.name).group(1).split("_")
			for portString in portsInName:
				ports.append(int(portString))
		if isSource(object):
			typename='sources'
			sources=getSources()
			objectNumber=sources.index(object)
			for port in ports:
				sendObjectValue(objectNumber,typename,"location",location,port)
				sendObjectValue(objectNumber,typename,"rotation",rotation,port)
#			print "UPDATE L Source "+str(objectNumber)+" Port"+str(port)+" "+str(location)
			return
		if isListener(object):
			listeners=getListeners()
			typename='listeners'
			objectNumber=listeners.index(object)
			for port in ports:
				sendObjectValue(objectNumber,typename,"location",location,port)
				sendObjectValue(objectNumber,typename,"rotation",rotation,port)
#			print "UPDATE L Listener "+str(objectNumber)+" Port"+str(port)+" "+str(location)
			return
		if not typename:
			return

# This lets you can import the script without running it
if __name__ == '__main__':
	main()