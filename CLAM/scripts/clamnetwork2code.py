#!/usr/bin/python

# This script generates equivalent code to the load of a given network xml.

# TODO: 
# - scons minimo i ver que linka
# - 


from xml.sax.handler import ContentHandler
from xml.sax import make_parser
import cStringIO
import sys
import re


exampleNetwork = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">

  <description>&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name="qrichtext" content="1" /&gt;&lt;style type="text/css"&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=" font-family:'FreeSans'; font-size:11pt; font-weight:400; font-style:normal;"&gt;
&lt;p style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</description>

  <processing id="AudioSink" position="388,42" size="128,111" type="AudioSink">
    <NSinks>1</NSinks>
  </processing>

  <processing id="AudioSink_1" position="381,188" size="131,111" type="AudioSink">
    <NSinks>1</NSinks>
  </processing>

  <processing id="AudioSink_2" position="347,284" size="131,111" type="AudioSink">
    <NSinks>1</NSinks>
  </processing>

  <processing id="AudioSource" position="53,40" size="132,108" type="AudioSource">
    <NSources>1</NSources>
  </processing>

  <processing id="MonoAudioFileReader" position="48,273" size="185,59" type="MonoAudioFileReader">
    <SourceFile>/home/david.garcia/CajitasDeArena/clam/NetworkEditor/example-data/jaume-voice.mp3</SourceFile>
    <SelectedChannel>0</SelectedChannel>
    <Loop>0</Loop>
  </processing>

  <processing id="Oscilloscope" position="190,130" size="157,123" type="Oscilloscope"/>

  <processing id="Oscilloscope_1" position="202,340" size="172,78" type="Oscilloscope"/>

  <port_connection>
    <out>AudioSource.1</out>
    <in>Oscilloscope.Input</in>
  </port_connection>

  <port_connection>
    <out>AudioSource.1</out>
    <in>AudioSink.1</in>
  </port_connection>

  <port_connection>
    <out>AudioSource.1</out>
    <in>AudioSink_1.1</in>
  </port_connection>

  <port_connection>
    <out>MonoAudioFileReader.Samples Read</out>
    <in>AudioSink_2.1</in>
  </port_connection>

  <port_connection>
    <out>MonoAudioFileReader.Samples Read</out>
    <in>Oscilloscope_1.Input</in>
  </port_connection>

</network>
"""


class NetworkHandler(ContentHandler) :
	def __init__(self) :
		self._result = """\
#include <CLAM/Network.hxx>
#include <sstream>
#include <CLAM/XMLStorage.hxx>

namespace CLAM {

void LoadConfig(ProcessingConfig & config, const std::string & xmlContent)
{
	std::istringstream os(xmlContent);
	CLAM::XMLStorage::Restore(config, os);
}

class NetworkFiller
{
public:
	NetworkFiller(){}

	void setUp(Network &net)
	{
"""
		self._result += "\n\tNetwork network;\n"
		self._currentPath = []
		self._connectionIn = None
		self._connectionOut = None
		self._processingName = None

	def network(self, clamVersion, id) :
		self._result += "\tnetwork.SetName(\"" + id + "\");\n"
#		print "Network:", id, "version", clamVersion

	def description(self) : pass

	def _formatId(self, id) : return re.sub('\W', '_',id.strip())

	def processing(self, id, type, position, size) :
		formattedId = self._formatId(id)
		self._result += '\tProcessing & _%s = network.AddProcessing("%s", "%s");\n'%(
			formattedId, id, type)
		self._result += '\tProcessingConfig * _%s_config = dynamic_cast<ProcessingConfig *>( _%s.GetConfig().DeepCopy()) ;\n'%(
			formattedId, formattedId)
		self._result += '\tLoadConfig(*_%s_config,\n'%formattedId
		self._result += '\t\t"<Configuration>\\n"\n'
		self._processingName = id

	def end_processing(self) :
		formattedId = self._formatId(self._processingName)
		self._result += '\t\t"</Configuration>\\n");\n'
		self._result += '\t_%s.Configure(*_%s_config);\n' % (
			formattedId, formattedId)
		self._result += '\tdelete _%s_config;\n\n' % (formattedId)
		self._processingName = None

	def end_control_connection(self) :
		self._result += "\tnetwork.ConnectControls(\"%s\", \"%s\");\n"%(
			self._connectionIn,
			self._connectionOut,
			)
		self._connectionOut = None
		self._connectionIn = None

	def end_port_connection(self) :
		self._result += "\tnetwork.ConnectPorts(\"%s\", \"%s\");\n"%(
			self._connectionIn,
			self._connectionOut,
			)
		self._connectionOut = None
		self._connectionIn = None

	def startElement(self, name, attrs) :
#		print "Start:", name
		self._currentPath.append(name)
		if not self._processingName :
			try : getattr(self,name)(**attrs)
			except AttributeError : pass
			return
		self._content = ""

	def endElement(self, name) :
		if self._processingName and name != "processing" :
			self._result += '\t\t\t"<%s>%s</%s>\\n"\n' % (
				name, self._content.strip(), name)
		self._currentPath.pop()
		try : getattr(self, "end_"+name)()
		except AttributeError : pass
#		print "End:", name
#		self.finalize(name)

	def characters(self, content) :
		strippedContent = content.strip()
		if not strippedContent: return
		if self._currentPath[-1] == "in" :
			self._connectionIn = strippedContent 
		if self._currentPath[-1] == "out" :
			self._connectionOut = strippedContent
		if self._processingName :
			self._content+=content

#		print "Content:", content

network = NetworkHandler()
saxparser = make_parser()
saxparser.setContentHandler(network)

datasource = cStringIO.StringIO(exampleNetwork)
saxparser.parse(datasource)

network._result += "\t}\n};\n}"
print network._result


if __name__ == "__main__" :
	import unittest
	class TestClamNetwork2Code(unittest.TestCase) :
		def test_formatId1(self) :
			n = NetworkHandler()
			mangled = n._formatId("Audio_Processing")
			self.assertEquals("Audio_Processing", mangled)

		def test_formatId2(self) :
			n = NetworkHandler()
			mangled = n._formatId("Audio(Processing.01)")
			self.assertEquals("Audio_Processing_01_", mangled)
	unittest.main()

