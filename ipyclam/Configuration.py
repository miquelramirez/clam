
class Configuration(object):
	"""A Configuration represents the static parameters of an Processing.
	Parameters are typed. If they are optional they can be None.
	You can obtain the configuration from an existing processing.
	Configurations maybe bound, unbound or hold.
	When a configuration is unbound they are unrelated to any processing.
	When they are bound to a processing, any change to a parameter
	reconfigures the processing. This can be quite bad if your are
	changing many parameters and the configuration is expensive,
	so you can hold() a configuration and then apply().
	"""

	def __init__(self, proxy):
		object.__setattr__(self, "_proxy", proxy)

	def __getitem__(self, name):
		return self._proxy[name]

	def __setitem__(self, name, value):
		if name == "_config":
			self._proxy.copyConfig( value._proxy )
			return
		self._proxy[name] = value

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
		return self._proxy.keys()

	def code(self, processingName, networkVar = "network", fullConfig = False):
		code = ""
		code += "\n".join([
				"%s.%s['%s'] = %s"%(
					networkVar, 
					processingName,
					attribute,
					self._proxy[attribute].__repr__(),
					)
				for attribute in self._proxy.keys()
				if fullConfig or self._proxy.nonDefault(attribute)
			])
		if code: code += "\n"
		return code

	def hold(self):
		self._proxy.hold()

	def apply(self):
		self._proxy.apply()

	def clone(self):
		return Configuration( self._proxy.clone() )

