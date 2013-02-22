from Connectors import Connectors
import Connector # TODO: Review if this can be ignored
from Exceptions import BadConnectorDirectionOrder
from Exceptions import SameConnectorDirection
from Exceptions import DifferentConnectorKind
from Exceptions import DifferentConnectorType

import unittest
import TestFixtures

class ConnectorsTests(unittest.TestCase):
	def empty(self):
		import Dummy_NetworkProxy
		return Dummy_NetworkProxy.Dummy_NetworkProxy()

	def fixture1(self):
		engine = self.empty()
		engine.addProcessing("DummyTypeProcessing1", "proc1")
		engine.addProcessing("DummyTypeProcessing2", "proc2")
		engine.connect("Port","proc1","OutPort1","proc2","Inport2")
		engine.connect("Port","proc2","Outport2","proc1","InPort2")
		engine.connect("Control","proc1","OutControl1","proc2","Incontrol2")
		engine.connect("Control","proc1","OutControl2","proc2","Incontrol2")
		engine.connect("Control","proc2","Outcontrol1","proc1","InControl2")
		return engine

	def test_iterable(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		listNames = [ connector.name for connector in ports ]
		self.assertEqual(listNames, [
			'InPort1',
			'InPort2',
			'InPort3',
			'InPort4',
			])

	def test_indexable(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		self.assertEqual(ports[0].name, 'InPort1')

	def test_lenImplementation(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		self.assertEqual(len(ports), 4)

	def test_getConnectorByNameAsDicionary(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		self.assertEqual(ports['InPort1'].name, 'InPort1')

	def test_getConnectorByNameAsAttribute(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		self.assertEqual(ports.InPort1.kind, Connector.Port)

	def test_dirFunction(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		self.assertEquals(['InPort1', 'InPort2', 'InPort3', 'InPort4'], dir(ports))

	def test_sliceable(self):
		ports = Connectors(self.fixture1(), "proc1", Connector.Port, Connector.In)
		portsSliced = ports[1:4:2]
		listNames = [ connector.name for connector in portsSliced ]
		self.assertEqual(listNames, ['InPort2', 'InPort4'])
		self.assertEqual(Connectors, type(portsSliced) )
		self.assertEqual(2, len(portsSliced) )

	# TODO: len, contains... on an slice

	def connectivityFixture(self) :
		engine = self.empty()
		engine.addProcessing("ProcessingWithPortsAndControls", "multi1")
		engine.addProcessing("ProcessingWithPortsAndControls", "multi2")
		engine.addProcessing("DummyControlSink", "csink")
		engine.addProcessing("DummyControlSource", "csource")
		engine.addProcessing("DummyPortSink", "psink")
		engine.addProcessing("DummyPortSource", "psource")
		engine.addProcessing("OtherControlSink", "othercsink")
		return engine

	def test_connect_to_wrongSelfDirecction(self):
		engine = self.connectivityFixture()
		inports = Connectors(engine, "multi1", Connector.Port, Connector.In)
		try :
			inports > "whatever"
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Output > Input")
		else :
			self.fail("Exception expected")

	def test_connect_from_wrongSelfDirecction(self):
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		try :
			outports < "whatever"
		except BadConnectorDirectionOrder, e:
			self.assertEqual(e.message,
				"Wrong connectors order: Input < Output")
		else :
			self.fail("Exception expected")

	def test_connect_to_processing(self) :
		import Processing
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		processing = Processing.Processing("multi2", engine)
		self.assertEqual(3, outports > processing)
		self.assertEqual([
			], engine.controlConnections())
		self.assertEqual([
			('multi1', 'OutPort1', 'multi2', 'InPort1'),
			('multi1', 'OutPort2', 'multi2', 'InPort2'),
			('multi1', 'OutPort3', 'multi2', 'InPort3'),
			], engine.portConnections())

	def test_connect_to_connectors(self) :
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		inports = Connectors(engine, "multi2", Connector.Port, Connector.In)
		self.assertEqual(3, outports > inports)
		self.assertEqual([
			], engine.controlConnections())
		self.assertEqual([
			('multi1', 'OutPort1', 'multi2', 'InPort1'),
			('multi1', 'OutPort2', 'multi2', 'InPort2'),
			('multi1', 'OutPort3', 'multi2', 'InPort3'),
			], engine.portConnections())

	def test_connect_to_connectors_wrongDirection(self) :
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		outports2 = Connectors(engine, "multi2", Connector.Port, Connector.Out)
		try :
			outports > outports2
			self.fail("Exception expected")
		except SameConnectorDirection, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"multi1.OutPort1 and multi2.OutPort1 "
				"have the same direction")

	def test_connect_to_connectors_wrongKind(self) :
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		incontrols = Connectors(engine, "multi2", Connector.Control, Connector.In)
		try :
			outports > incontrols
			self.fail("Exception expected")
		except DifferentConnectorKind, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"multi1.OutPort1 and multi2.InControl1 "
				"are different kinds of connectors")

	# TODO: Wrong type should not be a problem, just skip and not count it!!
	def test_connect_to_connectors_wrongType(self) :
		engine = self.connectivityFixture()
		outcontrols = Connectors(engine, "csource", Connector.Control, Connector.Out)
		incontrols = Connectors(engine, "othercsink", Connector.Control, Connector.In)
		try :
			outcontrols > incontrols
			self.fail("Exception expected")
		except DifferentConnectorType, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"csource.OutControl1 and othercsink.InControl1 "
				"handle different data types")

	def test_connect_to_connector(self) :
		engine = self.connectivityFixture()
		outcontrols = Connectors(engine, "multi1", Connector.Control, Connector.Out)
		incontrol = Connector.Connector(engine, "multi2", Connector.Control, Connector.In, "InControl1")
		self.assertEqual(3, outcontrols > incontrol)
		self.assertEqual([
			('multi1', 'OutControl1', 'multi2', 'InControl1'),
			('multi1', 'OutControl2', 'multi2', 'InControl1'),
			('multi1', 'OutControl3', 'multi2', 'InControl1'),
			], engine.controlConnections())
		self.assertEqual([
			], engine.portConnections())

	def test_connect_to_connector_wrongDirection(self) :
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		outport = Connector.Connector(engine, "multi2", Connector.Port, Connector.Out, "OutPort2")
		try :
			outports > outport
			self.fail("Exception expected")
		except SameConnectorDirection, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"multi1.OutPort1 and multi2.OutPort2 "
				"have the same direction")

	def test_connect_to_connector_wrongKind(self) :
		engine = self.connectivityFixture()
		outports = Connectors(engine, "multi1", Connector.Port, Connector.Out)
		incontrol = Connector.Connector(engine, "multi2", Connector.Control, Connector.In, "InControl2")
		try :
			outports > incontrol
			self.fail("Exception expected")
		except DifferentConnectorKind, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"multi1.OutPort1 and multi2.InControl2 "
				"are different kinds of connectors")

	# TODO: Wrong type should not be a problem, just skip and not count it!!
	def test_connect_to_connector_wrongType(self) :
		engine = self.connectivityFixture()
		outcontrols = Connectors(engine, "csource", Connector.Control, Connector.Out)
		incontrol = Connector.Connector(engine, "othercsink", Connector.Control, Connector.In, "InControl1")
		try :
			outcontrols > incontrol
			self.fail("Exception expected")
		except DifferentConnectorType, e :
			self.assertMultiLineEqual(e.message,
				"Unable to connect: "
				"csource.OutControl1 and othercsink.InControl1 "
				"handle different data types")


class ConnectorsTests_Clam(ConnectorsTests):
	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

if __name__ == '__main__':
	unittest.main()


