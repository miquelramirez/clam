from PeerConnectors import PeerConnectors
import Connector

import unittest
import TestFixtures
class PeerConnectorsTests(unittest.TestCase):
	def empty(self):
		import Dummy_Engine
		return Dummy_Engine.Dummy_Engine()

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
		peers = PeerConnectors(self.fixture1(), "proc2", Connector.Port, Connector.In, "Inport2")
		listConnectors = [ (connector.host.name, connector.name, connector.type, connector.index) for connector in peers ]
		self.assertEqual([
			('proc1', 'OutPort1', 'f', 0)
			], listConnectors)

	def test_len(self):
		peers = PeerConnectors(self.fixture1(), "proc2", Connector.Port, Connector.In, "Inport2")
		self.assertEqual(1, peers.__len__() )

class Clam_PeerConnectorsTests(PeerConnectorsTests):
	def empty(self):
		import Clam_Engine
		return Clam_Engine.Clam_Engine()

if __name__ == '__main__':
	unittest.main()

