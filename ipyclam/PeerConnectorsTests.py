from PeerConnectors import PeerConnectors
import Connector

import unittest
import TestFixtures
class PeerConnectorsTests(unittest.TestCase):
	def test_iterable(self):
		peers = PeerConnectors(TestFixtures.proxy(), "Processing2", Connector.Port, Connector.In, "Inport2")
		listConnectors = [ (connector.host.name, connector.name, connector.type, connector.index) for connector in peers ]
		self.assertEqual([
			('Processing1', 'OutPort1', 'f', 0)
			], listConnectors)

	def test_len(self):
		peers = PeerConnectors(TestFixtures.proxy(), "Processing2", Connector.Port, Connector.In, "Inport2")
		self.assertEqual(1, peers.__len__() )

class Clam_PeerConnectorsTests(PeerConnectorsTests):
	def proxy(self):
		import Clam_NetworkProxy
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		network = Network.Network(proxy)
		network.Processing1 = "DummyTypeProcessing1"
		network.Processing2 = "DummyTypeProcessing2"
		network.Processing1.OutPort1 > network.Processing2.Inport2
		network.Processing2.Outport2 > network.Processing1.InPort2
		network.Processing1.OutControl1 > network.Processing2.Incontrol2
		network.Processing1.OutControl2 > network.Processing2.Incontrol2
		network.Processing2.Outcontrol1 > network.Processing1.InControl2
		return proxy

if __name__ == '__main__':
	unittest.main()

