import sys, os
import sys

#---------------------------------------------------------------
# from __init__.py

def setup_global_environment( clam_env, conf ) :	
	# clam env
	# check for pkg-config, compiler support, bash features, et.
	if sys.platform == 'linux2' :
		if not conf.check_pkg_config( conf ) :
			print 'WARNING: pkg-config is not installed'
			clam_env['pkg_config_available'] = False
		else :
			clam_env['pkg_config_available'] = True

	if clam_env['double'] :
		clam_env.Append( CPPFLAGS=['-DCLAM_DOUBLE'] )
	else :
		clam_env.Append( CPPFLAGS=['-DCLAM_FLOAT'] )

	if not clam_env['checks'] :
		clam_env.Append( CPPFLAGS=['-DCLAM_DISABLE_CHECKS'] )

	if clam_env['release_asserts'] :
		clam_env.Append( CPPFLAGS=['-DCLAM_USE_RELEASE_ASSERTS'] )
	if clam_env['optimize_and_lose_precision'] :
		clam_env.Append( CPPFLAGS=['-DCLAM_OPTIMIZE'] )

	if sys.platform != 'win32' :
		if clam_env['release'] :
			clam_env.Append( CCFLAGS='-g -O2 -fomit-frame-pointer -Wall'.split(' ') )
		else :
			clam_env.Append( CCFLAGS='-g -Wall'.split(' ') )
	else :
		if clam_env['release'] :
			clam_env.Append( CPPFLAGS = ['-DWIN32'] )
			clam_env.Append( CCFLAGS = '/FD /GR /GX /MD /O2 /Og /G7 /GL /W3 /Zm1000' )	
			clam_env.Append( LINKFLAGS = ['/OPT:NOREF'] )
		else :
			clam_env.Append( CPPFLAGS = ['-DWIN32', '-D_DEBUG'] )
			clam_env.Append( CCFLAGS = '/D /FD /GR /GX /GZ /MDd /Od /W3 /ZI /Zm1000' )
			clam_env.Append( LINKFLAGS = ['/OPT:NOREF', '/OPT:NOICF', '/DEBUG'] )

	for includePath in clam_env['CPPPATH'] :
		print('include before pthread checking: ' + includePath )
	# pthreads testing
	result = conf.CheckCHeader('pthread.h')
	if not result :
		print "Could not find pthread (Posix Threads) library headers!"
		return False
	if sys.platform == 'win32' :
		result = conf.CheckLib( 'pthreadVCE', 'pthread_join' )
	else:
		result = conf.CheckLib( 'pthread', 'pthread_join' )
	
	if not result :
		print "Could not find pthread (Posix Threads) library binaries!"
		return False
	result = conf.check_pthread()
	if not result :
		print "Thorough pthread check failed! Check config.log for details..."
		return False
	
	if result :
		clam_env.Append( CPPFLAGS=['-DUSE_PTHREADS=1'] )
		#clam_env.Append( LIBS=['pthread'] )

	if sys.platform == 'win32' :
		clam_env.Append( CPPFLAGS=['-DWIN32'] )


	#if not sys.platform == 'win32' :
	if sys.platform == 'linux2' :
		clam_env.Append(LIBPATH=['/usr/local/lib','/opt/lib'])
	elif sys.platform == 'darwin' :
		clam_env.Append(LIBPATH=['/usr/local/lib','/usr/lib','/opt/local/lib'])

	return True

#---------------------------------------------------------------
# from audioio.py

def test_sndfile( audioio_env, conf ) :
	result = conf.CheckCHeader( 'sndfile.h' )
	if not result :
		print "Could not find libsndfile headers! Please check your libsndfile installation"
		return False
	if sys.platform != 'win32' :
		result = conf.CheckLib( library='sndfile', symbol='sf_open_fd' )
	else :
		result = conf.CheckLib( library='libsndfile', symbol='sf_open_fd' )
	if not result :
		print "Could not find libnsndfile binaries! Please check your libsndfile installation"
		return False
	result = conf.check_libsndfile()
	if not result :
		print "libsndfile compile/link/run tests failed! Check config.log for details..."
		return False

	audioio_env.Append( CPPFLAGS=['-DUSE_SNDFILE=1'])
	return True

def test_oggvorbis( audioio_env, conf ) :
	result = conf.CheckCHeader( 'ogg/ogg.h' )
	if not result :
		print "Could not find libogg headers! Please check your libogg installation"
		return False
	result = conf.CheckLib( library='ogg', symbol='oggpack_writeinit' )
	if not result :
		print "Could not find libogg binaries! Please check you libogg installation" 
		return False
	result = conf.check_libogg()
	if not result :
		print "libogg compile/link/run tests failed! Check config.log for details"
		return False
	result = conf.CheckCHeader( 'vorbis/vorbisenc.h' )
	if not result :
		print "Could not find libvorbis headers! Please check your libvorbis installation"
		return False
	audioio_env.Append( LIBS=['vorbisenc'] )
	result = conf.CheckLib( library='vorbis', symbol='vorbis_encode_setup_init')
	if not result :
		print "Could not find libvorbis binaries! Please check you libvorbis installation"
		return False
	result = conf.check_libvorbis()
	if not result :
		print "libvorbis compile/link/run test failed! Check config.log for details"
		return False	
	result = conf.CheckCHeader( 'vorbis/vorbisfile.h' )
	if not result :
		print "Could not find libvorbisfile headers! Please check your libvorbisfile installation"
		return False
	result = conf.CheckLib( library="vorbisfile", symbol="ov_test_open" )
	if not result :
		print "Could not find libvorbisfile binaries! Please check your libvorbisfile installation"
		return False
	result = conf.check_libvorbisfile()
	if not result :
		print "libvorbisfile compile/link/run test failed! Please check config.log for details"
		return False
	
	audioio_env.Append( CPPFLAGS=['-DUSE_OGGVORBIS=1'] )
	return True

def test_mad( audioio_env, conf ) :
	result = conf.CheckCHeader( 'mad.h' )
	if not result :
		print "Could not find libmad headers! Please check your libmad installation"
		return False
	if sys.platform != 'win32' :
		result = conf.CheckLib( library='mad', symbol='mad_stream_init' )
	else :
		result = conf.CheckLib( library='libmad', symbol='mad_stream_init' )
	if not result :
		print "Could not find libmad binaries! Please check your libmad installation"
		return False
	result = conf.check_libmad( )
	if not result :
		print "libmad compile/link/run tests failed! Please check config.log for details"
		return False
	audioio_env.Append( CPPFLAGS=['-DUSE_MAD=1'] )
	return True

def test_id3lib( audioio_env, conf ) :
	if sys.platform == 'win32' :
		audioio_env.Append( CPPFLAGS=['-DID3LIB_LINKOPTION=1'] )
		#pass
	result = conf.CheckCXXHeader( 'id3.h' )
	if not result :
		print "Could not find id3lib headers! Please check your id3lib installation"
		return False
	if sys.platform == 'win32' :
		result = conf.CheckLibWithHeader( 'id3lib_vc7', 'id3/tag.h', 'cxx', call='ID3_Tag myTag;' )
	else :
		audioio_env.Append( LIBS=['z'] )
		result = conf.CheckLibWithHeader( 'id3', 'id3/tag.h', 'cxx', call='ID3_Tag myTag;' )
		
	if not result :
		print "Could not find id3lib binaries! Please check your id3lib installation"
		return False
	if sys.platform == 'win32' :
		audioio_env.Append( LIBS='zlib_vc7' )
	result = conf.check_id3lib()
	if not result :
		print "id3lib compile/link/run tests failed! Please check config.log for details"
		return False
	audioio_env.Append( CPPFLAGS=['-DUSE_ID3=1'] )
	return True

def test_alsa_sdk( audioio_env, conf ) :
	result = conf.CheckCHeader( 'alsa/asoundlib.h' )
	if not result :
		print "Could not find libasound development headers! Please check your libasound installation"
		return False
	result = conf.CheckLib( library='asound', symbol='snd_card_next' )	
	if not result :
		print "Could not find libasound binaries! Please check your libasound installation"
		return False
	result = conf.check_libasound()
	if not result :
		print "libasound compile/link/run tests failed! Please check config.log for details..."
		return False
	audioio_env.Append( CPPFLAGS=['-DUSE_ALSA=1'] )
	return True

def test_directx_sdk( audioio_env, conf ) :
	result = conf.CheckHeader( 'dsound.h' )
	if not result :
		print "Could not find DirectX SDK header 'dsound.h'! Please check that the DirectX SDK is placed at the CLAM sandbox"
		return False
	audioio_env.Append( LIBS=['dxerr8','user32', 'gdi32'] )
	result = conf.CheckLibWithHeader('dsound', 'dsound.h', 'cxx', call='DirectSoundCreate(0,0,0);')
	if not result :
		print "Could not find DirectX SDK library 'dsound.lib! Please check that the DirectX SDK is placed at the CLAM sandbox"
		return False
	result = conf.check_directx()
	if not result :
		print "directx compile/link/run test failed! Please check config.log for details..."
		return False
	return True

def test_jack ( core_env, conf ) :
	result = conf.CheckCHeader( 'jack/jack.h' )
	if not result :
		print "jack headers not found!"
		print "Either install jack or disable jack support by issuing"
		print "$scons with_jack=no"
		return False
	result = conf.CheckLib( library='jack', symbol='jack_cpu_load' )
	if not result :
		print "jack binaries not found!"
		print "Either install jack or disable jack support by issuing"
		print "$scons with_jack=no"
		return False
	result = conf.check_jack()
	if not result :
		print "jack compile/link/run test failed! check config.log for details..."
		print "Either install jack or disable jack support by issuing"
		print "$scons with_jack=no"
		return False
	core_env.Append(CPPFLAGS=['-DUSE_JACK=1'])
	return True

def test_portaudio( audioio_env, conf ) :
	result = conf.CheckCHeader( 'portaudio.h' )
	if not result :
		print "Could not find portaudio header! Please check your portaudio installation"
		return False
	result = conf.CheckLib( library='portaudio', symbol='Pa_GetHostApiInfo' )
	if not result :
		print "Could not find portaudio binary v19 or higher! (v18 wont work) Please check your portaudio installation"
		return False
	result = conf.check_portaudio()
	if not result :
		print "portaudio compile/link/run tests failed! Please check config.log for details..."
		return False
	audioio_env.Append( CPPFLAGS=['-DUSE_PORTAUDIO=1'] )
	audioio_env.Append( LIBS=['portaudio'] )
	return True

def test_portmidi( audioio_env, conf ) :
	if sys.platform == 'linux2' :
		print "Bypassing portmidi checks: using ALSA MIDI facilities"
		return True
	result = conf.CheckHeader( 'portmidi.h' )
	if not result :
		print "Could not find portmidi header 'portmidi.h'! Check your portmidi installation..."
		return False
	audioio_env.Append( LIBS=['portmidi','porttime','winmm'] )
	result = conf.CheckLib( library='portmidi', symbol='Pm_OpenInput' )
	if not result :
		print "Could not find portmidi libraries! Check your portmidi installation..."
		return False

	result = conf.check_portmidi()
	if not result :
		print "Portmidi compile/link/run tests failed! Check config.log for details..."
		return False

	audioio_env.Append( CPPFLAGS=['-DUSE_PORTMIDI=1'] )
	return True

def setup_audioio_environment( audioio_env, conf ) :

	if audioio_env['with_sndfile'] :
		if not test_sndfile( audioio_env, conf ) : return False

	if audioio_env['with_oggvorbis'] :
		if not test_oggvorbis( audioio_env, conf ) : return False

	if audioio_env['with_mad'] :
		if not test_mad( audioio_env, conf ) : return False

	if audioio_env['with_id3'] :
		if not test_id3lib( audioio_env, conf ) : return False

	if sys.platform == 'linux2' and audioio_env['with_alsa'] :
		if not test_alsa_sdk( audioio_env, conf ) : return False

	if audioio_env['with_jack'] :
		if not test_jack (audioio_env, conf): return False

	if audioio_env['with_portmidi'] :
		if not test_portmidi( audioio_env, conf ) : return False

	if audioio_env['with_portaudio'] :
		if not test_portaudio( audioio_env, conf ) : return False

	if not sys.platform == 'linux2' :
		if sys.platform == 'win32' :
			if not test_directx_sdk( audioio_env, conf ) : return False

		if audioio_env['audio_backend'] == 'directx' :
			audioio_env.Append( CPPFLAGS=['-DUSE_DIRECTX=1'] )

		if audioio_env['audio_backend'] == 'portaudio' and not audioio_env['with_portaudio']:
			if not test_portaudio( audioio_env, conf ) : return False

		if audioio_env['audio_backend'] == 'rtaudio' :
			if sys.platform == 'win32' :
				audioio_env.Append( CPPFLAGS=['-D__WINDOWS_DS__'])
			else :
				audioio_env.Append( CPPFLAGS=['-D__MACOSX_CORE__'])
			audioio_env.Append( CPPFLAGS=['-DUSE_RTAUDIO=1'] )

	return True


#---------------------------------------------------------------
# from core.py

def test_xml_backend( core_env, conf ) :
	returningval = True
	
	if core_env['xmlbackend'] == 'both' or core_env['xmlbackend'] == 'xercesc' :
		failed = False
		if not failed and not conf.CheckCXXHeader('xercesc/util/PlatformUtils.hpp') :
			print "Could not find xerces c headers! Defaulting to the null xml backend"
			failed = True
		core_env['ENV'] = os.environ
		print('path of app: ' + core_env['ENV']['PATH'])
		if not failed and not conf.check_xerces_c( conf ) :
			print "xerces c code compile/link/run test failed! Check config.log for details..."
			failed = True
		
		if not failed :
			core_env.Append( CPPFLAGS=['-DUSE_XERCES_TRUE', '-DCLAM_USE_XML'] )
		else :
			core_env['xmlbackend'] = 'none'
			returningval = False
			
	if core_env['xmlbackend'] == 'both' or core_env['xmlbackend'] == 'xmlpp' :
		failed = False
		if core_env['pkg_config_available'] :
			res = conf.pkg_config_check_existence( conf, name='libxml++-2.6' )
			if res :
				core_env.ParseConfig( 'pkg-config --cflags --libs libxml++-2.6' )
			else :
				failed = True
		else :
			failed = True
			
		if not failed and not conf.check_xmlpp( conf ) : 
			print "\tERROR: libxml++ code compile/link/run test failed! Check config.log for details"
			failed = True
		
		if not failed:
			core_env.Append( CPPFLAGS=['-DUSE_XMLPP_TRUE','-DCLAM_USE_XML'] )
		else :
			core_env['xmlbackend'] = 'none'
			returningval = False
	
	return returningval

def test_ladspa ( core_env, conf ) :
	result = conf.CheckCHeader( 'ladspa.h' )
	if not result :
		print "ladspa SDK header was not found"
		print "Either install ladspa SDK or disable the option by issuing the option"
		print "$ scons with_ladspa=no"
		return False
		#core_env['with_ladspa'] = False
	result = conf.check_ladspa()
	if not result :
		print "ladspa SDK compile test failed! Check config.log for details"
		print "Check that your installed ladspa headers match the version required by CLAM"
		return False			

	core_env.Append( CPPFLAGS=['-DUSE_LADSPA=1'] )
	return True


def test_oscpack ( core_env, conf ) :
	result = conf.CheckCXXHeader( 'oscpack/ip/IpEndpointName.h' )
	if not result :	
		print "liboscpack headers not found!"
		print "Either install liboscpack or disable OSC support by issuing"
		print "$scons with_osc=no"
		return False
	result = conf.CheckLibWithHeader( 'oscpack', 'oscpack/ip/IpEndpointName.h', 'cxx', call='IpEndpointName("localhost",9999);')
	if not result :
		print "liboscpack binaries not found!"
		print "Either install liboscpack or disable OSC support by issuing"
		print "$scons with_osc=no"
		return False
	result = conf.check_oscpack()
	if not result :
		print "liboscpack compile/link/run test failed! check config.log for details..."
		print "Either install liboscpack or disable OSC support by issuing"
		print "$scons with_osc=no"
		return False
	core_env.Append( CPPFLAGS=['-DUSE_OSCPACK=1'] )
	return True

def setup_core_environment ( core_env, conf) :
	if not test_xml_backend( core_env, conf): return False

	if core_env.has_key('with_ladspa')  and core_env['with_ladspa'] == True and sys.platform == 'linux2':
		if not test_ladspa ( core_env, conf): return False
		
	
	if core_env.has_key('with_osc') and core_env['with_osc'] == True and sys.platform != 'win32':
		if not test_oscpack (core_env, conf): return False
	

	return True
#---------------------------------------------------------------
# from processing.py

def test_fftw3( env, conf) :
	if not env['with_fftw3'] : return True

	if not conf.CheckHeader( 'fftw3.h' ) :
		print "FFTW3 header not found"
		return False

	if not conf.CheckLib( 'fftw3', 'fftw_plan_dft_r2c_1d') :
		print "Unable to link FFTW3"
		return False

	env.Append( CPPFLAGS=['-DUSE_FFTW3=1'] )
	return True

def test_fftw_linux( env, conf ) :
		
	if env['double'] : # CLAM uses double for TData
		nonPrefixHeader = conf.CheckCHeader( 'fftw.h' )
		prefixHeader = conf.CheckCHeader( 'dfftw.h' )
		if not prefixHeader and not nonPrefixHeader :
			print "Could not find fftw library headers...!"
			return False
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
			print "Could not find '%s' headers!"%realHeader
			return False

		if not conf.CheckLib( lib, 'fftw_sizeof_fftw_real') :
			print "Could not find '%s'!"%lib
			return False

		if not conf.CheckLib( realLib, 'rfftw_create_plan' ) :
			print "Could not find' %s'!"%realLib
			return False

		if not double_prefixed :
			result = conf.check_fftw_double_wo_prefix()
		else :
			result = conf.check_fftw_double_w_prefix()
		if not result :
			print "fftw compile/link/run tests failed! Check config.log for details"
			return False
		if not double_prefixed :
			result = conf.check_rfftw_double_wo_prefix()
		else :
			result = conf.check_rfftw_double_w_prefix()
		if not result :
			print "rfftw compile/link/run tests failed! Check config.log for details"
			return False
		
	else :
		result = conf.CheckCHeader( 'sfftw.h' )
		if not result :
			print "Could not find fftw library headers...!"
			return False
		result = conf.CheckCHeader( 'srfftw.h' )
		if not result :
			print "Could not find some of the fftw library headers...!"
			return False
		result = conf.CheckLib( 'sfftw', 'fftw_sizeof_fftw_real')
		if not result :
			print "Could not find libsfftw"
			return False
		result = conf.CheckLib( 'srfftw', 'rfftw_create_plan' )
		if not result :
			print "Could not find libsrfftw"
			return False

		result = conf.check_fftw_float_w_prefix()
		if not result :
			print "fftw compile/link/run tests failed! Check config.log for details"
			return False
		result = conf.check_rfftw_float_w_prefix()
		if not result :
			print "rfftw compile/link/run tests failed! Check config.log for details"
			return False

	env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True

def test_fftw_win32( env, conf ) :
	print('testing fftw windows/double')
	result = conf.CheckCHeader( 'fftw.h' )
	if not result :
		print "Could not find fftw library headers...!"
		return False
	result = conf.CheckLib( 'fftw2st', 'fftw_sizeof_fftw_real')
	if not result :
		print "Could not find libfftw (or libdfftw)"
		return False
	result = conf.CheckLib( 'rfftw2st', 'rfftw_create_plan' )
	if not result :
		print "Could not find librfftw (or libdrfftw)"
		return False

	if env.has_key('double') :
		print('with double')
		result = conf.check_fftw_double_wo_prefix()
	else :
		print('with float')
		result = conf.check_fftw_float_wo_prefix()
	if not result :
		print "fftw compile/link/run tests failed!"
		return False
	if env.has_key('double') :
		result = conf.check_rfftw_double_wo_prefix()
	else :
		result = conf.check_rfftw_float_wo_prefix()
	if not result :
		print "rfftw compile/link/run tests failed!"
		return False
			
	env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True


def setup_processing_environment( env, conf ) :

	print " setup processing"
	if not test_fftw3(env, conf ) :
		print "Check that fftw is properly installed"
		print "Or deactivate its usage with the option: 'with_fftw3=no'"
		print "See config.log for details."
		return False

	if not env['with_fftw'] :
		return True
	if sys.platform == 'win32' :
		result = test_fftw_win32( env, conf )
	else :
		result = test_fftw_linux( env, conf )
	if not result:
		print "Check that fftw is properly installed"
		print "Or deactivate its usage with the option: 'with_fftw=no'"
		print "See config.log for details."

	return result


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
		result = conf.CheckCHeader('GL/gl.h')
		if not result :
			print "Could not find OpenGL headers! Please install the headers supplied by your OpenGL driver vendor"
			return False
		result = conf.CheckLib( 'GL', 'glBegin' )
		if not result :
			print "Could not find OpenGL library! Please install the development library supplied by your OpenGL driver vendor"
			return False
	else :
		env.Append( LIBS='opengl32' )
		
	result = conf.check_opengl()
	if not result :
		print "OpenGL compile/link/run test failed! Please check config.log for details..."
		return False

	if sys.platform != 'win32' :
		result = conf.CheckCHeader( 'GL/glu.h' )
		if not result :
			print "Could not find GLU (OpenGL Utility library) headers!  Please install the headers supplied by your OpenGL driver vendor" 
			return False
		result = conf.CheckLib( 'GLU', 'gluPerspective' )
		if not result : 
			print "Could not find GLU (OpenGL Utility library) binaries! Please install the development library supplied by your OpenGL driver vendor"
			return False
	else :
		env.Append( LIBS='glu32' )
	result = conf.check_glu()
	if not result :
		print "GLU compile/link/run tests failed! Check config.log for details..."
		return False

	env.Append( CPPFLAGS=['-DUSE_GL=1'] )

	return True


