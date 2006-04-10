import sys
import SCons

def enable_modules( self, libs, path) :
	if sys.platform == 'linux2': 
		self.ParseConfig('PKG_CONFIG_PATH=%s/lib/pkgconfig pkg-config %s --libs --cflags'%
			(
				path,
				' '.join(libs)))
		return

	if sys.platform != 'win32': raise "No CLAM support for your platform, sorry"

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
				foo = line.strip().split(' /')
				for token in foo[1:] :
					token = '/' + token
					# search for -X flags
					if(token.find(' -')!=-1):
						listOfRealTokens = token.split(' -')
						#print('new token to parse: ' + token)
						firstToken = listOfRealTokens[0]
						if "/I" in firstToken:
							cpppath[ firstToken.replace("/I","") ] = True
						elif "/D" in firstToken :
							cppflags[ firstToken ] = True
						else :
							ccflags[firstToken] = True
						for realToken in listOfRealTokens[1:]:
							#print('new token to parse: ' + realToken)
							ccflags['-'+realToken] = True
					else:
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

def generate(env) :
	import new
	method = new.instancemethod(enable_modules,env,SCons.Environment)
	env.EnableClamModules=method

def exists(env):
	return True

