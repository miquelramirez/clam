class PackageData :
	
	def __init__(self, name, version='0.0', depends = [] ) :
		self.name = name
		self.version = version
		self.depends = depends

	def create_pkg_descriptor( self, env, out_file ) :
		out = open(out_file, 'w')
	
		print >> out, "prefix = %s"%env['prefix']
		print >> out, "libdir = %s/lib"%env['prefix']
		print >> out, "includedir = %s/include"%env['prefix']
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
		print >> out, "Cflags: -I${includedir} %s -include CLAM/%s"%( " ".join(cppflags+cpppaths), env['preinclude'])

		out.close()
