import os

def store_headers_in_db(  db ) :
	for _,_,headerlist in os.walk( 'include/CLAM' ) :
		for header in headerlist :
			db[header] = True

def make_lib_names( source, target, env ) :
	target_dir = os.path.dirname( str(target[0]) )
	target_file = os.path.basename( str(target[0]) )
	cwd = os.getcwd()
	os.chdir( target_dir )
	os.system( "ldconfig -n ." )
	linkername = '.'.join(target_file.split('.')[0:2])
	soname = '.'.join( target_file.split('.')[0:3])
	os.system( "ln -sf %s %s"%(soname,linkername) )
	os.chdir( cwd )
