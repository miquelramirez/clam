import sys

class PackageData :
	
	def __init__(self, name, version='0.0', extra = [], depends = [] ) :
		self.name = name
		self.version = version
		self.depends = depends
		self.extra = extra
		if sys.platform != 'win32' :  #both linux2 and darwin have pkg-config
			self.create_pkg_descriptor = self.linux_create_pkg_descriptor
		else :  #win32
			self.create_pkg_descriptor = self.win32_create_pkg_descriptor

	def win32_create_pkg_descriptor( self, env, out_file ) :
		out = open(out_file, 'w' )

		print >> out, "prefix = %s"%env['prefix']
		print >> out, "libdir = %s\\lib"%env['prefix']
		print >> out, "includedir = %s\\include"%env['prefix']
		print >> out
		print >> out, "Name: %s"%self.name
		print >> out, "Version: %s"%self.version
		print >> out, "Description: C++ Framework for analysis, synthesis and transformation of music audio signals"
		print >> out, "Requires: %s"%" ".join(self.depends)
		print >> out, "Conflicts: "
		edict = env.Dictionary()
		libnames = [self.name] + self.extra 
		# extract libs from env
		libnames += edict['LIBS']
		libnames.remove( '$QT_LIB' )
		if len( env['QT_LIB'] ) > 0 :
			libnames.append( env['QT_LIB'] )
		libnames = [ '%s.lib'%name for name in libnames ]
		libnames_str = " ".join( libnames )
		libpaths = [ '%s\\lib'%env['prefix'] ]
		libpaths += edict['LIBPATH']
		#print libpaths
		libpaths = [ path.replace( "$QTDIR", env['ENV']['QTDIR'] ) for path in libpaths ]
		#print libpaths
		libpaths = [ '/LIBPATH:%s'%libpath for libpath in libpaths ]
		libpaths_str = ' '.join( libpaths )
		print >> out, "Libs: %s %s"%( libpaths_str , libnames_str)
		cppflags = edict.get('CPPFLAGS', [''])
		cppflags = [ flag for flag in cppflags if flag != '-D_USRDLL' ]
		ccflags = edict.get('CCFLAGS', [''])
		cppaths = edict.get('CPPPATH', [''])
		cpppaths = [ '/I'+path for path in cppaths ]
		print >> out, "Cflags: /I%s\\include /FICLAM/preinclude.hxx %s"%( env['prefix'] ," ".join(cppflags+cpppaths+ccflags))	

		out.close()

	def linux_create_pkg_descriptor( self, env, out_file ) :
		out = open(out_file, 'w')
	
		print >> out, "prefix = /%s"%env['prefix']
		print >> out, "libdir = /%s/lib"%env['prefix']
		print >> out, "includedir = /%s/include"%env['prefix']
		print >> out
		print >> out, "Name: %s"%self.name	
		print >> out, "Version: %s"%self.version
		print >> out, "Description: C++ Framework for analysis, synthesis and transformation of music audio signals"
		print >> out, "Requires: %s"%" ".join(self.depends)
		print >> out, "Conflicts: "
		print >> out, "Libs: -L${libdir} -l%s"%self.name
		edict = env.Dictionary()
		cppflags = edict.get('CPPFLAGS', [''])
		cppaths = edict.get('CPPPATH', [''])
		cpppaths = [ '-I'+path for path in cppaths ]
		print >> out, "Cflags: -I${includedir} %s"%" ".join(cppflags+cpppaths)

		out.close()
