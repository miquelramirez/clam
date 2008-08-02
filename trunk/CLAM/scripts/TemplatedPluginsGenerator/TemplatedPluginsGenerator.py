#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#TODO:
#* Take doxygen description from command line or a file

Usage = """Usage: ./TemplatedPluginsGenerator.py NEW_PLUGIN_NAME TEMPLATE_NAME [COPYRIGHT_HOLDER] [LICENSE] [YEAR]"""

import os, sys, shutil

def make_file( definitions_dict, replacement_str, filename, new_file_name="", to_lower_case=False ):
	template_dir = definitions_dict["template_name"]
	plugin_name = definitions_dict["plugin_name"]
	
	if not os.path.isdir('templates'):
		print "Error. There is not templates dir"
		sys.exit(3)
	try:
		f = open( "templates/" + template_dir + "/" + filename , 'r' )
	except IOError:
		return
	new_file = f.read(); f.close()
	
	if new_file_name=="": new_file_name=plugin_name
	if to_lower_case:
		new_file = new_file.replace( replacement_str.lower(), plugin_name.lower() )
	else:
		new_file = new_file.replace( replacement_str, plugin_name )
	if not os.path.isdir(definitions_dict["output_dir"]):
		definitions_dict["output_dir"] = "plugins"
		os.mkdir( definitions_dict["output_dir"] )
	if not os.path.isdir(definitions_dict["output_dir"]+"/"+plugin_name): os.mkdir( definitions_dict["output_dir"]+"/"+plugin_name )
	try:
		ext = "." + filename.split('.')[1]
	except:
		ext = ""
	try:
		f = open( definitions_dict["output_dir"]+"/" + plugin_name + "/" + new_file_name + ext, "w" )
		print "Creating " + definitions_dict["output_dir"]+ "/" + plugin_name + "/" + new_file_name + ext + " file"
	except:
		print definitions_dict["output_dir"] + "/" + plugin_name + "/" + plugin_name + ext
		print "Output file write error."
		sys.exit(2)
	if (ext==".hxx" or ext==".cxx") and definitions_dict["license"]!="null":
		f.write( definitions_dict['license_text'] + "\n\n" )
	f.write( new_file ); f.close()
#make_file()

def copy_file( template, filename ):
	if os.path.isfile("templates/" + template + "/" + filename):
		print "Copying " + filename + " file"
		shutil.copyfile( "templates/" + template + "/" + filename, definitions_dict["output_dir"] + "/" + definitions_dict["plugin_name"] + "/" + filename )
#copy_file()

def make_license_text(definitions_dict):
	try:
		f = open( "licenses/" + definitions_dict["license"] + ".txt", 'r' )
	except IOError:
		print "License file read error."
		print "License: " + plugin_dict["license"]
		sys.exit(2)
	license_text = f.read(); f.close()
	license_text = license_text.replace( "Copyright (c)", "Copyright (c) " + definitions_dict["year"] + " " + definitions_dict["copyright_holder"] )
	return license_text
#make_license_text()

def main(args): 
	""" Makes clam plugin code from a template
	
	Specification:
	- Available templates are folders with files named 'BaseProcessing.hxx', 'BaseProcessing.cxx', 'SConstruct', 'README' and 'options.cache' (all them are optional).
	- Available licenses are txt files at the 'licenses' folder.
	- C++ template files have an unique class named 'BaseProcessing' which will be renamed to the plugin name given at commnand prompt.
	- In the generated files (new plugin) 'BaseProcessing.hxx' and 'BaseProcessing.cxx' files will be renamed to the plugin name keeping the extension.
	- 'SConstruct' will be edited to give the to libraryName var the plugin name in lowercase.
	- 'README' and 'options.cache' will be verbatim copies.
	- The new plugin will be generated at the 'plugins' folder.
	
	
	 Examples:
	./TemplatedPluginsGenerator.py TestProcessing simple
	./TemplatedPluginsGenerator.py TestProcessing simple "Hernan Ordiales" GPL 2007
	./TemplatedPluginsGenerator.py TestProcessing simple "Hernan Ordiales" none 2007
	./TemplatedPluginsGenerator.py TestProcessing simple "by Hernán Ordiales
	 #* <code@ordia.com.ar>" GPL 2007
	./TemplatedPluginsGenerator.py TestProcessing simple "Fundació Barcelona Media Universitat Pompeu Fabra" GPL 2007
	./TemplatedPluginsGenerator.py TestProcessing simple "MUSIC TECHNOLOGY GROUP (MTG)
	 #*                         UNIVERSITAT POMPEU FABRA" GPL "2001-2004"
	"""

	if len(sys.argv) < 3:
		print "\nBad amount of input arguments\n", Usage, "\n"
		sys.exit(1)

	definitions_dict = {}
	definitions_dict["plugin_name"]	= sys.argv[1]
	definitions_dict["template_name"]	= sys.argv[2] # template folder name

	try:
		definitions_dict["copyright_holder"] = sys.argv[3]
	except:
		definitions_dict["copyright_holder"] = "" # default value

	try:
		definitions_dict["license"] = sys.argv[4]
	except:
		definitions_dict["license"] = "null" # default value

	try:
		definitions_dict["year"] = sys.argv[5]
		#print "Year: " + definitions_dict["year"]
		definitions_dict["year"] += " "
	except:
		definitions_dict["year"] = "" # default value

	#definitions_dict["output_dir"] = "plugins"
	definitions_dict["output_dir"] = "../../plugins"
	#definitions_dict["description"] = ""

	definitions_dict['license_text'] = make_license_text(definitions_dict)
	
	standard_name = "BaseProcessing"
	make_file( definitions_dict, standard_name, standard_name+".hxx" )
	make_file( definitions_dict, standard_name, standard_name+".cxx" )
	make_file( definitions_dict, standard_name, "SConstruct", "SConstruct", True )
	
	copy_file( definitions_dict["template_name"], "README" )
	copy_file( definitions_dict["template_name"], "options.cache" )
#main()


if __name__ == '__main__':
	main(sys.argv)
