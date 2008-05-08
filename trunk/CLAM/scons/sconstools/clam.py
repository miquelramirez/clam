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
		oldEnv = self['ENV'].items()
		pathSeparator = ";"
		self['ENV']['PKG_CONFIG_PATH'] = pathSeparator.join([
			self['ENV'],
			os.path.join(path,'lib','pkgconfig'),
		])
		self.ParseConfig('pkg-config %s --libs --cflags'% (path, ' '.join(libs)))
		self['ENV'] = dict(oldEnv)
		return

	raise "No CLAM support for your platform, sorry"

	# TODO: remove this pkgconfig parser for visualc

	descriptor_path = path + "\\lib\\pkgconfig\\"
	libpath = dict()
	ldlibs = dict()
	cppflags = dict()
	cpppath = dict()
	ccflags = dict()
	for lib in libs :
		desc_file = descriptor_path + lib + ".pc"
		instream = open( desc_file )

		for line in instream :
			tokens = line.strip().split(' ')
			if tokens[0] == 'Libs:' :
				for token in tokens[1:] :
					if "/LIBPATH:" in token :
						libpath[token.replace("/LIBPATH:", "")] = True
					else :
						ldlibs[token] = True
			elif tokens[0] == 'Cflags:':
				# remove Cflags: from the line
				line = line[len('Cflags:'):] 
				foo = line.strip().split(' /')
				for token in foo :
					# search for -X flags
					if(token.find(' -')!=-1):
						listOfRealTokens = token.split()
						#print('new token to parse: ' + token)
						for realToken in listOfRealTokens:
							ccflags[realToken] = True
					else:
						token = '/' + token
						#print('new token to parse: ' + token)
						if "/I" in token :
							cpppath[ token.replace("/I","") ] = True
						elif "/D" in token in token :
							cppflags[ token ] = True
						else :
							#print('token: ' + token )
							ccflags[token] = True
			else :
				pass

		instream.close()
	self.AppendUnique( LIBPATH = libpath.keys() )
	self.AppendUnique( LIBS = ldlibs.keys() )
	self.AppendUnique( CPPFLAGS = cppflags.keys() )
	self.AppendUnique( CCFLAGS = ccflags.keys() )
	self.AppendUnique( CPPPATH = cpppath.keys() )
	
#	sys.exit()


def generate(env) :
	import new
	import SCons
	method = new.instancemethod(enable_modules,env,SCons.Environment)
	env.EnableClamModules=method

def exists(env):
	return True

