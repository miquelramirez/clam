import os, shutil

###############
# Functions for creating a Mac OS X Application Bundle
###############

def create_mac_bundle(target,source, env) :
	icon='CLAM.icns'

	#Create subdirectories
	env.Install( str(source[0]) + '.app/Contents/MacOS/', source[0])
	env.Install( str(source[0]) + '.app/Contents/Resources/', '../resources/' + icon)
	
	#Create plist file
	plist = open ( 'Info.plist', 'w' )
	plist.write("""<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleExecutable</key>
	<string>%s</string>
	<key>CFBundleIconFile</key>
	<string>%s</string>
	<key>CFBundleName</key>
	<string>%s</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleSignature</key>
	<string>????</string>
</dict>
</plist>"""%(source[0],icon,source[0]))
				
	plist.close()
	env.Install( str(source[0]) + '.app/Contents/', 'Info.plist' )

def create_mac_bundle_message(target,source, env) :
	return "Creating Bundle for Mac OS X"
