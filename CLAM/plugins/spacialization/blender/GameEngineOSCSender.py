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
import GameLogic


listenerId="listener"
sourceId="source"

def getTypeOfObject(owner):
#	print owner
#	print owner.has_key('sound_type')

	if not owner.has_key('sound_type'):
		print "Warning: connected an object without sound_type attribute."
		return None
	return owner.get('sound_type')

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
	
def sendObjectValue(objectName,typeName,typeValue,value,port,ipToSend):
	message="/SpatDIF/%s/%s/%s" % (typeName,objectName,typeValue)
	print "sending ", value, " to path %s to port %i" % (message,port)
	if ipToSend:
		Message(message,value).sendto(ipToSend,port)
	else:
		Message(message,value).sendlocal(port)

def main(controller):
	typename=None
#	controller=GameLogic.getCurrentController()
	object=controller.owner

	typeName=getTypeOfObject(object)
	if typeName==None:
		return
	location=object.getPosition()
	ori=object.worldOrientation
	orientation=Blender.Mathutils.Matrix(ori[0],ori[1],ori[2]).transpose()
#	print Blender.Mathutils.Matrix(orientation[0],orientation[1],orientation[2]).transpose().toEuler()
	print orientation.toEuler()
	if object.isA('KX_Camera') and typeName=='listener': # if the listener is a camera, do a proper rotation acording to conventions: azimuth 0: seeing at x+
		rotationMatrix=Blender.Mathutils.Euler(90,0,-90).toMatrix().invert()
		orientation = rotationMatrix * orientation
	print "new: ", orientation.toEuler()
	roll, descention, yaw=orientation.toEuler()
	pitch = -descention
	rotation = (yaw,pitch,roll)

	if object.has_key('osc_ports'):
		ports=object.get('osc_ports').split()
	else:
		ports=[7000]
	name=object.name[2:].replace(".","_")

	ipToSend=None

	if object.has_key('osc_send_to_ip'):
		ipToSend=object.get('osc_send_to_ip')

	for port in ports:
		sendObjectValue(name,typeName,"xyz",location,port,ipToSend)
		sendObjectValue(name,typeName,"ypr",rotation,port,ipToSend)
	return

# This lets you can import the script without running it
if __name__ == '__main__':
	main(GameLogic.getCurrentController())
