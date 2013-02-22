
from Connectors import Connectors
import Connector # TODO: Review if this can be ignored

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
		self.assertEqual(listNames, ['InPort1', 'InPort2', 'InPort3', 'InPort4'])
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
		portsSliced = ports[1:4]
		listNames = [ connector.name for connector in portsSliced ]
		self.assertEqual(listNames, ['InPort2', 'InPort3', 'InPort4'])
		self.assertEqual(Connectors, type(portsSliced) )

class ConnectorsTests_Clam(ConnectorsTests):
	def empty(self):
		import Clam_NetworkProxy
		return Clam_NetworkProxy.Clam_NetworkProxy()

if __name__ == '__main__':
	unittest.main()
