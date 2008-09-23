#!BPY
# -*- coding: iso-8859-1 -*-

"""
Name: 'Scene CLAM Network exporter'
Blender: 244
Group: 'Exporter'
Tooltip: 'Generates CLAM Networks'
"""
__author__ = ("Natanael Olaiz", "CLAM Team")
__url__ = ("http://clam.iua.upf.edu/")
__email__ = ["clam@iua.upf.edu"]
__version__ = "CLAM 1.3.1 - GSOC 2008"
__bpydoc__ = """\
This script generates a CLAM Network with one OSC receiver for each audio
 object of the scene (contained in 'AudioSources' and 'AudioSinks' groups)
 and connect them to a ControlPrinter as monitor. Useful to use as a template
 network for the Blender scene.

Requires:
 - on Blender: BlenderOSCSender.py
 - on CLAM: MultiLibloSource OSC plugin

Usage:
 - Just run it.

This file is also used to generate the choreosequencer network.

TODO: 
 - merge the clam Network exporter routines in a class

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


# use bpy module of Blender:
from bpy import data
import Blender

def main():
	Blender.Window.FileSelector(GenerateNetworkOSCReceiver, "Generate CLAM network OSC monitor",Blender.sys.makename(ext='.clamnetwork'))
	
def GenerateNetworkOSCReceiver(filename):
	liblos=""
	connections=""
	printers=""
	argumentNumbers=3
	xPosition=50
	yPosition=150
	path="/SpatDIF/sources/%(objectNumber)s/xyz"
	port=7000
#create sources receivers and its monitors
	objectsList=list(data.groups['AudioSources'].objects)
	for source in data.groups['AudioSources'].objects:
		libloName="source_"+source.name
		printerName="printer_source_"+source.name
		objectNumber=str(objectsList.index(source))
		liblos+=makeLibloSource(libloName,path % vars(),(xPosition,yPosition),3,port)
		printers+=makeControlPrinter(printerName,(xPosition,yPosition+150),3)
		for o in range(3):
			sufix="_"+str(o)
			connections+=makeControlConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		xPosition+=300
	xPosition=50
	yPosition=600
#create sinks receivers and its monitors
	objectsList=list(data.groups['AudioSinks'].objects)
	path="/SpatDIF/sinks/%(objectNumber)s/xyz"
	for sink in data.groups['AudioSinks'].objects:
		libloName="sink_"+sink.name
		printerName="printer_sink_"+sink.name
		objectNumber=str(objectsList.index(sink))
		liblos+=makeLibloSource(libloName,path % vars(),(xPosition,yPosition),3,port)
		printers+=makeControlPrinter(printerName,(xPosition,yPosition+150),3)
		for o in range(3):
			sufix="_"+str(o)
			connections+=makeControlConnection(libloName,(path%vars()).replace("/","_")+sufix,printerName,"ControlPrinter"+sufix)
		xPosition+=300
#create sync receivers and it monitors
	path="/SpatDIF/sync/FrameChanged"
	libloName="sync_Framechanged"
	printerName="printer_sync_FrameChanged"
	liblos+=makeLibloSource(libloName,path,(50,0),1,port)
	printers+=makeControlPrinter(printerName,(300,0),1,(291,70))
	connections+=makeControlConnection(libloName,path.replace("/","_")+"_0",printerName,"In Control")
	f=open(filename,'w')
	networkId="Exported_Blender_scene_receiver_network"
	body=liblos+printers+connections
	f.write(Network % vars())
	f.close()
	print "OSC receivers CLAM Network exported as %(filename)s" % vars()

# This lets you can import the script without running it
if __name__ == '__main__':
	main()

def makeAudioSource(processingName,position,size=(130,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	typeName="AudioSource"
	processingConfig=""
	return Processing % vars()

def makeChoreoSequencer(processingName,choreoFilename,fps,position,size=(194,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	typeName="ChoreoSequencer"
	processingConfig=ChoreoSequencerConfig % vars()
	return Processing % vars()

def makeLibloSource(processingName,path,position,argumentsNumber,port=7000,size=(130,65)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	typeName="MultiLibloSource"
	processingConfig=LibloSourceConfig%vars()
	return Processing % vars()
	
def makeControlPrinter(processingName,position,argumentsNumber,size=(300,285)):
	processingPosition="%i,%i" % (position[0],position[1])
	processingSize="%i,%i" % (size[0],size[1])
	typeName="ControlPrinter"
	printerId=typeName
	processingConfig=ControlPrinterConfig%vars()
	return Processing % vars()

def makeControlConnection(sourceName,outName,targetName,inName):
	return ControlConnection % vars()

def makePortConnection(sourceName,outName,targetName,inName):
	return PortConnection % vars()

Network="""<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network id="%(networkId)s">
%(body)s
</network>
"""
Processing="""
  <processing id="%(processingName)s" position="%(processingPosition)s" size="%(processingSize)s" type="%(typeName)s">
  %(processingConfig)s
  </processing>
  """
ControlPrinterConfig="""  <Identifier>%(printerId)s</Identifier>
    <NumberOfInputs>%(argumentsNumber)s</NumberOfInputs>
    <GuiOnly>1</GuiOnly>"""
LibloSourceConfig="""  <OscPath>%(path)s</OscPath>
    <ServerPort>%(port)s</ServerPort>
    <NumberOfArguments>%(argumentsNumber)s</NumberOfArguments>"""
ChoreoSequencerConfig="""  <Filename>%(choreoFilename)s</Filename>
    <SourceIndex>0</SourceIndex>
    <FrameSize>512</FrameSize>
    <SampleRate>48000</SampleRate>
    <ControlsPerSecond>%(fps)i</ControlsPerSecond>
    <SizeX>1</SizeX>
    <SizeY>1</SizeY>
    <SizeZ>1</SizeZ>"""

ControlConnection="""
  <control_connection>
    <out>%(sourceName)s.%(outName)s</out>
    <in>%(targetName)s.%(inName)s</in>
  </control_connection>
"""
PortConnection="""
  <port_connection>
    <out>%(sourceName)s.%(outName)s</out>
    <in>%(targetName)s.%(inName)s</in>
  </port_connection>
"""
