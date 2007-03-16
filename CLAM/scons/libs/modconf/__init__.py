import sys

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
