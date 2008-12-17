#!BPY
"""
Name: 'work in progress of scene acoustic parameters generator-editor'
Blender: 247
"""
#
import Blender
import re
import os
import sys
sys.path.append(os.getcwd())
sys.path.append( "%s/clam/CLAM/plugins/spacialization/blender" % os.getenv("HOME") )
import Acoustic
import exporter

#class GUI:
Events = {  'wav_source':0, 'export': 1024 ,'impedance':4096 , 'impedance_real': 4096 , 'impedance_imaginary': 4096|8192, 'diffusion': 8192 }
EventsMask=1023

# buttons callbacks functions:
def callBackSourcesMenu(event,val):
	source=event&EventsMask
	if event&Events['export']==0:
		Acoustic.getSources()[source].properties['WAV_SOURCE']=Blender.Sound.Get()[val-1].getFilename()
		return
	#else:
	sources=Acoustic.getSources()[source].sel=val
	return
def callBackMaterialButtons(event,val):
	try:
		val=float(val)
	except:
		return
	material=Acoustic.getMaterials()[event&EventsMask]
	if event&Events['impedance']:
#		print "material: %s" % material
		impedance=Acoustic.getImpedance(material)
		if (event&Events['impedance_imaginary'])==Events['impedance_imaginary']:
			Acoustic.setImpedance(material,complex(impedance.real,val))
#			print "setting imag to %s" % val
			return
		else:
			Acoustic.setImpedance(material,complex(val,impedance.imag))
#			print "setting real to %s" % val
			return
	else:
#		print "setting diffusion to %s" % val
		Acoustic.setDiffusion(material,val)
	return
def callBackListener(event,val):
	print "callBackListener"
	print "event: %s\t-\tval: %s" % (event,val)
	listeners=Acoustic.getListeners()
	if val>0:
		newListener=listeners[val-1]
		for listener in listeners:
			if listener==newListener:
				listener.sel=1
			else:
				listener.sel=0
	return



# layouts position values:
yLayoutMarks = { 'sources': -70 }
#, 'listeners':offset- , 'materials': offset- }
xLayoutMarks = { 'labels': 0 , 'sublabels': 120, 'setters': 260, 'buttons': 490 }

# specific layouts makers:
def layoutSources(sources,label="audio sources",offset=(0,0)):
	yPosition=offset[1]+yLayoutMarks['sources']
	xPosition=offset[0]+xLayoutMarks['labels']
	Blender.Draw.Label(label,xPosition,yPosition,120,20)
	for object in sources:
		objectIndex=sources.index(object)
		xPosition=offset[0]+xLayoutMarks['sublabels']
		Blender.Draw.Label(object.name,xPosition,yPosition,150,20)
		xPosition=offset[0]+xLayoutMarks['setters']
		menu="imported sounds %t"
		counter=indexFile=1
		for sound in Blender.Sound.Get():
			if sound.getFilename()==Acoustic.getWav(object):
				indexFile=counter
			menu+="|"+sound.getFilename()+"%x"+str(counter)
			counter+=1
		menu=Blender.Draw.Menu(menu,objectIndex|Events['wav_source'],xPosition,yPosition,200,20,indexFile,"select sound file",callBackSourcesMenu)
		xPosition=offset[0]+xLayoutMarks['buttons']
		selected=object.sel
		export=Blender.Draw.Toggle ("select",objectIndex|Events['export'],xPosition,yPosition,100,20,selected,"select to include it on exportation",callBackSourcesMenu)
		yPosition-=30
	return yPosition
def layoutMaterials(materials,label="acoustic materials",offset=(0,0)):
	xPosition=offset[0]+xLayoutMarks['labels']
	yPosition=offset[1]
	Blender.Draw.Label(label,xPosition,yPosition,120,30)
	counter=0
	for material in materials: 
		xPosition=offset[0]+xLayoutMarks['sublabels']
		Blender.Draw.Label(material.name,xPosition,yPosition,120,30)
		impedance=Acoustic.getImpedance(material)
		diffusion=Acoustic.getDiffusion(material)
		xPosition=offset[0]+xLayoutMarks['setters']
		number=Blender.Draw.String("imp. real\t\t",counter|Events['impedance_real'],xPosition,yPosition,200,20,str(impedance.real),30,"set impedance (real)",callBackMaterialButtons)
		yPosition-=30
		number=Blender.Draw.String("imp. imag\t\t",counter|Events['impedance_imaginary'],xPosition,yPosition,200,20,str(impedance.imag),30,"set impedance (imaginary)",callBackMaterialButtons)
		yPosition-=30
		number=Blender.Draw.String("diff.\t\t\t\t",counter|Events['diffusion'],xPosition,yPosition,200,20,str(diffusion),30,"set diffusion",callBackMaterialButtons)
		yPosition-=40
		counter+=1
	return yPosition
def layoutListeners(listeners,label="listeners",offset=(0,0)):
	xPosition=offset[0]+xLayoutMarks['sublabels']
	yPosition=offset[1]
	listeners=Acoustic.getListeners()
	selectedListener=0
	# get first selected listener (or 0)
	for listener in listeners:
		if list(Blender.Scene.GetCurrent().objects.selected).count(listener)!=0:
			selectedListener=listeners.index(listener)+1
			break
	Blender.Draw.Label(label,xPosition,yPosition,120,30)
	menu="Listeners %t"
	counter=1
	xPosition=offset[0]+xLayoutMarks['setters']
	for listener in listeners:
		menu+="|"+listener.name+"%x"+str(counter)
		counter+=1
	menu=Blender.Draw.Menu(menu,0,xPosition,yPosition,200,20,selectedListener,"select listener to export",callBackListener)
	return yPosition
def layoutExporter(offset=(0,0)):
	xPosition=offset[0]+xLayoutMarks['buttons']
	yPosition=offset[1]
	Blender.Draw.PushButton("export!",12000,xPosition,yPosition,100,35,"export current scene",exporter.makeFilesFromCallBack)
	return yPosition
################################################################################
# GUI draw callback function:
def drawCallback():
	size=Blender.Window.GetAreaSize()
	xPos=0
	yPos=size[1]
	sources=Acoustic.getSources()
	yPos=layoutSources(sources,"Audio sources:",(xPos,yPos))-40
	materials=Acoustic.getMaterials()
	yPos=layoutMaterials(materials,"Acoustic materials:",(xPos,yPos))-40
	listeners=Acoustic.getListeners()
	yPos=layoutListeners(listeners,"Select listener:",(xPos,yPos))-10
	layoutExporter((xPos,yPos))
	return
# GUI events callback funcion:
def eventCallback(event,value):
	if event == Blender.Draw.ESCKEY:
		Blender.Draw.Exit()
	return

def main():
	if (Acoustic.initProperties()!=-1):
		Blender.Draw.Register(drawCallback,eventCallback)
#
#	scene=Blender.Scene.GetCurrent()
#	configureWavs(scene)
#	configureAcoustics(scene)
#
if __name__=='__main__':
	main()