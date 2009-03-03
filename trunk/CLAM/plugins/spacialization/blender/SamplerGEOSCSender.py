#!BPY
"""
Name: 'Blender GameEngine OSC Sender'
Blender: 248
Group: 'GameEngine'
Tooltip: 'Open Sound Control Sender of GameEngine Objects. To be used as controller linked to "play"/"playloop"/"stop" sensors'
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
	
def sendPlay(objectName,voice,value=1,loop=0,port=7000):
	message="/SpatDIF/source/%s/sampler/%s/setLoop" % (objectName,voice)
	Message(message,[int(loop)]).sendlocal(port)
	message="/SpatDIF/source/%s/sampler/%s/play" % (objectName,voice)
	Message(message,[int(value)]).sendlocal(port)

def main():
	typename=None
	controller=GameLogic.getCurrentController()
	object=controller.getOwner()
	listOfTriggeredSensors=[sensor for sensor in controller.getSensors() if sensor.triggered and sensor.positive]
	for sensor in listOfTriggeredSensors:
		ports=[7000]
		# try to get the ports on object name:
		if re.search('_p([0-9_]+)$',object.name)!=None:
			ports=[]
			portsInName=re.search("_p([0-9_]+)$",object.name).group(1).split("_")
			for portString in portsInName:
				ports.append(int(portString))
		# play without loop:
		if re.search("play_([0-9]+)",str(sensor)):
			loop=0
			value=1
			voice=re.search("play_([0-9]+)",str(sensor)).group(1)
		# play with loop:
		if re.search("playloop_([0-9]+)",str(sensor)):
			loop=1
			value=1
			voice=re.search("play_([0-9]+)",str(sensor)).group(1)
		# stop:
		if re.search("stop_([0-9]+)",str(sensor)):
			loop=0
			value=0
			voice=re.search("play_([0-9]+)",str(sensor)).group(1)
		for port in ports:
			sendPlay(object.name,voice,value,loop,port)
	return

# This lets you can import the script without running it
if __name__ == '__main__':
	main()
