import Clam_NetworkProxy
import DummyProcessingsModule
import unittest
import TestFixtures

class Clam_NetworkProxyTests(unittest.TestCase):

	def test_availableTypes(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertTrue('DummyProcessingWithInAndOutPorts' in proxy.availableTypes())
		self.assertTrue('DummyProcessingWithInAndOutControls' in proxy.availableTypes())

	def test_addProcessing(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithInAndOutPorts"
		self.assertEqual(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
			"<Network clamVersion=\"1.4.1\" id=\"Unnamed Network\">\n"
			"\n"
			"  <processing id=\"Processing1\" type=\"DummyProcessingWithInAndOutPorts\"/>\n"
			"\n"
			"</Network>\n"
			, proxy.xml())

	def test_processingNamesWhenEmpty(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertEqual([], proxy.processingNames())

	def test_processingNamesWhenAddingAProcessing(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSink"
		self.assertEqual(['Processing1'], proxy.processingNames())

	def test_addTwoProcessings(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSink"
		net.Processing2 = "AudioSource"
		self.assertEqual(['Processing1', 'Processing2'], proxy.processingNames())

	def test_hasProcessingWhenEmpty(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertFalse(proxy.hasProcessing('NotAddedProcessing'))

	def test_hasProcessingWhenAddingTwo(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.AddedProcessing1 = "AudioSink"
		net.AddedProcessing2 = "AudioSource"
		self.assertTrue(proxy.hasProcessing('AddedProcessing1'))
		self.assertTrue(proxy.hasProcessing('AddedProcessing2'))

	def test_processingType(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.AddedProcessing1 = "AudioSink"
		self.assertEqual("AudioSink", proxy.processingType("AddedProcessing1"))

	def test_processingHasPortWhenItDoesnt(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithoutInport2 = "DummyProcessingWithInAndOutPorts"
		self.assertFalse(proxy.processingHasConnector('ProcessingWithoutInport2', "Port", "In", "Inport2"))

	def test_processingHasInPortWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithInport = "DummyProcessingWithInAndOutPorts"
		self.assertTrue(proxy.processingHasConnector('ProcessingWithInport', "Port", "In", "Inport1"))

	def test_processingHasOutPortWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithOutport = "DummyProcessingWithInAndOutPorts"
		self.assertTrue(proxy.processingHasConnector('ProcessingWithOutport', "Port", "Out", "Outport1"))

	def test_processingHasControlWhenItDoesnt(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithoutIncontrol1 = "DummyProcessingWithInAndOutPorts"
		self.assertFalse(proxy.processingHasConnector('ProcessingWithoutIncontrol1', "Control", "In", "Incontrol1"))
   
	def test_processingHasInControlWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithIncontrol = "DummyProcessingWithInAndOutControls"
		self.assertTrue(proxy.processingHasConnector('ProcessingWithIncontrol', "Control", "In", "Incontrol1"))
   
	def test_processingHasOutControlWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.ProcessingWithOutcontrol = "DummyProcessingWithInAndOutControls"
		self.assertTrue(proxy.processingHasConnector('ProcessingWithOutcontrol', "Control", "Out", "Outcontrol1"))

	def test_getNetworkdescriptionWhenNotSet(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		self.assertEqual("", proxy.getDescription())

	def test_setNetworkdescription(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		proxy.setDescription("A network description")
		self.assertEqual("A network description", proxy.getDescription())

	def test_processingConnectorsWithInportAndOutport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.DummyProcessingWithInAndOutPort = "DummyProcessingWithInAndOutPorts"
		self.assertEqual(['Inport1'], proxy.processingConnectors('DummyProcessingWithInAndOutPort', "Port", "In"))
		self.assertEqual(['Outport1'], proxy.processingConnectors('DummyProcessingWithInAndOutPort', "Port", "Out"))

	def test_processingConnectorsWithIncontrolAndOutcontrol(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.DummyProcessingWithInAndOutControl = "DummyProcessingWithInAndOutControls"
		self.assertEqual(['Incontrol1'], proxy.processingConnectors('DummyProcessingWithInAndOutControl', "Control", "In"))
		self.assertEqual(['Outcontrol1'], proxy.processingConnectors('DummyProcessingWithInAndOutControl', "Control", "Out"))

	def test_processingConnectorsWithMultiplePortsAndControls(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.DummyProcessingWithMultiplePortsAndControls = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEqual(['Inport1', 'Inport2'], proxy.processingConnectors('DummyProcessingWithMultiplePortsAndControls', "Port", "In"))
		self.assertEqual(['Outport1', 'Outport2'], proxy.processingConnectors('DummyProcessingWithMultiplePortsAndControls', "Port", "Out"))
		self.assertEqual(['Incontrol1', 'Incontrol2'], proxy.processingConnectors('DummyProcessingWithMultiplePortsAndControls', "Control", "In"))
		self.assertEqual(['Outcontrol1', 'Outcontrol2'], proxy.processingConnectors('DummyProcessingWithMultiplePortsAndControls', "Control", "Out"))

	def test_connectTwoPorts(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		self.assertTrue(proxy.connect("Port", "Processing1", "1", "Processing2", "1"))

	def test_connectTwoControls(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		self.assertTrue(proxy.connect("Control", "Processing1", "output", "Processing2", "input"))

	def test_connectorTypeForPorts(self) :
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithInAndOutPorts"
		self.assertEquals("f", proxy.connectorType("Processing1", "Port", "In", "Inport1"))
		self.assertEquals("f", proxy.connectorType("Processing1", "Port", "Out", "Outport1"))

	def test_connectorTypeForControls(self) :
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithInAndOutControls"
		self.assertEquals("f", proxy.connectorType("Processing1", "Control", "In", "Incontrol1"))
		self.assertEquals("f", proxy.connectorType("Processing1", "Control", "Out", "Outcontrol1"))

	def test_connectionExistsWhenItDoesnt(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		self.assertFalse(proxy.connectionExists("Control", "Processing1", "output", "Processing2", "input"))

	def test_connectionExistsForTwoPortsWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		proxy.connect("Port", "Processing1", "1", "Processing2", "1")
		self.assertTrue(proxy.connectionExists("Port", "Processing1", "1", "Processing2", "1"))

	def test_connectionExistsForTwoControlsWhenItDoes(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		proxy.connect("Control", "Processing1", "output", "Processing2", "input")
		self.assertTrue(proxy.connectionExists("Control", "Processing1", "output", "Processing2", "input"))

	def test_connectorPeersOutport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		net.Processing3 = "AudioSink"
		proxy.connect("Port", "Processing1", "1", "Processing2", "1")
		proxy.connect("Port", "Processing1", "1", "Processing3", "1")
		self.assertEqual([("Processing2", "1"),("Processing3", "1")], proxy.connectorPeers("Processing1", "Port", "Out", "1"))

	def test_connectorPeersInport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		proxy.connect("Port", "Processing1", "1", "Processing2", "1")
		self.assertEqual([("Processing1", "1")], proxy.connectorPeers("Processing2", "Port", "In", "1"))

	def test_connectorPeersOutcontrol(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		net.Processing3 = "ControlSink"
		proxy.connect("Control", "Processing1", "output", "Processing2", "input")
		proxy.connect("Control", "Processing1", "output", "Processing3", "input")
		self.assertEqual([("Processing2", "input"),("Processing3", "input")], proxy.connectorPeers("Processing1", "Control", "Out", "output"))

	def test_connectorPeersIncontrol(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		net.Processing3 = "ControlSource"
		proxy.connect("Control", "Processing1", "output", "Processing2", "input")
		proxy.connect("Control", "Processing3", "output", "Processing2", "input")
		self.assertEqual([("Processing1", "output"),("Processing3", "output")], proxy.connectorPeers("Processing2", "Out", "In", "input"))

	def test_disconnectTwoPorts(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		net.Processing1["1"] > net.Processing2["1"]
		self.assertTrue(proxy.disconnect("Port", "Processing1", "1", "Processing2", "1"))

	def test_disconnectTwoControls(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		net.Processing1.output > net.Processing2.input
		self.assertTrue(proxy.disconnect("Control", "Processing1", "output", "Processing2", "input"))

	def test_connectorIndexInport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEquals(0, proxy.connectorIndex("Processing1", "Port", "In", "Inport1"))
		self.assertEquals(1, proxy.connectorIndex("Processing1", "Port", "In", "Inport2"))

	def test_connectorIndexOutport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEquals(0, proxy.connectorIndex("Processing1", "Port", "Out", "Outport1"))
		self.assertEquals(1, proxy.connectorIndex("Processing1", "Port", "Out", "Outport2"))

	def test_connectorIndexIncontrol(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEquals(0, proxy.connectorIndex("Processing1", "Control", "In", "Incontrol1"))
		self.assertEquals(1, proxy.connectorIndex("Processing1", "Control", "In", "Incontrol2"))

	def test_connectorIndexOutport(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEquals(0, proxy.connectorIndex("Processing1", "Control", "Out", "Outcontrol1"))
		self.assertEquals(1, proxy.connectorIndex("Processing1", "Control", "Out", "Outcontrol2"))

	def test_portConnections(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "AudioSource"
		net.Processing2 = "AudioSink"
		net.Processing3 = "DummyProcessingWithInAndOutPorts"
		net.Processing4 = "DummyProcessingWithInAndOutPorts"
		net.Processing1["1"] > net.Processing2["1"]
		net.Processing3.Outport1 > net.Processing4.Inport1
		self.assertEquals([
			('Processing1', '1', 'Processing2', '1'),
			('Processing3', 'Outport1', 'Processing4', 'Inport1')
		], proxy.portConnections())

	def test_processingRename(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.NameToBeChanged = "AudioSource"
		net.NameToBeChanged.name = "NewName"
		self.assertFalse(proxy.hasProcessing('NameToBeChanged'))
		self.assertTrue(proxy.hasProcessing('NewName'))
		
	def test_controlConnections(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "ControlSource"
		net.Processing2 = "ControlSink"
		net.Processing3 = "DummyProcessingWithInAndOutControls"
		net.Processing4 = "DummyProcessingWithInAndOutControls"
		net.Processing1.output > net.Processing2.input
		net.Processing3.Outcontrol1 > net.Processing4.Incontrol1
		self.assertEquals([
			('Processing1', 'output', 'Processing2', 'input'),
			('Processing3', 'Outcontrol1', 'Processing4', 'Incontrol1')
		], proxy.controlConnections())

	def test_processingDeletion(self):
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		proxy.addProcessing("ControlSource", "ProcessingToDelete")
		self.assertTrue(proxy.hasProcessing('ProcessingToDelete'))
		proxy.deleteProcessing("ProcessingToDelete")
		self.assertFalse(proxy.hasProcessing('ProcessingToDelete'))

	def test_processingConfig(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.Processing1 = "DummyProcessingWithCompleteConfiguration"
		self.assertEquals(42, net.Processing1.IntAttribute)

	def test_addprocessing_taken_name(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.p1 = "AudioSource"
		try:
			net.p1 = "AudioSink"
			self.fail("Exception expected")
		except RuntimeError, e:
			self.assertEquals("p1: name repeated", e.__str__())

	def test_addprocessing_taken_name(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.p1 = "AudioSource"
		net.p2 = "AudioSource"
		try:
			net.p1.name = "p2"
			self.fail("Exception expected")
		except KeyError, e:
			self.assertEquals(("A processing named 'p2' already exists", ), e.args)

	def test_connectWhenAlreadyConnected(self) :
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		proxy.addProcessing("DummyProcessingWithInAndOutPorts", "Processing1")
		proxy.addProcessing("DummyProcessingWithInAndOutPorts", "Processing2")
		proxy.connect("Port", "Processing1", "Outport1", "Processing2", "Inport1")
		try:
			proxy.connect("Port", "Processing1", "Outport1", "Processing2", "Inport1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("Processing1.Outport1 and Processing2.Inport1 already connected", ), e.args)

	def test_connect_missing_connector(self) :
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		proxy.addProcessing("DummyProcessingWithInAndOutPorts", "Processing1")
		proxy.addProcessing("DummyProcessingWithInAndOutPorts", "Processing2")
		try:
			proxy.connect("Port", "Processing1", "Outport1", "Processing2", "Inport")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("Processing2 does not have connector Inport", ), e.args)

	def test_connect_missing_processing(self) :
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		proxy.addProcessing("DummyProcessingWithInAndOutPorts", "Processing1")
		try:
			proxy.connect("Port", "Processing1", "Outport1", "NonExistingProcessing", "Inport1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("NonExistingProcessing does not exist", ), e.args)

	def test_set_config_attribute(self) :
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.p = "AudioSource"
		self.assertEquals(['1'], net.p._outports.__dir__() )
		net.p.NSources = 2
		self.assertEquals(['1', '2'], net.p._outports.__dir__() )
		self.assertEquals(2, net.p.NSources)

	def test_set_config_attribute_with_hold_apply(self) :
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.p = "AudioSource"
		c = net.p._config
		c.hold()
		c.NSources = 2
		self.assertEquals(1, net.p.NSources)
		c.apply()
		self.assertEquals(2, net.p.NSources)

	def test_clone_and_apply(self) :
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.p = "AudioSource"
		c = net.p._config.clone()
		c.NSources = 2
		self.assertEquals(1, net.p.NSources)
		self.assertEquals(2, c.NSources)
		net.p._config = c
		self.assertEquals(2, net.p.NSources)

	def test_connect_from_processing_to_processing(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.proc1 = "DummyProcessingWithMultiplePortsAndControls"
		net.proc2 = "DummyProcessingWithMultiplePortsAndControls"
		self.assertEquals(4, net.proc1 > net.proc2)
		self.assertEquals(
			"network.proc1 = 'DummyProcessingWithMultiplePortsAndControls'\n"
			"network.proc2 = 'DummyProcessingWithMultiplePortsAndControls'\n"
			"\n"
			"network.proc1.Outport1 > network.proc2.Inport1\n"
			"network.proc1.Outport2 > network.proc2.Inport2\n"
			"network.proc1.Outcontrol1 > network.proc2.Incontrol1\n"
			"network.proc1.Outcontrol2 > network.proc2.Incontrol2"
			, net.code())

	def test_connect_slice(self):
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		net = Network.Network(proxy)
		net.proc1 = "AudioSource"
		net.proc2 = "AudioSink"
		net.proc1.NSources = 6
		net.proc2.NSinks = 6
		net.proc1._outports[::2] > net.proc2._inports[::2]
		self.assertEquals(
			"network.proc1 = 'AudioSource'\n"
			"network.proc2 = 'AudioSink'\n"
			"network.proc1.['NSources'] = '6'\n"
			"network.proc2.['NSinks'] = '6'\n\n"
			"network.proc1[\"1\"] > network.proc2[\"1\"]\n"
			"network.proc1[\"3\"] > network.proc2[\"3\"]\n"
			"network.proc1[\"5\"] > network.proc2[\"5\"]\n"
			, net.code())


if __name__ == '__main__':
	unittest.main()