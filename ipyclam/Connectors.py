import Connector

class Connectors(object):
	def __init__(self, networkProxy, processingName, kind, direction) :
		connectorNames = networkProxy.processingConnectors(processingName, kind, direction)
		self._dict = dict()
		self._list = list()
		for conectorName in connectorNames:
			connector = Connector.Connector(networkProxy, processingName, kind=kind, direction=direction, name=conectorName)
			self._list.append(connector)
			self._dict[connector.name] = connector
	def __iter__(self):
		for connector in self._list:
			yield connector
	def __getitem__(self, index):
		if type(index) is int:
			return self._list[index]
		if type(index) is str:
			return self._dict[index]
		if type(index) is slice:
			import copy
			connectors = copy.copy(self)
			connectors._list = self._list[index]
			return connectors
	def __getattr__(self, name):
		if not name in self._dict.keys():
			raise AttributeError(name)
		return self._dict[name]
	def __len__(self):
		return len(self._list)
	def __dir__(self):
		return self._dict.keys()

	def connect(self, peer):
		index = 0;
		for connector in peer:
			self._list[index].connect(connector)
			index += 1

	def __gt__(self, peer) :
		return self.connect(peer)

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
