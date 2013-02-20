import Connector
import Configuration
import Connectors

class NotFound(Exception) : pass

class Processing(object):
	def __init__(self, name, proxy):
		assert(proxy.hasProcessing(name)) # TODO unit test
		self.__dict__["name"] = name
		self.__dict__["proxy"] = proxy
		self.__dict__["type"] = proxy.processingType(name)
		self.__dict__["_config"] = Configuration.Configuration(
			proxy.processingConfig(self.name))
		self.__dict__["_inports"] = Connectors.Connectors(
			proxy, name, Connector.Port, Connector.In)
		self.__dict__["_outports"] = Connectors.Connectors(
			proxy, name, Connector.Port, Connector.Out)
		self.__dict__["_incontrols"] = Connectors.Connectors(
			proxy, name, Connector.Control, Connector.In)
		self.__dict__["_outcontrols"] = Connectors.Connectors(
			proxy, name, Connector.Control, Connector.Out)
	def __getitem__(self, name):
		if name in dir(self._config):
			return self._config[name]
		if name in dir(self._inports):
			return self._inports[name]
		if name in dir(self._outports):
			return self._outports[name]
		if name in dir(self._incontrols):
			return self._incontrols[name]
		if name in dir(self._outcontrols):
			return self._outcontrols[name]
		raise KeyError(name)
	def __setitem__(self, name, value):
		if name == 'name':
			self.__dict__["proxy"].renameProcessing(self.__dict__[name], value)
			self.__dict__["name"] = value
			return
		self._config[name] = value
	def __getattr__(self, name):
		try:
			return self.__getitem__(name)
		except KeyError:
			raise AttributeError(name)
	def __setattr__(self, name, value):
		if name is 'type': raise AttributeError("Attribute 'type' is read only")
		self.__setitem__(name, value)
	def __dir__(self):
		return (
			dir(self._config) + 
			dir(self._inports) + 
			dir(self._outports) + 
			dir(self._incontrols) + 
			dir(self._outcontrols) + 
			[
				"type",
				"name",
				"connect",
				"_config", 
				"_inports", 
				"_outports", 
				"_incontrols", 
				"_outcontrols",
			]
		)

	def connect(self, peer):

		def peerConnectorSet(peer) :
			"""Returns the complementary connector set to peer"""
			if peer.kind == "Control":
				if peer.direction == "In" :
					return self._outcontrols
				else :
					return self._incontrols
			else :
				if peer.direction == "In" :
					return self._outports
				else :
					return self._inports

		if isinstance(peer, Processing):
			return (
				(self._outports > peer._inports) +
				(self._outcontrols > peer._incontrols) )

		if isinstance(peer, Connector.Connector) :
			connectors = peerConnectorSet(peer)
			return peer.connect(connectors)

		if isinstance(peer, Connectors.Connectors) :
			connectors = peerConnectorSet(peer[0])
			return peer.connect(connectors)

	def __gt__(self, peer) :
		return self.connect(peer)



