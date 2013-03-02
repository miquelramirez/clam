from Exceptions import BadConnectorDirectionOrder
from Exceptions import SameConnectorDirection
from Exceptions import DifferentConnectorKind
from Exceptions import DifferentConnectorType

In = "In"
Out = "Out"
Port = "Port"
Control = "Control"

class ConnectionExists(Exception):
	pass

class Connector(object):
	def __init__(self, networkProxy, host, kind=Port, direction=In, name = "Inport1"):
		self._engine = networkProxy
		self.__dict__["host"] = host
		self.__dict__["name"] = name
		self.__dict__["kind"] = kind
		self.__dict__["direction"] = direction

	def _hostname(self) :
		"Private helper to get the host name"
		return self.__dict__["host"]

	@property
	def name(self):
		"""The name of the port"""
		return self.__dict__["name"]

	@property
	def kind(self):
		"""The kind of the port"""
		return self.__dict__["kind"]

	@property
	def direction(self):
		"""The direction of the port"""
		return self.__dict__["direction"]

	@property
	def index(self):
		"""The index of the port"""
		return self._engine.connectorIndex(self._hostname(), self.kind, self.direction, self.name)

	@property
	def type(self):
		"""The type of the port"""
		return self._engine.connectorType(self._hostname(), self.kind, self.direction, self.name)

	@property
	def fullname(self):
		return "{0}.{1}".format(self.host.name, self.name)

	@property
	def host(self):
		import Processing
		return Processing.Processing(
			self.__dict__["host"],
			self._engine,
			)

	@property
	def peers(self):
		from PeerConnectors import PeerConnectors
		return PeerConnectors(self._engine, self._hostname(), self.kind, self.direction, self.name)

	def __gt__(self, peer) :
		if self.direction == In :
			raise BadConnectorDirectionOrder(
				"Wrong connectors order: Output > Input")
		return self.connect(peer)

	def __lt__(self, peer) :
		if self.direction == Out :
			raise BadConnectorDirectionOrder(
				"Wrong connectors order: Input < Output")
		return peer.connect(self)

	def connect(self, peer):
		if not isinstance(peer, Connector) :
			return peer.connect(self)

		if self.direction == peer.direction :
			raise SameConnectorDirection(
				"Unable to connect: %s and %s "
				"have the same direction"%(
					self.fullname, peer.fullname))
		if self.kind != peer.kind :
			raise DifferentConnectorKind(
				"Unable to connect: %s and %s "
				"are different kinds of connectors"%(
					self.fullname, peer.fullname))
		if self.type != peer.type :
			raise DifferentConnectorType(
				"Unable to connect: %s and %s "
				"handle different data types"%(
					self.fullname, peer.fullname))

		if self.direction == Out :
			fromSide = self
			toSide = peer
		else :
			fromSide = peer
			toSide = self
		
		fromProcessing = fromSide._hostname()
		fromConnector = fromSide.name
		toProcessing = toSide._hostname()
		toConnector = toSide.name

		if self._engine.connectionExists(self.kind,
				fromProcessing, fromConnector,
				toProcessing, toConnector):
			raise ConnectionExists(
				"%s and %s already connected"%(
					fromSide.fullname, toSide.fullname))

		return self._engine.connect(self.kind,
			fromProcessing, fromConnector,
			toProcessing, toConnector)

	def disconnect(self, peer = None):
		if peer is None:
			self.disconnectFromAllPeers()
			return
		import Processing
		if type(peer) == Processing.Processing:
			self.disconnectProcessing(peer)
			return
		self._engine.disconnect(self.kind,
			self._hostname(), self.name,
			peer._hostname(), peer.name)

	def disconnectProcessing(self, peer):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = self._engine.portConnections()
		elif self.kind == Control:
			connections = self._engine.controlConnections()
		connections = [connection for connection in connections]
		for connection in connections:
			if connection[0:2] == names and peer.name == connection[2]:
				self._engine.disconnect(self.kind, *connection)

	def disconnectFromAllPeers(self):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = self._engine.portConnections()
		elif self.kind == Control:
			connections = self._engine.controlConnections()
		connections = [connection for connection in connections]
		for connection in connections:
			if connection[0:2] == names:
				self._engine.disconnect(self.kind, *connection)

