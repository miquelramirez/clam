#!BPY
# -*- coding: iso-8859-1 -*-

"""
Name: 'CLAM & Blender scene demo creator'
Blender: 244
Group: 'Misc, Export'
Tooltip: 'Generate a Blender scene and make the sources move in circles around the coordinates origin'
"""
__author__ = ("Natanael Olaiz", "CLAM Team")
__url__ = ("http://clam.iua.upf.edu/")
__email__ = ["clam@iua.upf.edu"]
__version__ = "CLAM 1.3.1 - GSOC 2008"
__bpydoc__ = """\
This script first calls the Scene Maker script and then apply ipoCurves 
 (trajetories) to make the sources turn around the center of coordinates
 origin

Requires:
 - 'scene_maker.py' on path 

Usage:
 - Just call it

See also:
 - scene_maker.py
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
import math

MainSceneMaker="scene_maker.py"

def main():
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
