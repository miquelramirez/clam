#!/usr/bin/python
# -*- coding: UTF-8 -*-

# Specification:
#   - Available templates are folders with files named 'BaseProcessing.hxx', 'BaseProcessing.cxx', 'SConstruct', 'README' and 'options.cache' (all them are optional).
#   - Available licenses are txt files at the 'licenses' folder.
#   - C++ template files have an unique class named 'BaseProcessing' which will be renamed to the plugin name given at commnand prompt.
#   - In the generated files (new plugin) 'BaseProcessing.hxx' and 'BaseProcessing.cxx' files will be renamed to the plugin name keeping the extension.
#   - 'SConstruct' will be edited to give the to libraryName var the plugin name in lowercase.
#   - 'README' and 'options.cache' will be verbatim copies.
#   - The new plugin will be generated at the 'plugins' folder.


# Usage:
#   TemplatedPluginsGenerator NEW_PLUGIN_NAME TEMPLATE_NAME [COPYRIGHT_HOLDER] [LICENSE] [YEAR]

# Examples:
#./TemplatedPluginsGenerator TestProcessing simple
#./TemplatedPluginsGenerator TestProcessing simple "Hernan Ordiales" GPL 2007
#./TemplatedPluginsGenerator TestProcessing simple "Hernan Ordiales" none 2007
#./TemplatedPluginsGenerator TestProcessing simple "by Hernán Ordiales
 #* <code@ordia.com.ar>" GPL 2007
#./TemplatedPluginsGenerator TestProcessing simple "Fundació Barcelona Media Universitat Pompeu Fabra" GPL 2007
#./TemplatedPluginsGenerator TestProcessing simple "MUSIC TECHNOLOGY GROUP (MTG)
 #*                         UNIVERSITAT POMPEU FABRA" GPL "2001-2004"


#TODO:
#  - take the doxygen description from command line or a file
#  - GUI with pyqt?

import os, sys, shutil

if len(sys.argv) < 3:
	print "Bad amount of input arguments"
	print "Usage: TemplatedPluginsGenerator NEWPLUGINNAME TEMPLATENAME [COPYRIGHTHOLDER] [LICENSE] [YEAR]"
	sys.exit(1)

plugin = {}
plugin["name"]		= sys.argv[1]
plugin["template"]	= sys.argv[2] # template folder name

try:
	plugin["copyright_holder"] = sys.argv[3]
except:
	plugin["copyright_holder"] = "" # default value

try:
	plugin["license"] = sys.argv[4]
except:
	plugin["license"] = "null" # default value
try:
	f = open( "licenses/" + plugin["license"] + ".txt", 'r' )
except IOError:
	print "License file read error."
	print "License: " + plugin["license"]
	sys.exit(2)
	
try:
	plugin["year"] = sys.argv[5]
	#print "Year: " + plugin["year"]
	plugin["year"] += " "
except:
	plugin["year"] = "" # default value



license_text = f.read()
f.close
license_text = license_text.replace( "Copyright (c)", "Copyright (c) " + plugin["year"] + plugin["copyright_holder"] )


def make_file( template_dir, plugin_name, replacement_str, filename, new_file_name="", to_lower_case=False ):
	if not os.path.isdir('templates'):
		print "Error. There is not templates dir"
		sys.exit(3)
	try:
		f = open( "templates/" + template_dir + "/" + filename , 'r' )
	except IOError:
		return
	new_file = f.read()
	f.close
	
	if new_file_name=="": new_file_name=plugin_name
	if to_lower_case:
		new_file = new_file.replace( replacement_str.lower(), plugin_name.lower() )
	else:
		new_file = new_file.replace( replacement_str, plugin_name )
	if not os.path.isdir('plugins'): os.mkdir( 'plugins' )
	if not os.path.isdir("plugins/"+plugin_name): os.mkdir( "plugins/"+plugin_name )
	try:
		ext = "." + filename.split('.')[1]
	except:
		ext = ""
	try:
		g = open( "plugins/" + plugin_name + "/" + new_file_name + ext, "w" )
		print "Creating " + "plugins/" + plugin_name + "/" + new_file_name + ext + " file"
	except:
		print "plugins/" + plugin_name + "/" + plugin_name + ext
		print "Output file write error."
		sys.exit(2)
	if (ext==".hxx" or ext==".cxx") and plugin["license"]!="null":
		g.write( license_text + "\n\n" )
	g.write( new_file )
	g.close()

def copy_file( template, filename ):
	if os.path.isfile("templates/" + template + "/" + filename):
		print "Copying " + filename + " file"
		shutil.copyfile( "templates/" + template + "/" + filename, "plugins/" + plugin["name"] + "/" + filename )


standard_name = "BaseProcessing"
make_file( plugin["template"], plugin["name"], standard_name, standard_name+".hxx" )
make_file( plugin["template"], plugin["name"], standard_name, standard_name+".cxx" )
make_file( plugin["template"], plugin["name"], standard_name, "SConstruct", "SConstruct", True )

copy_file( plugin["template"], "README" )
copy_file( plugin["template"], "options.cache" )
