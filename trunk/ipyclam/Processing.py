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
		self.__dict__["_config"] = Configuration.Configuration(proxy.processingConfig(self.name))
		self.__dict__["_inports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.In)
		self.__dict__["_outports"] = Connectors.Connectors(proxy, name, Connector.Port, Connector.Out)
		self.__dict__["_incontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.In)
		self.__dict__["_outcontrols"] = Connectors.Connectors(proxy, name, Connector.Control, Connector.Out)
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
			self.__dict__["proxy"].processingRename(self.__dict__[name], value)
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
				"_config", 
				"_inports", 
				"_outports", 
				"_incontrols", 
				"_outcontrols",
			]
		)

	def connect(self, peer):
		if type(peer) == Processing:
			return self.connectProcessings(peer)
		if type(peer) == Connector.Connector:
			return self.connectWithConnector(peer)
		if type(peer) == Connectors.Connectors:
			return self.connectWithConnectors(peer)

	def __gt__(self, peer) :
		return self.connect(peer)

	#Helper method to connect ports and controls from processing to processing
	def connectProcessings(self, peer):
		inports = 0
		incontrols = 0
		for connector in peer.__dict__["_inports"]:
			if inports >= self.__dict__["_outports"].__len__():
				break
			if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], self.__dict__["_outports"][inports].name, connector.host.name, connector.name):
				self.__dict__["_outports"][inports] > connector
				inports += 1
		for connector in peer.__dict__["_incontrols"]:
			if incontrols >= self.__dict__["_outcontrols"].__len__():
				break
			if self.__dict__["proxy"].areConnectable("Control", self.__dict__["name"], self.__dict__["_outcontrols"][incontrols].name, connector.host.name, connector.name):
				self.__dict__["_outcontrols"][incontrols] > connector
				incontrols += 1
		return inports + incontrols

	#Helper method to connect compatible ports or controls to a single connector
	def connectWithConnector(self, peer):
		connections = 0
		if peer.kind == "Control":
			for connector in self.__dict__["_outcontrols"]:
				if self.__dict__["proxy"].areConnectable("Control", self.__dict__["name"], connector.name, peer.host.name, peer.name):
					connector > peer
					connections += 1
			return connections
		if peer.kind == "Port":
			for connector in self.__dict__["_outports"]:
				if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], connector.name, peer.host.name, peer.name):
					connector > peer
					connections += 1
			return connections

	#Helper method to connect compatible ports or controls to connectors
	def connectWithConnectors(self, peers):
		connections = 0
		if peers[0].kind == "Control":
			for connector in self.__dict__["_outcontrols"]:
				for peer in peers:
					if self.__dict__["proxy"].areConnectable("Control", self.__dict__["name"], connector.name, peer.host.name, peer.name):
						connector > peer
						connections += 1
			return connections
		if peers[0].kind == "Port":
			for connector in self.__dict__["_outports"]:
				for peer in peers:
					if self.__dict__["proxy"].areConnectable("Port", self.__dict__["name"], connector.name, peer.host.name, peer.name):
						connector > peer
						connections += 1
			return connections

