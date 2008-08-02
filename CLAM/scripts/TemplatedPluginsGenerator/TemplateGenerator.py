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
#* Add configuration options
#* Add typed controls support

Usage = """Usage: ./TemplateGenerator TEMPLATEFILE"""

import os, sys, shutil
import sets

def make_include(class_name):
	return "#include <CLAM/" + class_name + ".hxx>\n"
#make_include()

def make_base_processing_hxx_file(definitions_dict):
	""" BaseProcessing.hxx file """

	try:
		filename = "templates/%s/BaseProcessing.hxx"%(definitions_dict["template_name"])
		f = open(filename, "w")
	except:
		print "Output file write error. File: "+filename
		sys.exit(2)

	f.write( "#ifndef _BaseProcessing_\n" )
	f.write( "#define _BaseProcessing_\n\n" )

	inputs = definitions_dict["inputs"]
	outputs = definitions_dict["outputs"]
	incontrols = definitions_dict["incontrols"]
	outcontrols = definitions_dict["outcontrols"]

	#Includes
	f.write( make_include(definitions_dict["base_class_name"]) )
	available_port_types_set = sets.Set( (item[0] for item in inputs+outputs) )
	for class_name in available_port_types_set: f.write( make_include(class_name) )
	if len(incontrols)>0: f.write( make_include("InControl") )
	if len(outcontrols)>0: f.write( make_include("OutControl") )
	
	f.write( """
namespace CLAM {

""" )
	
	#Class
	f.write( """	/**	\\brief Short description
	*
	*	Description
	*/
	class BaseProcessing: public %s
	{	
"""%(definitions_dict["base_class_name"]) )
		
	#GetClassName
	f.write("""		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return \"BaseProcessing\"; }

""")
	
	member_style = definitions_dict["member_style"]
	#Ports
	f.write( "\t\t/** Ports **/\n" )
	for port_type,port_name in (inputs+outputs):
		f.write( "\t\t" + port_type + " " + member_style + port_name.replace(' ', '') + ";\n" )
	
	#Controls
	f.write( "\n\t\t/** Controls **/\n" )
	for cmin,cmax,control_name in incontrols:
		f.write( "\t\tInControl " + member_style + control_name.replace(' ', '') + ";\n" )
	for cmin,cmax,control_name in outcontrols:
		f.write( "\t\tOutControl " + member_style + control_name.replace(' ', '') + ";\n" )
	
	f.write( "\n\tpublic:\n" )
	
	#Constructor
	f.write( "\t\tBaseProcessing(const Config & config=Config())\n\t\t\t")
	members_list = []
	for port_type,port_name in (inputs+outputs):
		members_list.append( "\t\t\t" + member_style + port_name.replace(' ', '') + "(\"" + port_name + "\", this)" )
	for cmin,cmax,control_name in (incontrols+outcontrols):
		members_list.append( "\t\t\t" + member_style + control_name.replace(' ', '') + "(\"" + control_name + "\", this)" )
	
	if len(members_list)>0:
		f.write(":\n")
		for i in range(len(members_list)-1):
			f.write( members_list[i] + ",\n" )
		f.write( members_list[len(members_list)-1] )
		
	f.write( """
		{
			Configure( config );

""" )
	
	#InControls SetBounds, SetDefaultValue, DoControl
	for cmin,cmax,control_name in (incontrols):
		control_media = round((float(cmin)+float(cmax))/2.)
		f.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".SetBounds(" + cmin + "," + cmax + ");\n" )
		f.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".SetDefaultValue(" + str(control_media) + ");\n" )
		f.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".DoControl(" + str(control_media) + ");\n" )
	f.write( "\t\t}\n" )
	
	
	#Destructor
	f.write( "\n\t\t~BaseProcessing() {}\n" )
	
	#Do()
	f.write( """
		bool Do()
		{
			return true;
		}""" )

	f.write("""
	
	private:
		/** Child processings **/
	
	};

};//namespace CLAM

#endif // _BaseProcessing_
""")
	f.close()
#make_base_processing_hxx_file()

def make_base_processing_cxx_file(definitions_dict):
	""" BaseProcessing.cxx file """
	try:
		filename = "templates/%s/BaseProcessing.cxx"%(definitions_dict["template_name"])
		f = open(filename, "w")
	except:
		print "Output file write error. File: "+filename
		sys.exit(2)
	
	f.write( """#include "BaseProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BaseProcessing",
		"category", "Plugins",
		"description", "BaseProcessing",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BaseProcessing> reg = metadata;
}

}
""")
	f.close()
#make_base_processing_cxx_file()

def make_sconstruct_file(definitions_dict):
	""" Standard SConstruct file """

	try:
		filename = "templates/%s/SConstruct"%(definitions_dict["template_name"])
		f = open(filename,"w")
	except:
		print "Output file write error. File: "+filename
		sys.exit(2)

	f.write("""#! /usr/bin/python

import os, glob, sys

libraryName='baseprocessing'

def scanFiles(pattern, paths) :
	files = []
	for path in paths :
		files+=glob.glob(os.path.join(path,pattern))
	return files

def recursiveDirs(root) :
	return filter( (lambda a : a.rfind( ".svn")==-1 ),  [ a[0] for a in os.walk(root)]  )

options = Options('options.cache', ARGUMENTS)
options.Add(PathOption('clam_prefix', 'The prefix where CLAM was installed', '/bad/path'))
if sys.platform=='linux2' :
	options.Add(BoolOption('crossmingw', 'Activates the MinGW Windows crosscompiling mode', 'no'))

env = Environment(ENV=os.environ, options=options)
options.Save('options.cache', env)
Help(options.GenerateHelpText(env))
env.SConsignFile() # Single signature file

CLAMInstallDir = env['clam_prefix']
clam_sconstoolspath = os.path.join(CLAMInstallDir,'share','clam','sconstools')
if env['crossmingw'] :
	env.Tool('crossmingw', toolpath=[clam_sconstoolspath])
env.Tool('clam', toolpath=[clam_sconstoolspath])
env.EnableClamModules([
	'clam_core',
	'clam_audioio',
	'clam_processing',
	] , CLAMInstallDir)

sourcePaths = recursiveDirs(".")
sources = scanFiles('*.cxx', sourcePaths)
sources = dict.fromkeys(sources).keys()
if sys.platform=='linux2' :
	env.Append( CCFLAGS=['-g','-O3','-Wall'] )
libraries = [
	env.SharedLibrary(target=libraryName, source = sources),
	]

install = env.Install(os.path.join(CLAMInstallDir,'lib','clam'), libraries)

env.Alias('install', install)
env.Default(libraries)
""")
	f.close()
#make_sconstruct_file()

def main(args): 
	""" Generates a template for clam plugins from a config file
	
	Example of input file (template file):
	
	Name:TestTemplate
	BaseClass:Processing
	i:AudioInPort,Audio Input Name 1
	i:AudioInPort,AudioInput Name 2
	o:AudioOutPort,Audio Output Name
	ic:0,10,Input Control Name
	oc:0,0,Output Control Name
	
	Convention:
	i (input) | o (output): type,Name
	ic (input control) | oc (output control): integer/float, range
	"""

	if len(sys.argv) < 2:
		print "\nBad amount of input arguments.\n", Usage, "\n"
		sys.exit(1)

	try:
		f = open( "./" + sys.argv[1], 'r' )
	except IOError:
		print "Template file read error."
		sys.exit(2)

	definitions_dict = {}
	definitions_dict["inputs"] = list()
	definitions_dict["outputs"] = list()
	definitions_dict["incontrols"] = list()
	definitions_dict["outcontrols"] = list()

	# Style definitions:
#	definitions_dict["member_style"] = "m"
	definitions_dict["member_style"] = "_"

	filedata = f.read(); f.close()
	for elem in filedata.splitlines():
		if elem!='' and elem[0]!='#':
			(item, value) = elem.split(':')
			if   item=="Name": definitions_dict["template_name"] = value 
			elif item=="BaseClass": definitions_dict["base_class_name"] = value
			elif item=='i': definitions_dict["inputs"].append( value.split(',') )
			elif item=='o': definitions_dict["outputs"].append( value.split(',') )
			elif item=='ic': definitions_dict["incontrols"].append( value.split(',') )
			elif item=='oc': definitions_dict["outcontrols"].append( value.split(',') )

	if not os.path.isdir( "templates/" + definitions_dict["template_name"] ): os.mkdir( "templates/" + definitions_dict["template_name"] )

	print "Creating " + definitions_dict["template_name"] + " template"
	make_base_processing_hxx_file(definitions_dict)
	make_base_processing_cxx_file(definitions_dict)
	make_sconstruct_file(definitions_dict)
#main()


if __name__ == '__main__':
	main(sys.argv)
