#!/usr/bin/python

from PyQt4 import QtCore, QtGui
from GUI import Ui_GUI
from Tasker import Tasker, TaskerError
import sys, os
from threading import Thread

#TODO threading

def selectFile():
	s = QtGui.QFileDialog.getOpenFileName(
			None,
			"Choose a file",
			".",
			"Simac Task file (*.task)\nAll Files (*)"
		)
	if len(s) > 0:
		form.taskEdit.setText(s)

def selectDirectory():
	s = QtGui.QFileDialog.getExistingDirectory(
			None,
			"Choose a directory",
			".",
			QtGui.QFileDialog.ShowDirsOnly | QtGui.QFileDialog.DontResolveSymlinks
		)
	if len(s) > 0:
		form.pathEdit.setText(s)

def startProcess():
	if form.taskEdit.text()=="" or form.projectEdit.text()=="" \
		or form.pathEdit.text()=="":
		QtGui.QMessageBox.warning( None, "Data missing",
			"You must specify the task file, the project name and the destination directory\n",
			QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton)
		return
	try:
		tasker.setParameters( str(form.taskEdit.text()), str(form.projectEdit.text()), str(form.pathEdit.text()) )
		tasker.processTask()
		#qrect=form.geometry()
		#form.hide()
		window.hide()
		tasker.runAnnotator()
		window.show()
		#form.show()
		#form.setGeometry(qrect)
		modified=tasker.getModified()
		if len(modified)>0:
			choice=QtGui.QMessageBox.question( window, "Modified descriptor files",
				u"The following descriptor pool files will be uploaded:\n  -" + ('\n  - ').join(modified),
				QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
			if choice==0:	# 0,1 o ESC=-1
				tasker.uploadChanges()
		else:
			QtGui.QMessageBox.information( window, "Modified descriptor files",
				u"No descriptor pool was modified. Nothing will be uploaded.",
				QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton )
			
	except TaskerError, x:
		title=str(x).split('\n')[0]
		message=str(x).split('\n')[1:]
		QtGui.QMessageBox.warning( None,
			"%s" % title,
			"%s\n" % ('\n').join(message),
			QtGui.QMessageBox.Ok, QtGui.QMessageBox.NoButton )
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
	app.connect( form.taskButton, QtCore.SIGNAL( "clicked()" ), selectFile )
	app.connect( form.pathButton, QtCore.SIGNAL( "clicked()" ), selectDirectory )
	app.connect( form.goButton, QtCore.SIGNAL( "clicked()" ), startProcess )
	app.connect( form.exitButton, QtCore.SIGNAL( "clicked()" ), quit )
	app.connect( app, QtCore.SIGNAL( "lastWindowClosed()" ), quit )

def initWidgets():
	global outputfunction
	form.logEdit.setReadOnly( True )
	outputfunction=form.logEdit.append
	form.pathEdit.setText( os.getcwd() )

def main( args ):
	global app, form, tasker, window

	app = QtGui.QApplication( args )
	window = QtGui.QDialog()
	form = Ui_GUI()
	form.setupUi(window)
	window.show()

	initWidgets()
	if len(args) > 1:
		form.taskEdit.setText( args[1] )
		form.projectEdit.setText( os.path.split(args[1])[1].split('.task')[0] )
	if len(args) > 2:
		form.projectEdit.setText( args[2] )
	createConnections()

	tasker=Tasker( form.logEdit.append )

	sys.exit( app.exec_() )

if __name__ == "__main__":
	main( sys.argv )
