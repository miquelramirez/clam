#!/usr/bin/python

from xml.sax import make_parser
from xml.sax.handler import ContentHandler
import sys
import getopt

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

	def printTTL(self,clamnetwork,uri,name,doapfile='default.doap'):

		fileWithDoap = open(doapfile)

		#TODO change the static PATH for Dynamic PATH
		f = sys.stdout
		f.write("""\
@prefix lv2:  <http://lv2plug.in/ns/lv2core#>.
@prefix doap: <http://usefulinc.com/ns/doap#>.
@prefix foaf: <http://xmlns.com/foaf/0.1/> .
@prefix rdfs:  <http://www.w3.org/2000/01/rdf-schema#> .
@prefix units: <http://lv2plug.in/ns/extension/units#> .

<%(uri)s>
	a lv2:Plugin;
	lv2:binary <%(binary)s.so>;
	doap:name "%(name)s";
	doap:license <http://usefulinc.com/doap/licenses/gpl>;
%(doapDescription)s
	lv2:optionalFeature lv2:hardRtCapable ;
	lv2:port
""" %dict(
			uri=uri,
			binary="clam_lv2_example",
			#binary=name,
			name=name,
			doapDescription=fileWithDoap.read()
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
		fileWithDoap.close()



def printCLAM_PLUGIN(clamnetworks,uris):
	f = sys.stdout
	source = """\
#include "LV2NetworkExporter.hxx"
#include "LV2Library.hxx"
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

def parseCommandLine() :
	command = sys.argv[1]
	uribase = sys.argv[2]
	networks = sys.argv[3:]
	names = [os.path.splitext(os.path.basename(network))[0]
		for network in networks]
	uris = [os.path.join(uribase,name) for name in names ]
	return command, None, uribase, networks, names, uris

def main():

        args    =  sys.argv[1:]
	networks= []
	names   = []
	uris    = []
	uribase = "default/uri/lv2/"

        try:
                optlist1, args1 = getopt.getopt(args, "mtiu:h",  ["manifest", "ttl", "main","uribase", "help"])
		
        except getopt.error, msg:
                print "[1] for help use --help"
                sys.exit(2)

	createTtls = False
	createMain = False
	createManifest = False

        # process options
        for o, a in optlist1:
                if o in ("-m", "--manifest"):  	# to create the manifest.ttl 
			createManifest = True
	
		if o in ("-t", "--ttl"):       	# to create the ttl for all networks 
			createTtls = True 
		
		if o in ("-i", "--main"):       # to create the main
			createMain = True

		if o in ("-u", "--uribase"):
			uribase = a

                if o in ("-h", "--help"):       # print help
                        print >>sys.stderr, "the help was here"
                        sys.exit(0)

	networks = args1
	names = [os.path.splitext(os.path.basename(network))[0] for network in networks]
	uris  = [os.path.join(uribase,name) for name in names ]

	print >>sys.stderr, names
	print >>sys.stderr, uris
	
	if createTtls:
		for network, uri, name in zip(networks, uris, names) :
			parser = make_parser()   
			curHandler = ExporterHandler()
			parser.setContentHandler(curHandler)
			parser.parse(open(network))
			curHandler.printTTL(network,uri,name)
			return

	if createManifest:
		printManifest(uris,names)
		return

	if createMain:
		printCLAM_PLUGIN(networks,uris)
		return


if __name__ == '__main__' :
	main()

