#!/usr/bin/python
# -*- coding: utf-8 -*-

_copyright = u"""\
Copyright (C) 2010, Fundacio Barcelona Media.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Licence GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl2.html>

Written by Xavier Oliver, David Garcia Garzon and Angelo Scorza.
"""

_description = u"""\
This script generates different files needed to build LV2 plugin bundles
based on the information provided by a set of CLAM networks.
The script can be used in three modes: 'manifest' to generate the main
metadata index for the bundle, 'ttls' to generate a set of ttl's
for each plugin in the buncle, and, 'main' to generate the main
source file of the library.
If you are using SCons, clam provides builders to generate all those
files. See the LV2 clam example for more information on how to use it.
"""


from xml.sax import make_parser
from xml.sax.handler import ContentHandler
import sys, os
import getopt

network_test = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.1" id="Unnamed">

  <description>&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name="qrichtext" content="1" /&gt;&lt;style type="text/css"&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;"&gt;
&lt;p style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'DejaVu Sans'; font-size:11pt;"&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</description>

  <processing id="Input" position="151,180" size="128,108" type="AudioSource">
    <NSources>2</NSources>
  </processing>

  <processing id="Output" position="370,183" size="128,111" type="AudioSink">
    <NSinks>2</NSinks>
  </processing>

  <port_connection>
    <out>Input.1</out>
    <in>Output.1</in>
  </port_connection>

  <port_connection>
    <out>Input.2</out>
    <in>Output.2</in>
  </port_connection>

</network>

"""

class AudioPort():
	def __init__ (self, id, pos):
		self.pos      = pos
		self.name     = id
		self.numPorts = ""

	def setNumPorts(self,theNumPorts):
		self.numPorts = theNumPorts
	def getNumPorts(self):
		# if not specified by default one port
		return 1 if not self.numPorts else int(self.numPorts)

	def getSymbol(self) :
		return self.name.strip().replace(" ","_")
	def getName(self) :
		return self.name.strip().capitalize()

	def ttl(self, port_type, firstIndex) :
		return [ """\
	[
		a lv2:%(port_type)sPort, lv2:AudioPort;
		lv2:index %(index)s;
		lv2:symbol "%(symbol)s";
		lv2:name "%(name)s";
	]""" % dict(
			port_type = port_type,
			index = firstIndex+i,
			symbol = self.getSymbol()+"_%s"%(i+1),
			name = self.getName()+"_%s"%(i+1),
		)
		for i in range(self.getNumPorts()) ]

class ControlPort(AudioPort):
	def __init__ (self, id, pos):
		AudioPort.__init__(self, id, pos)
		self.MinValue = ""
		self.MaxValue = ""
		self.DefaultValue = ""

	def ttl(self, port_type, firstIndex) :
		return [ """\
	[
		a lv2:%(port_type)sPort, lv2:ControlPort;
		lv2:index %(index)s;
		lv2:symbol "%(symbol)s";
		lv2:name "%(name)s";
		lv2:default %(default)s;
		lv2:minimum %(minimum)s;
		lv2:maximum %(maximum)s;
	]""" % dict(
			port_type = port_type,
			index = firstIndex,
			symbol = self.getSymbol(),
			name = self.getName(),
			minimum = self.MinValue,
			maximum = self.MaxValue,
			default = self.DefaultValue,
		) ]

class ExporterHandler(ContentHandler):
 	def __init__ (self):
		self.label = ""
		self.audioAux = None	
		self.inputAudio = []
		self.outputAudio = []
		self.inputControl = []
		self.outputControl = []
		self.connections = dict(
			ControlSink = self.outputControl,
			ControlSource = self.inputControl,
			AudioSink = self.outputAudio,
			AudioBufferSink = self.outputAudio,
			AudioSource = self.inputAudio,
			AudioBufferSource = self.inputAudio,
			)

	def startElement(self, name, attrs):
		self.label = name
		if name == "processing":
			type = attrs.get('type')
			id = attrs.get('id')
			x,y = attrs.get('position').split(',')
			if type in ('AudioSink', 'AudioSource', 'AudioBufferSink', 'AudioBufferSource') :
				self.audioAux = AudioPort(id,float(y))
				self.audioAux.direction = "Output" if type=="AudioSink" else "Input"
			elif type in ('ControlSink', 'ControlSource') :
				self.audioAux = ControlPort(id,float(x))
				self.audioAux.direction = "Output" if type=="AudioSink" else "Input"
			else :
				self.audioAux = None
				return
			self.audioAux.type = type

	def characters (self, content):
		if self.audioAux is None : return
		if self.label == "processing" : return
		self.audioAux.__dict__[self.label] = self.audioAux.__dict__.get(self.label,"")+content
		if self.label in ("NSinks", "NSources") :
			self.audioAux.setNumPorts( self.audioAux.numPorts + content )

	def endElement(self, name):
		self.label = ""
		if name != "processing": return
		if self.audioAux is None : return
		if self.audioAux.type in ("ControlSource", "ControlSink") :
			if self.audioAux.DefaultValue == "" :
				self.audioAux.DefaultValue = str(
					(float(self.audioAux.MaxValue)-float(self.audioAux.MinValue))/2 
					+ float(self.audioAux.MinValue))
		self.connections[self.audioAux.type].append(self.audioAux)
		self.audioAux = None

	def printTTL(self,clamnetwork,uri,name,doapfile,binary):
	
		if binary==None:
			binary="clam_lv2_example"
	
		doapDescription = """\
	doap:license <http://usefulinc.com/doap/licenses/gpl>;
	doap:developer [
		foaf:name "clam-project";
		foaf:homepage <http://clam-project.org> ;
		foaf:mbox <mailto:clam-devel@lists.clam-project.org> ;
	] ;
	doap:maintainer [
		foaf:name "clam-project";
		foaf:homepage <http://clam-project.org> ;
		foaf:mbox <mailto:clam-devel@lists.clam-project.org> ;
	] ;
"""
		fileWithDoap = None

		if doapfile != None :
			try:
				fileWithDoap = open(doapfile)
			except IOError:
			        print >>sys.stderr, 'cannot open', doapfile
				fileWithDoap = None
			else:
				doapDescription = fileWithDoap.read()

		#TODO change the static PATH for Dynamic PATH
		f = sys.stdout
		f.write("""\
@prefix lv2:  <http://lv2plug.in/ns/lv2core#>.
@prefix doap: <http://usefulinc.com/ns/doap#>.
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix rdfs:  <http://www.w3.org/2000/01/rdf-schema#> .
@prefix units: <http://lv2plug.in/ns/extension/units#> .
@prefix uiext: <http://lv2plug.in/ns/extensions/ui#> .

<%(uri)s/gui>
        a uiext:GtkUI;
        uiext:binary <%(binary)s.so>;
.

<%(uri)s>
	a lv2:Plugin;
	lv2:binary <%(binary)s.so>;
	doap:name "%(name)s";
%(doapDescription)s
	lv2:optionalFeature lv2:hardRtCapable ;
	uiext:ui <%(uri)s/gui> ;
	lv2:port
""" %dict(
			uri=uri,
			binary=binary,
			name=name,
			doapDescription=doapDescription
			))

		ports = []
		for port in sorted(self.inputControl, key=lambda p:p.pos) :
			ports += port.ttl("Input",len(ports))
		for port in sorted(self.outputControl, key=lambda p:p.pos) :
			ports += port.ttl("Output",len(ports))
		for port in sorted(self.inputAudio, key=lambda p:p.pos) :
			ports += port.ttl("Input", len(ports))
		for port in sorted(self.outputAudio, key=lambda p:p.pos) :
			ports += port.ttl("Output", len(ports))
		f.write(",\n".join(ports) + ".\n")
		f.close()
		
		if fileWithDoap!= None:
			fileWithDoap.close()

def printCLAM_PLUGIN(clamnetworks,uris):
	f = sys.stdout
	source = """\
#include <CLAM/LV2NetworkExporter.hxx>
#include <CLAM/LV2Library.hxx>
#include <CLAM/EmbeddedFile.hxx>
#include <iostream>

%(embedded_files)s

extern "C"
const LV2_Descriptor * lv2_descriptor(uint32_t index)
{
	static LV2Library library;

%(static_exporters)s

	return library.pluginAt(index);
}
"""%dict(
		embedded_files = "".join( (
			'CLAM_EMBEDDED_FILE( network_%i, "%s" );\n'%(i,clamnetworks)
			for i, clamnetworks in enumerate(clamnetworks) )),
		static_exporters = "".join( (
			'\tstatic LV2NetworkExporter n%i(library, network_%i, "%s");\n'%(i,i,uri)
			for i, uri in enumerate(uris) )),
	)
	f.write(source)


def printManifest(uris,names):
	f = sys.stdout
	f.write("""\
@prefix lv2:  <http://lv2plug.in/ns/lv2core#>.
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#>.

"""+"".join(["""\
<%s>
	a lv2:Plugin;
	rdfs:seeAlso <%s.ttl>	.

"""%(uri, name) for uri, name in zip(uris, names)]))


import os

def test_back2back() :
	from audiob2b import runBack2BackProgram
	data_path="../../../clam-test-data/b2b/lv2_plugin/"
	f = open('test_othercable.clamnetwork','w')
	f.write(network_test)	
	f.close()
	back2BackTests = [
		("simple_ttl",
		"clam_lv2_generator.py --ttl -u %(uri)s %(network)s > %(target)s" 
			% dict(   uri="http://clam-project.org/examples/lv2"
				, network="test_othercable.clamnetwork"
				, target="output.ttl")
		, [
			"output.ttl",
		]),
	]
	runBack2BackProgram(data_path, sys.argv, back2BackTests)
	os.remove('test_othercable.clamnetwork')


def main():
	from optparse import OptionParser
	parser = OptionParser(
		usage="usage: %prog [options] network1 network2...",
		version="%prog 1.4\n"+_copyright,
		description=_description
		)
	parser.add_option("-m", "--manifest", dest='createManifest', action='store_true',
		help="Generates the manifest file")
	parser.add_option("-t", "--ttl", dest='createTtls', action='store_true',
		help="Generates ttl files for each network")
	parser.add_option("-i", "--main", dest='createMain', action='store_true',
		help="Generates the main C++ file for the plugin library")
	parser.add_option("-u", "--uribase", dest='uribase', default="default/uri/lv2/",
		help="Specifies the uri base for the plugins", metavar="URIBASE")
	parser.add_option("-d", "--doap", dest='doapfile',
		help="Specifies a doapfile with additional info (required by --ttls)", metavar="DOAPFILE")
	parser.add_option("-y", "--binary", dest='binary', action="store_true",
		help="Provides a name for the library binary (required by --ttls)")
	parser.add_option("-b", "--back2back", dest='back2back', action="store_true",
		help="Runs the script back-to-back test")
	options, args = parser.parse_args()

	options, args = parser.parse_args()

	if options.back2back :
		test_back2back()
		sys.exit(0)

	networks = args
	names = [os.path.splitext(os.path.basename(network))[0] for network in networks]
	uris  = [os.path.join(options.uribase,name) for name in names ]

	if options.createTtls:
		for network, uri, name in zip(networks, uris, names) :
			parser = make_parser()   
			curHandler = ExporterHandler()
			parser.setContentHandler(curHandler)
			parser.parse(open(network))
			curHandler.printTTL(network,uri,name,options.doapfile,options.binary)
			return

	if options.createManifest:
		printManifest(uris,names)
		return

	if options.createMain:
		printCLAM_PLUGIN(networks,uris)
		return

if __name__ == '__main__' :
	main()

