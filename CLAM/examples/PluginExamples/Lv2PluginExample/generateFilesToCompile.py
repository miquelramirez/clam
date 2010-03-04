#!/usr/bin/python

from xml.sax import make_parser
from xml.sax.handler import ContentHandler
import sys

class ControlPort():
	def __init__ (self):
		self.name     = ""
		self.units    = ""
		self.minimum  =	""
		self.maximum  =	""
		self.default  =	""
	def setMaximum(self,theMaximum):
		self.maximum = theMaximum
	def setMinimum(self,theMinimum):
		self.minimum = theMinimum
	def setDefault(self,theDefault):
		self.default = theDefault
	def setName(self,theName):
		self.name = theName
	def setUnits(self,theUnits):
		self.units = theUnits

class AudioPort():
	
	def __init__ (self):
		self.name     = ""
		self.numPorts = 0

	def getName(self):
		return self.name
	def setName(self,theName):
		self.name = theName
	def setNumPorts(self,theNumPorts):
		self.numPorts = int(theNumPorts)

class ExporterHandler(ContentHandler):
 	
 	def __init__ (self):
		self.inputAudio = []
		self.outputAudio = []
		self.inputControl = []
		self.outputControl = []
		self.flagInAudio = 0;
		self.flagOutAudio = 0;
		self.flagInControl = 0;
		self.flagOutControl = 0;
		self.label = ""
		self.audioAux = None	
		self.controlAux = None

	def startElement(self, name, attrs):
		self.label = name
		if name == "processing":
			if attrs.get('type') == 'AudioSink':
				self.flagOutAudio = 1
				self.audioAux = AudioPort()
				self.audioAux.setName(attrs.get('id'))				
			elif attrs.get('type') == 'AudioSource':
				self.flagInAudio = 1
				self.audioAux = AudioPort()
				self.audioAux.setName(attrs.get('id'))
			elif attrs.get('type') == 'ControlSink':
				self.flagOutControl = 1
				self.controlAux = ControlPort()
				self.controlAux.setName(attrs.get('id'))
			elif attrs.get('type') == 'ControlSource':
				self.flagInControl = 1
				self.controlAux = ControlPort()
				self.controlAux.setName(attrs.get('id'))
	
	def characters (self, ch):
		if self.label == "NSinks" or self.label == "NSources":
			self.audioAux.setNumPorts( self.audioAux.numPorts + int(ch) )
		elif self.label == "MinValue":
			self.controlAux.setMinimum( self.controlAux.minimum + ch )
		elif self.label == "MaxValue":
			self.controlAux.setMaximum( self.controlAux.maximum + ch )
		elif self.label == "DefaultValue":
			self.controlAux.setDefault( self.controlAux.default + ch )
		elif self.label == "UnitName":
			self.controlAux.setUnits( self.controlAux.units + ch )
		

	def endElement(self, name):
		self.label = ""
		if name == "processing":
			if self.flagOutAudio:
				self.outputAudio.append(self.audioAux)
				self.flagOutAudio = 0
				self.audioAux = None
			elif self.flagInAudio:
				self.inputAudio.append(self.audioAux)
				self.flagInAudio = 0
				self.audioAux = None
			elif self.flagOutControl:
				if self.controlAux.default == "":
					self.controlAux.setDefault(repr(((float(self.controlAux.maximum)-float(self.controlAux.minimum))/2)+float(self.controlAux.minimum)))
				self.outputControl.append(self.controlAux)
				self.flagOutControl = 0
				self.controlAux = None
			elif self.flagInControl:
				if self.controlAux.default == "":
					self.controlAux.setDefault(repr(((float(self.controlAux.maximum)-float(self.controlAux.minimum))/2)+float(self.controlAux.minimum)))
				self.inputControl.append(self.controlAux)
				self.flagInControl = 0
				self.controlAux = None
	
	def numTotalPorts(self):
		numTotalPorts = 0
		for inAudio in self.inputAudio:
			numTotalPorts += int(inAudio.numPorts)
		for outAudio in self.outputAudio:
			numTotalPorts += int(outAudio.numPorts)			
		numTotalPorts +=len(self.inputControl)+len(self.outputControl)
		return numTotalPorts
	
	def printTTL(self,clamnetwork,uri,name):

#TODO change the static PATH for Dynamic PATH
		f = open(name.lower()+'.ttl', 'w')
		f.write('@prefix lv2:  <http://lv2plug.in/ns/lv2core#>.\n')
		f.write('@prefix doap: <http://usefulinc.com/ns/doap#>.\n\n')
		f.write('<'+uri+'>\n')
		f.write('\ta lv2:Plugin;\n')
		f.write('\tlv2:binary <'+name.capitalize()+'.so>;\n')
  		f.write('\tdoap:name "'+name.capitalize()+'";\n')
  		f.write('\tdoap:license <http://usefulinc.com/doap/licenses/gpl>;\n\n')
  		
		f.write('\tlv2:port\n')
		
				
		index =0
		for inControl in self.inputControl:
			f.write('\t[\n')
			f.write('\t\ta lv2:ControlPort, lv2:InputPort;\n')
			f.write('\t\tlv2:index '+repr(index)+';\n')
			f.write('\t\tlv2:symbol "'+inControl.name.strip().replace(" ","_")+'";\n')
			f.write('\t\tlv2:name "'+inControl.name.strip().capitalize()+'";\n')
			f.write('\t\tlv2:default '+inControl.default+';\n')
			f.write('\t\tlv2:minimum '+inControl.minimum+';\n')
			f.write('\t\tlv2:maximum '+inControl.maximum+';\n')
			index = index+1
			if index == self.numTotalPorts():
				f.write('\t].\n')
			else:	
				f.write('\t],\n')		
		for outControl in self.outputControl:
			f.write('\t[\n')
			f.write('\t\ta lv2:ControlPort, lv2:OutputPort;\n')
			f.write('\t\tlv2:index '+repr(index)+';\n')
			f.write('\t\tlv2:symbol "'+outControl.name.strip().replace(" ","_")+'";\n')
			f.write('\t\tlv2:name "'+outControl.name.strip().capitalize()+'";\n')
			f.write('\t\tlv2:default '+outControl.default+';\n')
			f.write('\t\tlv2:minimum '+outControl.minimum+';\n')
			f.write('\t\tlv2:maximum '+outControl.maximum+';\n')
			index = index+1
			if index == self.numTotalPorts():
				f.write('\t].\n')
			else:	
				f.write('\t],\n')		
		for inAudio in self.inputAudio:
			for i in range(1,int(inAudio.numPorts)+1):
				f.write('\t[\n')
				f.write('\t\ta lv2:AudioPort, lv2:InputPort;\n')
				f.write('\t\tlv2:index '+repr(index)+';\n')
				f.write('\t\tlv2:symbol "'+(inAudio.name+"_"+repr(i)).strip().replace(" ","_")+'";\n')
				f.write('\t\tlv2:name "'+(inAudio.name+"_"+repr(i)).strip().capitalize()+'";\n')
				index = index+1
				if index == self.numTotalPorts():
					f.write('\t].\n')
				else:	
					f.write('\t],\n')	
				
		for outAudio in self.outputAudio:
			for i in range(1,int(outAudio.numPorts)+1):
				f.write('\t[\n')
				f.write('\t\ta lv2:AudioPort, lv2:OutputPort;\n')
				f.write('\t\tlv2:index '+repr(index)+';\n')
				f.write('\t\tlv2:symbol "'+(outAudio.name+"_"+repr(i)).strip().replace(" ","_")+'";\n')
				f.write('\t\tlv2:name "'+(outAudio.name+"_"+repr(i)).strip().capitalize()+'";\n')
				index = index+1
				if index == self.numTotalPorts():
					f.write('\t].\n')
				else:	
					f.write('\t],\n')	
		f.close()


def printCLAM_PLUGIN(clamnetworks,uris):
	f = open('clam_plugin.cxx', 'w')
	f.write('#include "LV2NetworkExporter.hxx"\n')
	f.write('#include "LV2Library.hxx"\n')
	f.write('#include <CLAM/EmbeddedFile.hxx>\n')	
	f.write('#include <iostream>\n\n')
	
	i=0
	for clamnetwork in clamnetworks:
		f.write('CLAM_EMBEDDED_FILE( network'+repr(i)+' ,"'+clamnetwork+'");\n')
		i=i+1
	
	f.write('\nextern "C" const LV2_Descriptor * lv2_descriptor(uint32_t index)\n')
	f.write('{\n')
	f.write('\tstatic LV2Library library;\n')

	i=0
	for uri in uris:
		f.write('\tstatic LV2NetworkExporter n'+repr(i)+'(library,network'+repr(i)+',"'+uri+'");\n')
		i=i+1
	
	f.write('\treturn library.pluginAt(index);\n')
	f.write('}\n')
	f.close()

def openConfigFile():
	networks = []
	uris = []
	names = []
	f = open('network_and_URI.config',"r")
	lines = f.readlines()
	bundle = lines[0].strip()
	for line in lines[1:]:
		network,uri,name = line.split("\t")
		network = network.strip()
		uri = uri.strip()
		name = name.strip()
		uris.append(uri)
		networks.append(network)
		names.append(name)

	f.close()
	return bundle,networks,uris,names


def printMakeFile(bundle, names):
	f = open('Makefile', 'w')
	f.write("BUNDLE = %s.lv2\n"%bundle)
	f.write("INSTALL_DIR = /usr/local/lib/lv2\n\n\n")
	f.write("$(BUNDLE): manifest.ttl ")
	for name in names:
		f.write(name.lower()+".ttl ")
	f.write(name.lower().capitalize()+".so\n")
	f.write("\trm -rf $(BUNDLE)\n")
	f.write("\tmkdir $(BUNDLE)\n")
	f.write("\tcp manifest.ttl ")
	for name in names:
		f.write(name.lower()+".ttl ")
	f.write(name.lower().capitalize()+".so $(BUNDLE)\n\n\n")

	f.write(name.lower().capitalize()+".so: LV2NetworkPlayer.cxx clam_plugin.cxx LV2NetworkExporter.cxx\n")
	f.write("\tg++ -shared -fPIC -DPIC clam_plugin.cxx LV2NetworkExporter.cxx LV2NetworkPlayer.cxx `pkg-config --cflags --libs lv2core clam_core clam_processing clam_audioio` -o "+name.lower().capitalize()+".so  -l asound\n\n")
	f.write("install: $(BUNDLE)\n\tmkdir -p $(INSTALL_DIR)\n\trm -rf $(INSTALL_DIR)/$(BUNDLE)\n\tcp -R $(BUNDLE) $(INSTALL_DIR)\n\n")
	f.write("clean:\n\trm -rf $(BUNDLE) "+ name.lower().capitalize()+".so ")
	for name in names:
		f.write(name.lower()+".ttl ")
	f.write(" clam_plugin.cxx Makefile\n")

	f.close()


def printManifest(uris,names):
	f = open('manifest.ttl', 'w')
	f.write('@prefix lv2:  <http://lv2plug.in/ns/lv2core#>.\n')
	f.write('@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#>.\n\n')

	for i in range(0,len(uris)):
		f.write("<"+uris[i]+">\n\t a lv2:Plugin;\n")
		f.write("\trdfs:seeAlso <"+names[i]+".ttl>.\n\n")
		
	f.close()

def main():
	

	bundle,clamnetworks,uris,names = openConfigFile()

	printCLAM_PLUGIN(clamnetworks,uris)

	for i in range(0,len(clamnetworks)):
		parser = make_parser()   
		curHandler = ExporterHandler()
		parser.setContentHandler(curHandler)
		parser.parse(open(clamnetworks[i]))
		curHandler.printTTL(clamnetworks[i],uris[i],names[i])
	
	printManifest(uris,names)
	printMakeFile(bundle, names)



if __name__ == '__main__' :
	main()	
