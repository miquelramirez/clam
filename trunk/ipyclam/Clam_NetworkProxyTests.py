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
		proxy.connect("Port", "Processing1", "1", "Processing2", "1")
		self.assertEqual([("Processing2", "1")], proxy.connectorPeers("Processing1", "Port", "Out", "1"))

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
		proxy.connect("Control", "Processing1", "output", "Processing2", "input")
		self.assertEqual([("Processing2", "input")], proxy.connectorPeers("Processing1", "Control", "Out", "output"))

if __name__ == '__main__':
	unittest.main()