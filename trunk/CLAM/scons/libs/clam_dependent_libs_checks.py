import sys, os
import sys

#---------------------------------------------------------------
# from __init__.py

def config_error(str) :
	print str
	print 'Check the config.log file for details'
	return False

def setup_global_environment( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']
	# clam env
	# check for pkg-config, compiler support, bash features, et.
	if sys.platform in ('linux2','win32') :
		if not conf.check_pkg_config( conf ) :
			print 'WARNING: pkg-config is not installed. Checks will be harder.'
			env['pkg_config_available'] = False
		else :
			env['pkg_config_available'] = True

	if env['double'] :
		env.Append( CPPFLAGS=['-DCLAM_DOUBLE'] )
	else :
		env.Append( CPPFLAGS=['-DCLAM_FLOAT'] )

	if not env['checks'] :
		env.Append( CPPFLAGS=['-DCLAM_DISABLE_CHECKS'] )

	if env['release_asserts'] :
		env.Append( CPPFLAGS=['-DCLAM_USE_RELEASE_ASSERTS'] )
	if env['optimize_and_lose_precision'] :
		env.Append( CPPFLAGS=['-DCLAM_OPTIMIZE'] )

	if env['release'] :
		print 'COMPILING IN RELEASE MODE'
		env.Append( CCFLAGS='-g -O2 -fomit-frame-pointer -Wall'.split(' ') )
	else :
		print 'COMPILING IN DEBUG MODE'
		env.Append( CCFLAGS='-g -Wall'.split(' ') )
		env.Append( CPPFLAGS = ['-D_DEBUG'] )

	# pthreads testing
	if not conf.CheckCHeader('pthread.h') :
		return config_error( "Could not find pthread (Posix Threads) library headers!" )
	if not conf.CheckLib( 'pthread', 'pthread_join' ) :
		return config_error( "Could not find pthread (Posix Threads) library binaries!" )
	if not conf.CheckLibrarySample('pthread', 'c', None, pthread_test_code ) : 
		return config_error( "Could not find pthread (Posix Threads) library binaries!" )
	env.Append( CPPFLAGS=['-DUSE_PTHREADS=1'] )

	if sys.platform == 'linux2' :
		env.Append(LIBPATH=['/usr/local/lib','/opt/lib'])
	elif sys.platform == 'darwin' :
		env.Append(LIBPATH=['/usr/local/lib','/usr/lib','/opt/local/lib'])

	return True

#---------------------------------------------------------------
# from audioio.py

def test_that_lib(env, conf, name, libNames, header, symbol, extra=lambda : True) :
	if not conf.CheckCHeader( header ) :
		return config_error( "Could not find %s headers! Please check your %s installation" %(name, name) )
	found = False
	for libName in libNames :
		if conf.CheckLib( library=libName, symbol=symbol ) :
			found = True
			break
	if not found : 
		return config_error( "Could not find %s binaries! Please check your %s installation"%(name,name) )

	if not extra() :
		return config_error( "%s compile/link/run tests failed!"%name )
	return True

def test_sndfile( env, conf ) :
	if not test_that_lib(env, conf,
			name='libsndfile', 
			libNames=['sndfile','libsndfile'],
			header='sndfile.h',
			symbol='sf_open_fd') :
		return False
	if not conf.CheckLibrarySample('libsndfile', 'c', None, libsndfile_test_code ) :
		return False
	return True

def test_oggvorbis( env, conf ) :
	if not conf.CheckPkgConfigFile(['vorbisfile', 'vorbisenc']) :
		return False
	if not conf.CheckLibrarySample('libogg', 'c', None, libogg_test_code ) : return False
	if not conf.CheckLibrarySample('libvorbis', 'c', None, libvorbis_test_code ) : return False
	if not conf.CheckLibrarySample('libvorbisfile', 'c', None, libvorbisfile_test_code) : return False
	return True

def test_mad( env, conf ) :
	if not conf.CheckPkgConfigFile(['mad']) :
		return False
	if not conf.CheckLibrarySample('libmad', 'c', None, libmad_test_code ) :
		return config_error( "libmad compile/link/run tests failed!" )
	return True

def test_id3lib( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']

	libName = 'id3'
	if sys.platform == 'win32' : libName = 'id3lib_vc7'
	env.Append( LIBS=[libName] )
	if sys.platform == 'win32' :
		if crosscompiling :
			env.Append( CPPFLAGS=['-DID3LIB_LINKOPTION=3'] )
		else :
			env.Append( CPPFLAGS=['-DID3LIB_LINKOPTION=1'] )

	zlib = 'z'
	if sys.platform == 'win32' : zlib_vc = 'zlib_vc7'
	if not conf.CheckLib(zlib, 'uncompress') :
		return config_error( "Could not link zlib. Please, check your zlib/id3lib installation" )

	if not conf.CheckCXXHeader( 'id3.h' ) :
		return config_error( "Could not find id3lib headers! Please check your id3lib installation" )

	if not conf.CheckLibWithHeader( libName, 'id3/tag.h', 'cxx', call='ID3_Tag myTag;' ) :
		return config_error( "Could not find id3lib binaries! Please check your id3lib installation" )
	if not conf.CheckLibrarySample(libName, 'c++', None, id3lib_test_code) :
		return config_error( "id3lib compile/link/run tests failed!" )
	return True

def test_alsa_sdk( env, conf ) :
	if not conf.CheckPkgConfigFile('alsa') :
		return False
	if not conf.CheckLibrarySample( 'libasound', 'c', None, libasound_test_code ) :
		return config_error( "libasound compile/link/run tests failed!" )
	return True

def test_directx_sdk( env, conf ) :
	if not conf.CheckHeader( 'dsound.h' ) :
		return config_error( "Could not find DirectX SDK header 'dsound.h'! Please check that the DirectX SDK is placed at the CLAM sandbox" )
	env.Append( LIBS=['dxerr8','user32', 'gdi32'] )
	if not conf.CheckLibWithHeader('dsound', 'dsound.h', 'cxx', call='DirectSoundCreate(0,0,0);') :
		return config_error( "Could not find DirectX SDK library 'dsound.lib! Please check that the DirectX SDK is placed at the CLAM sandbox" )
	if not conf.CheckLibrarySample('directx', 'c++', None, directx_test_code ) :
		return config_error( "directx compile/link/run test failed!" )
	return True

def test_jack ( env, conf ) :
	if not conf.CheckPkgConfigFile('jack') :
		return False
	if not conf.CheckLibrarySample('jack', 'c', None, jack_test_code ) :
		return config_error( "jack compile/link/run test failed!" )
	return True

def test_portaudio( env, conf ) :
	if not conf.CheckCHeader( 'portaudio.h' ) :
		return config_error( "Could not find portaudio header! Please check your portaudio installation" )
	if not conf.CheckLib( library='portaudio', symbol='Pa_GetHostApiInfo' ) :
		return config_error( "Could not find portaudio binary v19 or higher! (v18 wont work) Please check your portaudio installation" )
	if not conf.CheckLibrarySample('portaudio', 'c', None, portaudio_test_code ) :
		return config_error( "portaudio compile/link/run tests failed!" )
	return True

def test_portmidi( env, conf ) :
	if sys.platform == 'linux2' :
		print "Bypassing portmidi checks: using ALSA MIDI facilities"
		return True
	if not conf.CheckHeader( 'portmidi.h' ) :
		return config_error( "Could not find portmidi header 'portmidi.h'! Check your portmidi installation..." )
	env.Append( LIBS=['portmidi'] )
	if sys.platform == 'win32' :
		env.Append( LIBS=['winmm'] )
	if not conf.CheckLib( library='portmidi', symbol='Pm_OpenInput' ) :
		return config_error( "Could not find portmidi libraries! Check your portmidi installation..." )

	if not conf.CheckLibrarySample('portmidi', 'c', None, portmidi_test_code ) :
		return config_error( "Portmidi compile/link/run tests failed!" )

	return True

def setup_audioio_environment( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']

	if env['with_sndfile'] :
		if not test_sndfile( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_SNDFILE=1'])

	if env['with_oggvorbis'] :
		if not test_oggvorbis( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_OGGVORBIS=1'] )
		env.Append( CPPFLAGS=['-DWITH_VORBIS=1'] )

	if env['with_mad'] :
		if not test_mad( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_MAD=1'] )
		env.Append( CPPFLAGS=['-DWITH_MAD=1'] )

	if env['with_id3'] :
		if not test_id3lib( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_ID3=1'] )

	if (sys.platform == 'linux2' and not crosscompiling) and env['with_alsa'] :
		if not test_alsa_sdk( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_ALSA=1'] )

	if env['with_jack'] and not crosscompiling:
		if not test_jack (env, conf):
			print "Either install jack or disable jack support by issuing"
			print "$scons with_jack=no"
			return False
		env.Append(CPPFLAGS=['-DUSE_JACK=1'])

	if env['with_portmidi'] :
		if not test_portmidi( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_PORTMIDI=1'] )

	if env['with_portaudio'] :
		if not test_portaudio( env, conf ) : return False
		env.Append( CPPFLAGS=['-DUSE_PORTAUDIO=1'] )

	if not sys.platform == 'linux2' or crosscompiling :
		if sys.platform == 'win32' :
			if not test_directx_sdk( env, conf ) : return False

		if env['audio_backend'] == 'directx' :
			env.Append( CPPFLAGS=['-DUSE_DIRECTX=1'] )

		if env['audio_backend'] == 'portaudio' and not env['with_portaudio']:
			if not test_portaudio( env, conf ) : return False
			env.Append( CPPFLAGS=['-DUSE_PORTAUDIO=1'] )

		if env['audio_backend'] == 'rtaudio' :
			if sys.platform == 'win32' :
				env.Append( CPPFLAGS=['-D__WINDOWS_DS__'])
			else :
				env.Append( CPPFLAGS=['-D__MACOSX_CORE__'])
			env.Append( CPPFLAGS=['-DUSE_RTAUDIO=1'] )

	return True


#---------------------------------------------------------------
# from core.py

def test_xml_backend( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']

	if env['xmlbackend'] in ('both','xercesc') :
		if not conf.CheckCXXHeader('xercesc/util/PlatformUtils.hpp') :
			return config_error( "Could not find xerces c headers! Defaulting to the null xml backend" )
		env.Append(ENV=os.environ)
		print('path of app: ' + env['ENV']['PATH'])
		if not conf.CheckLibrarySample('xerces-c', 'c++', 'xerces-c', xerces_test_code, 'xerces-c_2' ) :
			return config_error( "xerces c code compile/link/run test failed!" )

		env.Append( CPPFLAGS=['-DUSE_XERCES=1', '-DCLAM_USE_XML'] )

	if env['xmlbackend'] in ('both','xmlpp') :
		if env['pkg_config_available'] :
			if not conf.CheckPkgConfigFile("libxml++-2.6"):
				return config_error( "Error: pkg-config could not find libxml options." )
		if not conf.CheckLibrarySample( 'libxml++', 'c++', None, xmlpp_test_code ) :
			return config_error( "libxml++ code compile/link/run test failed!" )
		env.Append( CPPFLAGS=['-DUSE_XMLPP=1','-DCLAM_USE_XML'] )
	return True

def test_ladspa ( env, conf ) :
	if not env.has_key('with_ladspa') : return True
	if not env['with_ladspa'] : return True
	if not conf.CheckCHeader( 'ladspa.h' ) :
		return config_error( "ladspa SDK header was not found" )
	if not conf.CheckLibrarySample('ladspa', 'c', None, ladspa_test_code ) :
		return config_error( "ladspa SDK compile test failed!" )

	env.Append( CPPFLAGS=['-DUSE_LADSPA=1'] )
	return True

def setup_core_environment ( env, conf) :
	if not test_xml_backend( env, conf): return False
	if env['xmlbackend'] != 'none' :
		env.Append( CPPFLAGS=['-DCLAM_USE_XML'] )
	if not test_ladspa ( env, conf):
		return config_error(
			"Either install properly ladspa SDK or disable LADSPA support by issuing\n"
			"$scons with_ladspa=no" )
	return True
#---------------------------------------------------------------
# from processing.py

def test_fftw3( env, conf) :
	if not conf.CheckPkgConfigFile('fftw3') :

		if not conf.CheckHeader( 'fftw3.h' ) :
			return config_error( "FFTW3 header not found" )

		for libname in ['fftw3', 'fftw3-3', None] :
			if not libname: break
			if conf.CheckLib( libname, 'fftw_plan_dft_r2c_1d') : break
		if not libname :
			return config_error( "Unable to link FFTW3" )

	env.Append( CPPFLAGS=['-DUSE_FFTW3=1'] )
	return True

def setup_processing_environment( env, conf ) :

	if env['with_fftw3'] and not test_fftw3(env, conf ) :
		return config_error( "Check fftw3 installation\n"
			"Or disable it with the option: 'with_fftw3=no'" )
	return True


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

# libxml++ package-check
xmlpp_test_code = """
#include <libxml++/libxml++.h>
int main( int argc, char** argv )
{
	xmlpp::Document document;
	return 0;
}
"""

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
		if (rc) exit(-1);
	}
	pthread_exit(NULL);
}
"""

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

libsndfile_test_code = """\
#include <sndfile.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
"""

libogg_test_code = """\
#include <ogg/ogg.h>
int main( int argc, char** argv )
{
	oggpack_buffer b;
	oggpack_writeinit(&b);
	return 0;
}
"""

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

libvorbisfile_test_code = """\
#include <vorbis/vorbisfile.h>
int main( int argc, char** argv )
{
	OggVorbis_File vf;
//	ov_test_open( &vf );
	return 0;
}
"""

libmad_test_code = """\
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
"""

libasound_test_code = """\
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
"""

id3lib_test_code = """\
#include <id3.h>
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
"""

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


portaudio_test_code = """\
#include <portaudio.h>
int main()
{
	int version = Pa_GetVersion();
	return 0;
}
"""

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



