#!BPY
"""
Name: 'Acoustic Module'
Blender: 247
Tooltip: 'Module to manage acoustic parameters and sound objects. Used by other Blender python spacialization scripts.'
"""
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
import re

# definitions and options:
# materials:
_useDataBlockMaterials=True
_useObjectMaterials=True

# string patterns on name:
_materialsPatternId='Abs[0-9]+Dif[0-9]+'
_listenersPatternId='listener'
_sourcesPatternId='source'

# properties keys:
_sourcesWavPropertyId='WAV_SOURCE'
_acousticsPropertyId='Acoustic_parameters'

def AcousticParameters():
	return	{ 'impedance': { 'real': 10000.0, 'imag': 0.0 },
			 'diffusion': 0.15 }
def SourcesParameters():
	return { _sourcesWavPropertyId: str() }
# TODO: make it a class??
#class Acoustic:
#	def __init__(self,scene):
#	...

def testObjectName(patternId,object,caseSensitive=False):
	name=object.name
	if caseSensitive==False:
		name=name.lower()
		patternId=patternId.lower()
	return (re.search(patternId,name) != None)

def getProperty(object,propertyName):
	try:
		property=object.properties[propertyName]
	except:
		try:
			property=object.getData().properties[propertyName]
		except:
			return None # the property doesn't exist in datablock or instance.
		return property # the property is on datablock
	return property # the property is on object instance

def makeProperty(object,propertyName,propertyType):
	object.properties[propertyName]=propertyType
	return getProperty(object,propertyName)

# listeners related methods
def isListener(object):
	return testObjectName(_listenersPatternId,object)
def getListeners(scene=Blender.Scene.GetCurrent()):
	listeners=[]
	for object in scene.objects:
		if isListener(object):
			listeners.append(object)
	return listeners

# acoustic sources related methods
def isSource(object):
	return testObjectName(_sourcesPatternId,object)
def getSources(scene=Blender.Scene.GetCurrent()):
	sources=[]
	for object in scene.objects:
		if isSource(object):
			sources.append(object)
	return sources

# acoustic materials related methods
def isAcoustic(material):
	return testObjectName(_materialsPatternId,material,True)
# getAcousticObjects: get the list of objects which have materials with a given pattern
def getObjectMaterials(object,scene=Blender.Scene.GetCurrent()):
	acousticMaterials=[]
	materials=[]
	if _useDataBlockMaterials==True:
		try:
			materials+=object.getData().getMaterials()
		except:
			None			
	if _useObjectMaterials==True:
		try:
			materials+=object.getMaterials()
		except:
			None
	if len(materials)==0:
		return None
	for material in materials:
		if isAcoustic(material)==True:
			acousticMaterials.append(material)
	return acousticMaterials


def getAcousticObjects(scene=Blender.Scene.GetCurrent()):
	objects=[]
	for object in scene.objects:
		if (getObjectMaterials(object,scene)!=None):
			objects.append(object)
	return objects
	

def getMaterials():
	acousticMaterials=[]
	for material in Blender.Material.Get():
		if isAcoustic(material):
			acousticMaterials.append(material)
	return acousticMaterials

def getImpedance(material):
	acoustic=getProperty(material,_acousticsPropertyId)
	assert acoustic, "getImpedance not found with material " + material.name
	#print acoustic['impedance']['real'],acoustic['impedance']['imag']
	return complex(acoustic['impedance']['real'],acoustic['impedance']['imag'])
def setImpedance(material,value):
	acoustic=getProperty(material,_acousticsPropertyId)
	acoustic['impedance']['real']=value.real
	acoustic['impedance']['imag']=value.imag
	material.properties[_acousticsPropertyId]=acoustic
	return
def getDiffusion(material):
	acoustic=getProperty(material,_acousticsPropertyId)
	return acoustic['diffusion']
def setDiffusion(material,value):
	acoustic=getProperty(material,_acousticsPropertyId)
	acoustic['diffusion']=value
	material.properties[_acousticsPropertyId]=acoustic
	return
def getWav(object):
	property=getProperty(object,_sourcesWavPropertyId)
	return property
def setWav(object,value):
	makeProperty(object,_sourcesWavPropertyId,value)
	return

# getActors: get all selected listener(s?) and sources
# note that actually, you can export only one listener, so is expected to have only one selected listener
def getActors(scene=Blender.Scene.GetCurrent()):
	actors=[]
	for listener in getListeners(scene):
		if listener.sel==1:
			actors.append(listener)
	for source in getSources(scene):
		if source.sel==1:
			actors.append(source)
	return actors

# generate all missing properties
def initProperties(scene=Blender.Scene.GetCurrent()):
	for material in getMaterials():
		if getProperty(material,_acousticsPropertyId)==None:
			acoustic=AcousticParameters()
			makeProperty(material,_acousticsPropertyId,acoustic)
	sounds=Blender.Sound.Get()
	for source in getSources(scene):
		if getProperty(source,_sourcesWavPropertyId)==None:
			if len(sounds)>0:
				fileName=sounds[0].getFilename()
				makeProperty(source,_sourcesWavPropertyId,fileName)
			else:
				print "Error: there are sources, but there is any available sound file. Aborting.."
				return -1
