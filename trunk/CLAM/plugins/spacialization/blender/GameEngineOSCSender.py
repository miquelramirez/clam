#!BPY
"""
Name: 'Blender GameEngine OSC Sender'
Blender: 248
Group: 'Object, Animation, GameEngine'
Tooltip: 'Open Sound Control Sender of GameEngine Objects. To be used as controller linked to "moving_source"/"fixed_source"/"listener" sensors'
"""
__author__ = ("Barcelona Media Audio Group")
__url__ = ("http://www.barcelonamedia.org/laboratoris/6/en")
__bpydoc__ = """\
This script allows to send objects positions/rotations through Open Sound Control,
 with a very simple adaption of SpatDIF. 

Requires: 
 - Python OSC Sender (OSC.py) by Stefan Kersten on '../../osc/oscpython' 
    (CLAM SVN relative path) or '~/src/liblo'
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

from sys import path
from os import getenv
import Blender
import re


CheckObjectsNames=False
listenerId="listener"
sourceId="source"
def checkObjectName(object):
	if object.name.lower().find(listenerId)!=-1:
		return "listener"
	if object.name.lower().find(sourceId)!=-1:
		return "source"
	return None



def getTypeOfObject(controller):
	dictTypes={"moving_source": "source",
		"fixed_source": "source",
		"listener": "listener" }
	for sensorName in dictTypes.keys():
		try:
			sensor=controller.getSensor(sensorName)
		except:
			continue
		if sensor.triggered and sensor.positive:
			return dictTypes[sensorName]
	return False

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
	
def sendObjectValue(objectName,typeName,typeValue,value,port):
	message="/SpatDIF/%s/%s/%s" % (typeName,objectName,typeValue)
	Message(message,value).sendlocal(port)

def main():
	typename=None
	controller=GameLogic.getCurrentController()
	object=controller.getOwner()
#	print object.name

	
	if CheckObjectsNames and checkObjectName(object)==None:
		return

	location=object.getPosition()
#	print location
	ori=object.orientation
#	print ori
	matr=Blender.Mathutils.Matrix(ori[0],ori[1],ori[2])
	roll, descention, yaw=matr.transpose().toEuler()
	pitch = -descention
	yaw+=90
	rotation = (yaw,pitch,roll)
	ports=[7000]
	# try to get the ports on object name:
	if re.search('_p([0-9_]+)$',object.name)!=None:
		ports=[]
		portsInName=re.search("_p([0-9_]+)$",object.name).group(1).split("_")
		for portString in portsInName:
			ports.append(int(portString))

	typeName=getTypeOfObject(controller)
	if typeName==False:
		return
	for port in ports:
		sendObjectValue(object.name.replace(".","_"),typeName,"xyz",location,port)
		sendObjectValue(object.name.replace(".","_"),typeName,"ypr",rotation,port)
#		print "UPDATE L Source "+str(objectNumber)+" Port"+str(port)+" "+str(location)
	return

# This lets you can import the script without running it
if __name__ == '__main__':
	main()
