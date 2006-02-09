#!/usr/bin/python

from qt import *
from GUI import GUI
from Tasker import Tasker, TaskerError
import sys, os

form=None
app=None
outputfunction=None
tasker=None

def selectFile():
	s = QFileDialog.getOpenFileName(
		"",
		"Simac Task file (*.task)\nAll Files (*)",
		None,
		"open file dialog"
		"Choose a file" )
	global form
	if len(s) > 0:
		form.taskEdit.setText(s)

def selectDirectory():
	s = QFileDialog.getExistingDirectory(
		"",
		None,
		None,
		"Choose a directory" )
	global form
	if len(s) > 0:
		form.pathEdit.setText(s)

def startProcess():
	global form,tasker
	if form.taskEdit.text()=="" or form.projectEdit.text()=="" \
		or form.pathEdit.text()=="":
		QMessageBox.warning( None, "Data missing",
			"You must specify the task file, the project name and the destination directory\n",
			"I see")
	else:
		try:
			tasker.setParameters( str(form.taskEdit.text()), str(form.projectEdit.text()), str(form.pathEdit.text()) )
			tasker.processTask()	
			printLog(u"\n  == CLAM-Annotator ==\n")
			printLog(u" - Launching...\n")
			form.hide()
			#tasker.runAnnotator()
			form.show()
			printLog(u" - Finalized\n")
			tasker.uploadChanges()
		except TaskerError, x:
			title=str(x).split('\n')[0]
			message=str(x).split('\n')[1:]
			QMessageBox.warning( None, "%s" % title,
				"%s\n" % ('\n').join(message),
				"I see" )
		else:
			form.goButton.setEnabled(False)
			form.uploadButton.setEnabled(True)
			form.taskEdit.setEnabled(False)
			form.taskButton.setEnabled(False)
			form.projectEdit.setEnabled(False)
			form.pathEdit.setEnabled(False)
			form.pathButton.setEnabled(False)
			form.uploadButton.setFocus()

def uploadChanges():
	global form
	
	form.uploadButton.setEnabled(False)

	#your code here

	printLog(u"Uploaded (fake)\n")
	form.exitButton.setFocus()

def printLog( message ):
	global outputfunction
	outputfunction(message)

def quit():
	global app
	print "quit"
	app.quit()

def createConnections():
	global form, app
	app.connect( form.taskButton, SIGNAL( "clicked()" ), selectFile )
	app.connect( form.pathButton, SIGNAL( "clicked()" ), selectDirectory )
	app.connect( form.goButton, SIGNAL( "clicked()" ), startProcess )
	app.connect( form.uploadButton, SIGNAL( "clicked()" ), uploadChanges )
	app.connect( form.exitButton, SIGNAL( "clicked()" ), quit )
	app.connect( app, SIGNAL( "lastWindowClosed()" ), quit )

def initWidgets():
	global form, outputfunction
	form.uploadButton.setEnabled( False )
	form.logEdit.setReadOnly( True )
	outputfunction=form.logEdit.append
	form.pathEdit.setText( os.getcwd() )

def main( args ):
	global form,app,tasker

	app=QApplication( args )
	form=GUI()
	form.show()
	app.setMainWidget( form )

	initWidgets()
	if len(args) == 2:
		form.taskEdit.setText( args[1] )
	createConnections()

	tasker=Tasker( form.logEdit.append )

	app.exec_loop()

if __name__ == "__main__":
	main( sys.argv )
