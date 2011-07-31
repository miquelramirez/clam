
from Connectors import Connectors
import Connector # TODO: Review if this can be ignored

import unittest
import TestFixtures
class ConnectorsTests(unittest.TestCase):
	def proxy(self):
		return TestFixtures.proxy()

	def test_iterable(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		listNames = [ connector.name for connector in ports ]
		self.assertEqual(listNames, ['InPort1', 'InPort2', 'InPort3', 'InPort4'])
	def test_indexable(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports[0].name, 'InPort1')
	def test_lenImplementation(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(len(ports), 4)
	def test_getConnectorByNameAsDicionary(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports['InPort1'].name, 'InPort1')
	def test_getConnectorByNameAsAttribute(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports.InPort1.kind, Connector.Port)
	def test_dirFunction(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEquals(['InPort1', 'InPort2', 'InPort3', 'InPort4'], dir(ports))
	def test_sliceable(self):
		ports = Connectors(self.proxy(), "Processing1", Connector.Port, Connector.In)
		portsSliced = ports[1:4]
		listNames = [ connector.name for connector in portsSliced ]
		self.assertEqual(listNames, ['InPort2', 'InPort3', 'InPort4'])
		self.assertEqual(Connectors, type(portsSliced) )

class Clam_ConnectorsTests(ConnectorsTests):
	def proxy(self):
		import Clam_NetworkProxy
		import Network
		proxy = Clam_NetworkProxy.Clam_NetworkProxy()
		network = Network.Network(proxy)
		network.Processing1 = "DummyTypeProcessing1"
		network.Processing2 = "DummyTypeProcessing2"
		return proxy

if __name__ == '__main__':
	unittest.main()
