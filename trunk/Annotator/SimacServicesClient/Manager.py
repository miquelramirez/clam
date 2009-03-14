#!/usr/bin/python

# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
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
    python Manager.py download <taskfile> <projectname> <path>
    python Manager.py runannotator <taskfile> <projectname> <path>
    python Manager.py upload <taskfile> <projectname> <path>

  -To clean all the task-related files:
    python Manager.py clean <taskfile> <projectname> <path>

  -To get a list of all the modified descriptor files:
    python Manager.py listmodified <taskfile> <projectname> <path>
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
		tasker.processTask( taskfile, projectname, path )
		tasker.runAnnotator( taskfile, projectname, path )
		
		tasktime = os.path.getmtime( taskfile )
		modifiedlist = tasker.listModified( taskfile, projectname, path ).strip()
		if modifiedlist == '':
			modifiedlist = []
		else:
			modifiedlist = modifiedlist.split('\n')

		if len( modifiedlist )==0:
			print "\n - No descriptor pool modified. Exiting without uploading anything.\n"
		else:
			print "\n - The following descriptor pools will be uploaded:\n  -" + ( '\n  - ' ).join( modifiedlist )
			answer = raw_input( "\n > Do you want to do it? (y/n)  " ) 
			if answer.strip() == 'y':
				if tasker.uploadChanges( taskfile, projectname, path ) == -1:
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
	if len( argv ) != 5:
		usage()
	
	tasker=Tasker()
	tasker.runAnnotator( argv[2], argv[3], argv[4] )

def TaskerListmodified( argv ):
	if len( argv ) != 5:
		usage()
	
	tasker=Tasker()
	sys.stdout.write(tasker.listModified( argv[2], argv[3], argv[4] ))

def TaskerUpload( argv ):
	if len( argv ) != 5:
		usage()
	
	tasker=Tasker()
	tasker.uploadChanges( argv[2], argv[3], argv[4] )

def TaskerClean( argv ):
	if len( argv ) != 2:
		usage()
	
	tasker=Tasker()
	tasker.clean( argv[2], argv[3], argv[4] )

def main( argv ):
	if len( sys.argv ) == 1:
		usage()
	#################################### important compte amb el path que passen per parametre!

	try:
		if argv[1] == "do":
			TaskerDo( argv )
		elif argv[1] == "download":
			TaskerProcesstask( argv )
		elif argv[1] == "runannotator":
			TaskerRunannotator( argv )
		elif argv[1] == "listmodified":
			TaskerListmodified( argv )
		elif argv[1] == "upload":
			TaskerUpload( argv )
		elif argv[1] == "clean":
			TaskerClean( argv )
		else:
			usage()
	except TaskerError, err:
		print >> sys.stderr, err
		sys.exit(-1)
	sys.exit(0)

if __name__ == "__main__" :
	main( sys.argv )
