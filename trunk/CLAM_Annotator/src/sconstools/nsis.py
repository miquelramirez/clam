def create_setup( target, source, env ) :
	os.system( "makensis clam_annotator.nsi" )

def create_setup_message( target, source, env ) :
	return "generating Annotator Installer"

def generate(env) :
	"""Add Builders and construction variables for qt to an Environment."""
	print "Lodading nsis tool..."
	env.Append( BUILDERS={'Nsis' : 
			env.Builder( action=env.Action(create_setup, create_setup_message ))
		} )

def exists(env) :
	return True

