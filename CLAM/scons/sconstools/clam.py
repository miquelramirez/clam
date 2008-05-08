import sys

def enable_modules( self, libs, path) :
	if sys.platform in ['linux2','darwin'] : 
		self.ParseConfig('PKG_CONFIG_PATH=%s/lib/pkgconfig pkg-config %s --libs --cflags'%
			(
				path,
				' '.join(libs)))
		return

	# TODO join this if compatible with the linux version
	if sys.platform in ['win32'] : 
		import os
		oldEnv = self['ENV'].items()
		pathSeparator = ";"
		self['ENV']['PKG_CONFIG_PATH'] = pathSeparator.join([
			self['ENV']['PKG_CONFIG_PATH'],
			os.path.join(path,'lib','pkgconfig'),
		])
		self.ParseConfig('pkg-config %s --libs --cflags'% ' '.join(libs))
		self['ENV'] = dict(oldEnv)
		return

	raise "No CLAM support for your platform, sorry"


def generate(env) :
	import new
	import SCons
	method = new.instancemethod(enable_modules,env,SCons.Environment)
	env.EnableClamModules=method

def exists(env):
	return True

