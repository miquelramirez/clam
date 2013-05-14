#!/usr/bin/python
from decorator import decorator

@decorator
def notified(f, self, *args, **kwd) :
	method = getattr(self.__dict__['adaptee'],f.__name__)
	result = method(*args, **kwd)
	self.notify()
	return result

class Notifier(object) :

	def __init__(self, adaptee) :
		self.adaptee = adaptee

	def __getattr__(self, name) :
		return Notifier.MethodAdapter(self.adaptee, name)


	class MethodAdapter(object) :
		def __init__(self, receptor, method) :
			self.receptor = receptor
			self.method = method
		def __call__(self, *args, **kwd) :
			method = getattr(self.receptor, self.method)
			return method(*args, **kwd)

class Notifier_EngineDecorator(Notifier) :
	def __init__(self, adaptee, callback) :
		super(Notifier_EngineDecorator, self).__init__(adaptee)
		self._callback = callback
	
	@notified
	def processingConfig(self, name) : pass

	@notified
	def renameProcessing(self, oldName, newName): pass

	@notified
	def connectorIndex(self, unitName, kind, direction, connectorName) :
		pass

	@notified
	def addProcessing(self, type, name) : pass

	@notified
	def processingHasConnector(self,
			unitName, kind, direction, connectorName):
		pass

	@notified
	def connect(self,
			kind, fromUnit, fromConnector, toUnit, toConnector) :
		pass

	@notified
	def disconnect(self, kind,
			fromUnit, fromConnector,
			toUnit, toConnector) :
		pass

	@notified
	def setDescription(self, description): pass

	@notified
	def deleteProcessing(self, unitName): pass

	def notify(self) :
		self._callback()




