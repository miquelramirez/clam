#!BPY
"""
Name: 'Exporter Module'
Blender: 247
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
import bpy
import BPyNMesh
import re
import os
import sys
sys.path.append( os.getcwd() )
sys.path.append( "%s/clam/CLAM/plugins/spacialization/blender" % os.getenv("HOME") )
import math
import Acoustic

# #blender exported
_geometryHeader="#3ds exported\n"
_geometryDataFilename="geometry.data"
_wavsDataFilename="exported_wavs.data"
_choreoFilename="exported_coreo.data"
_choreoHeader="#ClamChoreoVersion 1.1\n"
#_choreoHeader="#ClamChoreoVersion 1.2\n"

#options:
_convertToTriangles=True

#templates:
FaceLineTemplate="%(verts)s %(impedanceReal)f %(impedanceImag)f %(diffusion)f %(materialName)s\n"
ChoreoLineTemplate="%(frame)i\t0\t%(targetAzimuth)f\t%(targetElevation)f\t%(targetX)f\t%(targetY)f\t%(targetZ)f\t%(sourcesPositions)s\n"
WavLineTemplate="%(wavFilename)s\t%(objectName)s\n"


# Normalizacion parameters (Null at start):
_normalizationOffset=[0,0,0]
#Blender.Mathutils.Vector(0,0,0)
_normalizationScale=[0,0,0]
#Blender.Mathutils.Vector(0,0,0)


def getGeometryLimits(objectsList):
	object=objectsList[0]
	location=object.mat.translationPart()
	minLocation=[location[0],location[1],location[2]]
	maxLocation=[location[0],location[1],location[2]]
	for object in objectsList:
		if type(object.getData())!=Blender.Types.NMeshType:
			continue
		vectorList=object.getBoundBox()
		for vector in vectorList:
			for dimension in range(3):
				if vector[dimension]<minLocation[dimension]:
					minLocation[dimension]=vector[dimension]
				if vector[dimension]>maxLocation[dimension]:
					maxLocation[dimension]=vector[dimension]
	minLocation=(minLocation[0],minLocation[1],minLocation[2])
	maxLocation=(maxLocation[0],maxLocation[1],maxLocation[2])
	return minLocation,maxLocation 

def wavsExport(scene):
	buffer=""
	for source in Acoustic.getSources():
		wavFilename=Acoustic.getWav(source)
		if wavFilename==0 or source.sel==0:
			continue
		objectName=source.name
		buffer+=WavLineTemplate % vars()
	return buffer	

def geometryExport(scene):
	buffer=_geometryHeader
	actors=[]
	actors=Acoustic.getActors()
#	print "---------- acoustic actors: %s" % list(actors)
#	for object in scene.objects:
#	print "---------- acoustic objects: %s" % list(Acoustic.getAcousticObjects(scene))
	for object in Acoustic.getAcousticObjects(scene):
		print "Reading object: %s" % object.name
#		data=bpy.data.meshes[object.getData().name]
		if type(object.getData())!=Blender.Types.NMeshType:
			print "no mesh, skipping..."
			continue
		if actors.count(object)!=0:
			print "%s is an actor, skipping" % object.name
			continue

		BPyNMesh.ApplySizeAndRotation(object)
#		data=object.getData() # do it again after transformation
		data=bpy.data.meshes[object.getData().name]
		print "Exporting %s..."%object.name
		bufferObject= "<%s>\n" % object.name
		bufferObject+= "<VERTS>\n"
#		location=object.getLocation()
		location=object.mat.translationPart()
		for vert in data.verts:
			bufferObject+="%f %f %f\n" % (vert.co[0]+location[0],vert.co[1]+location[1],vert.co[2]+location[2])
		bufferObject+="<FACES>\n"
		materials=Acoustic.getObjectMaterials(object)
		if materials==None: 
			print "doesnt contains acoustic material, skipping..."
			continue #if doesnt have any acoustic (by name) material, doesnt export

		# else, use first linked material with acoustic properties 
		impedance=complex(0,0)
		diffusion=0.0
		materialName=""
		for material in materials:
			print material.name
			impedance=Acoustic.getImpedance(material)
			if (impedance==None):
				continue
			diffusion=Acoustic.getDiffusion(material)
			if (diffusion==None):
				continue
			materialName=material.name
			break
		if (impedance==None) or (diffusion==None):
			print "any material have acoustic parameters, skipping..."
			continue 
		impedanceReal=impedance.real
		impedanceImag=impedance.imag
		if _convertToTriangles==True:
			print "Converting mesh to triangles..."
#			bpy.data.meshes[data.name].quadToTriangle(0)
			bpy.data.meshes[object.getData().name].quadToTriangle(0)
		for face in data.faces:
			verts=""
			for vert in face.verts[::-1]:
				verts+="%s " % str(vert.index+1)
			bufferObject+=FaceLineTemplate % vars()
		buffer+=bufferObject
		print "Added Object: %s" % object.name
	return buffer	

def getNormalizationParameters(objects):
	minLocation=[0,0,0]
	maxLocation=[0,0,0]
	for frame in range(Blender.Get('staframe'),Blender.Get('endframe')):
		Blender.Set('curframe',frame)
		minLocationFrame,maxLocationFrame=getGeometryLimits(objects)
		for dimension in range(3):
			if minLocationFrame[dimension]<minLocation[dimension]:
				minLocation[dimension]=minLocationFrame[dimension]
			if maxLocationFrame[dimension]>maxLocation[dimension]:
				maxLocation[dimension]=maxLocationFrame[dimension]
	for dimension in range(3):
		distance=maxLocation[dimension]-minLocation[dimension]
		if distance==0:
			_normalizationScale[dimension]=0
		else:
			_normalizationScale[dimension]=1./distance
		_normalizationOffset[dimension]=minLocation[dimension]
	return _normalizationOffset,_normalizationScale


def normalizePosition(originalPosition,offset=_normalizationOffset,scale=_normalizationScale,version1=False):
	normalizedPosition=[0,0,0]
	for dimension in range(3):
		normalizedPosition[dimension]=(originalPosition[dimension]-offset[dimension])
		if version1==True:
			normalizedPosition[dimension]*=scale[dimension]
	return normalizedPosition


def choreoExport (scene,normalize=True):
	SelectedObjects = Blender.Object.GetSelected()
	listeners=Acoustic.getListeners()
	sources=[]
	target=None
	buffer=""
	for source in Acoustic.getSources():
		if source.sel==1:
			sources.append(source)
	for listener in listeners:
		if listener.sel==1:
			target=listener
			break
	if target==None or not sources:
		Blender.Draw.PupMenu('You have to select one listener and at least one source objects!')
		return
	allAcousticObjects=Acoustic.getAcousticObjects(scene)
	# print list(allAcousticObjects)
	#TODO: refactor this (checked three times!)
	if normalize==True:
		normalizationOffset,normalizationScale=getNormalizationParameters(allAcousticObjects)
	for frame in range(Blender.Get('staframe'),Blender.Get('endframe')):
		Blender.Set('curframe',frame)
		roll,descention,azimuth=target.mat.toEuler()
		targetElevation=(-descention)%360
		if targetElevation>180:
			targetElevation=targetElevation-360
		targetRoll=(roll)%360
		targetAzimuth=(azimuth)%360
		if normalize==True:
			targetX,targetY,targetZ=normalizePosition(listener.mat.translationPart(),normalizationOffset,normalizationScale,False)
		else:
			targetX,targetY,targetZ=listener.mat.translationPart()
		sourcesPositions=""
		for source in sources:
			if source.sel==1:
				if normalize==True:
					sourceX,sourceY,sourceZ=normalizePosition(source.mat.translationPart(),normalizationOffset,normalizationScale,False)
				else:
					sourceX,sourceY,sourceZ=source.mat.translationPart()
				sourcesPositions+=" %f\t%f\t%f" % (sourceX,sourceY,sourceZ)
		buffer+=ChoreoLineTemplate % vars()
	return buffer

def main():
	scene=Blender.Scene.GetCurrent()

	buffer=geometryExport(scene)
	file=open(_geometryDataFilename,'w')
	geometryFilename="%s/%s" % (os.getcwd(),file.name)
	print "Exporting geometry data file: %s..." % geometryFilename
	file.write(buffer)
	file.close()
	print "done!"

	buffer=wavsExport(scene)
	file=open(_wavsDataFilename,'w')
	wavsFilename="%s/%s" % (os.getcwd(),file.name)
	print "Exporting wavs data file: %s..." % wavsFilename
	file.write(buffer)
	file.close()
	print "done!"

	buffer=choreoExport(scene,(_choreoHeader=="#ClamChoreoVersion 1.1\n"))
	file=open(_choreoFilename,'w')
	choreoFilename="%s/%s" % (os.getcwd(),file.name)
	print "Exporting coreo sequence on file: %s..." % choreoFilename 
	file.write(_choreoHeader)
	file.write(buffer)
	file.close()
	print "done!"

	Blender.Draw.PupBlock('Export finished',['Data files sucessfully exported','Geometry file: %s' % geometryFilename, 'Wavs data file: %s' % wavsFilename, 'Choreo file: %s' % choreoFilename])

def makeFilesFromCallBack(event,value):
	main()

if __name__=='__main__':
	main()
