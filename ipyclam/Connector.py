In = "In"
Out = "Out"
Port = "Port"
Control = "Control"

class ConnectionExists(Exception):
	pass

class BadConnectorDirectionOrder(Exception):
	pass

class DifferentConnectorKind(Exception):
	pass

class SameConnectorDirection(Exception):
	pass

class DifferentConnectorType(Exception):
	pass

class Connector(object):
	def __init__(self, networkProxy, host, kind=Port, direction=In, name = "Inport1"):
		self._proxy = networkProxy
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
		return self._proxy.connectorIndex(self._hostname(), self.kind, self.direction, self.name)

	@property
	def type(self):
		"""The type of the port"""
		return self._proxy.connectorType(self._hostname(), self.kind, self.direction, self.name)

	@property
	def host(self):
		import Processing
		return Processing.Processing(
			self.__dict__["host"],
			self._proxy,
			)

	@property
	def peers(self):
		from PeerConnectors import PeerConnectors
		return PeerConnectors(self._proxy, self._hostname(), self.kind, self.direction, self.name)

	def connect(self, peer):
		if not isinstance(peer, Connector) :
			return peer.connect(self)

		if self.direction == peer.direction :
			raise SameConnectorDirection(
				"Same direction: %s %s"%(self.name, peer.name))
		if self.kind != peer.kind :
			raise DifferentConnectorKind(
				"Different kind: %s %s"%(self.name, peer.name))
		if self.type != peer.type :
			raise DifferentConnectorType(
				"Different type: %s %s"%(self.name, peer.name))

		if self.direction == Out :
			fromProcessing = self._hostname()
			fromConnector = self.name
			toProcessing = peer._hostname()
			toConnector = peer.name
		else :
			fromProcessing = peer._hostname()
			fromConnector = peer.name
			toProcessing = self._hostname()
			toConnector = self.name

		if self._proxy.connectionExists(self.kind,
				fromProcessing, fromConnector,
				toProcessing, toConnector):
			raise ConnectionExists(
				"%s.%s and %s.%s already connected"%(
					fromProcessing, fromConnector,
					toProcessing, toConnector))

		self._proxy.connect(self.kind,
			fromProcessing, fromConnector,
			toProcessing, toConnector)

	def __gt__(self, peer) :
		if self.direction == In and peer.direction == Out:
			raise BadConnectorDirectionOrder("Wrong connectors order: Output > Input")
		self.connect(peer)

	def __lt__(self, peer) :
		if self.direction == Out and peer.direction == In:
			raise BadConnectorDirectionOrder("Wrong connectors order: Input < Output")
		peer.connect(self)

	def disconnect(self, peer = None):
		if peer == None:
			self.disconnectFromAllPeers()
			return
		import Processing
		if type(peer) == Processing.Processing:
			self.disconnectProcessing(peer)
			return
		self._proxy.disconnect(self.kind, self._hostname(), self.name, peer._hostname(), peer.name)

	def disconnectProcessing(self, peer):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = [connection for connection in self._proxy.portConnections()]
			for connection in connections:
				if (connection[0:2] == names) & (peer.name == connection[2]):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, peer.name, connection[3])
		else:
			connections = [connection for connection in self._proxy.controlConnections()]
			for connection in connections:
				if (connection[0:2] == names) & (peer.name == connection[2]):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, peer.name, connection[3])

	def disconnectFromAllPeers(self):
		names = (self._hostname(), self.name)
		if self.kind == Port:
			connections = [connection for connection in self._proxy.portConnections()]
			for connection in connections:
				if (connection[0:2] == names):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, connection[2], connection[3])
		if self.kind == Control:
			connections = [connection for connection in self._proxy.controlConnections()]
			for connection in connections:
				if (connection[0:2] == names):
					self._proxy.disconnect(self.kind, self._hostname(), self.name, connection[2], connection[3])

