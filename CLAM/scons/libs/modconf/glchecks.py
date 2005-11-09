import sys

def check_opengl( env, conf ) :
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

