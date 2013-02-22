import Connector
import Processing

class Connectors(object):
	def __init__(self, networkProxy, processingName, kind, direction) :
		connectorNames = networkProxy.processingConnectors(processingName, kind, direction)
		self._dict = dict()
		self._list = list()
		self.direction = direction
		self.kind = kind
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
		if isinstance(peer, Processing.Processing) :
			return peer.connect(self)
		if isinstance(peer, Connector.Connector) :
			return sum((
				connector.connect(peer)
				for connector in self ))
		if isinstance(peer, Connectors) :
			return sum((
				mine.connect(peers)
					for mine,peers in zip(self,peer) ))

	def __gt__(self, peer) :
		if self.direction == "In" :
			raise Connector.BadConnectorDirectionOrder(
				"Wrong connectors order: Output > Input")
		return self.connect(peer)

	def __lt__(self, peer) :
		if self.direction == "Out" :
			raise Connector.BadConnectorDirectionOrder(
				"Wrong connectors order: Input < Output")
		return peer.connect(self)


