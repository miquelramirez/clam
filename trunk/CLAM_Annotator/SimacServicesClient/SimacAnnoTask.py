#!/usr/bin/python

from qt import *
from GUI import GUI
from Tasker import *
import sys

form=None
app=None
outputfunction=None
client=None

def selectFile():
	s = QFileDialog.getOpenFileName(
		"",
		"Simac Task file (*.task)\nAll Files (*)",
		None,
		"open file dialog"
		"Choose a file" )
	global form
	form.taskEdit.setText(s)

def startProcess():
	global form,client
	if form.taskEdit.text()=="" or form.projectEdit.text()=="":
		QMessageBox.warning( None, "Data missing",
			"You must specify the task file and the project name\n",
			"I see")
	else:
		printLog("go")
		try:
			client.setParameters( str(form.taskEdit.text()), str(form.projectEdit.text()) )
			client.processTask()	
			form.hide()
			#os.system("Annotator %s.pro" % client.projectname)
			form.show()
	
		except ClientError, x:
			QMessageBox.warning( None, "%s" % str(x).split('\n')[0],
				"%s\n" % str(x).split('\n')[1],
				"I see")
		else:
			form.goButton.setEnabled(False)
			form.uploadButton.setEnabled(True)
			form.taskEdit.setEnabled(False)
			form.projectEdit.setEnabled(False)
			form.taskButton.setEnabled(False)

def uploadChanges():
	global form
	printLog("Uploaded")
	
	form.goButton.setEnabled(True)
	form.uploadButton.setEnabled(False)

def printLog( message ):
	outputfunction(message)

def quit():
	pass
	#print "quit"

def createConnections():
	global form, app
	app.connect( form.taskButton, SIGNAL( "clicked()" ), selectFile )
	app.connect( form.goButton, SIGNAL( "clicked()" ), startProcess )
	app.connect( form.uploadButton, SIGNAL( "clicked()" ), uploadChanges )
	app.connect( app, SIGNAL( "lastWindowClosed()" ), quit )

def initWidgets():
	global form, outputfunction
	form.uploadButton.setEnabled( False )
	form.logEdit.setReadOnly( True )
	outputfunction=form.logEdit.append

def main( args ):
	global form,app,client

	app=QApplication( args )
	form=GUI()
	form.show()
	app.setMainWidget( form )

	initWidgets()
	if len(args) == 2:
		form.taskEdit.setText( args[1] )
	createConnections()

	client=Client( form.logEdit.append )

	app.exec_loop()

if __name__ == "__main__":
	main( sys.argv )
