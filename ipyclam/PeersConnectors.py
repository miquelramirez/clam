import unittest
import definition
from Connector import Connector

class PeersConnectors(object):
	def __init__(self, host, port, kind, direction, proxy, type) :
		connectors = proxy.processingConnections(host, port, kind, direction)
		self._peers = []
		for i, connector in enumerate(connectors):
			self._peers.append(Connector(proxy, connector[0], name=connector[1], kind=kind, direction=direction, index=i+1))
	def __iter__(self):
		for connector in self._peers:
			yield connector

class PeersConnectorsTests(unittest.TestCase):				
	def test_iterable(self):
		peers = PeersConnectors( "Processing2", "Inport2", Connector.Port, Connector.Out, definition.proxy(), 'type1' )
		listConnectors = [ (connector.host, connector.name) for connector in peers ]
		self.assertEqual([
						('Processing1', 'Outport1'),
						('Processing1', 'Outport2')
						], listConnectors)

if __name__ == '__main__':
	unittest.main()