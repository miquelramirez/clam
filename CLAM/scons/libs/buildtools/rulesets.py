from SCons.Action import *
from buildtools import *
from buildtools.pkggen import PackageData

import sys

def lib_rules(name, version, headers, sources, install_dirs, env, moduleDependencies=[]) :
	if not env.GetOption('clean') : # David: I don't understand why you don't want to clean it
		pkg_data = PackageData( 'clam_%s'%name, version )
		pkg_data.create_pkg_descriptor( env, 'clam_%s.pc'%name )

	env.Prepend(CPPPATH=['include']+['../%s/include'%module for module in moduleDependencies])
	env.Append(LIBS=['clam_%s'%module for module in moduleDependencies ])
	env.Prepend(LIBPATH=['../%s'%module for module in moduleDependencies])
	#audioio_env.Append( ARFLAGS= ['/OPT:NOREF', '/OPT:NOICF', '/DEBUG'] )

	if sys.platform != 'win32' :
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
	tgt = env.Alias( name, linkername_lib )

	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	env.AddPostAction( install_headers, "chmod 644 $TARGET" )

	install_lib = env.Install( install_dirs.lib, lib)
	env.AddPostAction( install_lib, Action(make_lib_names, make_lib_names_message ) )

	install_descriptor = env.Install( install_dirs.lib+'/pkgconfig', lib_descriptor )

	install_tgt = env.Alias( 'install_' + name, [install_headers, install_lib, install_descriptor] )

	runtime_lib = env.Install( install_dirs.lib, lib )
	runtime_soname = env.SonameLink( install_dirs.lib + '/' + soname, runtime_lib )

	env.Alias( 'install_'+name+'_runtime', [runtime_lib, runtime_soname] )
	env.Append(CPPDEFINES="CLAM_MODULE='\"%s\"'"%name)

#	static_lib = env.Library( 'clam_'+name, sources )
#	install_static = env.Install( install_dirs.lib, static_lib )

	dev_linkername =  env.LinkerNameLink( install_dirs.lib+'/'+linker_name, install_dirs.lib+'/'+soname) 
	env.Alias( 'install_'+name+'_dev', [install_headers,dev_linkername, install_descriptor]) #, install_static] )

	return tgt, install_tgt

def win32_lib_rules( name, version, headers, sources, install_dirs, env, moduleDependencies =[] ) :
	static_lib = env.Library( 'clam_' + name, sources )
	tgt = env.Alias(name, static_lib)
	lib_descriptor = env.File( 'clam_'+name+'.pc' )
	install_static = env.Install( install_dirs.lib, static_lib )
	install_descriptor = env.Install( install_dirs.lib+'/pkgconfig', lib_descriptor )
	install_headers = env.Install( install_dirs.inc+'/CLAM', headers )
	install_tgt = env.Alias('install_'+name, [install_headers,install_static,install_descriptor])
	return tgt, install_tgt

