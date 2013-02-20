import Clam_NetworkProxy
import DummyProcessingsModule
import unittest
import TestFixtures

class Clam_NetworkProxyTests(unittest.TestCase):

	def engine(self) :
		return Clam_NetworkProxy.Clam_NetworkProxy()

	def test_availableTypes(self):
		engine = self.engine()
		self.assertTrue('DummyProcessingWithInAndOutPorts' in engine.availableTypes())
		self.assertTrue('DummyProcessingWithInAndOutControls' in engine.availableTypes())

	def test_xml(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		self.assertEqual(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
			"<Network clamVersion=\"1.4.1\" id=\"Unnamed Network\">\n"
			"\n"
			"  <processing id=\"proc1\" type=\"DummyProcessingWithInAndOutPorts\"/>\n"
			"\n"
			"</Network>\n"
			, engine.xml())

	def test_processingNames_whenEmpty(self):
		engine = self.engine()
		self.assertEqual([], engine.processingNames())

	def test_processingNames_whenAddingAProcessing(self):
		engine = self.engine()
		engine.addProcessing("AudioSink","proc1")
		self.assertEqual(['proc1'], engine.processingNames())

	def test_processingNames_whenAddingTwoProcessings(self):
		import Network
		engine = self.engine()
		engine.addProcessing("AudioSink","proc1")
		engine.addProcessing("AudioSource","proc2")
		self.assertEqual(['proc1', 'proc2'], engine.processingNames())

	def test_addProcessing_takenName(self):
		import Network
		engine = self.engine()
		engine.addProcessing("AudioSink","SameName")
		try :
			engine.addProcessing("AudioSource","SameName")
		except AssertionError, e :
			self.assertEqual(e.message,
				"Processing 'SameName' already exists")
		else :
			self.fail("Runtime error expected")
		self.assertEqual(['SameName'], engine.processingNames())

	def test_addProcessing_badType(self):
		import Network
		engine = self.engine()
		try :
			engine.addProcessing("BadType","Name")
		except RuntimeError, e :
			self.assertTrue(e.message.startswith(
				"GetCreatorSafe invoked with a non existent key: BadType\n")
				)
		else :
			self.fail("Runtime error expected")
		self.assertEqual([], engine.processingNames())

	def test_hasProcessing_whenEmpty(self):
		engine = self.engine()
		self.assertFalse(engine.hasProcessing('Missing'))

	def test_hasProcessing_whenAdding(self):
		engine = self.engine()
		self.assertFalse(engine.hasProcessing('proc1'))
		engine.addProcessing("AudioSink","proc1")
		self.assertTrue(engine.hasProcessing('proc1'))
		self.assertFalse(engine.hasProcessing('Missing'))

	def test_processingType(self):
		engine = self.engine()
		engine.addProcessing("AudioSink", "AddedProcessing1")
		self.assertEqual("AudioSink", engine.processingType("AddedProcessing1"))

	def test_processingType_whenMissing(self):
		engine = self.engine()
		try :
			engine.processingType("Missing")
		except AssertionError, e :
			self.assertEqual("Processing 'Missing' not found", e.message)
		else:
			self.fail("Failed assertion expected")

	def test_processingHasConnector_missingProcessing(self):
		engine = self.engine()
		try :
			engine.processingHasConnector(
				'MissingProcessing', "Port", "In", "Inport1")
		except AssertionError, e :
			self.assertEqual(e.message,
				"Processing 'MissingProcessing' not found")
		else :
			self.fail("Failed assertion expected")

	def test_processingHasConnector_InPort_missing(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertFalse(engine.processingHasConnector(
			'proc', "Port", "In", "Missing"))

	def test_processingHasConnector_InPort(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertTrue(engine.processingHasConnector(
			'proc', "Port", "In", "Inport1"))

	def test_processingHasConnector_OutPort(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertTrue(engine.processingHasConnector(
			'proc', "Port", "Out", "Outport1"))

	def test_processingHasConnector_OutPort_missing(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertFalse(engine.processingHasConnector(
			'proc', "Port", "Out", "Missing"))

	def test_processingHasConnector_InControl_missing(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertFalse(engine.processingHasConnector(
			'proc', "Control", "In", "Missing"))

	def test_processingHasConnector_InControl(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc")
		self.assertTrue(engine.processingHasConnector(
			'proc', "Control", "In", "Incontrol1"))

	def test_processingHasConnector_OutControl(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc")
		self.assertTrue(engine.processingHasConnector(
			'proc', "Control", "Out", "Outcontrol1"))

	def test_processingHasConnector_OutControl_missing(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc")
		self.assertFalse(engine.processingHasConnector(
			'proc', "Control", "Out", "Missing"))

	def test_getDescription_whenNotSet(self):
		engine = self.engine()
		self.assertEqual("", engine.getDescription())

	def test_setDescription_whenSet(self):
		engine = self.engine()
		engine.setDescription("A network description")
		self.assertEqual("A network description", engine.getDescription())

	def test_processingConnectors_withInportAndOutport(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertEqual(['Inport1'], engine.processingConnectors('proc', "Port", "In"))
		self.assertEqual(['Outport1'], engine.processingConnectors('proc', "Port", "Out"))

	def test_processingConnectors_withIncontrolAndOutcontrol(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc")
		self.assertEqual(['Incontrol1'], engine.processingConnectors('proc', "Control", "In"))
		self.assertEqual(['Outcontrol1'], engine.processingConnectors('proc', "Control", "Out"))

	def test_processingConnectors_withMultiplePortsAndControls(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithMultiplePortsAndControls", "proc")
		self.assertEqual(['Inport1', 'Inport2'],
			engine.processingConnectors('proc', "Port", "In"))
		self.assertEqual(['Outport1', 'Outport2'],
			engine.processingConnectors('proc', "Port", "Out"))
		self.assertEqual(['Incontrol1', 'Incontrol2'],
			engine.processingConnectors('proc', "Control", "In"))
		self.assertEqual(['Outcontrol1', 'Outcontrol2'],
			engine.processingConnectors('proc', "Control", "Out"))

	def test_connect_ports(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		self.assertTrue(engine.connect("Port", "proc1", "1", "proc2", "1"))

	def test_connect_controls(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		self.assertTrue(engine.connect("Control", "proc1", "output", "proc2", "input"))

	def test_connectorType_forPorts(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc")
		self.assertEquals("f", engine.connectorType("proc", "Port", "In", "Inport1"))
		self.assertEquals("f", engine.connectorType("proc", "Port", "Out", "Outport1"))

	def test_connectorType_forControls(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc")
		self.assertEquals("f", engine.connectorType("proc", "Control", "In", "Incontrol1"))
		self.assertEquals("f", engine.connectorType("proc", "Control", "Out", "Outcontrol1"))

	def test_connectionExists_onControls_whenItDoesnt(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		self.assertFalse(engine.connectionExists("Control", "proc1", "output", "proc2", "input"))

	def test_connectionExists_onPorts_whenItDoesnt(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		self.assertFalse(engine.connectionExists("Port", "proc1", "1", "proc2", "1"))

	def test_connectionExists_onPorts_whenItDoes(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		engine.connect("Port", "proc1", "1", "proc2", "1")
		self.assertTrue(engine.connectionExists("Port", "proc1", "1", "proc2", "1"))

	def test_connectionExists_onControls_whenItDoes(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		engine.connect("Control", "proc1", "output", "proc2", "input")
		self.assertTrue(engine.connectionExists("Control", "proc1", "output", "proc2", "input"))

	def test_connectorPeers_outport(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		engine.addProcessing("AudioSink","proc3")
		engine.connect("Port", "proc1", "1", "proc2", "1")
		engine.connect("Port", "proc1", "1", "proc3", "1")
		self.assertEqual([
			("proc2", "1"),
			("proc3", "1"),
			], engine.connectorPeers("proc1", "Port", "Out", "1"))

	def test_connectorPeers_inport(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		engine.connect("Port", "proc1", "1", "proc2", "1")
		self.assertEqual([
			("proc1", "1"),
			], engine.connectorPeers("proc2", "Port", "In", "1"))

	def test_connectorPeers_outcontrol(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		engine.addProcessing("ControlSink","proc3")
		engine.connect("Control", "proc1", "output", "proc2", "input")
		engine.connect("Control", "proc1", "output", "proc3", "input")
		self.assertEqual([
			("proc2", "input"),
			("proc3", "input"),
			], engine.connectorPeers("proc1", "Control", "Out", "output"))

	def test_connectorPeers_incontrol(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		engine.addProcessing("ControlSource","proc3")
		engine.connect("Control", "proc1", "output", "proc2", "input")
		engine.connect("Control", "proc3", "output", "proc2", "input")
		self.assertEqual([
			("proc1", "output"),
			("proc3", "output"),
			], engine.connectorPeers("proc2", "Control", "In", "input"))

	def test_disconnect_ports(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		engine.connect("Port", "proc1", "1", "proc2", "1")
		self.assertTrue(engine.disconnect("Port", "proc1", "1", "proc2", "1"))
		self.assertEqual([
			], engine.connectorPeers("proc2", "Port", "In", "1"))

	def test_disconnect_controls(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		engine.connect("Control", "proc1", "output", "proc2", "input")
		self.assertTrue(engine.disconnect("Control", "proc1", "output", "proc2", "input"))
		self.assertEqual([
			], engine.connectorPeers("proc2", "Control", "In", "input"))

	def test_connectorIndex_inport(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithMultiplePortsAndControls", "proc")
		self.assertEquals(0, engine.connectorIndex("proc", "Port", "In", "Inport1"))
		self.assertEquals(1, engine.connectorIndex("proc", "Port", "In", "Inport2"))

	def test_connectorIndex_outport(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithMultiplePortsAndControls", "proc")
		self.assertEquals(0, engine.connectorIndex("proc", "Port", "Out", "Outport1"))
		self.assertEquals(1, engine.connectorIndex("proc", "Port", "Out", "Outport2"))

	def test_connectorIndex_incontrol(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithMultiplePortsAndControls", "proc")
		self.assertEquals(0, engine.connectorIndex("proc", "Control", "In", "Incontrol1"))
		self.assertEquals(1, engine.connectorIndex("proc", "Control", "In", "Incontrol2"))

	def test_connectorIndex_outcontrol(self):
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithMultiplePortsAndControls", "proc")
		self.assertEquals(0, engine.connectorIndex("proc", "Control", "Out", "Outcontrol1"))
		self.assertEquals(1, engine.connectorIndex("proc", "Control", "Out", "Outcontrol2"))

	def test_portConnections(self):
		import Network
		engine = self.engine()
		net = Network.Network(engine)
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSink","proc2")
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc3")
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc4")
		net.proc1["1"] > net.proc2["1"]
		net.proc3.Outport1 > net.proc4.Inport1
		self.assertEquals([
			('proc1', '1', 'proc2', '1'),
			('proc3', 'Outport1', 'proc4', 'Inport1')
		], engine.portConnections())

	def test_controlConnections(self):
		engine = self.engine()
		engine.addProcessing("ControlSource","proc1")
		engine.addProcessing("ControlSink","proc2")
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc3")
		engine.addProcessing("DummyProcessingWithInAndOutControls", "proc4")
		engine.connect("Control", "proc1", "output", "proc2","input")
		engine.connect("Control", "proc3", "Outcontrol1", "proc4","Incontrol1")
		self.assertEquals([
			('proc1', 'output', 'proc2', 'input'),
			('proc3', 'Outcontrol1', 'proc4', 'Incontrol1')
		], engine.controlConnections())

	def test_deleteProcessing(self):
		engine = self.engine()
		engine.addProcessing("ControlSource", "ProcessingToDelete")
		self.assertTrue(engine.hasProcessing('ProcessingToDelete'))
		engine.deleteProcessing("ProcessingToDelete")
		self.assertFalse(engine.hasProcessing('ProcessingToDelete'))

	def test_deleteProcessing_missing(self):
		engine = self.engine()
		try :
			engine.deleteProcessing("Missing")
		except AssertionError, e:
			self.assertEquals(e.message,
				"Processing 'Missing' not found")
		else :
			self.fail("Assertion expected")

	def test_renameProcessing(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","NameToBeChanged")
		engine.renameProcessing("NameToBeChanged", "NewName")
		self.assertFalse(engine.hasProcessing('NameToBeChanged'))
		self.assertTrue(engine.hasProcessing('NewName'))
		
	def test_renameProcessing_missing(self):
		engine = self.engine()
		try:
			engine.renameProcessing("missing", "proc2")
		except AssertionError, e:
			self.assertEquals(e.message,
				"Processing 'missing' not found")
		else:
			self.fail("Exception expected")

	def test_renameProcessing_takenName(self):
		engine = self.engine()
		engine.addProcessing("AudioSource","proc1")
		engine.addProcessing("AudioSource","proc2")
		try:
			engine.renameProcessing("proc1", "proc2")
		except AssertionError, e:
			self.assertEquals(("Processing 'proc2' already exists", ), e.args)
		else:
			self.fail("Exception expected")

	def test_connect_whenAlreadyConnected(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc2")
		engine.connect("Port", "proc1", "Outport1", "proc2", "Inport1")
		try:
			engine.connect("Port", "proc1", "Outport1", "proc2", "Inport1")
		except AssertionError, e:
			self.assertEquals("proc1.Outport1 and proc2.Inport1 already connected", e.message)
		else:
			self.fail("Exception expected")

	def test_connect_missingFromPort(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc2")
		try:
			engine.connect("Port", "proc1", "Missing", "proc2", "Inport1")
		except AssertionError, e:
			self.assertEquals(("proc1 does not have connector Missing", ), e.args)
		else:
			self.fail("Exception expected")

	def test_connect_missingToPort(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc2")
		try:
			engine.connect("Port", "proc1", "Outport1", "proc2", "Missing")
		except AssertionError, e:
			self.assertEquals(("proc2 does not have connector Missing", ), e.args)
		else:
			self.fail("Exception expected")

	def test_connect_missingToProcessing(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		try:
			engine.connect("Port", "proc1", "Outport1", "Missing", "Inport1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(("Processing 'Missing' not found", ), e.args)

	def test_connect_missingFromProcessing(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		try:
			engine.connect("Port", "Missing", "Outport1", "proc1", "Inport1")
		except AssertionError, e:
			self.assertEquals(e.message,
				"Processing 'Missing' not found" )
		else :
			self.fail("Exception expected")

	def test_connect_missingToProcessing(self) :
		engine = self.engine()
		engine.addProcessing("DummyProcessingWithInAndOutPorts", "proc1")
		try:
			engine.connect("Port", "Missing", "Outport1", "proc1", "Inport1")
			self.fail("Exception expected")
		except AssertionError, e:
			self.assertEquals(e.message,
				"Processing 'Missing' not found" )

	# TODO: Config tests

	# TODO: Tests for playback and the like

"""
class Dummy_NetworkTest(Clam_NetworkProxyTests) :
	def engine(self) :
		import Dummy_NetworkProxy
		return Dummy_NetworkProxy.Dummy_NetworkProxy()
"""


if __name__ == '__main__':
	unittest.main()

