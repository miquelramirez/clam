import sys

def setup_audioio_environment( audioio_env, conf ) :

	if audioio_env['with_sndfile'] :
		result = conf.CheckCHeader( 'sndfile.h' )
		if not result :
			print "Could not find libsndfile headers! Please check your libsndfile installation"
			return False
		result = conf.CheckLib( library='sndfile', symbol='sf_open_fd' )
		if not result :
			print "Could not find libnsndfile binaries! Please check your libsndfile installation"
			return False
		result = conf.check_libsndfile()
		if not result :
			print "libsndfile compile/link/run tests failed! Check config.log for details..."
			return False
		
		audioio_env.Append( CPPFLAGS=['-DUSE_SNDFILE=1'])
	if audioio_env['with_oggvorbis'] :
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
	if audioio_env['with_mad'] :
		result = conf.CheckCHeader( 'mad.h' )
		if not result :
			print "Could not find libmad headers! Please check your libmad installation"
			return False
		result = conf.CheckLib( library='mad', symbol='mad_stream_init' )
		if not result :
			print "Could not find libmad binaries! Please check your libmad installation"
			return False
		result = conf.check_libmad( )
		if not result :
			print "libmad compile/link/run tests failed! Please check config.log for details"
			return False
		audioio_env.Append( CPPFLAGS=['-DUSE_MAD=1'] )
	if audioio_env['with_id3'] :
		result = conf.CheckCXXHeader('id3/tag.h')
		if not result :
			print "Could not find id3lib headers! Please check your id3lib installation"
			return False
		result = conf.CheckLibWithHeader( 'id3', 'id3/tag.h', 'cxx', call='ID3_Tag myTag;' )
		if not result :
			print "Could not find id3lib binaries! Please check your id3lib installation"
			return False
		result = conf.check_id3lib()
		if not result :
			print "id3lib compile/link/run tests failed! Please check config.log for details"
			return False
		audioio_env.Append( CPPFLAGS=['-DUSE_ID3=1'] )
	if sys.platform == 'linux2' and audioio_env['with_alsa'] :
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
	if not sys.platform == 'linux2' :
		if sys.platform == 'win32' and audioio_env['audio_backend'] == 'directx' :
			raise RuntimeError, "Not implemented yet!"
		if audioio_env['audio_backend'] == 'portaudio' :
			raise RuntimeError, "Not implemented yet!"
		if audioio_env['audio_backend'] == 'rtaudio' :
			raise RuntimeError, "Not implemented yet!"
	if audioio_env['with_portmidi'] :
		raise RuntimeError, "Not implemented yet!"
	
	return True
