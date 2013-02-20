from Dummy_NetworkProxy import Dummy_NetworkProxy
# TODO: BadProcessingType and BadProcessingName should be in a common module for Clam and Dummy
from Dummy_NetworkProxy import BadProcessingName
from Dummy_NetworkProxy import BadProcessingType
import Connector

import unittest

class Dummy_NetworkProxyTest(unittest.TestCase) :

	def definition(self) :
		return [
		[
			dict(
				type = "AudioSource",
				name = "Processing1",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["InPort1", "type1"],
					["InPort2", "type1"]
				],
				outports = [
					["OutPort1", "type1"],
					["OutPort2", "type1"]
				],
				incontrols = [
					["InControl1", "type2"],
					["InControl2", "type2"]
				],
				outcontrols = [
					["OutControl1", "type2"],
					["OutControl2", "type2"]
				],
			),
			dict(
				type = "AudioSink",
				name = "Processing2",
				config = dict(
					ConfigParam1 = "default1",
					ConfigParam2 = "default2",
				),
				inports = [
					["Inport1", "type1"],
					["Inport2", "type1"]
				],
				outports = [
					["Outport1", "type1"],
					["Outport2", "type1"]
				],
				incontrols = [
					["Incontrol1", "type2"],
					["Incontrol2", "type2"]
				],
				outcontrols = [
					["Outcontrol1", "type2"],
					["Outcontrol2", "type2"]
				],
			),
		],
		[
			("Processing1", "OutPort1", "Processing2", "Inport2"),
			("Processing1", "OutPort2", "Processing2", "Inport2"),
			("Processing2", "Outport2", "Processing1", "InPort2"),
		],
		[
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),
		],
		""
	]

	def test_type(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("AudioSource", proxy.processingType("Processing1"))

	def test_inports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"InPort1",
				"InPort2",
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.In))

	def test_outports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"OutPort1",
				"OutPort2",
			], proxy.processingConnectors(
				"Processing1", Connector.Port, Connector.Out))

	def test_incontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"InControl1",
				"InControl2",
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.In))

	def test_outcontrols(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				"OutControl1",
				"OutControl2",
			], proxy.processingConnectors(
				"Processing1", Connector.Control, Connector.Out))

	def test_withBadProcessingName(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertRaises(KeyError, proxy.processingType, "Processing3")

	def test_typeSecondProcessing(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("AudioSink", proxy.processingType("Processing2"))

	def test_portConnectionsOut(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				("Processing1", "OutPort1"),
				("Processing1", "OutPort2")]
				, proxy.connectorPeers("Processing2", Connector.Port, Connector.In, "Inport2"))
	
	def test_portConnectionsIn(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
				("Processing2", "Inport2")]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_connectorType(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		type = proxy.connectorType("Processing1", Connector.Port, Connector.In, "InPort1")
		self.assertEquals("type1", type)

	def test_connectorInfo(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		index = proxy.connectorIndex("Processing1", Connector.Port, Connector.In, "InPort1")
		self.assertEquals(0, index)

	def test_processingsName(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		listProcessings = [ processName for processName in proxy.processingNames() ]
		self.assertEquals(['Processing1', 'Processing2'], listProcessings)

	def test_hasProcessing_whenItIsThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertTrue(proxy.hasProcessing('Processing1'))

	def test_hasProcessing_whenItIsNotThere(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertFalse(proxy.hasProcessing('NotExisting'))
	
	def test_processingsName_empty(self):
		proxy = Dummy_NetworkProxy()
		self.assertEquals([], proxy.processingNames())

	def test_addProcessing_withOneMinimalProcessing(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("MinimalProcessing", "AProcessing")
		self.assertEquals(['AProcessing'], proxy.processingNames())
		self.assertEquals('MinimalProcessing', proxy.processingType('AProcessing'))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AProcessing', Connector.Control, Connector.Out))

	def test_addProcessing_withInPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSink", "APortSink")
		self.assertEquals(['APortSink'], proxy.processingNames())
		self.assertEquals('DummyPortSink', proxy.processingType('APortSink'))
		self.assertEquals(["InPort1"], proxy.processingConnectors('APortSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSink', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSink", "AControlSink")
		self.assertEquals(['AControlSink'], proxy.processingNames())
		self.assertEquals('ControlSink', proxy.processingType('AControlSink'))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Port, Connector.Out))
		self.assertEquals(["InControl1"], proxy.processingConnectors('AControlSink', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSink', Connector.Control, Connector.Out))

	def test_addProcessing_withOutPort(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "APortSource")
		self.assertEquals(['APortSource'], proxy.processingNames())
		self.assertEquals('DummyPortSource', proxy.processingType('APortSource'))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Port, Connector.In))
		self.assertEquals(["OutPort1"], proxy.processingConnectors('APortSource', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.In))
		self.assertEquals([], proxy.processingConnectors('APortSource', Connector.Control, Connector.Out))

	def test_addProcessing_withInControl(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "AControlSource")
		self.assertEquals(['AControlSource'], proxy.processingNames())
		self.assertEquals('ControlSource', proxy.processingType('AControlSource'))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Port, Connector.In))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Port, Connector.Out))
		self.assertEquals([], proxy.processingConnectors('AControlSource', Connector.Control, Connector.In))
		self.assertEquals(["OutControl1"], proxy.processingConnectors('AControlSource', Connector.Control, Connector.Out))

	def test_addProcessing_withNameAlreadyAdded(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "ARepeatedName")
		try:
			proxy.addProcessing("ControlSource", "ARepeatedName")
			self.fail("Exception expected")
		except BadProcessingName, e:
			self.assertEquals("'ARepeatedName: Name repeated'", e.__str__())

	def test_addProcessing_withBadType(self) :
		proxy = Dummy_NetworkProxy()
		try:
			proxy.addProcessing("ABadType", "Processing1")
			self.fail("Exception expected")
		except BadProcessingType, e:
			self.assertEquals("'BadProcessingType(ABadType)'", e.__str__())

	def test_connect_ports(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "Processing1")
		proxy.addProcessing("DummyPortSink", "Processing2")
		proxy.connect(Connector.Port, "Processing1", "OutPort1", "Processing2", "InPort1")
		self.assertEquals([
				("Processing2", "InPort1")]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_connect_controls(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "Processing2")
		proxy.connect(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl1")
		self.assertEquals([
				("Processing2", "InControl1")]
				, proxy.connectorPeers("Processing1", Connector.Control, Connector.Out, "OutControl1"))

	def test_connect_missingProcessing(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "NonExistingProcessing", "InControl1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("NonExistingProcessing does not exist", ), e.args)

	def test_connect_missingconnector(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "ProcessingWithNoIncontrol2")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "ProcessingWithNoIncontrol2", "InControl2")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("ProcessingWithNoIncontrol2 does not have connector InControl2", ), e.args)

	def test_connect_connectorsWithDifferentTypes(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("OtherControlSink", "ProcessingWithOtherControlType")
		try:
			proxy.connect(Connector.Control, "Processing1", "OutControl1", "ProcessingWithOtherControlType", "InControl1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("OutControl1 and InControl1 have incompatible types", ), e.args)

	def test_portConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "OutPort1", "Processing2", "Inport2"),
			("Processing1", "OutPort2", "Processing2", "Inport2"),
			("Processing2", "Outport2", "Processing1", "InPort2"),
		], proxy.portConnections())

	def test_controlConnections(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			("Processing1", "OutControl1", "Processing2", "Incontrol2"),
			("Processing1", "OutControl2", "Processing2", "Incontrol2"),
			("Processing2", "Outcontrol1", "Processing1", "InControl2"),			
		], proxy.controlConnections())

	def test_availableTypes(self):
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals([
			'DummyPortSource',
			'DummyPortSink',
			'ControlSource', 
			'OtherControlSink',
			'MinimalProcessing',
			'SeveralInPortsProcessing',
			'ControlSink',
			].sort(), proxy.availableTypes().sort())

	def test_areConnectable_whenSameDataType(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Source")
		proxy.addProcessing("ControlSink", "Sink")
		self.assertTrue(
			proxy.areConnectable(
				Connector.Control, "Source", "OutControl1", "Sink", "InControl1"))

	def test_areConnectable_whenDifferentDataType(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Source")
		proxy.addProcessing("OtherControlSink", "Sink")
		self.assertFalse(
			proxy.areConnectable(
				Connector.Control, "Source", "OutControl1", "Sink", "InControl1"))

	def test_processingHasConnector(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		self.assertTrue(proxy.processingHasConnector("Processing1", Connector.Control, Connector.Out, "OutControl1"))
		self.assertFalse(proxy.processingHasConnector("Processing1", Connector.Control, Connector.In, "InControl1"))

	def test_connectionExists_withControls_whenDoesNotExists(self) :
		proxy = Dummy_NetworkProxy()
		self.assertFalse(proxy.connectionExists(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl2"))

	def test_connectionExists_withControls_whenExists(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("ControlSource", "Processing1")
		proxy.addProcessing("ControlSink", "Processing2")
		self.assertTrue(proxy.connectionExists(Connector.Control, "Processing1", "OutControl1", "Processing2", "InControl1"))

	def test_connectionExists_withPorts_whenDoesNotExists(self) :
		proxy = Dummy_NetworkProxy()
		self.assertFalse(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1"))

	def test_connectionExists_withPorts_whenExists(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "Processing3")
		proxy.addProcessing("DummyPortSink", "Processing4")
		proxy.connect(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1")
		self.assertTrue(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort1"))
		self.assertFalse(proxy.connectionExists(Connector.Port, "Processing3", "OutPort1", "Processing4", "InPort2"))

	def test_connectWhenAlreadyConnected(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "Processing5")
		proxy.addProcessing("DummyPortSink", "Processing6")
		proxy.connect(Connector.Port, "Processing5", "OutPort1", "Processing6", "InPort1")
		try:
			proxy.connect(Connector.Port, "Processing5", "OutPort1", "Processing6", "InPort1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("Processing5.OutPort1 and Processing6.InPort1 already connected", ), e.args)


	def test_disconnect_ports(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.disconnect(Connector.Port, "Processing1", "OutPort1", "Processing2", "Inport2")
		self.assertEquals([]
				, proxy.connectorPeers("Processing1", Connector.Port, Connector.Out, "OutPort1"))

	def test_disconnect_controls(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.disconnect(Connector.Control, "Processing1", "OutControl1", "Processing2", "Incontrol2")
		self.assertEquals([]
				, proxy.connectorPeers("Processing1", Connector.Control, Connector.Out, "OutControl1"))

	def test_processingRenaming(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		proxy.renameProcessing("Processing1", "ProcessingRenamed")
		self.assertFalse(proxy.hasProcessing("Processing1"))
		self.assertTrue(proxy.hasProcessing("ProcessingRenamed"))

	def test_processingRenamingWithExistingNameFails(self) :
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "ExistingName")
		proxy.addProcessing("DummyPortSink", "ProcessingToRename")
		# TOFIX: KeyError should be used just when a non existing key, and args should be the kay
		try:
			proxy.renameProcessing("ProcessingToRename", "ExistingName")
		except KeyError, e:
			self.assertEquals(("A processing named 'ExistingName' already exists", ), e.args)

	def test_getNetworkDescriptionEmpty(self):
		proxy = Dummy_NetworkProxy()
		self.assertEquals("", proxy.getDescription())

	def test_setNetworkDescription(self):
		proxy = Dummy_NetworkProxy()
		proxy.setDescription("A description")
		self.assertEquals("A description", proxy.getDescription())

	def test_deleteProcessing(self):
		proxy = Dummy_NetworkProxy()
		proxy.addProcessing("DummyPortSource", "Processing1")
		self.assertTrue(proxy.hasProcessing('Processing1'))
		proxy.deleteProcessing("Processing1")
		self.assertFalse(proxy.hasProcessing('Processing1'))

	def test_config(self) :
		proxy = Dummy_NetworkProxy(*self.definition())
		self.assertEquals("default1", proxy.processingConfig("Processing1")["ConfigParam1"] )
		self.assertEquals("default2", proxy.processingConfig("Processing1")["ConfigParam2"] )

if __name__ == '__main__':
	unittest.main()


