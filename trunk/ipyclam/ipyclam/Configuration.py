
class Configuration(object):
	"""A Configuration represents the static parameters of a Processing.
	Parameters are typed. If they are optional they can be None.
	You can obtain the configuration from an existing processing.
	Configurations maybe bound, unbound or hold.
	When a configuration is unbound they are unrelated to any processing.
	When they are bound to a processing, any change to a parameter
	reconfigures the processing. This can be quite bad if your are
	changing many parameters and the configuration is expensive,
	so you can hold() a bound configuration and then apply().
	"""

	def __init__(self, engine):
		object.__setattr__(self, "_engine", engine)

	def __getitem__(self, name):
		return self._engine[name]

	def __setitem__(self, name, value):
		if name == "_config":
			self._engine.copyConfig( value._engine )
			return
		self._engine[name] = value

	def __getattr__(self, name):
		try:
			return self.__getitem__(name)
		except KeyError, e:
			raise AttributeError(e.args[0])

	def __setattr__(self, name, value):
		try:		
			self.__setitem__(name, value)
		except KeyError, e:
			raise AttributeError(e.args[0])
	def __dir__(self):
		return self._engine.keys()

	def code(self, processingName, networkVar = "network", fullConfig = False):
		code = ""
		code += "\n".join([
				"%s.%s['%s'] = %s"%(
					networkVar, 
					processingName,
					attribute,
					self._engine[attribute].__repr__(),
					)
				for attribute in self._engine.keys()
				if fullConfig or self._engine.nonDefault(attribute)
			])
		if code: code += "\n"
		return code

	def hold(self):
		self._engine.hold()

	def apply(self):
		self._engine.apply()

	def clone(self):
		return Configuration( self._engine.clone() )

	# TODO Untested
	def __enter__(self) :
		self.hold()
		return self

	# TODO Untested
	def __exit__(self, type, value, traceback) :
		if type is None :
			self.apply()




