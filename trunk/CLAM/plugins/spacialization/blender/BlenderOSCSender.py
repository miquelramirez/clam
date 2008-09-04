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
    the 'AudioSources' and 'AudioSinks' groups will be sent on every new frame
or
- Link the script to an ObjectUpdate event of an object of the 'AudioSources' 
    or 'AudioSinks' groups and the position of the object will be sent on every
    ObjectUpdate event
"""
#
# Copyright (c) 2008 Fundació Barcelona Media Universitat Pompeu Fabra
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
import Blender

SourcesGroupName='AudioSources'
SinksGroupName='AudioSinks'

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

def main():
	if (configured==0):
		return
	if Blender.event=='FrameChanged':
		frame=Blender.Get('curframe')
		Message("/SpatDIF/sync/FrameChanged",[frame]).sendlocal(7000)
		sendGroupObjectsPositions(data.groups[SourcesGroupName],'sources')
		sendGroupObjectsPositions(data.groups[SinksGroupName],'sinks')
	if Blender.bylink==True and Blender.event=='ObjectUpdate':
		object=Blender.link
		location=object.getLocation()
		sources=list(data.groups[SourcesGroupName].objects)
		sinks=list(data.groups[SinksGroupName].objects)
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
