import Processing
import ProcessingTypes
import Notifier_EngineDecorator

class Network(object):
	def __init__(self, engine=None):
		if engine is None :
			try :
				import Clam_Engine
				engine = Clam_Engine.Clam_Engine()
			except ImportError: pass
		self.__dict__['_engine'] = Notifier_EngineDecorator.Notifier_EngineDecorator(engine)
		self.__dict__['methods'] = ["types", "code", "xml"]

	def __getitem__(self, name):
		if not self._engine.hasProcessing(name) :
			raise KeyError(name)
		return Processing.Processing(engine=self._engine, name=name)

	def __getattr__(self, name):
		if name == "description":
			return self._engine.getDescription()
		if not self._engine.hasProcessing(name) :
			raise AttributeError(name)
		return Processing.Processing(engine=self._engine, name=name)

	def __dir__(self):
		return self._engine.processingNames() + ["description"]

	def __contains__(self, processingName) :
		return self._engine.hasProcessing(processingName)

	@property
	def types(self) :
		return ProcessingTypes.ProcessingTypes(self._engine)

	def code(self, networkVar = "network", fullConfig = False):

		codeLines = []

		def isidentifier(k):
			"""
			Return True if the string k can be used as the name of a valid
			Python keyword argument, otherwise return False.
			"""
			import keyword
			import tokenize
			import re
			# Don't allow python reserved words as arg names
			if k in keyword.kwlist:
				return False
			return re.match('^' + tokenize.Name + '$', k) is not None


		def configCode(networkVar, fullConfig):
			configCode = ""
			for name in self._engine.processingNames():
				configCode += self.__getitem__(name)._config.code(name, networkVar, fullConfig)
			return configCode
		def appendAttribute(name):
			"""Choses among attribute or subscripting depending whether
			the name can be used as attribute
			TODO: consider also object attributes (description, config...)
			"""
			if not isidentifier(name) :
				return "[\"%s\"]"%name
			return "."+name

		code = ""
		if self._engine.getDescription() != "":
			code += "%s.description = %s\n"%(
				networkVar,
				repr(self._engine.getDescription()),
				)
		code += "\n".join([
			"%s%s = '%s'"%(
				networkVar,
				appendAttribute(name),
				self._engine.processingType(name),
				)
			for name in self._engine.processingNames()])
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
				in self._engine.portConnections()])
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
				in self._engine.controlConnections()])
		return code


	def __setattr__(self, name, type) :
		if name == "description":
			self._engine.setDescription(type)
			return
		if name == "backend":
			self._engine.setBackend(type)
			return
		if name in self.__dict__['methods']:
			raise AssertionError("Wrong processing name: %s is a method"%(name))
		# TODO: fail on existing attributes (not processings)
		self._engine.addProcessing(type, name)

	def __setitem__(self, name, type) :
		self.__setattr__(name, type)

	def __delattr__(self, name):
		if not self._engine.hasProcessing(name) :
			raise AttributeError(name)
		self._engine.deleteProcessing(name)

	def __delitem__(self, name):
		if not self._engine.hasProcessing(name) :
			raise KeyError(name)
		self.__delattr__(name)

	# TODO: Workaround to be able to use the function from the C++ engine
	def xml(self):
		return self._engine.xml()

	def __repr__(self) :
		return self.code()

	def play(self) :
		self._engine.play()

	def pause(self) :
		self._engine.pause()

	def isStopped(self) :
		return self._engine.isStopped()

	def isPlaying(self) :
		return self._engine.isPlaying()

	def isPaused(self) :
		return self._engine.isPaused()

	def stop(self) :
		self._engine.stop()

	def processingNames(self):
		return self._engine.processingNames()

	def load(self, filename):
		self._engine.load(filename)

	def save(self, filename):
		self._engine.save(filename)

	def bindUi(self, pyqt_object) :
		self._engine.bindUi(pyqt_object)

