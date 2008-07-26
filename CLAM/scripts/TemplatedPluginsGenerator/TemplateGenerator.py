#!/usr/bin/python
# -*- coding: UTF-8 -*-

# Usage: ./TemplateGenerator TEMPLATEFILE

# Example of input file (template file):
# Name:TestTemplate
# BaseClass:Processing
# i:AudioInPort,Audio Input Name 1
# i:AudioInPort,AudioInput Name 2
# o:AudioOutPort,Audio Output Name
# ic:0,10,Input Control Name
# oc:0,0,Output Control Name
#
# Convention:
# i (input) | o (output): type,Name
# ic (input control) | oc (output control): integer/float, range


# TODO:
#* pyqt or NetworkEditor GUI instead of the input file
#* Add config options


import os, sys, shutil
import sets

# Style definitions:
#member_style = "m"
member_style = "_"

if len(sys.argv) < 2:
	print """
	Bad amount of input arguments.

	Usage: TemplateGenerator TEMPLATEFILE
	"""
	sys.exit(1)

try:
	f = open( "./" + sys.argv[1], 'r' )
except IOError:
	print "Template file read error."
	sys.exit(2)

def add_include( class_name ):
	return "#include <CLAM/" + class_name + ".hxx>\n"
#add_include()

inputs	     = []
outputs      = []
incontrols   = []
outcontrols  = []

filedata = f.read()
f.close()
for elem in filedata.splitlines():
	if elem!='' and elem[0]!='#':
		(item, value) = elem.split(':')
		if   item=="Name":		plugin_name = value 
		elif item=="BaseClass":	base_class_name = value
		elif item=='i':			inputs.append( value.split(',') )
		elif item=='o': 		outputs.append( value.split(',') )
		elif item=='ic': 		incontrols.append( value.split(',') )
		elif item=='oc': 		outcontrols.append( value.split(',') )

if not os.path.isdir( "templates/" + plugin_name ):
	os.mkdir( "templates/" + plugin_name )

print "Creating " + plugin_name + " template"

# BaseProcessing.hxx file
try:
	filename = "templates/%s/BaseProcessing.hxx"%(plugin_name)
	f = open(filename, "w")
except:
	print "Output file write error. File: "+filename
	sys.exit(2)

f.write( "#ifndef _BaseProcessing_\n" )
f.write( "#define _BaseProcessing_\n\n" )

#Includes
f.write( add_include(base_class_name) )
available_port_types_set = sets.Set( (item[0] for item in inputs+outputs) )
for class_name in available_port_types_set: f.write( add_include(class_name) )
if len(incontrols)>0: f.write( add_include("InControl") )
if len(outcontrols)>0: f.write( add_include("OutControl") )

f.write("""
namespace CLAM {

""")

#Class
f.write("""	/**	\\brief Short description
	 *
	 *	Description
	 */
	class BaseProcessing: public %s
	{	
"""%(base_class_name))
	
#GetClassName
f.write("""		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const { return \"BaseProcessing\"; }

""")

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
f.write( "\t\tBaseProcessing(const Config & config=Config())\n\t\t\t:\n")
members_list = []
for port_type,port_name in (inputs+outputs):
	members_list.append( "\t\t\t" + member_style + port_name.replace(' ', '') + "(\"" + port_name + "\", this)" )
for cmin,cmax,control_name in (incontrols+outcontrols):
	members_list.append( "\t\t\t" + member_style + control_name.replace(' ', '') + "(\"" + control_name + "\", this)" )

for i in range(len(members_list)-1):
	f.write( members_list[i] + ",\n" )
f.write( members_list[len(members_list)-1] )
	
f.write( """
		{
			Configure( config );

""")

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
f.write("""
		bool Do()
		{
			return true;
		}""")


f.write( "\n\tprivate:\n" )

f.write( "\n\t/** Child processings **/\n\n" )

f.write( "\t};\n\n" )

f.write( "};//namespace CLAM\n\n" )
f.write( "#endif // _BaseProcessing_\n\n" )
f.close()


# BaseProcessing.cxx file
try:
	filename = "templates/%s/BaseProcessing.cxx"%(plugin_name)
	f = open(filename, "w")
except:
	print "Output file write error. File: "+filename
	sys.exit(2)

f.write( """
#include "BaseProcessing.hxx"
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

# Standard SConstruct file
try:
	filename = "templates/%s/SConstruct"%(plugin_name)
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
