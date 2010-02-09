import sys

#---------------------------------------------------------------
# from file_retriever.py
import re, os, glob

class _FileRetriever :
	hdr_extensions = ['h', 'H', 'hxx', 'hpp']
	src_extensions = ['c', 'C', 'cxx', 'cpp', r'c\+\+', 'cc']
	headerREs = [ re.compile( r'^.+\.%s$'%ext) for ext in hdr_extensions ]
	sourceREs = [ re.compile( r'^.+\.%s$'%ext) for ext in src_extensions ]
	out_inc = 'include/CLAM'
	out_src = 'src'

	def __init__(self, basedir, folders, blacklist  ) :
		self.scantargets = [ basedir+'/'+folder for folder in folders ]
		self.blacklisted = [ re.compile( entry ) for entry in blacklist ]
		self.headers = []
		self.sources = []
		self.origTargetHeaders = []
		self.origTargetSources = []

	def is_blacklisted( self, filename ) :
		for entry in self.blacklisted :
			if entry.search(filename) is not None :
				print "blacklisted", filename
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
		join = os.path.join
		for target in self.scantargets :
			if not os.path.isdir( target ) : # is a file
				base = os.path.dirname(target)
				for file in glob.glob(target ) :
					if self.is_blacklisted(file) :
						continue
					if self.is_header(file) :
						self.origTargetHeaders.append((file, join(self.out_inc, os.path.basename(file))))
					if self.is_source(file) :
						self.origTargetSources.append((file, join(self.out_src, os.path.basename(file))))
			else : # is a dir
				for file in os.listdir(target) :
					if self.is_blacklisted(file) :
						continue
					if self.is_header(file) :
						self.origTargetHeaders.append((join(target,file), join(self.out_inc,file)))
					if self.is_source(file) :
						self.origTargetSources.append((join(target,file), join(self.out_src,file)))


def retrieveSources(env, folders, blacklist) :
	if not os.path.exists('src') :
		os.mkdir('src')
	if not os.path.exists('include' ) :
		os.makedirs('include/CLAM')

	file_retriever = _FileRetriever( '../../..', folders, blacklist )

	file_retriever.scan_without_copy()

	realHeaders = []
	for orig,target in file_retriever.origTargetHeaders :
		env.CopyFileAndUpdateIncludes(target, orig)
		realHeaders.append(target)

	realSources = []
	for orig,target in file_retriever.origTargetSources:
		env.CopyFileAndUpdateIncludes(target,orig)
		realSources.append(target)

	return realHeaders, realSources


#---------------------------------------------------------------
# from generic_checks.py
generic_checks = dict()

def pkg_config_check_existence(context, *args, **kwargs):
	name = kwargs['name']
	context.Message( 'Checking for %s registered in pkg-config... ' % name )
	crosscompiling=context.env.get('crossmingw')
	if crosscompiling : 
		pkgconfig = 'wine pkg-config'
	else :
		pkgconfig = 'pkg-config'
	ret = context.TryAction('%s --exists \'%s\'' % (pkgconfig,name))[0]
	context.Result( ret )
	return ret

generic_checks['pkg_config_check_existence'] = pkg_config_check_existence

#---------------------------------------------------------------
# from installdirs.py

class InstallDirs :
	def __init__(self, environ ) :
		if environ['prefix_for_packaging'] != '.':
			self.prefix = environ['prefix_for_packaging'] 
		else: self.prefix = environ['prefix']
		self.lib  = self.prefix + '/lib'
		self.pc   = self.prefix + '/lib/pkgconfig'
		self.bin  = self.prefix + '/bin'
		self.inc  = self.prefix + '/include'
		self.data = self.prefix + '/share'
		self.doc  = self.prefix + '/share/doc'

# Configuration checkers to add to default ones

def CheckPkgConfigFile(context, libname) :
	if str(libname) != libname :
		libname = " ".join(libname)
	context.Message( 'Checking for %s pkg-config file... ' % libname)
	pkgconfig = context.env['PKG_CONFIG']
	if pkgconfig == '': pkgconfig = "pkg-config"
	#TODO: Trying native windows, remove the commented out line if other platforms still work
#	command = "PKG_CONFIG_PATH='$PKG_CONFIG_PATH' %s --cflags --libs %s" % (pkgconfig, libname)
	command = "%s --cflags --libs %s" % (pkgconfig, libname)
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
	crosscompiling = context.env.get('crossmingw')
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
	crosscompiling = env.get('crossmingw')
	if not env.has_key('PKG_CONFIG') :
		env['PKG_CONFIG'] = 'pkg-config'
		if crosscompiling :
			env['PKG_CONFIG'] = 'wine pkg-config'
	if env.has_key('sandbox_path') :
		pathSeparator = ':'
		if sys.platform is "win32" or crosscompiling :
			pathSeparator = ';'
		env['ENV']['PKG_CONFIG_PATH'] = pathSeparator.join([
			os.path.join(env['sandbox_path'],'local','lib','pkgconfig'),
		])
		env['PKG_CONFIG_PATH'] = env['ENV']['PKG_CONFIG_PATH']
	
	ret, _  = context.TryAction(env.Action('$PKG_CONFIG --help'))
	if not ret : del env['PKG_CONFIG']
	context.Result( ret )
	return ret

tool_checks['check_pkg_config'] = check_pkg_config

#---------------------------------------------------------------
# from rulesets.py
from SCons.Action import *

def lib_rules(name, version, folders, blacklist, install_dirs, env, moduleDependencies=[]) :
	pcfile = env.PkgConfigFile(
		package = "clam_%s"%name,
		version = version,
		prefix = env['prefix'],
		description = "C++ Framework for analysis, synthesis and transformation of music audio signals",
		url = 'http://clam-project.org',
		requires = ["clam_%s"%module for module in moduleDependencies],
		cflags = env.Dictionary().get('CPPFLAGS', ['']) + ['-I'+path for path in env.Dictionary().get('CPPPATH', ['']) ],
		)

	# This is needed for local compilation of modules
	env.Prepend(CPPPATH=['include']+['../%s/include'%module for module in moduleDependencies])
	env.Prepend(CPPPATH=['include/CLAM']+['../%s/include/CLAM'%module for module in moduleDependencies])
	env.Append(LIBS=['clam_%s'%module for module in moduleDependencies ])
	env.Prepend(LIBPATH=['../%s'%module for module in moduleDependencies])
	#audioio_env.Append( ARFLAGS= ['/OPT:NOREF', '/OPT:NOICF', '/DEBUG'] )

	headers, sources = retrieveSources(env, folders, blacklist)

	crosscompiling = env.get('crossmingw')
	if sys.platform != 'win32' and not crosscompiling :
		return posix_lib_rules( name, version, headers , sources, pcfile, install_dirs, env )
	else :
		return win32_lib_rules( name, version, headers , sources, pcfile, install_dirs, env )

def posix_lib_rules( name, version, headers, sources, pcfile, install_dirs, env, moduleDependencies=[]) :

	versionnumbers = tuple(version.split('.'))

	if len(versionnumbers) != 3:
		print " ERROR in buildtools.posix_lib_rules: version name does not follow CLAM standard "
		print "   Check the variable 'version' in the main SConstruct"
		sys.exit(1)

	if sys.platform == 'linux2' :
		# Linker name: it a soft link without version numbers, to be specified
		# to the linker when compiling binaries against the lib. Just needed
		# for development of such binaries.
		# Soname: is the name of the link that dependant executables will look
		# for at runtime. Such name is embedded in the library by using "-Wl,-soname,..."
		# and when linking a dependant exectuable is also embeded in it.
		# Lib name: the actual fully versioned name of the library.
		# Links with the soname and the lib name point to it.
		# This method enables upgrading the lib without recompiling programs when
		# the ABI is kept. 
		libversion = "%s.%s.%s"%versionnumbers
		linker_name = 'libclam_'+name+'.so'
		soname      = 'libclam_'+name+'.so.%s.%s' % versionnumbers[:2]
		libname     = 'libclam_'+name+'.so.%s.%s.%s' % versionnumbers
		env.Append(SHLINKFLAGS=['-Wl,-soname,%s'%soname ] )
		lib = env.SharedLibrary( 'clam_' + name, sources, SHLIBSUFFIX='.so.%s'%libversion )
		soname_lib     = env.SonameLink( soname, lib )          # lib***.so.X.Y -> lib***.so.X.Y.Z
		linkername_lib = env.LinkerNameLink( linker_name, lib ) # lib***.so     -> lib***.so.X.Y.Z
	else : #darwin
		linker_name = 'libclam_'+name+'.dylib'
		soname      = 'libclam_'+name+'.%s.%s.dylib' % versionnumbers[:2]
		libname     = 'libclam_'+name+'.%s.%s.%s.dylib' % versionnumbers
		env.AppendUnique( CCFLAGS=['-fno-common'] ) # TODO : check whether scons adds this for shcc
		env.AppendUnique( SHLINKFLAGS=[
				'-dynamic',
				'-Wl,-install_name,%s'%os.path.join(install_dirs.lib,soname),
				'-compatibility_version %s.%s'%versionnumbers[:2], # TODO: Test this
				'-current_version %s.%s.%s'%versionnumbers,        # TODO: Test this
				] )
		lib = env.SharedLibrary( 'clam_'+name, sources, SHLIBSUFFIX='.%s.dylib'%version )
		soname_lib =     env.LinkerNameLink( soname, lib )     # lib***.X.Y.dylib -> lib***.X.Y.Z.dylib
		linkername_lib = env.LinkerNameLink( linker_name, lib) # lib***.dylib     -> lib***.X.Y.Z.dylib

	env.Append(CPPDEFINES=[('CLAM_MODULE',name)])

	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	env.AddPostAction( install_headers, "chmod 644 $TARGET" )
	install_lib = env.Install( install_dirs.lib, lib)
	# force generating local soname and linkername when installing
	env.Depends(install_lib, [linkername_lib, soname_lib] )
	install_pc = env.Install( install_dirs.pc, pcfile )
	install_soname = env.SonameLink( os.path.join( install_dirs.lib, soname), install_lib )
	install_linkername =  env.LinkerNameLink( os.path.join(install_dirs.lib,linker_name), install_lib)
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
		install_pc,
# 		install_static,
	])

	install_tgt = env.Alias( 'install_' + name, [
		'install_'+name+'_dev',
		'install_'+name+'_runtime',
	])

	return tgt, install_tgt

def win32_lib_rules( name, version, headers, sources, pcfile, install_dirs, env, moduleDependencies =[] ) :
	lib = env.SharedLibrary( 'clam_' + name, sources)
	tgt = env.Alias(name, lib)
	install_lib = env.Install( install_dirs.lib, lib )
	install_pc = env.Install( install_dirs.pc, pcfile )
	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	install_tgt = env.Alias('install_'+name, [
		install_headers,
		install_lib,
		install_pc
	])
	return tgt, install_tgt


# Code taken from http://www.scons.org/wiki/LongCmdLinesOnWin32 (first way as in 2008.05.05)
# This is to avoid long command line problems in win32.
# We should remove it as soon as scons does it by itself.
def SetupSpawn( env ):
	if env['PLATFORM'] != 'win32': return

	import win32file
	import win32event
	import win32process
	import win32security
	import string

	def my_spawn(sh, escape, cmd, args, spawnenv):
		for var in spawnenv:
			spawnenv[var] = spawnenv[var].encode('ascii', 'replace')

		sAttrs = win32security.SECURITY_ATTRIBUTES()
		StartupInfo = win32process.STARTUPINFO()
		newargs = string.join(map(escape, args[1:]), ' ')
		cmdline = cmd + " " + newargs

		# check for any special operating system commands
		if cmd == 'del':
			for arg in args[1:]:
				win32file.DeleteFile(arg)
			exit_code = 0
		else:
			# otherwise execute the command.
			hProcess, hThread, dwPid, dwTid = win32process.CreateProcess(None, cmdline, None, None, 1, 0, spawnenv, None, StartupInfo)
			win32event.WaitForSingleObject(hProcess, win32event.INFINITE)
			exit_code = win32process.GetExitCodeProcess(hProcess)
			win32file.CloseHandle(hProcess);
			win32file.CloseHandle(hThread);
		return exit_code

	env['SPAWN'] = my_spawn


def create_custom_builders( env ) :
	def generate_so_name( target, source, env ) :
		source_dir = os.path.dirname( str(source[0]) )
		cwd = os.getcwd()
		os.chdir( source_dir )
		if sys.platform == 'linux2' :
			os.system( "/sbin/ldconfig -n ." )
		os.chdir(cwd)
		return None

	bld = env.Builder( action=Action(generate_so_name,
		"== Generating soname $TARGET") )
	env.Append( BUILDERS={'SonameLink' : bld} )

	def generate_linker_name( target, source, env ) :
		source_file = os.path.basename( str(source[0]) )
		os.system( "ln -sf %s %s"%(source_file,str(target[0]) ))
		return None

	bld = env.Builder( action=Action(generate_linker_name,
		"== Generating linker name $TARGET to $SOURCE") )
	env.Append( BUILDERS={'LinkerNameLink' : bld} )

	import shutil
	bld = env.Builder( action =  Action( 
		lambda target, source, env:
			shutil.copy(str(source[0]), str(target[0])),
			"== Build copying $SOURCE"))
	env.Append( BUILDERS={'CopyFileAndUpdateIncludes' : bld} )	


