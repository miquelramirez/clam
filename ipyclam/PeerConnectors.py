import Connector

class PeerConnectors(object):
	def __init__(self, hostName, connectorName, kind, direction, proxy, type) :
		self._proxy = proxy
		self._connectorName = connectorName
		self._hostName = hostName
		self._kind = kind
		self._direction = direction

	def __iter__(self):
		if self._direction == Connector.In:
			direction = Connector.Out
		else:
			direction = Connector.In

		for peerHost, peerConnector in self._proxy.connectorPeers(self._hostName, self._connectorName, self._kind, self._direction):
			index, type = self._proxy.connectorInfo(self._hostName, peerConnector, self._kind, self._direction)
			yield Connector.Connector(self._proxy, peerHost, peerConnector, self._kind, direction, index, type)


import unittest
import definition
class PeerConnectorsTests(unittest.TestCase):				
	def test_iterable(self):
		peers = PeerConnectors( "Processing2", "Inport2", Connector.Port, Connector.Out, definition.proxy(), 'type1' )
		listConnectors = [ (connector.host.name, connector.name, connector.type, connector.index) for connector in peers ]
		self.assertEqual([
			('Processing1', 'Outport1', 'type1', 0),
			('Processing1', 'Outport2', 'type1', 1)
			], listConnectors)

if __name__ == '__main__':
	unittest.main()
