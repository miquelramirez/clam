import unittest
from Connector import Connector

class Connectors(object):
	def __init__(self, kind, direction, portNames):
		self._dict = dict()
		self._list = list()
		for i, port in enumerate(portNames):
			connector = Connector(name=port[0], kind=kind, direction=direction, index=i+1, type=port[1])
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

class ConnectorsTests(unittest.TestCase):
	def setUp(self):
		pass
	def test_iterable(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		listNames = [ connector.name for connector in ports ]
		self.assertEqual(listNames, ['inport1', 'inport2', 'inport3'])
	def test_indexable(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		self.assertEqual(ports[0].name, 'inport1')
	def test_lenImplementation(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		self.assertEqual(len(ports), len(portNames))
	def test_getConnectorByNameAsDicionary(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		self.assertEqual(ports['inport1'].name, 'inport1')
	def test_getConnectorByNameAsAttribute(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		self.assertEqual(ports.inport1.kind, Connector.Port)
	def test_dirFunction(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		self.assertEquals(['inport1', 'inport2', 'inport3'], dir(ports))
	def test_sliceable(self):
		portNames = [['inport1', 'type1'], ['inport2', 'type1'], ['inport3', 'type1'], ['inport4', 'type1']]
		ports = Connectors(Connector.Port, Connector.In, portNames)
		portsSliced = ports[1:4]
		self.assertEqual(portsSliced[0].name, 'inport2')
		self.assertEqual(portsSliced[1].name, 'inport3')
		self.assertEqual(portsSliced[2].name, 'inport4')

if __name__ == '__main__':
	unittest.main()