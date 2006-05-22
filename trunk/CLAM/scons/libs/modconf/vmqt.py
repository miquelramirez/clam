from modconf.glchecks import *

def setup_vmqt_environment( vmqt_env, conf ) :

	if not check_opengl( vmqt_env, conf ) :
		return False

	try:
		vmqt_env.Append( CPPPATH=['%s'%vmqt_env['qt_includes']] )
		vmqt_env.Append( LIBPATH=['%s'%vmqt_env['qt_libs']] )
	except KeyError :
		vmqt_env.Append( CPPPATH=['%s/include/qt3'%vmqt_env['QTDIR']] )
		vmqt_env.Append( LIBPATH=['%s/lib'%vmqt_env['QTDIR']] )

	result = conf.CheckCXXHeader( 'qapplication.h' )
	if not result :
		print "Could not find Qt 3 headers! Check your Qt 3 installation..."
		return False

	result = conf.CheckLibWithHeader( vmqt_env['QT_LIB'], 'qapplication.h', 'cxx', call='int argc = 1; QApplication qapp(argc,0);')
	if not result :
		print "Could not find multithreaded Qt 3 binaries (libqt-mt)! Check your Qt 3 installation..."
		return False

	result = conf.check_qt()
	if not result :
		print "Qt 3 compile/link/run tests failed! Check config.log for details..."
		return False

	vmqt_env.Append( CPPFLAGS=['-DUSE_QT=1'] )
	if sys.platform == 'win32' :
		vmqt_env.Append( CPPFLAGS=['-DQT_DLL', '-DQT_THREAD_SUPPORT', '-DVISUAL'] )
		# Win32 required libs
		vmqt_env.Append( LIBS=['advapi32', 'comctl32', 'comdlg32', 'gdi32', 'kernel32', 'ole32', 'oleaut32', 'shell32', 'user32', 'uuid', 'winspool','wsock32'] )
		if vmqt_env['with_portmidi'] :
			vmqt_env.Append( LIBS=['portmidi','porttime','winmm'] )
	return True

