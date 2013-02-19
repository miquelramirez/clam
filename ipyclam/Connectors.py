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
		if isinstance(peer, Connector.Connector) :
			for connector in self._list :
				connector.connect(peer)
			return
		index = 0;
		for connector in peer:
			self._list[index].connect(connector)
			index += 1

	def __gt__(self, peer) :
		return self.connect(peer)


