#!/usr/bin/python

from qt import *
from GUI import GUI
import sys

form=None
app=None

def selectFile():
	s = QFileDialog.getOpenFileName(
		"",
		"Simac Task file (*.tsk)\nAll Files (*)",
		None,
		"open file dialog"
		"Choose a file" )
	global form
	form.taskEdit.setText(s)
	print s

def startProcess():
	global form
	if form.taskEdit.text()=="" or form.projectEdit.text()=="":
		QMessageBox.warning( None, "Data missing",
			"You must specify the task file and the project name\n",
			"I see")
	else:
		log("go")
		form.goButton.setEnabled(False)
		form.uploadButton.setEnabled(True)

def uploadChanges():
	global form
	log("uploaded")
	
	form.goButton.setEnabled(True)
	form.uploadButton.setEnabled(False)

def log( message ):
	global form
	form.logEdit.append(message)

def quit():
	print "quit"

def createConnections():
	global form, app
	app.connect( form.taskButton, SIGNAL( "clicked()" ), selectFile )
	app.connect( form.goButton, SIGNAL( "clicked()" ), startProcess )
	app.connect( form.uploadButton, SIGNAL( "clicked()" ), uploadChanges )
	app.connect( app, SIGNAL( "lastWindowClosed()" ), quit )

def initWidgets():
	global form
	form.uploadButton.setEnabled( False )
	form.logEdit.setReadOnly( True )

def main( args ):
	global form,app

	app=QApplication( args )
	form=GUI()
	form.show()
	app.setMainWidget( form )

	initWidgets()
	if len(args) == 2:
		form.taskEdit.setText( args[1] )
	createConnections()

	app.exec_loop()

if __name__ == "__main__":
	main( sys.argv )
