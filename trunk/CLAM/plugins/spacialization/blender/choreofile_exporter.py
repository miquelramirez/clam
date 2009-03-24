#!BPY
"""
Name: 'CLAM ChoreoSequencer file exporter'
Blender: 244
Group: 'Export'
Tooltip: 'Generate a CLAM ChoreoSequencer readable file with one source and one listener'
"""
__author__ = ("Natanael Olaiz", "CLAM Team")
__url__ = ("http://clam-project.org/")
__email__ = ["clam@iua.upf.edu"]
__version__ = "CLAM 1.3.1 - GSOC 2008"
__bpydoc__ = """\
This script exports a CLAM spacialization plugin ChoreoSequencer readable file.

Requires:
 - 'network_scene_exporter.py' script on the same path if you want to export 
     also a ready made CLAM network monitor.

Usage:
 - Select one object member of 'Audio_Listeners' group and one of the 'Audio_Sources'
    one as a listener and a source, then run the script. It will ask for a 
    ChoreoSequencer file first, and then (if 'network_scene_exporter.py' is on
    path) for a network filename.
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

import Blender
from bpy import data
from sys import path
import os
import BlenderOSCSender

path.append ( os.getcwd() )
path.append ( "%s/clam/CLAM/plugins/spacialization/blender" % os.getenv("HOME") )
import exporter

if Blender.sys.exists("network_scene_exporter.py")==1:
	path.append(Blender.sys.dirname("network_scene_exporter.py"))
	import network_scene_exporter as SceneExporter

def WriteSceneAsChoreo (choreoFilename):
#	Window.WaitCursor(1)
	bufferToWrite=exporter.choreoExport(Blender.Scene.GetCurrent(),False)
	if bufferToWrite:
		f=open(choreoFilename,'w')
		f.write(bufferToWrite)
		f.close()
		print "Exported choreo file: %s" % choreoFilename
	WriteChoreoNetwork(choreoFilename)

# generate CLAM network with the choreo sequencer and monitors
def WriteChoreoNetwork(choreoFilename):
	networkFilename=choreoFilename.replace(".choreo",".clamnetwork")
	networkId="ChoreoSequencer_Blender_exported_network"
	audioSourceName="AudioSync"
	audioSourceOutName="AudioOut"
	choreoSyncInName="sync"
	processings=""
	connections=""
	fps=Blender.Scene.GetCurrent().getRenderingContext().framesPerSec()
	choreoSequencerId="ChoreoSequencer"
	processings+=SceneExporter.makeAudioSource(audioSourceName,(-115,59))
	processings+=SceneExporter.makeChoreoSequencer(choreoSequencerId,choreoFilename,fps,(82,59))
	connections+= SceneExporter.makePortConnection(audioSourceName,audioSourceOutName,choreoSequencerId,choreoSyncInName)

	printersParameters=[\
		("target xyz",(37,184),3,(203,165)), \
		("target azimuth - zenith",(37,353),2,(203,130)), \
		("source xyz",(235,184),3,(203,165)), \
		("source azimuth - zenith",(235,353),2,(203,130)) \
	]
	choreoOuts = [\
		"target X","target Y","target Z", \
		"target azimuth","target elevation", \
		"source X","source Y","source Z", \
		"source azimuth","source elevation" \
	]
	outputNumber=0
	for parameters in printersParameters:
		printerName=parameters[0]
		printerPos=parameters[1]
		printerInputs=parameters[2]
		printerSize=parameters[3]
		processings+=SceneExporter.makeControlPrinter(printerName,printerPos,printerInputs,printerSize)
		for outIndex,outConnector in enumerate(choreoOuts[outputNumber:outputNumber+printerInputs]):
			connections+=SceneExporter.makeControlConnection(choreoSequencerId,outConnector,printerName,"ControlPrinter_"+str(outIndex))
			outputNumber+=1
	f=open(networkFilename,'w')
	body=processings+connections
	f.write(SceneExporter.Network % vars())
	f.close()
	print "Exported CLAM network: %s" % networkFilename

def main():
	Blender.Window.FileSelector(WriteSceneAsChoreo, "Export choreo sequencer file", Blender.sys.makename(ext=".choreo"))
		 
if __name__=='__main__':
	main()
