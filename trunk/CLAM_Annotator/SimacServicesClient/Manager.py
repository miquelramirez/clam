#!/usr/bin/python

import os,sys
import os.path

from Tasker import Tasker, TaskerError


############################################ TASKER-MANAGEMENT FUNCTIONS

def usage():
	print( """
	SimacServices Annotator Client

 Usage:

  -To do all the process in a single step:
    python Manager.py do <taskfile> <projectname> [ <path> ]
	  NOTE: it will not perform an automatic clean
   
  -To do every step individually (in this order):
    python Manager.py processtask <taskfile> <projectname> <path>
    python Manager.py runannotator
    python Manager.py upload <audiofiles>

  -To clean all the task-related files:
    python Manager.py clean
""" )
	sys.exit( 0 )

def TaskerDo( argv ):
	try:
		if len( argv ) != 4 and len( argv ) != 5:
			usage()

		if len( argv ) >= 4:
			taskfile = argv[2]
			projectname = argv[3]
		if len (argv ) == 5:
			path =  argv[4] + os.sep
		else:
			path = os.getcwd() + os.sep

		tasker=Tasker()
		tasker.setParameters( taskfile, projectname, path )
		tasker.processTask()
		tasker.runAnnotator()
		
		tasktime = os.path.getmtime( taskfile )
		modifiedlist = []
		for song in tasker.config['songlisting'].keys():
			songpool = path + os.sep + song + ".pool"
			if os.path.getmtime( songpool ) > tasktime:
				modifiedlist.append( song )

		if len( modifiedlist )==0:
			print "\n - No descriptor pool modified. Exiting without uploading anything.\n"
		else:
			print "\n - The following descriptor pools will be uploaded:\n  -" + ( '\n  - ' ).join( modifiedlist )
			answer = raw_input( "\n > Do you want to do it? (y/n)  " ) 
			if answer.strip() == 'y':
				if tasker.uploadChanges( modifiedlist ) == -1:
					print "\n - Error uploading descriptors\n"
				else:
					print "\n - Descriptors uploaded OK\n"
			else:
				print "\n - No descriptor uploaded\n"
		#tasker.clean()
	except TaskerError, err:
		print >> sys.stderr, err
		sys.exit(-1)

def TaskerProcesstask( argv ):
	if len( argv ) != 5:
		usage()
	
	tasker=Tasker()
	tasker.processTask( argv[2], argv[3], argv[4] )

def TaskerRunannotator( argv ):
	if len( argv ) != 2:
		usage()
	
	tasker=Tasker()
	tasker.runAnnotator()

def TaskerUpload( argv ):
	poollist = argv[2:]
	if len( poollist ) == 0:
		usage()
	
	tasker=Tasker()
	tasker.uploadChanges( poollist )

def TaskerClean( argv ):
	if len( argv ) != 2:
		usage()
	
	tasker=Tasker()
	tasker.clean()

if __name__ == "__main__" :
	if len( sys.argv ) == 1:
		usage()

	try:
		if sys.argv[1] == "do":
			TaskerDo( sys.argv )
		elif sys.argv[1] == "processtask":
			TaskerProcesstask( sys.argv )
		elif sys.argv[1] == "runannotator":
			TaskerRunannotator( sys.argv )
		elif sys.argv[1] == "upload":
			TaskerUpload( sys.argv )
		elif sys.argv[1] == "clean":
			TaskerClean( sys.argv )
		else:
			usage()
	except TaskerError, err:
		print err
