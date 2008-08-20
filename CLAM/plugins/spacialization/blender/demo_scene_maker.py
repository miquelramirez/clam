#!BPY
from bpy import data
import Blender
import math

MainSceneMaker="scene_maker.py"

def main():
#TODO: this doesn't clear all the data!
	if Blender.sys.exists(MainSceneMaker)!=1:
		Blender.Draw.PupMenu('Cannot found the scene creator (scene_maker.py). Aborting')
		return 0

	Blender.Run(MainSceneMaker)

	scene=data.scenes.active
	sources=data.groups['AudioSources'].objects
	for object in sources:
		object.setIpo(makeCircleIpo(object,object.name+"demo",6,2*math.pi*list(sources).index(object)/len(sources)))
		firstFrame=Blender.Get('staframe')
		x=object.ipo.getCurve()[0].evaluate(firstFrame)
		y=object.ipo.getCurve()[1].evaluate(firstFrame)
		z=object.ipo.getCurve()[2].evaluate(firstFrame)
		object.setLocation((x,y,z))

def makeCircleIpo(object,name,points,startPhase=0):
	frames=Blender.Get('endframe')-Blender.Get('staframe')
	newIpo=Blender.Ipo.New('Object',name)
	LocX=newIpo.addCurve('LocX')
	LocY=newIpo.addCurve('LocY')
	LocZ=newIpo.addCurve('LocZ')
	LocX.setInterpolation('Bezier')
	LocX.setExtrapolation('Cyclic_extrapolation')
	LocY.setInterpolation('Bezier')
	LocY.setExtrapolation('Cyclic_extrapolation')
	LocZ.setInterpolation('Bezier')
	LocZ.setExtrapolation('Cyclic_extrapolation')
	for parameter in range(0,points):
		t=parameter*1./points
		curveXPos=int(t*frames)+Blender.Get('staframe') #lineal time
		angle=2*math.pi*t + startPhase # just one cicle
		LocX.append((curveXPos,math.cos(angle)*3.5))
		LocY.append((curveXPos,math.sin(angle)*3.5))
		LocZ.append((curveXPos,(t*5)-2.5))
	return newIpo

# This lets you can import the script without running it
if __name__ == '__main__':
	main()
