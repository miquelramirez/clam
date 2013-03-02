import Connector

class PeerConnectors(object):
	def __init__(self, engine, hostName, kind, direction, connectorName) :
		self._engine = engine
		self._connectorName = connectorName
		self._hostName = hostName
		self._kind = kind
		self._direction = direction

	def __len__(self):
		return self._engine.connectorPeers(
				self._hostName, 
				self._kind, 
				self._direction,
				self._connectorName, 
			).__len__()

	def __iter__(self):
		if self._direction == Connector.In:
			peerDirection = Connector.Out
		else:
			peerDirection = Connector.In
		for peerHost, peerConnector in self._engine.connectorPeers(
				self._hostName, 
				self._kind, 
				self._direction,
				self._connectorName, 
			):
			yield Connector.Connector(
				self._engine, 
				peerHost, 
				self._kind, 
				peerDirection,
				peerConnector, 
			)

