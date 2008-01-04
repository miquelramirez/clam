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

	def __init__(self) :
		self.prefix = ''
		self.lib = ''
		self.bin = ''
		self.inc = ''
		self.data = ''
		self.doc = ''

	def compose( self, environ ) :
		if environ['prefix_for_packaging'] != '.' :
			self.prefix = environ['prefix_for_packaging']
			self.lib =  environ['prefix_for_packaging']+'/lib'
			self.bin    = environ['prefix_for_packaging']+'/bin'
			self.inc    = environ['prefix_for_packaging']+'/include'
			self.data   = environ['prefix_for_packaging']+'/share'
			self.doc    =environ['prefix_for_packaging']+'/share/doc'
		else:
			self.prefix = environ['prefix']
			self.lib =  environ['prefix']+'/lib'
			self.bin    = environ['prefix']+'/bin'
			self.inc    = environ['prefix']+'/include'
			self.data   = environ['prefix']+'/share'
			self.doc    = environ['prefix']+'/share/doc'


#---------------------------------------------------------------
# from thorough_checks.py


class ThoroughPackageCheck :

	def __init__( self, name, lang, lib, test_code, winlib=None ) :
		self.name = name
		self.lib = lib
		self.winlib = winlib
		self.test_code = test_code
		self.lang = lang
		self.test_code_extension = None
		if self.lang == 'c' :
			self.test_code_extension = '.c'
		elif self.lang == 'c++' :
			self.test_code_extension = '.cxx'
		else :
			raise RuntimeError, "%s language is not supported for specifying test code"

	def __call__( self, context, *args, **kwargs ) :
		context.Message( 'Checking that %s sample program compiles...'%self.name )
		result = context.TryCompile( self.test_code, self.test_code_extension )
		context.Result(result)
		if not result : return False

		context.Message( 'Checking that %s sample program links...'%self.name )
		try :
			lastLIBS = context.env['LIBS']
		except KeyError :
			lastLIBS = None
		crosscompiling = context.env.has_key('crossmingw') and context.env['crossmingw']
		if sys.platform == 'win32' or crosscompiling :
			lib = self.winlib
		if not lib :
			lib = self.lib
		if lib :
			context.env.Append( LIBS=lib )
		result = context.TryLink( self.test_code, self.test_code_extension )
		context.Result(result)
		if not result :
			context.env.Replace( LIBS=lastLIBS )
			return False

		context.Message( 'Checking that %s sample program runs... '%self.name )
		result, errmsg = context.TryRun( self.test_code, self.test_code_extension )
		context.Result(result)

		return result


#---------------------------------------------------------------
# from tool_checks.py

tool_checks = dict()

def check_pkg_config(context, *args, **kwords):
	context.Message( 'Checking for pkg-config... ' )
	crosscompiling = context.env.has_key('crossmingw') and context.env['crossmingw']
	if crosscompiling : 
		pkgconfig = 'wine pkg-config'
	else :
		pkgconfig = 'pkg-config'
	ret, _  = context.TryAction(pkgconfig + ' --help') 
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
		if sys.platform != 'win32' :  #both linux2 and darwin have pkg-config
			self.create_pkg_descriptor = self.linux_create_pkg_descriptor
		else :  #win32
			self.create_pkg_descriptor = self.win32_create_pkg_descriptor

	def win32_create_pkg_descriptor( self, env, out_file ) :
		out = open(out_file, 'w' )

		print >> out, "prefix = %s"%env['prefix']
		print >> out, "libdir = ${prefix}\\lib"
		print >> out, "includedir = ${prefix}\\include"
		print >> out
		print >> out, "Name: %s"%self.name
		print >> out, "Version: %s"%self.version
		print >> out, "Description: C++ Framework for analysis, synthesis and transformation of music audio signals"
		print >> out, "Requires: %s"%" ".join(self.depends)
		print >> out, "Conflicts: "
		edict = env.Dictionary()
		libnames = [self.name] + self.extra
		# extract libs from env
		libnames += edict['LIBS']
		libnames = [ '%s.lib'%name for name in libnames ]
		libnames_str = " ".join( libnames )
		libpaths = [ '%s\\lib'%env['prefix'] ]
		libpaths += edict['LIBPATH']
		#print libpaths
		libpaths = [ '/LIBPATH:%s'%libpath for libpath in libpaths ]
		libpaths_str = ' '.join( libpaths )
		print >> out, "Libs: %s %s"%( libpaths_str , libnames_str)
		cppflags = edict.get('CPPFLAGS', [''])
		cppflags = [ flag for flag in cppflags if flag != '-D_USRDLL' ]
		ccflags = edict.get('CCFLAGS', [''])
		cppaths = edict.get('CPPPATH', [''])
		cpppaths = [ '/I'+path for path in cppaths ]
		print >> out, "Cflags: %s"%" ".join(cppflags+cpppaths+ccflags)

		out.close()

	def linux_create_pkg_descriptor( self, env, out_file ) :
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


#---------------------------------------------------------------
# from package_checks.py

import sys

package_checks = dict()


# xerces-c package check
xerces_test_code = """
#include <xercesc/util/PlatformUtils.hpp>

int main( int argc, char** argv )
{
	namespace xercesc=XERCES_CPP_NAMESPACE;

	xercesc::XMLPlatformUtils::Initialize();

	return 0;
}
"""

package_checks['check_xerces_c'] = ThoroughPackageCheck( 'xerces-c', 'c++', 'xerces-c', xerces_test_code, 'xerces-c_2' )

# libxml++ package-check
xmlpp_test_code = """
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}
"""

package_checks['check_xmlpp'] = ThoroughPackageCheck( 'libxml++', 'c++', 'xml++-2.6', xmlpp_test_code )

# pthreads test
pthread_test_code = """
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 2

void* print_hello(void *thread_id)
{
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc, t;
	for(t=0;t<NUM_THREADS;t++)
	{
		rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);
		if (rc)
		{
			exit(-1);
		}
	}

	pthread_exit(NULL);
}

"""

package_checks['check_pthread'] = ThoroughPackageCheck( 'pthread', 'c', None, pthread_test_code, 'pthreadGC2' )

double_fftw_wo_prefix_test_code = """\
#include <fftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		fprintf(stderr, "expecting fftw to be using doubles, and it is using floats!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_double_wo_prefix'] = ThoroughPackageCheck( 'fftw using doubles without prefixed binaries/headers', 'c', None, double_fftw_wo_prefix_test_code )

double_fftw_w_prefix_test_code = """\
#include <dfftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		fprintf(stderr, "expecting fftw to be using doubles, and it is using floats!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_double_w_prefix'] = ThoroughPackageCheck( 'fftw using doubles with prefixed binaries/headers', 'c', None, double_fftw_w_prefix_test_code )

double_rfftw_wo_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_double_wo_prefix'] = ThoroughPackageCheck( 'rfftw using doubles without prefixed binaries/headers', 'c', None, double_rfftw_wo_prefix_test_code )

double_rfftw_w_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_double_w_prefix'] = ThoroughPackageCheck( 'rfftw using doubles with prefixed binaries/headers', 'c', None, double_rfftw_w_prefix_test_code )

float_fftw_w_prefix_test_code = """\
#include <sfftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		fprintf(stderr, "expecting fftw to be using floats, and it is using doubles!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_float_w_prefix'] = ThoroughPackageCheck( 'fftw using floats with prefixed binaries/headers', 'c', None, float_fftw_w_prefix_test_code )


float_rfftw_w_prefix_test_code = """\
#include <srfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_float_w_prefix'] = ThoroughPackageCheck( 'rfftw using floats with prefixed binaries/headers', 'c', None, float_rfftw_w_prefix_test_code )

float_fftw_wo_prefix_test_code = """\
#include <fftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		fprintf(stderr, "expecting fftw to be using floats, and it is using doubles!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_float_wo_prefix'] = ThoroughPackageCheck( 'fftw using floats with not prefixed binaries/headers', 'c', None, float_fftw_wo_prefix_test_code )

float_rfftw_wo_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_float_wo_prefix'] = ThoroughPackageCheck( 'rfftw using floats with not prefixed binaries/headers','c', None, float_rfftw_wo_prefix_test_code )

liboscpack_test_code = """\
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	return 0;
}
"""

package_checks['check_oscpack'] = ThoroughPackageCheck( 'oscpack', 'c++', None, liboscpack_test_code )

ladspa_test_code = """\
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
"""

package_checks['check_ladspa'] = ThoroughPackageCheck( 'ladspa', 'c', None, ladspa_test_code )

libsndfile_test_code = """\
#include <sndfile.h>

int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
"""

package_checks['check_libsndfile'] = ThoroughPackageCheck( 'libsndfile', 'c', None, libsndfile_test_code )

libogg_test_code = """\
#include <ogg/ogg.h>

int main( int argc, char** argv )
{
	oggpack_buffer b;

	oggpack_writeinit(&b);

	return 0;
}
"""

package_checks['check_libogg'] = ThoroughPackageCheck( 'libogg', 'c', None, libogg_test_code )

libvorbis_test_code = """\
#include <vorbis/vorbisenc.h>

int main( int argc, char** argv )
{
	vorbis_info vi;
	vorbis_info_init( &vi );
	vorbis_encode_setup_init( &vi );

	return 0;
}
"""

package_checks['check_libvorbis'] = ThoroughPackageCheck( 'libvorbis', 'c', None, libvorbis_test_code )

libvorbisfile_test_code = """\
#include <vorbis/vorbisfile.h>

int main( int argc, char** argv )
{
	OggVorbis_File vf;

//	ov_test_open( &vf );

	return 0;
}

"""

package_checks['check_libvorbisfile'] = ThoroughPackageCheck( 'libvorbisfile', 'c', None, libvorbisfile_test_code )

libmad_test_code = """\
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
"""

package_checks['check_libmad'] = ThoroughPackageCheck( 'libmad', 'c', None, libmad_test_code )

libasound_test_code = """\
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
"""

package_checks['check_libasound'] = ThoroughPackageCheck( 'libasound', 'c', None, libasound_test_code )

id3lib_test_code = """\
#include <id3.h>
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
"""

package_checks['check_id3lib'] = ThoroughPackageCheck( 'id3lib', 'c++', None, id3lib_test_code )

opengl_test_code = """\
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

int main( int argc, char** argv )
{
	glBegin(GL_LINES);
		glVertex2f(0.0,0.0);
		glVertex2f(1.0,0.0);
	glEnd();

	return 0;
}"""

package_checks['check_opengl'] = ThoroughPackageCheck( 'opengl', 'c', None, opengl_test_code )

glu_test_code = """\
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>

int main( int argc, char** argv )
{
	GLUquadricObj *sphereObj;
	sphereObj = gluNewQuadric();

	gluSphere(sphereObj, 1.0, 8, 8);

	return 0;
}"""

package_checks['check_glu'] = ThoroughPackageCheck( 'glu', 'c', None, glu_test_code )

directx_test_code = r"""\
#include <windows.h>
#include <dsound.h>

int main()
{
	LPGUID mGUID = 0;
	LPDIRECTSOUND mDS;
	HRESULT hr = DirectSoundCreate( mGUID, &mDS, NULL );

	return 0;
}

"""

package_checks['check_directx'] = ThoroughPackageCheck( 'directx', 'c++', None, directx_test_code )

portaudio_test_code = """\
#include <portaudio.h>

int main()
{
	int version = Pa_GetVersion();

	return 0;
}

"""

package_checks['check_portaudio'] = ThoroughPackageCheck( 'portaudio', 'c', None, portaudio_test_code )

portmidi_test_code = """\
#include <portmidi.h>
#include <stdlib.h>

int main()
{
	PmStream* mHandleIn = NULL;
	PmError err = Pm_OpenInput( &mHandleIn, 0, NULL, 100, NULL, NULL );

	return 0;
}

"""

package_checks['check_portmidi'] = ThoroughPackageCheck( 'portmidi', 'c', None, portmidi_test_code )

jack_test_code = """\
#include <jack/jack.h>
#include <stdlib.h>
jack_client_t *client;
int main()
{

	client = jack_client_new ("foo");

	if ( client != NULL )
		jack_client_close (client);

	return 0;
}
"""

package_checks['check_jack'] = ThoroughPackageCheck( 'jack', 'c', None, jack_test_code )

