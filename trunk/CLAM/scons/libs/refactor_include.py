#!/usr/bin/python
from buildtools.include_rename import *
import os, re, shelve, getopt, sys

hdrNormRE = re.compile( r"#include \"(?P<hdr>.+)\"" )
isSourceRE = re.compile(r"[^\.#]+\.[cC][xp]?[xp]?\Z" )
isHeaderRE = re.compile( r"[^\.#].+\.[hH][xp]?[xp]?\Z" )

def compilable( filename ) :
	if isSourceRE.search(filename) is not None :
		return True
	if isHeaderRE.search(filename) is not None :
		return True
	return False

def sources( folder ) :
	for root, _, filelist in os.walk( folder ) :
		for filename in filelist :
			if compilable(filename) :
				yield "%s/%s"%(root,filename)	


def process_folder( header_db, folder, mustRecurse ) :
	count = 0
	for fname in sources( folder ) :
		print >> sys.stdout, "Updating file",fname
		count+=1
		update_includes( header_db, fname )
	print count, "files updated..."

def usage() :
	print """
This script processes inplace the input file/folder looking for CLAM includes and converts
them to the 'library form'. For instance, if a source file happens to contain

	#include "Processing.hxx"

it will be converted to :

	#include <CLAM/Processing.hxx>

The script accepts the following parameters:
	-h, --help			Shows this help
	-l, --list		List all headers in the database	
	-i, --input	<path>	File or folder to convert
	-a, --add  <header>  Adds a new header to the header_db.db
	-d, --delete  <header>  Adds a new header to the header_db.db
	"""

def main() :

	try:
		opts, args = getopt.getopt( sys.argv[1:], "hi:a:d:rl", ["help","input=","add=", "delete=", "recurse", "list"] )
	except getopt.GetoptError :
		usage()
		sys.exit(2)
	
	inputFname = ""
	mustRecurse = False

	for option,argument in opts :
		if option in ( "-h", "--help" ) :
			usage()
			sys.exit(1)
		if option in ( "-i", "--input" ) :
			inputFname = argument
		if option in ( "-l", "--list" ) : 
			existingHeaders = shelve.open("header.db", "r")
			print existingHeaders.keys()
			sys.exit(0)
		 
	if option in ("-a", "--add") :
		existingHeaders = shelve.open("header.db", writeback=True)
		if existingHeaders.has_key(argument) :
			print argument, "already exists in the database "
			sys.exit(1)
		existingHeaders[argument] = True
		existingHeaders.close()
		print argument, "successfully added"
		sys.exit(0)
	
	if option in ("-d", "--delete") :
		existingHeaders = shelve.open("header.db", writeback=True)
		if not existingHeaders.has_key(argument) :
			print argument, "doesn't exist in the database "
			sys.exit(1)
		del existingHeaders[argument]
		existingHeaders.close()
		print argument, "successfully deleted"
		sys.exit(0)
		
	if option in ( "-r", "--recurse" ) :
		mustRecurse = True

	if len(inputFname) == 0 :
		print "No input was specified!!!"
		usage()
		sys.exit(2)

	if not os.path.exists( inputFname ) :
		print "Specified input %s is not a file or folder!!!"
		usage()
		sys.exit(2)

	header_db = shelve.open( "header.db", "r" )

	if len( header_db.keys() ) == 0 :
		print "There is some problem with the CLAM Headers Database, either it has"
		print "not been generated or it has got somehow corrupted. Rerun scons to regenerate it."
		sys.exit(2)

	if os.path.isfile( inputFname ) :
		update_includes( header_db, inputFname )
	else :
		process_folder( header_db, inputFname, mustRecurse )

	header_db.close()

if __name__ == "__main__" :
	main()
else :
	print "This is an executable script!"
	sys.exit(2)
