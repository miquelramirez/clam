import re
import SCons.Util
nsisFiles_re = re.compile(r'^\s*File "([^"]*)"', re.M)

def generate(env) :
	"""Add Builders and construction variables for qt to an Environment."""
	print "Lodading nsis tool..."

	Builder = SCons.Builder.Builder

	def scanNsisContent(node, env, path, arg):
		contents = node.get_contents()
		includes = nsisFiles_re.findall(contents)
		return includes
	nsisscanner = env.Scanner(name = 'nsisfile',
		function = scanNsisContent,
		argument = None,
		skeys = ['.nsi'])
	nsisbuilder = Builder(
		action = 'makensis $SOURCE',
		source_scanner = nsisscanner,
#		src_suffix = '.nsi',
		single_source = True
		)
	env.Append( BUILDERS={'Nsis' : nsisbuilder} )

def exists(env) :
	return True

