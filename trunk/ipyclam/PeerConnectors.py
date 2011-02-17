import Connector

class PeerConnectors(object):
	def __init__(self, proxy, hostName, kind, direction, connectorName) :
		self._proxy = proxy
		self._connectorName = connectorName
		self._hostName = hostName
		self._kind = kind
		self._direction = direction

	def __iter__(self):
		for peerHost, peerConnector in self._proxy.connectorPeers(self._hostName, self._connectorName, self._kind, self._direction):
			yield Connector.Connector(self._proxy, peerHost, peerConnector, self._kind, self._direction)


import unittest
import TestFixtures
class PeerConnectorsTests(unittest.TestCase):				
	def test_iterable(self):
		peers = PeerConnectors(TestFixtures.proxy(), "Processing2", Connector.Port, Connector.Out, "Inport2")
		listConnectors = [ (connector.host.name, connector.name, connector.type, connector.index) for connector in peers ]
		self.assertEqual([
			('Processing1', 'OutPort1', 'type1', 0),
			('Processing1', 'OutPort2', 'type1', 1)
			], listConnectors)

if __name__ == '__main__':
	unittest.main()
