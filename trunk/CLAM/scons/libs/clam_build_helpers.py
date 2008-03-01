import sys, os

#---------------------------------------------------------------
# from custom_builders.py

def generate_copy_files( target, source, env ) :
	if sys.platform == 'win32' :
		copyCmd = 'copy'
	else :
		copyCmd = 'cp'
	command = "%s %s %s"%(copyCmd, source[0], target[0])
	os.system( command )
	update_includes_without_db(str(target[0]))
	return None

def generate_copy_files_message( target, source, env ) :
	return "== Copying and updating includes : %s" % target[0]


def generate_so_name( target, source, env ) :
	source_dir = os.path.dirname( str(source[0]) )
	source_file = os.path.basename( str(source[0]) )
	cwd = os.getcwd()
	os.chdir( source_dir )
	if sys.platform == 'linux2' :
		os.system( "/sbin/ldconfig -n ." )
	os.chdir(cwd)
	return None

def generate_so_name_message( target, source, env ) :
	return "generating %s with /sbin/ldconfig -n"%target[0]

def generate_linker_name( target, source, env ) :
	source_file = os.path.basename( str(source[0]) )
	os.system( "ln -sf %s %s"%(source_file,str(target[0]) ))
	return None

def generate_linker_name_message( target, source, env ) :
	return "Linking linker name %s to %s"%(target[0], source[0])

#---------------------------------------------------------------
# from file_retriever.py
import re, os, glob
import sys

class FileRetriever :
	__hdr_extensions = ['h', 'H', 'hxx', 'hpp']
	__src_extensions = ['c', 'C', 'cxx', 'cpp', r'c\+\+', 'cc']
	if sys.platform == 'win32' :
		__copy_cmd = 'copy'
	else :
		__copy_cmd = 'cp'


	def __init__(self, basedir, folders, blacklist  ) :
		self.out_inc = 'include/CLAM'
		self.out_src = 'src'
		self.headerREs = []

		for ext in self.__hdr_extensions :
			self.headerREs.append( re.compile( r'^.+\.%s$'%ext ) )

		self.sourceREs = []

		for ext in self.__src_extensions :
			self.sourceREs.append( re.compile( r'^.+\.%s$'%ext  ) )

		self.scantargets = [ basedir+'/'+folder for folder in folders ]

		self.blacklisted = []

		for entry in blacklist :
			self.blacklisted.append( re.compile( entry ) )

		self.headers = []
		self.sources = []
		self.origTargetHeaders = []
		self.origTargetSources = []

	def __setup_file( self, src, tgt, echo = True ) :
		norm_src = os.path.normcase( src )
		norm_tgt = os.path.normcase( tgt )
		command = "%s %s %s"%(self.__copy_cmd, norm_src, norm_tgt)
		if echo : print command
		os.system( command )

	def is_blacklisted( self, filename ) :
		for entry in self.blacklisted :
			if entry.search(filename) is not None :
				return True

		return False

	def is_header( self, filename ) :
		for regexp in self.headerREs :
			if regexp.search( filename ) is not None :
				return True
		return False

	def is_source( self, filename ) :
		for regexp in self.sourceREs :
			if regexp.search( os.path.basename(filename) ) is not None :
				return True
		return False

	def scan_without_copy( self ) :
		for target in self.scantargets :
			if not os.path.isdir( target ) : # is a file
				base = os.path.dirname(target)
				for file in glob.glob(target ) :
					if self.is_blacklisted(file) :
						continue
					if self.is_header(file) :
						self.origTargetHeaders.append((file, '%s/%s'%(self.out_inc,os.path.basename(file))))
					if self.is_source(file) :
						self.origTargetSources.append((file, '%s/%s'%(self.out_src,os.path.basename(file))))
			else : # is a dir
				for file in os.listdir(target) :
					if self.is_blacklisted(file) :
						continue
					if self.is_header(file) :
						self.origTargetHeaders.append((target+'/'+file, '%s/%s'%(self.out_inc,file)))
					if self.is_source(file) :
						self.origTargetSources.append((target+'/'+file, '%s/%s'%(self.out_src,file)))


#---------------------------------------------------------------
# from generic_checks.py
generic_checks = dict()

def pkg_config_check_existence(context, *args, **kwargs):
	name = kwargs['name']
	context.Message( 'Checking for %s registered in pkg-config... ' % name )
	crosscompiling=context.env.has_key('crossmingw') and context.env['crossmingw']
	if crosscompiling : 
		pkgconfig = 'wine pkg-config'
	else :
		pkgconfig = 'pkg-config'
	ret = context.TryAction('%s --exists \'%s\'' % (pkgconfig,name))[0]
	context.Result( ret )
	return ret

generic_checks['pkg_config_check_existence'] = pkg_config_check_existence

#---------------------------------------------------------------
# from include_rename.py
import os, re

hdrNormRE = re.compile( r'(?P<prefix>.*)include\s*["](?P<hdr>.+)["].*', re.IGNORECASE )

def update_includes_without_db(source, target=None ) :

	if target is None :
		target = source

	newFile = []
	isDirty = False
	fileHandle = file( source, "r" )
	for line in fileHandle :
		match = hdrNormRE.search( line )
		if match is not None :
			newFile.append( hdrNormRE.sub('\g<prefix>include <CLAM/\g<hdr>>', line ))
			isDirty = True
		else :
			newFile.append( line )

	fileHandle.close()

	if isDirty == True :
		fileHandle = file( target, "w" )
		fileHandle.write( "".join( newFile ) )
		fileHandle.close()

	return target

#---------------------------------------------------------------
# from installdirs.py

class InstallDirs :
	def __init__(self, environ ) :
		if environ['prefix_for_packaging'] != '.':
			self.prefix = environ['prefix_for_packaging'] 
		else: self.prefix = environ['prefix']
		self.lib  = self.prefix + '/lib'
		self.bin  = self.prefix + '/bin'
		self.inc  = self.prefix + '/include'
		self.data = self.prefix + '/share'
		self.doc  = self.prefix + '/share/doc'

# Configuration checkers to add to default ones

def CheckPkgConfigFile(context, libname) :
	if str(libname) != libname :
		libname = " ".join(libname)
	context.Message( 'Checking for %s pkg-config file... ' % libname)
	_,pkgconfig = context.TryAction( "echo $PKG_CONFIG" )
	if pkgconfig == '': pkgconfig = "pkg-config"
	command = "PKG_CONFIG_PATH='$PKG_CONFIG_PATH' %s --cflags --libs %s" % (pkgconfig, libname)
	ret,_=context.TryAction( command )
	context.Result(ret)
	if not ret: return False
	context.env.ParseConfig( command )
	return True

def CheckLibrarySample(context, name, lang, lib, test_code, winlib=None ) :
	if lang == 'c' :
		test_code_extension = '.c'
	elif lang == 'c++' :
		test_code_extension = '.cxx'
	else :
		raise RuntimeError, "%s language is not supported for specifying test code"

	context.Message('Checking that %s sample program compiles...'%name )
	result = context.TryCompile( test_code, test_code_extension )
	context.Result(result)
	if not result : return False

	context.Message( 'Checking that %s sample program links...'%name )
	try :
		prevLIBS = context.env['LIBS']
	except KeyError :
		prevLIBS = None
	crosscompiling = context.env.has_key('crossmingw') and context.env['crossmingw']
	if sys.platform == 'win32' or crosscompiling and winlib :
		lib = winlib
	context.env.Append( LIBS=lib )
	result = context.TryLink( test_code, test_code_extension )
	context.Result(result)
	if not result :
		context.env.Replace( LIBS=prevLIBS )
		return False

	context.Message( 'Checking that %s sample program runs... '%name )
	result, errmsg = context.TryRun( test_code, test_code_extension )
	context.Result(result)

	return result

#---------------------------------------------------------------
# from tool_checks.py

tool_checks = dict()

def check_pkg_config(context, *args, **kwords):
	context.Message( 'Checking for pkg-config... ' )
	env = context.env
	crosscompiling = env.has_key('crossmingw') and env['crossmingw']
	if not env.has_key('PKG_CONFIG') :
		env['PKG_CONFIG'] = 'pkg-config'
		env['PKG_CONFIG_PATH_SEP']=':'
		if crosscompiling : 
			env['PKG_CONFIG'] = 'wine pkg-config'
			env['PKG_CONFIG_PATH_SEP']=';'
	if env.has_key('sandbox_path') :
		env['ENV']['PKG_CONFIG_PATH'] = env['PKG_CONFIG_PATH_SEP'].join([
			os.path.join(env['sandbox_path'],'local','lib','pkgconfig'),
			os.path.join(env["sandbox_path"],'gtk','lib','pkgconfig'),
		])
		env['PKG_CONFIG_PATH'] = env['ENV']['PKG_CONFIG_PATH']
	
	ret, _  = context.TryAction(env.Action('$PKG_CONFIG --help'))
	if not ret : del env['PKG_CONFIG']
	context.Result( ret )
	return ret

tool_checks['check_pkg_config'] = check_pkg_config

#---------------------------------------------------------------
# from pkggen.py
import sys

class PackageData :

	def __init__(self, name, version='0.0', extra = [], depends = [] ) :
		self.name = name
		self.version = version
		self.depends = depends
		self.extra = extra

	def create_pkg_descriptor( self, env, out_file ) :
		out = open(out_file, 'w')
		print >> out, "prefix = %s"%env['prefix']
		print >> out, "libdir = ${prefix}/lib"
		print >> out, "includedir = ${prefix}/include"
		print >> out
		print >> out, "Name: %s"%self.name
		print >> out, "Version: %s"%self.version
		print >> out, "Description: C++ Framework for analysis, synthesis and transformation of music audio signals"
		print >> out, "Requires: %s"%" ".join(self.depends)
		print >> out, "Conflicts: "
		print >> out, "Libs: -L${libdir} -l%s"%self.name
		edict = env.Dictionary()
		cppflags = edict.get('CPPFLAGS', [''])
		cppaths = edict.get('CPPPATH', [''])
		cpppaths = [ '-I'+path for path in cppaths ]
		print >> out, "Cflags: -I${includedir} %s"%" ".join(cppflags+cpppaths)

		out.close()

#---------------------------------------------------------------
# from rulesets.py
from SCons.Action import *

import sys

def lib_rules(name, version, headers, sources, install_dirs, env, moduleDependencies=[]) :
	if not env.GetOption('clean') : # David: I don't understand why you don't want to clean it
		pkg_data = PackageData( 'clam_%s'%name, version )
		pkg_data.create_pkg_descriptor( env, 'clam_%s.pc'%name )

	env.Prepend(CPPPATH=['include']+['../%s/include'%module for module in moduleDependencies])
	env.Append(LIBS=['clam_%s'%module for module in moduleDependencies ])
	env.Prepend(LIBPATH=['../%s'%module for module in moduleDependencies])
	#audioio_env.Append( ARFLAGS= ['/OPT:NOREF', '/OPT:NOICF', '/DEBUG'] )

	crosscompiling = env.has_key('crossmingw') and env['crossmingw']
	if sys.platform != 'win32' and not crosscompiling :
		return posix_lib_rules( name, version, headers , sources, install_dirs, env )
	else :
		return win32_lib_rules( name, version, headers , sources, install_dirs, env )

def posix_lib_rules( name, version, headers, sources, install_dirs, env, moduleDependencies=[]) :

	#for file in sources :
	#	print "file to compile: " + str(file)
	lib_descriptor = env.File( 'clam_'+name+'.pc' )

	# We expect a version like " X.Y-possibleextrachars "
	versionnumbers = version.split('.')

	if len(versionnumbers) != 3:
		print " ERROR in buildtools.posix_lib_rules: version name does not follow CLAM standard "
		print "   Check the variable 'version' in the main SConstruct"
		sys.exit(1)

	if sys.platform == 'linux2' :
		libname = 'libclam_'+name+'.so.%s.%s.%s' % (versionnumbers[0], versionnumbers[1], versionnumbers[2])
		middle_linker_name = 'libclam_'+name+'.so.%s.%s' % (versionnumbers[0], versionnumbers[1])
		soname = 'libclam_'+name+'.so.%s' % versionnumbers[0]
		linker_name = 'libclam_'+name+'.so'
		env.Append(SHLINKFLAGS=['-Wl,-soname,%s'%soname ] )
		lib = env.SharedLibrary( 'clam_' + name, sources, SHLIBSUFFIX='.so.%s'%version )
		soname_lib = env.SonameLink( soname, lib )				# lib***.so.X.Y -> lib***.so.X.Y.Z
		linkername_lib = env.LinkerNameLink( linker_name, soname_lib )		# lib***.so -> lib***.so.X
		env.Depends(lib, ['../%s/libclam_%s.so.%s'%(module,module,versionnumbers[0]) for module in moduleDependencies ])
	else : #darwin
		soname = 'libclam_'+name+'.%s.dylib' % versionnumbers[0]
		middle_linker_name = 'libclam_'+name+'.%s.%s.dylib' % (versionnumbers[0], versionnumbers[1])
		linker_name = 'libclam_'+name+'.dylib'
		env.Append( CCFLAGS=['-fno-common'] )
		env.Append( SHLINKFLAGS=['-dynamic',
				'-Wl,-install_name,%s'%(install_dirs.lib + '/' + 'libclam_' + name + '.%s.dylib'%(version))] )
		lib = env.SharedLibrary( 'clam_' + name, sources, SHLIBSUFFIX='.%s.dylib'%version )
		soname_lib = env.LinkerNameLink( middle_linker_name, lib )		# lib***.X.Y.dylib -> lib***.X.Y.Z.dylib
		middlelinkername_lib = env.LinkerNameLink( soname, soname_lib )		# lib***.so.X -> lib***.so.X.Y
		linkername_lib = env.LinkerNameLink( linker_name, middlelinkername_lib)		# lib***.dylib -> lib***.X.dylib

	env.Append(CPPDEFINES="CLAM_MODULE='\"%s\"'"%name)

	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	env.AddPostAction( install_headers, "chmod 644 $TARGET" )
	install_lib = env.Install( install_dirs.lib, lib)
	install_descriptor = env.Install( install_dirs.lib+'/pkgconfig', lib_descriptor )
	install_soname = env.SonameLink( install_dirs.lib + '/' + soname, install_lib )
	install_linkername =  env.LinkerNameLink( install_dirs.lib+'/'+linker_name, install_lib)
#	static_lib = env.Library( 'clam_'+name, sources )
#	install_static = env.Install( install_dirs.lib, static_lib )

	tgt = env.Alias( name, linkername_lib )
	env.Alias( 'install_'+name+'_runtime', [
		install_lib,
		install_soname
	])
	env.Alias( 'install_'+name+'_dev', [
		install_headers,
		install_linkername,
		install_descriptor,
# 		install_static,
	])

	install_tgt = env.Alias( 'install_' + name, [
		'install_'+name+'_dev',
		'install_'+name+'_runtime',
	])

	return tgt, install_tgt

def win32_lib_rules( name, version, headers, sources, install_dirs, env, moduleDependencies =[] ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']
	if crosscompiling :
		lib = env.SharedLibrary( 'clam_' + name, sources)
	else :
		lib = env.Library( 'clam_' + name, sources )
	tgt = env.Alias(name, lib)
	lib_descriptor = env.File( 'clam_'+name+'.pc' )
	install_static = env.Install( install_dirs.lib, lib )
	install_descriptor = env.Install( install_dirs.lib+'/pkgconfig', lib_descriptor )
	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	install_tgt = env.Alias('install_'+name, [install_headers,install_static,install_descriptor])
	return tgt, install_tgt


