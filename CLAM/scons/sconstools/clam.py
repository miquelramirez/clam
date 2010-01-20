import sys

def ClamQuietCompilation(env) :
	env['CXXCOMSTR'] = '== Compiling C++ $SOURCE'
	env['CCCOMSTR'] = '== Compiling C $SOURCE'
	env['SHCXXCOMSTR'] = '== Compiling shared C++ $SOURCE'
	env['SHCCCOMSTR'] = '== Compiling shared C $SOURCE'
	env['LINKCOMSTR'] = '== Linking $TARGET'
	env['SHLINKCOMSTR'] = '== Linking library $TARGET'
	env['QT4_RCCCOMSTR'] = '== Embeding resources $SOURCE'
	env['QT4_UICCOMSTR'] = '== Compiling interface $SOURCE'
	env['QT4_LRELEASECOMSTR'] = '== Compiling translation $TARGET'
	env['QT4_MOCFROMHCOMSTR'] = '== Generating metaobjects for $SOURCE'
	env['QT4_MOCFROMCXXCOMSTR'] = '== Generating metaobjects for $SOURCE'

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
	env.AddMethod(enable_modules, "EnableClamModules")
	env.AddMethod(ClamQuietCompilation)

def exists(env):
	return True

