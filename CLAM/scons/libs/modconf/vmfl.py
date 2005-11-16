from modconf.glchecks import *
import sys

def setup_vmfl_environment( vmfl_env, conf ) :

	if not check_opengl( vmfl_env, conf ) :
		return False

	vmfl_env.Append( LINKFLAGS=[''] )
	if sys.platform == 'linux2' :
		result = conf.check_fltk_config()
		if not result :
			print "fltk-config tool was not found on your system. Please check your FLTK installation"
			return False
	
		vmfl_env.ParseConfig('fltk-config --use-gl --use-images --use-glut --use-forms --cxxflags --ldflags' )
	elif sys.platform == 'darwin' :
		result = conf.check_fltk_config()
		if not result :
			print "fltk-config cagada"
			return False
		vmfl_env.Append( CPPPATH=['-I/usr/local/include/FL/images'] )
		vmfl_env.Append( LINKFLAGS=['-framework', 'Carbon', '-framework', 'ApplicationServices'] )
		vmfl_env.Append( LIBS=['fltk_images', 'fltk_png', 'z', 'fltk_jpeg', 'fltk_gl', 'fltk_forms', 'fltk'] )
	elif sys.platform == 'win32' :
		# Manual configuration
		result = conf.CheckCXXHeader( 'FL/Fl.H' )
		if not result :
			print "Could not find header 'FL/Fl.H', please check your fltk configuration"
			return False
		# FLTK modules
		vmfl_env.Append( LIBS=['fltkforms_vc7', 'fltkgl_vc7', 'fltkimages_vc7'] )
		# Win32 API required libs
		vmfl_env.Append( LIBS=['advapi32', 'comctl32', 'comdlg32', 'gdi32', 'kernel32', 'ole32', 'oleaut32', 'shell32', 'user32', 'uuid', 'winspool','wsock32'] )
		result = conf.CheckLibWithHeader( 'fltk_vc7', 'FL/Fl.H', 'cxx', call='Fl::run();' ) 
		if not result :
			print "Could not find FLTK libs, please check your fltk installation"
			return False
		
	result = conf.check_fltk()
	if not result :
		print "FLTK compile/link/run test failed! Please check config.log for details"
		return False

	vmfl_env.Append( CPPFLAGS=['-DUSE_FLTK=1'] )
	return True
