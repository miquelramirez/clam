#!BPY
# -*- coding: iso-8859-1 -*-

"""
Name: 'CLAM scene generator'
Blender: 244
Group: 'Export'
Tooltip: 'Create a new scene with m audio sinks and n audio sources'
"""
__author__ = ("Natanael Olaiz", "CLAM Team")
__url__ = ("http://clam.iua.upf.edu/")
__email__ = ["clam@iua.upf.edu"]
__version__ = "CLAM 1.3.1 - GSOC 2008"
__bpydoc__ = """\
This script generates a new blender scene with audio sinks and sources 
(for now just UVSpheres, within the groups 'AudioSinks' and 'AudioSources').

Optional:
 - 'BlenderOSCSender.py' on path, to link the OSC sender to the new scene 
     FrameChanged event (sending the objects positions via OSC messages).
 - 'network_scene_exporter.py' on path, to generate an OSC CLAM monitor
     for the created objects.
 - 'choreofile_exporter.py' on path, to generate a ChoreoSequencer 
     readable file.

Usage:
 - Just run it to create a new scene.

TODO:
 - the CLAM syntax uses the dot '.' as a connection symbol. As the CLAM Networks 
    objects names are inherited from the Blender ones, the generated network could
    fail on read. 
    Furthermore, the dot '.' is used for duplicated objects in Blender!
 - it doesn't clear all the previous scene

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
import Blender

#comment the filenames of the scripts you don't want to use
NetworkExporterFilename="network_scene_exporter.py"
OSCSenderFilename="BlenderOSCSender.py"
ChoreoExporterFilename="choreofile_exporter.py"

def main():
	SourcesNumber,SinksNumber=ShowConfigurationDialog(1,1)
	newscene=data.scenes.new()
	data.scenes.active=newscene
#TODO: this doesn't clear all the data!
	for group in data.groups:
		data.groups.unlink(group)
	for scene in data.scenes:
		for ob in scene.objects:
			scene.objects.unlink(ob)
		if scene!=newscene:
			data.scenes.unlink(scene)
	AddSinks(newscene,SinksNumber.val)
	AddSources(newscene,SourcesNumber.val)
	if (NetworkExporterFilename and Blender.sys.exists(NetworkExporterFilename)==1): # if is an existent filename
		NetworkExporter=Blender.Text.Load(NetworkExporterFilename)
		Blender.Run(NetworkExporter.name)
	if (OSCSenderFilename and Blender.sys.exists(OSCSenderFilename)==1):
		OSCSender=Blender.Text.Load(OSCSenderFilename)
		data.scenes.active.addScriptLink(OSCSender.name,'FrameChanged')
	if (ChoreoExporterFilename and Blender.sys.exists(ChoreoExporterFilename)==1):
		Blender.Text.Load(ChoreoExporterFilename)
		
def ShowConfigurationDialog(defaultSources=1,defaultSinks=1):
	Draw=Blender.Draw
	SourcesNumber=Draw.Create(defaultSources)
	SinksNumber=Draw.Create(defaultSinks)
	
	if not Draw.PupBlock('Define the objects to create',[\
	('Sources number:',SourcesNumber,1,100,'number of sources'),\
	('Sinks number:',SinksNumber,1,100,'number of sinks'),\
	]):
	    return 0,0
	else:
	    return SourcesNumber,SinksNumber

def AddSinks(scene,sinksNumber):
	group=data.groups.new('AudioSinks')
	offsetX=(sinksNumber-1)*3./(-2)
	mesh=Blender.Mesh.Primitives.UVsphere(32,32,1)
	mesh.name="omni_sink_model"
	for i in range(sinksNumber):
		object=scene.objects.new(mesh,'sink'+str(i))
		object.setLocation([offsetX,-5,0])
		offsetX+=3
		group.objects.link(object)
	
def AddSources(scene,sourcesNumber):
	group=data.groups.new('AudioSources')
	offsetX=(sourcesNumber-1)*3./(-2)
	mesh=Blender.Mesh.Primitives.UVsphere(32,32,1)
	mesh.name="omni_source_model"	
	for i in range(sourcesNumber):
		object=scene.objects.new(mesh,'source'+str(i))
		object.setLocation([offsetX,5,0])
		offsetX+=3
		group.objects.link(object)

# This lets you can import the script without running it
if __name__ == '__main__':
	main()
