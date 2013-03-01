import Connector
import Processing

class Connectors(object):
	def __init__(self, engine, processingName, kind, direction, index=slice(None)) :
		self._dict = dict()
		self._list = list()
		self._engine = engine
		self._slice = index
		self.processing = processingName
		self.direction = direction
		self.kind = kind
		if self.direction is None : return
		self._initConnections()

	def _initConnections(self) :
		connectorNames = self._engine.processingConnectors(
			self.processing, self.kind, self.direction)
		self._dict = dict()
		self._list = list()
		for conectorName in connectorNames[self._slice]:
			connector = Connector.Connector(
				self._engine,
				self.processing,
				kind=self.kind,
				direction=self.direction,
				name=conectorName)
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
			# TODO: assert if slice already used
			assert self._slice == slice(None,None,None), \
				"Slice of an slice is not supported"
			return Connectors(
				self._engine,
				self.processing,
				self.kind,
				self.direction,
				index=index)
	def __getattr__(self, name):
		if not name in self._dict.keys():
			raise AttributeError(name)
		return self._dict[name]
	def __len__(self):
		return len(self._list)
	def __dir__(self):
		return self._dict.keys()

	def connect(self, peer):
		def kind(self, peer) :
			if self.kind : return self.kind
			if not hasattr(peer,"kind") : return Connector.Port
			if peer.kind : return peer.kind
			return Connector.Port
		def direction(self, peer) :
			if not hasattr(peer,"direction") : return Connector.Out
			if peer.direction == Connector.In : return Connector.Out
			return Connector.In
		if self.direction is None :
			asPeer = Connectors(
				self._engine,
				self.processing,
				kind(self,peer),
				direction(self,peer),
				self._slice)
			return asPeer.connect(peer)
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
		def kind(self, peer) :
			if self.kind : return self.kind
			if not hasattr(peer, "kind") : return Connector.Port
			if peer.kind : return peer.kind
			return Connector.Port
		if self.direction is None :
			asOutput = Connectors(
				self._engine,
				self.processing,
				kind(self,peer),
				"Out",
				self._slice)
			return asOutput > peer
		return self.connect(peer)

	def __lt__(self, peer) :
		if self.direction == "Out" :
			raise Connector.BadConnectorDirectionOrder(
				"Wrong connectors order: Input < Output")
		def kind(self, peer) :
			if self.kind : return self.kind
			if not hasattr(peer, "kind") : return Connector.Port
			if peer.kind : return peer.kind
			return Connector.Port
		if self.direction is None :
			asInput = Connectors(
				self._engine,
				self.processing,
				kind(self, peer),
				"In",
				self._slice)
			return asInput < peer
		return peer.connect(self)


