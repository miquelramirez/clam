import os,sys

def make_lib_names( source, target, env ) :
	target_dir = os.path.dirname( str(target[0]) )
	target_file = os.path.basename( str(target[0]) )
	cwd = os.getcwd()
	os.chdir( target_dir )

	pieces=target_file.split('.')
	
	if sys.platform == 'linux2':
		os.system( "/sbin/ldconfig -n ." )
		linkername = '.'.join(pieces[0:2])
		soname = '.'.join( pieces[0:3])
		biglinkername = '.'.join( pieces[0:4] )
	else : #darwin
		linkername = '.'.join(pieces[0:1]) + '.dylib'
		soname = '.'.join( pieces[0:2]) + '.dylib'
		biglinkername = '.'.join( pieces[0:3] ) + '.dylib'

	if False: #TODO remove this debugging aid
		print 'soname', soname
		print 'linkername', linkername
		print 'biglinkername', biglinkername
		print 'target_file', target_file

		print soname, '<-', linkername
		print biglinkername, '<-', soname
		print target_file, '<-', biglinkername
	
	os.system( "ln -sf %s %s"%(soname,linkername) )
	os.system( "ln -sf %s %s"%(biglinkername,soname) )
	os.system( "ln -sf %s %s"%(target_file,biglinkername) )
	os.system( "chmod 755 %s"%target_file )
	os.system( "chmod 755 %s"%soname )
	os.system( "chmod 755 %s"%linkername )
	os.system( "chmod 755 %s"%biglinkername )
	os.chdir( cwd )

def make_lib_names_message( source, target, env ) :
	return "Generate soname and linker name for %s"%target[0]


