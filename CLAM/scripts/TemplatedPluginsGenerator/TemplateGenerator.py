#!/usr/bin/python
# -*- coding: UTF-8 -*-


# Usage: ./TemplateGenerator TEMPLATEFILE

#Example of input file (template file):
#Name:TestTemplate
#BaseClass:Processing
#i:AudioInPort,Audio Input Name 1
#i:AudioInPort,AudioInput Name 2
#o:AudioOutPort,Audio Output Name
#ic:0,10,Input Control Name
#oc:0,0,Output Control Name
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
member_style = "m"
#member_style = "_"

if len(sys.argv) < 2:
	print "Bad amount of input arguments"
	print "Usage: TemplateGenerator TEMPLATEFILE"
	sys.exit(1)

try:
	f = open( "./" + sys.argv[1], 'r' )
except IOError:
	print "Template file read error."
	sys.exit(2)

def add_include( class_name ):
	return "#include <CLAM/" + class_name + ".hxx>\n"

inputs	     = []
outputs      = []
incontrols   = []
outcontrols  = []

temp = f.read()
f.close()
for elem in temp.split('\n'):
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

# BaseProcessing.hxx
try:
	g = open( "templates/" + plugin_name + "/BaseProcessing.hxx", "w" )
	print "Creating " + plugin_name + " template"
except:
	print "tOutput file write error. File: templates/" + plugin_name + "/BaseProcessing.hxx"
	sys.exit(2)

g.write( "#ifndef _BaseProcessing_\n" )
g.write( "#define _BaseProcessing_\n\n" )

#Includes
g.write( add_include(base_class_name) )
available_port_types_set = sets.Set( (item[0] for item in inputs+outputs) )
for class_name in available_port_types_set: g.write( add_include(class_name) )
if len(incontrols)>0: g.write( add_include("InControl") )
if len(outcontrols)>0: g.write( add_include("OutControl") )

g.write( "\nnamespace CLAM {\n\n" )

#Class
g.write( "\t/**	\\brief Short description\n\
	 *\n\
	 *	Description\n\
	 */\n\
	class BaseProcessing: public " + base_class_name + "\n\
	{	\n")
	
#GetClassName
g.write( "\t\t/** This method returns the name of the object\n\
		 *  @return Char pointer with the name of object\n\
		 */\n\
		const char *GetClassName() const { return \"BaseProcessing\"; }\n\n")

#Ports
g.write( "\t\t/** Ports **/\n" )
for port_type,port_name in (inputs+outputs):
	g.write( "\t\t" + port_type + " " + member_style + port_name.replace(' ', '') + ";\n" )

#Controls
g.write( "\n\t\t/** Controls **/\n" )
for cmin,cmax,control_name in incontrols:
	g.write( "\t\tInControl " + member_style + control_name.replace(' ', '') + ";\n" )
for cmin,cmax,control_name in outcontrols:
	g.write( "\t\tOutControl " + member_style + control_name.replace(' ', '') + ";\n" )

g.write( "\n\tpublic:\n" )

#Constructor
g.write( "\t\tBaseProcessing(const Config & config=Config())\n\t\t\t:\n")
members_list = []
for port_type,port_name in (inputs+outputs):
	members_list.append( "\t\t\t" + member_style + port_name.replace(' ', '') + "(\"" + port_name + "\", this)" )
for cmin,cmax,control_name in (incontrols+outcontrols):
	members_list.append( "\t\t\t" + member_style + control_name.replace(' ', '') + "(\"" + control_name + "\", this)" )

for i in range(len(members_list)-1):
	g.write( members_list[i] + ",\n" )
g.write( members_list[len(members_list)-1] + "\n" )
	
g.write( "\
		{\n\
			Configure( config );\n\n")

#InControls SetBounds, SetDefaultValue, DoControl
for cmin,cmax,control_name in (incontrols):
	control_media = round((float(cmin)+float(cmax))/2.)
	g.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".SetBounds(" + cmin + "," + cmax + ");\n" )
	g.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".SetDefaultValue(" + str(control_media) + ");\n" )
	g.write( "\t\t\t" +  member_style + control_name.replace(' ', '') + ".DoControl(" + str(control_media) + ");\n" )
				
g.write( "\t\t}\n" )


#Destructor
g.write( "\n\t\t~BaseProcessing() {}\n\n" )

#Do()
g.write( "\
		bool Do()\n\
		{\n\
			return true;\n\
		}\n" )


g.write( "\n\tprivate:\n" )

g.write( "\n\t/** Child processings **/\n\n" )

g.write( "\t};\n\n" )

g.write( "};//namespace CLAM\n\n" )
g.write( "#endif // _BaseProcessing_\n" )
g.close()


# BaseProcessing.cxx
try:
	g = open( "templates/" + plugin_name + "/BaseProcessing.cxx", "w" )
	#print "Creating " + plugin_name + " template"
except:
	print "templates/" + plugin_name + "/BaseProcessing.cxx"
	print "Output file write error."
	sys.exit(2)

g.write( "#include \"BaseProcessing.hxx\"\n\
#include <CLAM/ProcessingFactory.hxx>\n\
\n\
namespace CLAM\n\
{\n\
\n\
namespace Hidden\n\
{\n\
	static const char * metadata[] = {\n\
		\"key\", \"BaseProcessing\",\n\
		\"category\", \"Plugins\",\n\
		\"description\", \"BaseProcessing\",\n\
		0\n\
	};\n\
	static FactoryRegistrator<ProcessingFactory, BaseProcessing> reg = metadata;\n\
}\n\
\n\
}\n\
\n")
g.close()
