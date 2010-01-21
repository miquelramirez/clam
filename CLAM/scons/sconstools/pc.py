import os

pkgConfigTemplate = """\
name = @name@
prefix = @prefix@
libdir = ${prefix}/lib
includedir = ${prefix}/include

Name: ${name}
Description: @description@
Url: @url@
Version: @version@
Requires: @requires@
Conflicts: @conflicts@
Libs: -L${libdir} -l${name} @libs@
Cflags: -I${includedir} @cflags@
"""
def PkgConfigFile(env, package, version, prefix, description=None, url=None, requires=[], conflicts=[], cflags=[], libs=[]) :
	return env.Textfile(target = package,
		source = [env.Value(pkgConfigTemplate)],
		TEXTFILESUFFIX='.pc',
		SUBST_DICT=[
			('@prefix@', prefix ),
			('@name@', package ),
			('@version@', version ),
			('@description@', "C++ Framework for analysis, synthesis and transformation of music audio signals" ),
			('@url@', 'http://clam-project.org' ),
			('@requires@', ", ".join(requires) ),
			('@conflicts@', ", ".join(conflicts) ),
			('@cflags@', " ".join(cflags) ),
			('@libs@', " ".join(libs) ),
			]
		)

def generate(env) :
	try: env.Textfile
	except : env.Tool('textfile', toolpath=[os.path.dirname(__file__)])
	env.AddMethod(PkgConfigFile)

def exists(env) :
	return True

