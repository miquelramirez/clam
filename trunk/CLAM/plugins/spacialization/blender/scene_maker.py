#!BPY
from bpy import data
import Blender

def AddSinks(scene,sinksNumber):
	group=data.groups.new('AudioSinks')
	offsetX=(sinksNumber-1)*3./(-2)
	for i in range(sinksNumber):
		object=scene.objects.new(Blender.Mesh.Primitives.UVsphere(32,32,1),'sink'+str(i))
		object.setLocation([offsetX,-10,0])
		offsetX+=3
		group.objects.link(object)
	
def AddSources(scene,sourcesNumber):
	group=data.groups.new('AudioSources')
	offsetX=(sourcesNumber-1)*3./(-2)
	for i in range(sourcesNumber):
		object=scene.objects.new(Blender.Mesh.Primitives.UVsphere(32,32,1),'source'+str(i))
		object.setLocation([offsetX,10,0])
		offsetX+=3
		group.objects.link(object)

def main():
	Draw=Blender.Draw
	SourcesNumber=Draw.Create(1)
	SinksNumber=Draw.Create(1)
	if not Draw.PupBlock('',[\
	('Sources number:',SourcesNumber,1,100,'number of sources'),\
	('Sinks number:',SinksNumber,1,100,'number of sinks'),\
	]):
		return
	scene=data.scenes.new()
	data.scenes.active=scene
	AddSinks(scene,SinksNumber.val)
	AddSources(scene,SourcesNumber.val)

main()