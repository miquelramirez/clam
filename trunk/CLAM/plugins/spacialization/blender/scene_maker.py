#!BPY
from bpy import data
import Blender

#comment the filenames of the scripts you don't want to use
NetworkExporterFilename="network_scene_exporter.py"
OSCSenderFilename="BlenderOSCSender.py"

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
		
def ShowConfigurationDialog(defaultSources=1,defaultSinks=1):
	Draw=Blender.Draw
	SourcesNumber=Draw.Create(defaultSources)
	SinksNumber=Draw.Create(defaultSinks)
	
	if not Draw.PupBlock('',[\
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
