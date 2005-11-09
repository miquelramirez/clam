import sys


def test_fftw_linux( processing_env, conf ) :
	if processing_env['double']  or sys.platform == 'win32' : # CLAM uses double for TData
		double_prefixed = False
		result = conf.CheckCHeader( 'fftw.h' )
		if not result :
			result = conf.CheckCHeader( 'dfftw.h' )
			if result : 
				double_prefixed = True
			else :
				print "Could not find fftw library headers...!"
				print "Install the library or deactivate its usage issuing the following command:"
				print "$ scons with_fftw=no"
				return False
		if not double_prefixed :
			result = conf.CheckCHeader( 'rfftw.h' )
		else :
			result = conf.CheckCHeader( 'drfftw.h' )
		if not result :
			print "Could not find some of the fftw library headers...!"
			print "Please check that all fftw headers are installed"
			return False
		if not double_prefixed :
			result = conf.CheckLib( 'fftw', 'fftw_sizeof_fftw_real')
		else :
			result = conf.CheckLib( 'dfftw', 'fftw_sizeof_fftw_real')
		if not result :
			print "Could not find libfftw (or libdfftw)"
			print "Please check that fftw is properly installed"
			return False
		if not double_prefixed :
			result = conf.CheckLib( 'rfftw', 'rfftw_create_plan' )
		else :
			result = conf.CheckLib( 'drfftw', 'rfftw_create_plan' )
		if not result :
			print "Could not find librfftw (or libdrfftw)"
			print "Please check that fftw is properly installed"
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
			print "Install the library or deactivate its usage issuing the following command:"
			print "$ scons with_fftw=no"
			return False
		result = conf.CheckCHeader( 'srfftw.h' )
		if not result :
			print "Could not find some of the fftw library headers...!"
			print "Please check that all fftw headers are installed"
			return False
		result = conf.CheckLib( 'sfftw', 'fftw_sizeof_fftw_real')
		if not result :
			print "Could not find libsfftw"
			print "Please check that fftw is properly installed"
			return False
		result = conf.CheckLib( 'srfftw', 'rfftw_create_plan' )
		if not result :
			print "Could not find libsrfftw"
			print "Please check that fftw is properly installed"
			return False

		result = conf.check_fftw_float_w_prefix()
		if not result :
			print "fftw compile/link/run tests failed! Check config.log for details"
			return False
		result = conf.check_rfftw_float_w_prefix()
		if not result :
			print "rfftw compile/link/run tests failed! Check config.log for details"
			return False

	processing_env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True

def test_fftw_win32( processing_env, conf ) :
	if processing_env.has_key('double')  or sys.platform == 'win32' : # CLAM uses double for TData
		print('testing fftw windows/double')
		double_prefixed = False
		result = conf.CheckCHeader( 'fftw.h' )
		if not result :
			print "Could not find fftw library headers...!"
			print "Install the library or deactivate its usage issuing the following command:"
			print "$ scons with_fftw=no"
			return False
		result = conf.CheckLib( 'fftw2st', 'fftw_sizeof_fftw_real')
		if not result :
			print "Could not find libfftw (or libdfftw)"
			print "Please check that fftw is properly installed"
			return False
		result = conf.CheckLib( 'rfftw2st', 'rfftw_create_plan' )
		if not result :
			print "Could not find librfftw (or libdrfftw)"
			print "Please check that fftw is properly installed"
			return False

		if processing_env.has_key('double') :
			print('with double')
			result = conf.check_fftw_double_wo_prefix()
		else :
			print('with float')
			result = conf.check_fftw_float_wo_prefix()
		if not result :
			print "fftw compile/link/run tests failed! Check config.log for details"
			return False
		if processing_env.has_key('double') :
			result = conf.check_rfftw_double_wo_prefix()
		else :
			result = conf.check_rfftw_float_wo_prefix()
		if not result :
			print "rfftw compile/link/run tests failed! Check config.log for details"
			return False
			
	processing_env.Append( CPPFLAGS=['-DUSE_FFTW=1'] )

	return True


def setup_processing_environment( processing_env, conf ) :
	if processing_env['with_fftw'] :
		if sys.platform == 'win32' :
			result = test_fftw_win32( processing_env, conf )
		else :
			result = test_fftw_linux( processing_env, conf )
		
	if result :
		return True
	else :
		return False
