import Processing
import ProcessingTypes

class Network(object):
	def __init__(self, networkProxy):
		self.__dict__['_proxy'] = networkProxy
		self.__dict__['methods'] = ["types", "code", "xml"]

	def __getitem__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise KeyError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __getattr__(self, name):
		if name == "description":
			return self._proxy.getDescription()
		if not self._proxy.hasProcessing(name) :
			raise AttributeError(name)
		return Processing.Processing(proxy=self._proxy, name=name)

	def __dir__(self):
		return self._proxy.processingNames() + ["description"]

	@property
	def types(self) :
		return ProcessingTypes.ProcessingTypes(self._proxy)

	def code(self, networkVar = "network", fullConfig = False):

		codeLines = []

		def configCode(networkVar, fullConfig):
			configCode = ""
			for name in self._proxy.processingNames():
				configCode += self.__getitem__(name)._config.code(name, networkVar, fullConfig)
			return configCode
		def appendAttribute(name):
			if name[0].isdigit() or not name.isalnum() :
				return "[\"%s\"]"%name
			else :
				return "."+name
		code = ""
		if self._proxy.getDescription() != "":
			code += "%s.description = %s\n"%(
				networkVar,
				repr(self._proxy.getDescription()),
				)
		code += "\n".join([
			"%s%s = '%s'"%(
				networkVar,
				appendAttribute(name),
				self._proxy.processingType(name),
				)
			for name in self._proxy.processingNames()])
		code += "\n"
		code += configCode(networkVar, fullConfig)
		code += "\n"
		code += "\n".join([
				"%s%s%s > %s%s%s"%(
					networkVar,
					appendAttribute(fromProcessing),
					appendAttribute(fromConnector),
					networkVar,
					appendAttribute(toProcessing),
					appendAttribute(toConnector),
					)
				for fromProcessing, fromConnector, toProcessing, toConnector
				in self._proxy.portConnections()])
		code += "\n"
		code += "\n".join([
				"%s%s%s > %s%s%s"%(
					networkVar,
					appendAttribute(fromProcessing),
					appendAttribute(fromConnector),
					networkVar,
					appendAttribute(toProcessing),
					appendAttribute(toConnector),
					)
				for fromProcessing, fromConnector, toProcessing, toConnector
				in self._proxy.controlConnections()])
		return code


	def __setattr__(self, name, type) :
		if name == "description":
			self._proxy.setDescription(type)
			return
		if name == "backend":
			self._proxy.setBackend(type)
			return
		if name in self.__dict__['methods']:
			raise AssertionError("Wrong processing name: %s is a method"%(name))
		# TODO: fail on existing attributes (not processings)
		self._proxy.addProcessing(type, name)

	def __setitem__(self, name, type) :
		self.__setattr__(name, type)

	def __delattr__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise AttributeError(name)
		self._proxy.deleteProcessing(name)

	def __delitem__(self, name):
		if not self._proxy.hasProcessing(name) :
			raise KeyError(name)
		self.__delattr__(name)
	# TODO: Workaround to be able to use the function from the C++ proxy
	def xml(self):
		return self._proxy.xml()

	def __repr__(self) :
		return self.code()

	def play(self) :
		self._proxy.play()

	def pause(self) :
		self._proxy.pause()

	def stop(self) :
		self._proxy.stop()

	def bindUi(self, pyqt_object) :
		self._proxy.bindUi(pyqt_object)

	def loadUi(self, uiFilename) :
		return self._proxy.loadUi(uiFilename)

	def createWidget(self, className) :
		return self._proxy.createWidget(className)

	def processingNames(self):
		return self._proxy.processingNames()

	def load(self, filename):
		self._proxy.load(filename)

	def save(self, filename):
		self._proxy.save(filename)
