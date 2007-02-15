import sys

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


