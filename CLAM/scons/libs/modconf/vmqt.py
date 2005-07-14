from modconf.glchecks import *

def setup_vmqt_environment( vmqt_env, conf ) :

	if not check_opengl( vmqt_env, conf ) :
		return False

	vmqt_env.Append( CPPPATH=['%s'%vmqt_env['qt_includes']] )
	vmqt_env.Append( LIBPATH=['%s'%vmqt_env['qt_libs']] )

	result = conf.CheckCXXHeader( 'qapplication.h' )
	if not result :
		print "Could not find Qt 3 headers! Check your Qt 3 installation..."
		return False

	result = conf.CheckLibWithHeader( 'qt-mt', 'qapplication.h', 'cxx', call='QApplication qapp(0,0);')
	if not result :
		print "Could not find multithreaded Qt 3 binaries (libqt-mt)! Check your Qt 3 installation..."
		return False

	result = conf.check_qt()
	if not result :
		print "Qt 3 compile/link/run tests failed! Check config.log for details..."
		return False

	vmqt_env.Append( CPPFLAGS=['-DUSE_QT=1'] )
	return True

