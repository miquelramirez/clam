#!/usr/bin/python
# -*- coding: UTF-8 -*-


# Usage: ./TemplateGenerator TEMPLATEFILE

#Example of input file (template file):
#Name:Test
#BaseClass:Processing
#i:AudioInPort
#i:AudioInPort
#o:AudioOutPort
#ic:i,0,100
#oc:f,0,100
#
# Convention:
# i (input) | o (output): type
# ic (input control) | oc (output control): integer/float, range


# TODO:
#* Add controls range
#* pyqt or NetworkEditor GUI instead of the input file
#* Add config options
#* Define ports and controls names through the input file (i:AudioInPort, Audio Input)


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

inputs	    = []
outputs     = []
incontrols  = []
outcontrols = []

temp = f.read()
f.close()
for elem in temp.split('\n'):
	if elem!='' and elem[0]!='#':
		(item, value) = elem.split(':')
		if   item=="Name":		plugin_name = value 
		elif item=="BaseClass":	base_class_name = value
		elif item=='i': 		inputs.append( value )
		elif item=='o': 		outputs.append( value )
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
for class_name in sets.Set(inputs+outputs):
	g.write( add_include(class_name) )
	#print class_name
if len(incontrols)>0:
	g.write( add_include("InControl") )


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
for i in range(len(inputs)):
	g.write( "\t\t" + inputs[i] + " " + member_style + "Input" + inputs[i].title() + str(i) + ";\n" )
for i in range(len(outputs)):
	g.write( "\t\t" + outputs[i] + " " + member_style + "Output" + outputs[i].title() + str(i) + ";\n" )

#Controls
g.write( "\n\t\t/** Controls **/\n" )
for i in range(len(incontrols)):
	g.write( "\t\tInControl " + member_style + "InputControl" + str(i) + ";\n" )


g.write( "\n\tpublic:\n" )

#Constructor
g.write( "\t\tBaseProcessing(const Config & config=Config())\n\t\t\t:\n")
for i in range(len(inputs)):
	g.write( "\t\t\t" + member_style + "Input" + inputs[i].title() + str(i) + "(\"Input " + str(i) + "\", this),\n" )
for i in range(len(outputs)):
	g.write( "\t\t\t" + member_style + "Output" + outputs[i].title() + str(i) + "(\"Output " + str(i) + "\", this),\n" )
for i in range(len(incontrols)):
	g.write( "\t\t\t" + member_style + "InputControl" + str(i) + "(\"Control " + str(i) + "\", this)" )
	if i==len(incontrols)-1:
		g.write("\n")
	else:
		g.write(",\n")
	
g.write( "\
		{\n\
			Configure( config );\n\
		}\n" )

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
