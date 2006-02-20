#!/usr/bin/python

from qt import *
from GUI import GUI
from Tasker import Tasker, TaskerError
import sys, os
from threading import Thread

#TODO threading


def selectFile():
	s = QFileDialog.getOpenFileName(
		"",
		"Simac Task file (*.task)\nAll Files (*)",
		None,
		"open file dialog"
		"Choose a file" )
	if len(s) > 0:
		form.taskEdit.setText(s)

def selectDirectory():
	s = QFileDialog.getExistingDirectory(
		"",
		None,
		None,
		"Choose a directory" )
	if len(s) > 0:
		form.pathEdit.setText(s)

def startProcess():
	if form.taskEdit.text()=="" or form.projectEdit.text()=="" \
		or form.pathEdit.text()=="":
		QMessageBox.warning( None, "Data missing",
			"You must specify the task file, the project name and the destination directory\n",
			"I see")
	else:
		try:
			tasker.setParameters( str(form.taskEdit.text()), str(form.projectEdit.text()), str(form.pathEdit.text()) )
			tasker.processTask()
			qrect=form.geometry()
			form.hide()
			tasker.runAnnotator()
			form.show()
			form.setGeometry(qrect)
			modified=tasker.getModified()
			if len(modified)>0:
				choice=QMessageBox.information( form, "Modified descriptor files",
					u"The following descriptor pool files will be uploaded:\n  -" + ('\n  - ').join(modified),
					"No way", "Go ahead" )
				if choice==1:	# 0,1 o ESC=-1
					tasker.uploadChanges()
			else:
				QMessageBox.information( form, "Modified descriptor files",
					u"No descriptor pool was modified. Nothing will be uploaded.",
					"OK" )
				
		except TaskerError, x:
			title=str(x).split('\n')[0]
			message=str(x).split('\n')[1:]
			QMessageBox.warning( None, "%s" % title,
				"%s\n" % ('\n').join(message),
				"I see" )
			outputfunction(u"\n  *** Error found. Correct it and then restart the process ***\n")
		else:
			form.goButton.setEnabled(False)
			form.taskEdit.setEnabled(False)
			form.taskButton.setEnabled(False)
			form.projectEdit.setEnabled(False)
			form.pathEdit.setEnabled(False)
			form.pathButton.setEnabled(False)
			form.exitButton.setFocus()

def quit():
	#print "quit"
	app.quit()

def createConnections():
	app.connect( form.taskButton, SIGNAL( "clicked()" ), selectFile )
	app.connect( form.pathButton, SIGNAL( "clicked()" ), selectDirectory )
	app.connect( form.goButton, SIGNAL( "clicked()" ), startProcess )
	app.connect( form.exitButton, SIGNAL( "clicked()" ), quit )
	app.connect( app, SIGNAL( "lastWindowClosed()" ), quit )

def initWidgets():
	global outputfunction
	form.logEdit.setReadOnly( True )
	outputfunction=form.logEdit.append
	form.pathEdit.setText( os.getcwd() )

def main( args ):
	global app, form, tasker

	app=QApplication( args )
	form=GUI()
	form.show()
	app.setMainWidget( form )

	initWidgets()
	if len(args) > 1:
		form.taskEdit.setText( args[1] )
	if len(args) > 2:
		form.projectEdit.setText( args[2] )
	createConnections()

	tasker=Tasker( form.logEdit.append )

	app.exec_loop()

if __name__ == "__main__":
	main( sys.argv )
