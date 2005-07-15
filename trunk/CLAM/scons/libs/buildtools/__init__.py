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
	os.system( "chmod 755 %s"%target_file )
	os.system( "chmod 755 %s"%soname )
	os.system( "chmod 755 %s"%linkername )
	os.chdir( cwd )

def make_lib_names_message( source, target, env ) :
	return "Generate soname and linker name for %s"%target[0]
