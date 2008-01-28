import sys, os
import sys

#---------------------------------------------------------------
# from __init__.py

from clam_build_helpers import *


def config_error(str) :
	print str
	print 'Check the config.log file for details'
	return False

def setup_global_environment( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']
	# clam env
	# check for pkg-config, compiler support, bash features, et.
	if sys.platform == 'linux2' :
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

	for pthreadLib in ['pthread', 'pthreadGC2', 'pthreadVCE', 'notfound' ] :
		if not conf.CheckLib( pthreadLib, 'pthread_join' ) : continue
		if conf.check_pthread() : break # It works
	if pthreadLib is 'notfound' :
		return config_error( "Could not find pthread (Posix Threads) library binaries!" )

	env.Append( CPPFLAGS=['-DUSE_PTHREADS=1'] )
	env.Append( LIBS=[pthreadLib] )

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
	if not test_that_lib(env, conf, name='libsndfile', 
			libNames=['sndfile','libsndfile'],
			header='sndfile.h',
			symbol='sf_open_fd',
			extra=conf.check_libsndfile) :
		return False
	env.Append( CPPFLAGS=['-DUSE_SNDFILE=1'])
	return True

def test_oggvorbis( env, conf ) :
	if conf.CheckPkgConfigFile(['vorbisfile', 'vorbisenc']) :
		if not conf.check_libogg() : return False
		if not conf.check_libvorbis() : return False
		if not conf.check_libvorbisfile() : return False
		if not conf.CheckLibrarySample('libvorbisfile', 'c', None, libogg_test_code) : return False
		env.Append( CPPFLAGS=['-DUSE_OGGVORBIS=1'] )
		return True
	if not test_that_lib(env, conf,
			name='libogg', 
			libNames=['ogg'],
			header='ogg/ogg.h',
			symbol='oggpack_writeinit',
			extra=conf.check_libogg) :
		return False
	if not test_that_lib(env, conf,
			name='libvorbis', 
			libNames=['vorbis'],
			header='vorbis/codec.h',
			symbol='vorbis_book_decode') :
		return False
	env.PrependUnique(LIBS=['libvorbisenc'])
	if not test_that_lib(env, conf,
			name='libvorbisenc', 
			libNames=['vorbisenc'],
			header='vorbis/vorbisenc.h',
			symbol='vorbis_encode_setup_init',
			extra=conf.check_libvorbis) :
		return False
	if not test_that_lib(env, conf, name='libvorbisfile', 
			libNames=['vorbisfile'],
			header='vorbis/vorbisfile.h',
			symbol='ov_test_open',
			extra=conf.check_libvorbisfile) :
		return False

	env.Append( CPPFLAGS=['-DUSE_OGGVORBIS=1'] )
	return True

def test_mad( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']
	if not conf.CheckCHeader( 'mad.h' ) :
		return config_error( "Could not find libmad headers! Please check your libmad installation" )
	libName = 'mad'
	if not conf.CheckLib( library=libName, symbol='mad_stream_init' ) :
		return config_error( "Could not find libmad binaries! Please check your libmad installation" )
	if not conf.check_libmad( ) :
		return config_error( "libmad compile/link/run tests failed!" )
	env.Append( CPPFLAGS=['-DUSE_MAD=1'] )
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
	env.Append( CPPFLAGS=['-DUSE_ID3=1'] )
	return True

def test_alsa_sdk( env, conf ) :
	if not conf.CheckCHeader( 'alsa/asoundlib.h' ) :
		return config_error( "Could not find libasound development headers! Please check your libasound installation" )
	if not conf.CheckLib( library='asound', symbol='snd_card_next' ) :
		return config_error( "Could not find libasound binaries! Please check your libasound installation" )
	if not conf.check_libasound() :
		return config_error( "libasound compile/link/run tests failed!" )
	env.Append( CPPFLAGS=['-DUSE_ALSA=1'] )
	return True

def test_directx_sdk( env, conf ) :
	if not conf.CheckHeader( 'dsound.h' ) :
		return config_error( "Could not find DirectX SDK header 'dsound.h'! Please check that the DirectX SDK is placed at the CLAM sandbox" )
	env.Append( LIBS=['dxerr8','user32', 'gdi32'] )
	if not conf.CheckLibWithHeader('dsound', 'dsound.h', 'cxx', call='DirectSoundCreate(0,0,0);') :
		return config_error( "Could not find DirectX SDK library 'dsound.lib! Please check that the DirectX SDK is placed at the CLAM sandbox" )
	if not conf.check_directx() :
		return config_error( "directx compile/link/run test failed!" )
	return True

def test_jack ( env, conf ) :
	if not conf.CheckCHeader( 'jack/jack.h' ) :
		return config_error( "jack headers not found!" )
	if not conf.CheckLib( library='jack', symbol='jack_cpu_load' ) :
		return config_error( "jack binaries not found!" )
	if not conf.check_jack() :
		return config_error( "jack compile/link/run test failed!" )
	env.Append(CPPFLAGS=['-DUSE_JACK=1'])
	return True

def test_portaudio( env, conf ) :
	if not conf.CheckCHeader( 'portaudio.h' ) :
		return config_error( "Could not find portaudio header! Please check your portaudio installation" )
	if not conf.CheckLib( library='portaudio', symbol='Pa_GetHostApiInfo' ) :
		return config_error( "Could not find portaudio binary v19 or higher! (v18 wont work) Please check your portaudio installation" )
	if not conf.check_portaudio() :
		return config_error( "portaudio compile/link/run tests failed!" )
	env.Append( CPPFLAGS=['-DUSE_PORTAUDIO=1'] )
	env.Append( LIBS=['portaudio'] )
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

	if not conf.check_portmidi() :
		return config_error( "Portmidi compile/link/run tests failed!" )

	env.Append( CPPFLAGS=['-DUSE_PORTMIDI=1'] )
	return True

def setup_audioio_environment( env, conf ) :
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']

	if env['with_sndfile'] :
		if not test_sndfile( env, conf ) : return False

	if env['with_oggvorbis'] :
		if not test_oggvorbis( env, conf ) : return False
		env.Append( CPPFLAGS=['-DWITH_VORBIS=1'] )

	if env['with_mad'] :
		if not test_mad( env, conf ) : return False
		env.Append( CPPFLAGS=['-DWITH_MAD=1'] )

	if env['with_id3'] :
		if not test_id3lib( env, conf ) : return False

	if (sys.platform == 'linux2' and not crosscompiling) and env['with_alsa'] :
		if not test_alsa_sdk( env, conf ) : return False

	if env['with_jack'] and not crosscompiling:
		if not test_jack (env, conf):
			print "Either install jack or disable jack support by issuing"
			print "$scons with_jack=no"
			return False

	if env['with_portmidi'] :
		if not test_portmidi( env, conf ) : return False

	if env['with_portaudio'] :
		if not test_portaudio( env, conf ) : return False

	if not sys.platform == 'linux2' or crosscompiling :
		if sys.platform == 'win32' :
			if not test_directx_sdk( env, conf ) : return False

		if env['audio_backend'] == 'directx' :
			env.Append( CPPFLAGS=['-DUSE_DIRECTX=1'] )

		if env['audio_backend'] == 'portaudio' and not env['with_portaudio']:
			if not test_portaudio( env, conf ) : return False

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
		if not conf.check_xerces_c( conf ) :
			return config_error( "xerces c code compile/link/run test failed!" )

		env.Append( CPPFLAGS=['-DUSE_XERCES=1', '-DCLAM_USE_XML'] )

	if env['xmlbackend'] in ('both','xmlpp') :
		if env['pkg_config_available'] :
			if not conf.CheckPkgConfigFile("libxml++-2.6"):
				return config_error( "Error: pkg-config could not find libxml options." )
		if not conf.check_xmlpp( conf ) :
			return config_error( "libxml++ code compile/link/run test failed!" )
		env.Append( CPPFLAGS=['-DUSE_XMLPP=1','-DCLAM_USE_XML'] )
	return True

def test_ladspa ( env, conf ) :
	if not env.has_key('with_ladspa') : return True
	if not env['with_ladspa'] : return True
	if not conf.CheckCHeader( 'ladspa.h' ) :
		return config_error( "ladspa SDK header was not found" )
	if not conf.check_ladspa() :
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
	crosscompiling=env.has_key('crossmingw') and env['crossmingw']
	if not env['with_fftw3'] : return True

	if not conf.CheckHeader( 'fftw3.h' ) :
		return config_error( "FFTW3 header not found" )

	for libname in ['fftw3', 'fftw3-3', None] :
		if not libname: break
		if conf.CheckLib( libname, 'fftw_plan_dft_r2c_1d') : break
	if not libname :
		return config_error( "Unable to link FFTW3" )

	env.Append( CPPFLAGS=['-DUSE_FFTW3=1'] )
	return True

def test_fftw_linux( env, conf ) :
	if not env['with_fftw'] : return True

	if env['double'] : # CLAM uses double for TData
		nonPrefixHeader = conf.CheckCHeader( 'fftw.h' )
		prefixHeader = conf.CheckCHeader( 'dfftw.h' )
		if not prefixHeader and not nonPrefixHeader :
			return config_error("Could not find fftw library headers...!")
		double_prefixed = not nonPrefixHeader
		if double_prefixed :
			realHeader = 'rfftw.h'
			lib = 'fftw'
			realLib = 'rfftw'
		else:
			realHeader = 'drfftw.h'
			lib = 'dfftw'
			realLib = 'drfftw'

		if not conf.CheckCHeader( realHeader ) :
			return config_error( "Could not find '%s' headers!"%realHeader )

		if not conf.CheckLib( lib, 'fftw_sizeof_fftw_real') :
			return config_error( "Could not find '%s'!"%lib )

		if not conf.CheckLib( realLib, 'rfftw_create_plan' ) :
			return config_error( "Could not find' %s'!"%realLib )

		if not double_prefixed :
			if not conf.check_fftw_double_wo_prefix() :
				return config_error( "fftw compile/link/run tests failed!" )
			if not conf.check_rfftw_double_wo_prefix() :
				return config_error( "rfftw compile/link/run tests failed!" )
		else :
			if not conf.check_fftw_double_w_prefix() :
				return config_error( "fftw compile/link/run tests failed!" )
			if not conf.check_rfftw_double_w_prefix() :
				return config_error( "rfftw compile/link/run tests failed!" )
	else :
		if not conf.CheckCHeader( 'sfftw.h' ) :
			return config_error( "Could not find fftw library headers...!" )
		if not conf.CheckCHeader( 'srfftw.h' ) :
			return config_error( "Could not find some of the fftw library headers...!" )
		if not conf.CheckLib( 'sfftw', 'fftw_sizeof_fftw_real') :
			return config_error( "Could not find libsfftw" )
		if not conf.CheckLib( 'srfftw', 'rfftw_create_plan' ) :
			return config_error( "Could not find libsrfftw" )

		if not conf.check_fftw_float_w_prefix() :
			return config_error( "fftw compile/link/run tests failed!" )
		if not conf.check_rfftw_float_w_prefix() :
			return config_error( "rfftw compile/link/run tests failed!" )

	env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True

def test_fftw_win32( env, conf ) :
	if not env['with_fftw'] : return True
	print('testing fftw windows/double')
	if not conf.CheckCHeader( 'fftw.h' ) :
		return config_error( "Could not find fftw library headers...!" )
	if not conf.CheckLib( 'fftw2st', 'fftw_sizeof_fftw_real') :
		return config_error( "Could not find libfftw (or libdfftw)" )
	if not conf.CheckLib( 'rfftw2st', 'rfftw_create_plan' ) :
		return config_error( "Could not find librfftw (or libdrfftw)" )

	if env.has_key('double') :
		print('with double')
		if not conf.check_fftw_double_wo_prefix() :
			return config_error( "fftw compile/link/run tests failed!" )
		if not conf.check_rfftw_double_wo_prefix() :
			return config_error( "rfftw compile/link/run tests failed!" )
	else :
		print('with float')
		if not conf.check_fftw_float_wo_prefix() :
			return config_error( "fftw compile/link/run tests failed!" )
		if not conf.check_rfftw_float_wo_prefix() :
			return config_error( "rfftw compile/link/run tests failed!" )

	env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True


def setup_processing_environment( env, conf ) :

	if not test_fftw3(env, conf ) :
		return config_error( "Check that fftw is properly installed\n"
			"Or deactivate its usage with the option: 'with_fftw3=no'" )

	if sys.platform == 'win32' :
		result = test_fftw_win32( env, conf )
	else :
		result = test_fftw_linux( env, conf )
	if not result:
		return config_error( "Check that fftw is properly installed\n"
			"Or deactivate its usage with the option: 'with_fftw=no'" )

	return True


#---------------------------------------------------------------
# from glchecks.py

def check_opengl( env, conf ) :
	#Testing for OpenGL under Mac OS X is too complex (a platform-dependent context with additional
	# dependencies must be managed) and any sane Mac OS X should have OpenGL properly installed
	if sys.platform == 'darwin' :
		env.Append( CPPFLAGS=['-DUSE_GL=1'] )
		env.Append( LINKFLAGS=['-framework', 'OpenGL', '-framework', 'AGL'] )
		return True

	if sys.platform != 'win32' :
		if not conf.CheckCHeader('GL/gl.h') :
			return config_error( "Could not find OpenGL headers! Please install the headers supplied by your OpenGL driver vendor" )
		if not conf.CheckLib( 'GL', 'glBegin' ) :
			return config_error( "Could not find OpenGL library! Please install the development library supplied by your OpenGL driver vendor" )
	else :
		env.Append( LIBS='opengl32' )

	if not conf.check_opengl() :
		return config_error( "OpenGL compile/link/run test failed! Please check config.log for details..." )

	if sys.platform != 'win32' :
		if not conf.CheckCHeader( 'GL/glu.h' ) :
			return config_error( "Could not find GLU (OpenGL Utility library) headers!  Please install the headers supplied by your OpenGL driver vendor" )
		if not conf.CheckLib( 'GLU', 'gluPerspective' ) :
			return config_error( "Could not find GLU (OpenGL Utility library) binaries! Please install the development library supplied by your OpenGL driver vendor" )
	else :
		env.Append( LIBS='glu32' )
	if not conf.check_glu() :
		return config_error( "GLU compile/link/run tests failed! Check config.log for details..." )

	env.Append( CPPFLAGS=['-DUSE_GL=1'] )

	return True


