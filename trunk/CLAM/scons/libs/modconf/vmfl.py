from modconf.glchecks import *

def setup_vmfl_environment( vmfl_env, conf ) :

	if not check_opengl( vmfl_env, conf ) :
		return False

	vmfl_env.Append( LINKFLAGS=[''] )
	result = conf.check_fltk_config()
	if not result :
		print "fltk-config tool was not found on your system. Please check your FLTK installation"
		return False
	
	vmfl_env.ParseConfig('fltk-config --cflags --cxxflags --ldflags --libs --use-gl --use-images --use-glut --use-forms')

	
	result = conf.check_fltk()
	if not result :
		print "FLTK compile/link/run test failed! Please check config.log for details"
		return False

	vmfl_env.Append( CPPFLAGS=['-DUSE_FLTK=1'] )
	return True
