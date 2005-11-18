import sys

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
		print "$scons with_jack_support=no"
		return False
	result = conf.CheckLib( library='jack', symbol='jack_cpu_load' )
	if not result :
		print "jack binaries not found!"
		print "Either install jack or disable jack support by issuing"
		print "$scons with_jack_support=no"
		return False
	result = conf.check_jack()
	if not result :
		print "jack compile/link/run test failed! check config.log for details..."
		print "Either install jack or disable jack support by issuing"
		print "$scons with_jack_support=no"
		return False
	core_env.Append(CPPFLAGS=['-DUSE_JACK=1'])
	return True

def test_portaudio( audioio_env, conf ) :
	result = conf.CheckCHeader( 'portaudio.h' )
	if not result :
		print "Could not find portaudio header! Please check your portaudio installation"
		return False
	result = conf.CheckLib( library='portaudio', symbol='Pa_GetVersion' )
	if not result :
		print "Could not find portaudio binary! Please check your portaudio installation"
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
		result = test_sndfile( audioio_env, conf )
		if not result : return False
	if audioio_env['with_oggvorbis'] :
		result = test_oggvorbis( audioio_env, conf )
		if not result : return False
	if audioio_env['with_mad'] :
		result = test_mad( audioio_env, conf )
		if not result : return False
	if audioio_env['with_id3'] :
		result = test_id3lib( audioio_env, conf ) 
		if not result : return False
	if sys.platform == 'linux2' and audioio_env['with_alsa'] :
		result = test_alsa_sdk( audioio_env, conf )
		if not result : return False
	if audioio_env['with_jack_support'] and sys.platform != 'win32' :
		if not test_jack (audioio_env, conf): return False
	if not sys.platform == 'linux2' :
		if sys.platform == 'win32' :
			result = test_directx_sdk( audioio_env, conf )			
			if not result : return False
		if audioio_env['audio_backend'] == 'directx' :
			audioio_env.Append( CPPFLAGS=['-DUSE_DIRECTX=1'] )		
		if audioio_env['audio_backend'] == 'portaudio' :
			#raise RuntimeError, "Not implemented yet!" --> Now implemented!
			result = test_portaudio( audioio_env, conf )
			if not result : return False
		if audioio_env['audio_backend'] == 'rtaudio' :
			if sys.platform == 'win32' :
				audioio_env.Append( CPPFLAGS=['-D__WINDOWS_DS__'])
			else :
				audioio_env.Append( CPPFLAGS=['-D__MACOSX_CORE__'])
			audioio_env.Append( CPPFLAGS=['-DUSE_RTAUDIO=1'] )	
	if audioio_env['with_portmidi'] :
		result = test_portmidi( audioio_env, conf )
		if not result : return False
	
	return True
