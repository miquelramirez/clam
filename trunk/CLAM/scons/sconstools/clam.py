import sys, os, glob

from SCons.Action import Action
from SCons.Builder import Builder

def scanFiles(env, pattern, paths, blacklist=[]) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return [file for file in files if file not in blacklist]

def recursiveDirs(env, root) :
	return filter( 
		(lambda a : a.rfind( ".svn")==-1 ), 
		[ a[0] for a in os.walk(root)]  )

def ClamModule(env, moduleName, version,
		description="",
		url='http://clam-project.org',
		clamDependencies=[],
		otherDependencies=[],
		sources=[],
		headers=[],
		) :
	try: env.PkgConfigFile
	except : env.Tool('pc', toolpath=[os.path.dirname(__file__)])
	
	libraryName = 'clam_'+moduleName
	if not env['verbose'] : env.ClamQuietCompilation()

	# The empty plugin linking to the module library
	envPlugin = env.Clone()
	envPlugin.AppendUnique(
		LIBS=[libraryName],
		LIBPATH=['.'],
		)
	plugin = envPlugin.LoadableModule(target=libraryName+'_plugin', source = [])

	# pkg-config file
	pcfile = env.PkgConfigFile(
		package = libraryName,
		version = version,
		prefix = env['prefix'],
		description = description,
		url = url,
		requires = clamDependencies+otherDependencies,
		cflags = [],
		)

	versionNumbers=tuple(version.split("."))

	if sys.platform == 'linux2' :
		# * Lib name: the actual fully versioned name of the library.
		# * Soname: is the name of a link that dependant executables will look
		# for at runtime. It does not contain the bugfix version. This enables
		# abi compatible versions.
		# The soname is embeded into the library by using "-Wl,-soname,..."
		# and the linker embeds it also in the dependant binaries.
		# * Linker name: it a soft link without version numbers, pointing to
		# the current development library version to be provided to the linker
		# in order the build system to be version agnostic.
		linkname = 'libclam_'+moduleName+'.so'
		soname   = 'libclam_'+moduleName+'.so.%s.%s' % versionNumbers[:2]
		libname  = 'libclam_'+moduleName+'.so.%s.%s.%s' % versionNumbers
		env.Append(SHLINKFLAGS=['-Wl,-soname,%s'%soname ] )
		lib = env.SharedLibrary( libraryName, sources,
				SHLIBSUFFIX='.so.%s.%s.%s'%versionNumbers )
		localSoName   = env.LinkerNameLink( soname, lib )   # lib***.so.XY -> lib***.so.XY.Z
		localLinkName = env.LinkerNameLink( linkname, lib ) # lib***.so    -> lib***.so.XY.Z
		libraries = [lib, localSoName, localLinkName]
	installedLib = env.Install(os.path.join(env['prefix'],'lib'), lib)
	install = [
		installedLib,
		env.LinkerNameLink( os.path.join(env['prefix'],'lib',linkname), installedLib),
		env.SonameLink( os.path.join(env['prefix'],'lib',soname), installedLib),
		env.Install(os.path.join(env['prefix'],'lib','clam'), plugin),
		env.Install(os.path.join(env['prefix'],'lib','pkgconfig'), pcfile),
		env.Install(os.path.join(env['prefix'],'include','CLAM',moduleName), headers),
		]

	return install, (libraries, plugin, pcfile)

	

def ClamQuietCompilation(env) :
	env['CXXCOMSTR'] = '== Compiling C++ $SOURCE'
	env['CCCOMSTR'] = '== Compiling C $SOURCE'
	env['SHCXXCOMSTR'] = '== Compiling shared C++ $SOURCE'
	env['SHCCCOMSTR'] = '== Compiling shared C $SOURCE'
	env['LINKCOMSTR'] = '== Linking $TARGET'
	env['SHLINKCOMSTR'] = '== Linking library $TARGET'
	env['QT4_RCCCOMSTR'] = '== Embeding resources $SOURCE'
	env['QT4_UICCOMSTR'] = '== Compiling interface $SOURCE'
	env['QT4_LRELEASECOMSTR'] = '== Compiling translation $TARGET'
	env['QT4_MOCFROMHCOMSTR'] = '== Generating metaobjects for $SOURCE'
	env['QT4_MOCFROMCXXCOMSTR'] = '== Generating metaobjects for $SOURCE'

def enable_modules( self, libs, path) :
	if sys.platform in ['linux2','darwin'] : 
		self.ParseConfig('PKG_CONFIG_PATH=%s/lib/pkgconfig pkg-config %s --libs --cflags'%
			(
				path,
				' '.join(libs)))
		return

	# TODO join this if compatible with the linux version
	if sys.platform in ['win32'] : 
		import os
		oldEnv = self['ENV'].items()
		pathSeparator = ";"
		self['ENV']['PKG_CONFIG_PATH'] = pathSeparator.join([
			self['ENV']['PKG_CONFIG_PATH'],
			os.path.join(path,'lib','pkgconfig'),
		])
		self.ParseConfig('pkg-config %s --libs --cflags'% ' '.join(libs))
		self['ENV'] = dict(oldEnv)
		return

	raise "No CLAM support for your platform, sorry"


def generate(env) :
	def generate_so_name( target, source, env ) :
		source_dir = os.path.dirname( str(source[0]) )
		cwd = os.getcwd()
		os.chdir( source_dir )
		if sys.platform == 'linux2' :
			os.system( "/sbin/ldconfig -n ." )
		os.chdir(cwd)

	bld = Builder( action=Action(generate_so_name,
		"== Generating soname $TARGET") )
	env.Append( BUILDERS={'SonameLink' : bld} )

	def generate_linker_name( target, source, env ) :
		source_file = os.path.basename( str(source[0]) )
		os.system( "ln -sf %s %s"%(source_file,str(target[0]) ))

	bld = Builder( action=Action(generate_linker_name,
		"== Generating linker name $TARGET to $SOURCE") )
	env.Append( BUILDERS={'LinkerNameLink' : bld} )

	import shutil
	bld = Builder( action =Action( 
		lambda target, source, env:
			shutil.copy(str(source[0]), str(target[0])),
			"== Build copying $SOURCE"))

	env.Append( BUILDERS={'CopyFileAndUpdateIncludes' : bld} )	
	env.AddMethod(enable_modules, "EnableClamModules")
	env.AddMethod(ClamQuietCompilation)
	env.AddMethod(ClamModule)
	env.AddMethod(scanFiles)
	env.AddMethod(recursiveDirs)

def exists(env):
	return True

