#!/usr/bin/python

# This script generates equivalent code to the load of a given network xml.

# TODO: 
# - test processing
# - test processing with config
# - description
# - texts
# - processing size and positon
# - command line interface
# - read network from file
# - change the class name from command line
# - take the class name from the file name
# - solve the problem of parsing xmlentities surrounded by spaces (spaces get striped by sax)
# LIMITATIONS:
# - Works just with configuration parameters that are plain data (no subelements, no attributes)
# - Processing names may colide when having non alphanumeric chars in the same positions


import xml.sax
import cStringIO
import sys
import re



class NetworkHandler(xml.sax.handler.ContentHandler) :
	def __init__(self) :
		self._result = ""
		self._currentPath = []
		self._connectionIn = None
		self._connectionOut = None
		self._processingName = None

	def network(self, clamVersion, id) :
		self._result += "\tnetwork.SetName(\"" + id + "\");\n"
#		print "Network:", id, "version", clamVersion

	def description(self) : pass

	def _formatId(self, id) : return re.sub('\W', '_',id.strip())

	def processing(self, id, type, position="(10,10)", size="(10,10)") :
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
			self._connectionOut,
			self._connectionIn,
			)
		self._connectionOut = None
		self._connectionIn = None

	def end_port_connection(self) :
		self._result += "\tnetwork.ConnectPorts(\"%s\", \"%s\");\n"%(
			self._connectionOut,
			self._connectionIn,
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
				name, xml.sax.saxutils.escape(self._content.strip()), name)
		self._currentPath.pop()
		try : getattr(self, "end_"+name)()
		except AttributeError : pass

	def characters(self, content) :
		strippedContent = content.strip()
		if not strippedContent: return
		if self._currentPath[-1] == "in" :
			self._connectionIn = strippedContent 
		if self._currentPath[-1] == "out" :
			self._connectionOut = strippedContent
		if self._processingName :
			self._content+=content

	def getCode(self, name="NetworkFiller") :
		return ("""\
#include <CLAM/Network.hxx>
#include <sstream>
#include <CLAM/XMLStorage.hxx>

namespace CLAM {

void LoadConfig(ProcessingConfig & config, const std::string & xmlContent)
{
	std::istringstream os(xmlContent);
	CLAM::XMLStorage::Restore(config, os);
}

class %(name)s
{
public:
	%(name)s(){}

	void setUp(Network &network)
	{
""" + 
	self._result + 
"""\
	}
};
}""")%dict(name=name)

	def setupCode(self) :
		return self._result




if __name__ == "__main__" :
	if "--test" not in sys.argv :
		networkfile = sys.argv[1]
		exampleNetwork = file(networkfile).read()
		network = NetworkHandler()
		saxparser = xml.sax.make_parser()
		saxparser.setContentHandler(network)

		datasource = cStringIO.StringIO(exampleNetwork)
		saxparser.parse(datasource)

		print network.getCode()
		open("original.clamnetwork","w").write(exampleNetwork)


	else :
		import unittest
		sys.argv.remove("--test")
		class TestClamNetwork2Code(unittest.TestCase) :
			def parse(self, xmlContent, handler) :
				saxparser = xml.sax.make_parser()
				saxparser.setContentHandler(handler)
				saxparser.parse(cStringIO.StringIO(xmlContent))
				
			def test_formatId1(self) :
				n = NetworkHandler()
				mangled = n._formatId("Audio_Processing")
				self.assertEquals("Audio_Processing", mangled)

			def test_formatId2(self) :
				n = NetworkHandler()
				mangled = n._formatId("Audio(Processing.01)")
				self.assertEquals("Audio_Processing_01_", mangled)

			def test_emptyNetwork(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
</network>
"""
				self.parse(xml,n)
				self.assertEquals(u"""\
	network.SetName("Unnamed");
""",n.setupCode())
			def test_portConnection(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <port_connection>
    <out>proc1.outport</out>
    <in>proc2.inport</in>
  </port_connection>

</network>
"""
				self.parse(xml,n)
				self.assertEquals(u"""\
	network.SetName("Unnamed");
	network.ConnectPorts("proc1.outport", "proc2.inport");
""",n.setupCode())
				
			def test_controlConnection(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <control_connection>
    <out>proc1.outport</out>
    <in>proc2.inport</in>
  </control_connection>

</network>
"""
				self.parse(xml,n)
				self.assertEquals(u"""\
	network.SetName("Unnamed");
	network.ConnectControls("proc1.outport", "proc2.inport");
""",n.setupCode())

			def test_processing(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
  </processing>
</network>
"""
				self.parse(xml,n)
				self.assertEquals("""\
	network.SetName("Unnamed");
	Processing & _myid = network.AddProcessing("myid", "MyType");
	ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
	LoadConfig(*_myid_config,
		"<Configuration>\\n"
		"</Configuration>\\n");
	_myid.Configure(*_myid_config);
	delete _myid_config;

""",n.setupCode())


			def test_processing_withConfigParameters(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
    <parameter>a value</parameter>
  </processing>
</network>
"""
				self.parse(xml,n)
				self.assertEquals("""\
	network.SetName("Unnamed");
	Processing & _myid = network.AddProcessing("myid", "MyType");
	ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
	LoadConfig(*_myid_config,
		"<Configuration>\\n"
			"<parameter>a value</parameter>\\n"
		"</Configuration>\\n");
	_myid.Configure(*_myid_config);
	delete _myid_config;

""",n.setupCode())
			
			def test_processingConfig_withEscaping(self) :
				n = NetworkHandler()
				xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
    <parameter>&gt;&lt;&amp;</parameter>
  </processing>
</network>
"""
				self.parse(xml,n)
				self.assertEquals("""\
	network.SetName("Unnamed");
	Processing & _myid = network.AddProcessing("myid", "MyType");
	ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
	LoadConfig(*_myid_config,
		"<Configuration>\\n"
			"<parameter>&gt;&lt;&amp;</parameter>\\n"
		"</Configuration>\\n");
	_myid.Configure(*_myid_config);
	delete _myid_config;

""",n.setupCode())
    

		sys.exit(unittest.main())





