import Connector

class PeerConnectors(object):
	def __init__(self, proxy, hostName, kind, direction, connectorName) :
		self._proxy = proxy
		self._connectorName = connectorName
		self._hostName = hostName
		self._kind = kind
		self._direction = direction

	def __len__(self):
		return self._proxy.connectorPeers(
				self._hostName, 
				self._kind, 
				self._direction,
				self._connectorName, 
			).__len__()

	def __iter__(self):
		if self._direction == Connector.In:
			peerDirection = Connector.Out
		else:
			peerDirection = Connector.In
		for peerHost, peerConnector in self._proxy.connectorPeers(
				self._hostName, 
				self._kind, 
				self._direction,
				self._connectorName, 
			):
			yield Connector.Connector(
				self._proxy, 
				peerHost, 
				self._kind, 
				peerDirection,
				peerConnector, 
			)

import unittest
import TestFixtures
class PeerConnectorsTests(unittest.TestCase):				
	def _test_iterable(self):
		peers = PeerConnectors(TestFixtures.proxy(), "Processing2", Connector.Port, Connector.In, "Inport2")
		listConnectors = [ (connector.host.name, connector.name, connector.type, connector.index) for connector in peers ]
		self.assertEqual([
			('Processing1', 'OutPort1', 'f', 0),
			('Processing1', 'OutPort2', 'f', 1)
			], listConnectors)

	def _test_len(self):
		peers = PeerConnectors(TestFixtures.proxy(), "Processing2", Connector.Port, Connector.In, "Inport2")
		self.assertEqual(2, peers.__len__() )

if __name__ == '__main__':
	unittest.main()

