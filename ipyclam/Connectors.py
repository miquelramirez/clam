import Connector

class Connectors(object):
	def __init__(self, networkProxy, processingName, kind, direction) :
		portNames = networkProxy.processingConnectors(processingName, kind, direction)
		self._dict = dict()
		self._list = list()
		for portName in portNames:
			connector = Connector.Connector(networkProxy, processingName, kind=kind, direction=direction, name=portName)
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
			return self._list[index]
	def __getattr__(self, name):
		if not name in self._dict.keys():
			raise AttributeError(name)
		return self._dict[name]
	def __len__(self):
		return len(self._list)
	def __dir__(self):
		return self._dict.keys()

import unittest
import TestFixtures
class ConnectorsTests(unittest.TestCase):				
	def test_iterable(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		listNames = [ connector.name for connector in ports ]
		self.assertEqual(listNames, ['InPort1', 'InPort2', 'InPort3', 'InPort4'])
	def test_indexable(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports[0].name, 'InPort1')
	def test_lenImplementation(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(len(ports), 4)
	def test_getConnectorByNameAsDicionary(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports['InPort1'].name, 'InPort1')
	def test_getConnectorByNameAsAttribute(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEqual(ports.InPort1.kind, Connector.Port)
	def test_dirFunction(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		self.assertEquals(['InPort1', 'InPort2', 'InPort3', 'InPort4'], dir(ports))
	def test_sliceable(self):
		ports = Connectors(TestFixtures.proxy(), "Processing1", Connector.Port, Connector.In)
		portsSliced = ports[1:4]
		self.assertEqual(portsSliced[0].name, 'InPort2')
		self.assertEqual(portsSliced[1].name, 'InPort3')
		self.assertEqual(portsSliced[2].name, 'InPort4')

if __name__ == '__main__':
	unittest.main()
