#!/usr/bin/scons -f

def _command_check(context, message, command) :
	"Helper function to reduce command trying code"
#	print "command:", command
	context.Message(message)
	runok, output = context.TryAction(command)
	context.Result(runok)
	return runok

def CheckPkgConfigProgram(context, minimum_version=None) :
	envPkgConfig = context.env['ENV'].get("PKG_CONFIG", "pkg-config")
	context.env.SetDefault(PKG_CONFIG=envPkgConfig)
	options = "--version"
	message=""

	if minimum_version :
		options = "--atleast-pkgconfig-version=%s"%minimum_version
		message+=" >= %s"%minimum_version

	if context.env["PKG_CONFIG"]!="pkg-config" or True :
		message+=" [PKG_CONFIG=%s]" % context.env["PKG_CONFIG"]

	runok = _command_check(
		context,
		"Checking for pkg-config availability%s..."%(message),
		"echo boo # $PKG_CONFIG %s>&1"%options
		)
	return runok

class PkgConfigNotDefined(Exception) :
	def __str__(self) :
		return "Call CheckPkgConfigProgram before calling CheckPkgConfigModule (or define PKG_CONFIG)."

def CheckPkgConfigModule(context, module,
	version=None,
	minimum_version=None,
	maximum_version=None,
	flags="--cflags --libs",
	variables={}
	) :
	if not context.env.has_key("PKG_CONFIG") :
		raise PkgConfigNotDefined()
	options = ""
	message = ""
	if   minimum_version :
		options += "--atleast-version="+minimum_version
		message += " >= %s"%minimum_version
	elif maximum_version :
		options += "--max-version="+maximum_version
		message += " <= %s"%maximum_version
	elif version :
		options += "--exact-version="+version;
		message += " == %s"%maximum_version
	if not options : options = "--exists"
	runok = _command_check(
		context,
		"Checking for pc module %s%s..."%(module,message),
		"$PKG_CONFIG %s %s>&2"%(module, options)
		)
	if variables :
		variables_options = " ".join([
			"--variable=%s='%s'"%item for item in variables.items()])
		print variables
	if runok and flags :
		context.env.ParseConfig("$PKG_CONFIG '%s' %s"%(module,flags))
	return runok

checks = dict(
	CheckPkgConfigProgram=CheckPkgConfigProgram,
	CheckPkgConfigModule=CheckPkgConfigModule,
)

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
def PkgConfigConfigureChecks(env) :
	return dict(
		CheckPkgConfigProgram=CheckPkgConfigProgram,
		CheckPkgConfigModule=CheckPkgConfigModule,
	)

def generate(env) :
	env.Tool('textfile')
	env.AddMethod(PkgConfigFile)
	env.AddMethod(PkgConfigConfigureChecks)
	

def exists(env) :
	return True


################################################################
# The rest is for testing, run the script with 'scons -f'

if __name__=="SCons.Script" :

	import unittest
	import os

	class SConfPkgConfigTest(unittest.TestCase) :

		mytestmodule = """\
name = mytestmodule
var1 = var1value
Name: ${name}
Description: A description
Url: http://clam-project.org
Version: 1.4.0
Requires:
Conflicts: 
Libs: -Lalibdir -lmytestmodulelib
Cflags: -Iaincludedir
"""
		def setUp(self) :
			file("mytestmodule.pc","w").write(SConfPkgConfigTest.mytestmodule)
			import os
			import SCons.SConf
			SCons.SConf.SetCacheMode("force") # Force the test to ignore cache
			env = Environment(ENV = os.environ)

			env["ENV"]["PKG_CONFIG_PATH"]="."

			generate(env)
			self.config = env.Configure( custom_tests = env.PkgConfigConfigureChecks())

		def tearDown(self) :
			self.config.Finish()
			os.remove("mytestmodule.pc")

		# Checking command availability

		def test_CheckPkgConfigProgram_setToTrue(self) :
			self.config.env["PKG_CONFIG"]="true"
			self.assertEqual(1,
				self.config.CheckPkgConfigProgram())

		def test_CheckPkgConfigProgram_setToFalse(self) :
			self.config.env["PKG_CONFIG"]="false"
			self.assertEqual(0,
				self.config.CheckPkgConfigProgram())

		def test_CheckPkgConfigProgram_changingToABadCommand(self) :
			self.config.env["PKG_CONFIG"]="badcommand"
			self.assertEqual(0,
				self.config.CheckPkgConfigProgram())

		def test_CheckPkgConfigProgram_byDefault(self) :
			self.assertEqual(1,
				self.config.CheckPkgConfigProgram())

		def test_CheckPkgConfigProgram_changingCommandUsingENV(self) :
			self.config.env["ENV"]["PKG_CONFIG"]="badcommand"
			self.assertEqual(
				0, self.config.CheckPkgConfigProgram())

		# Checking command version

		def test_CheckPkgConfigProgram_minimum_whenUnder(self) :
			self.config.CheckPkgConfigProgram()
		  	self.assertEqual(
				1, self.config.CheckPkgConfigProgram(minimum_version="0.0"))

		def test_CheckPkgConfigProgram_minimum_whenOver(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual(0,
				self.config.CheckPkgConfigProgram(minimum_version="136.4"))

		# Checking module availability

		def test_CheckModule_beforeCheckingProgram_fails(self) :
			try :
				self.assertEqual(1,
					self.config.CheckPkgConfigModule("mytestmodule"))
				self.fail("Exception expected")
			except PkgConfigNotDefined : pass

		def test_CheckModule_whenBadModule(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((0),
				self.config.CheckPkgConfigModule("badmodule"))
		def test_CheckModule_whenAvailable(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((1),
				self.config.CheckPkgConfigModule("mytestmodule"))
		def test_CheckModule_minimum_whenUnder(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((1),
				self.config.CheckPkgConfigModule("mytestmodule", minimum_version="1.3.9"))
		def test_CheckModule_minimum_whenOver(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((0),
				self.config.CheckPkgConfigModule("mytestmodule", minimum_version="1.4.1"))
		def test_CheckModule_maximum_whenUnder(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((0),
				self.config.CheckPkgConfigModule("mytestmodule", maximum_version="1.3.9"))
		def test_CheckModule_maximum_whenOver(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((1),
				self.config.CheckPkgConfigModule("mytestmodule", maximum_version="1.4.1"))
		def test_CheckModule_version_whenUnder(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((0),
				self.config.CheckPkgConfigModule("mytestmodule", version="1.3.9"))
		def test_CheckModule_version_whenOver(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((0),
				self.config.CheckPkgConfigModule("mytestmodule", version="1.4.1"))
		def test_CheckModule_version_whenMatches(self) :
			self.config.CheckPkgConfigProgram()
			self.assertEqual((1),
				self.config.CheckPkgConfigModule("mytestmodule", version="1.4.0"))

		def test_CheckModule_addsFlag_byDefault(self) :
			self.config.CheckPkgConfigProgram()
			self.config.CheckPkgConfigModule("mytestmodule")
			self.assertEqual(
				(
					['mytestmodulelib'],
					['alibdir'],
					['aincludedir'],
				),(
					self.config.env["LIBS"],
					self.config.env["LIBPATH"],
					self.config.env["CPPPATH"],
				))

		def test_CheckModule_flags_withFlagsToFalse(self) :
			self.config.CheckPkgConfigProgram()
			self.config.CheckPkgConfigModule("mytestmodule", flags=False)
			self.assertEqual(
				([],[],[]),
				(
					self.config.env.get("LIBS",[]),
					self.config.env.get("LIBPATH",[]),
					self.config.env.get("CPPPATH",[]),
				))

		def test_CheckModule_flags_withFlagsToLibs(self) :
			self.config.CheckPkgConfigProgram()
			self.config.CheckPkgConfigModule("mytestmodule", flags="--libs")
			self.assertEqual(
				(
					['mytestmodulelib'],
					['alibdir'],
					[],
				),(
					self.config.env.get("LIBS",[]),
					self.config.env.get("LIBPATH",[]),
					self.config.env.get("CPPPATH",[]),
				))


	unittest.TextTestRunner().run(unittest.makeSuite(SConfPkgConfigTest))

