import os,string

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

def parse_conf(env, output):
	dict = {
		'ASFLAGS'       : [],
		'CCFLAGS'       : [],
		'CPPFLAGS'      : [],
		'CPPPATH'       : [],
		'LIBPATH'       : [],
		'LIBS'          : [],
		'LINKFLAGS'     : [],
	}
	static_libs = []
    
	params = string.split(output)

	ind = -1
	for i in range(len(params)) :
		if params[i] == '-include' :
			ind = i
	if ind != -1 :
		new_parm = ' '.join( params[ind:ind+2] )
	env.AppendUnique( CCFLAGS=new_parm )
		
	params = params[:ind]+params[ind+2:]



	for arg in params:
		if arg[0] != '-':
			static_libs.append(arg)
		elif arg[:2] == '-L':
			dict['LIBPATH'].append(arg[2:])
		elif arg[:2] == '-l':
			dict['LIBS'].append(arg[2:])
		elif arg[:2] == '-I':
			dict['CPPPATH'].append(arg[2:])
		elif arg[:4] == '-Wa,':
			dict['ASFLAGS'].append(arg)
		elif arg[:4] == '-Wl,':
			dict['LINKFLAGS'].append(arg)
		elif arg[:4] == '-Wp,':
			dict['CPPFLAGS'].append(arg)
		elif arg == '-pthread':
			dict['CCFLAGS'].append(arg)
			dict['LINKFLAGS'].append(arg)
		else:
			dict['CCFLAGS'].append(arg)

	apply(env.AppendUnique, (), dict)
	return static_libs

class Manifest :
	
	def __init__(self) :
		pass

	def store( self, hdrs, srcs ) :
		hdr_out = open( 'header.manifest' , 'w' )
		for filename in hdrs :
			print >> hdr_out, filename
		hdr_out.close()

		src_out = open( 'source.manifest', 'w' )
		for filename in srcs :
			print >> src_out, filename
		src_out.close()

	def load( self, hdrs, srcs ) :
		try:
			hdr_in = open('header.manifest')
		except IOError:
			return
		for line in hdr_in :
			hdrs.append( line.strip() )
		hdr_in.close()

		try:
			src_in = open('source.manifest')
		except IOError:
			return 
		for line in src_in :
			srcs.append( line.strip() ) 
		src_in.close()	


