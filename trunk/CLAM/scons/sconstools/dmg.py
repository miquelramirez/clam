import os

def create_dmg( target, source, env) :
	os.system( "mkdir DMG" )
	os.system( "cp README.txt DMG" )
	os.system( "cp -r Annotator.app DMG" )
	os.system( "sudo hdiutil create -srcfolder DMG -volname CLAM_Annotator -uid 0 CLAM_Annotator-%s.dmg"%version )
	os.system( "rm -rf DMG" )

def create_dmg_message( target, source, env):
	return "Creating DMG package"

def generate(env) :
	"""Add Builders and construction variables for qt to an Environment."""
	print "Lodading dmg tool..."
	env.Append( BUILDERS={'Dmg' : 
			env.Builder( action=env.Action(create_dmg, create_dmg_message ))
		} )

def exists(env) :
	return True

