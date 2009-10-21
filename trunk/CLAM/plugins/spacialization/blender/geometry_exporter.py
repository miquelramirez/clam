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

ListOfGeometriesToExport=['geometry','occlusion_geometry','environment_geometry'] # as the default selection is to export all kind of geometries
FilenameToExport="geometry.data"

# buttons callbacks functions:

def callBackListener(event,val):
	global ListOfGeometriesToExport
	print "callBackListener"
	print "event: %s\t-\tval: %s" % (event,val)
	ListOfGeometriesToExport=[]
	if val==1 or val==2:
		ListOfGeometriesToExport.append('geometry')
	if val==1 or val==3:
		ListOfGeometriesToExport.append('occlusion_geometry')
	if val==1 or val==4:
		ListOfGeometriesToExport.append('environment_geometry')
	return

def callbackExportButton(event,value):
	global ListOfGeometriesToExport
	print ListOfGeometriesToExport
	scene=Blender.Scene.GetCurrent()
	buffer=""
	for typeOfGeometry in ListOfGeometriesToExport:
		buffer+="# exported \'%s\' \n" % typeOfGeometry
		buffer+=exporter.geometryExport(scene,typeOfGeometry,True)
#	print buffer
	global FilenameToExport
	file=open(FilenameToExport,'w')
	file.write(buffer)
	file.close()
	return
	

def callbackChangeFilename(event,value):
	global FilenameToExport
#	print "callbackChangeFilename!"
#	print "event: %s - value: %s" % ( str(event),str(value) )
	FilenameToExport=str(value)
	return


# layouts position values:
yLayoutMarks = { 'sources': -70 }
#, 'listeners':offset- , 'materials': offset- }
xLayoutMarks = { 'labels': 0 , 'sublabels': 120, 'setters': 260, 'buttons': 120}

# specific layouts makers:
def layoutListeners(listeners,label="listeners",offset=(0,0)):
	global FilenameToExport
	xPosition=offset[0]+xLayoutMarks['sublabels']
	yPosition=offset[1]
	Blender.Draw.Label(label,xPosition-60,yPosition-5,120,30)
	menu="Export geometry%t"
	counter=1
	xPosition=offset[0]+xLayoutMarks['setters']
	menu+="|All geometries%x1|Only complete geometries%x2|Occlusion geometries%x3|Environment geometries%x4"
	menu=Blender.Draw.Menu(menu,0,xPosition,yPosition,200,20,1,"select geometry to export",callBackListener)
	yPosition-=30
	menu2=Blender.Draw.String("Name of output file:\t\t",10,xPosition-200,yPosition, 400,20,FilenameToExport,255,'select the file name',callbackChangeFilename)
	return yPosition
def layoutExporter(offset=(0,0)):
	xPosition=offset[0]+xLayoutMarks['buttons']
	yPosition=offset[1]
	Blender.Draw.PushButton("export!",12000,xPosition,yPosition,100,35,"export current scene",callbackExportButton)
	return yPosition
################################################################################
# GUI draw callback function:
def drawCallback():
	size=Blender.Window.GetAreaSize()
	xPos=100
	yPos=size[1]-100
	listeners=Acoustic.getListeners()
	yPos=layoutListeners(listeners,"Geometries to export:",(xPos,yPos))-100
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

if __name__=='__main__':
	main()
