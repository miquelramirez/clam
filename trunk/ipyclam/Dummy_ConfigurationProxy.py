class Dummy_ConfigurationProxy(object):
	def __init__(self, config):
		self._default = config.copy()
		self._dict = config

	def __getitem__(self, name):
		if type(self._dict[name]) == dict:
			import Configuration
			return Configuration.Configuration( Dummy_ConfigurationProxy(self._dict[name]) )
		return self._dict[name]

	def __setitem__(self, name, value):
		if value == None:
			self._dict[name] = value
			return
		if type(value) != type(self._default[name]):
			raise TypeError("%s value expected, got %s"%( type(self._default[name]).__name__, type(value).__name__))
		self._dict[name] = value

	def keys(self):
		return self._dict.keys()

	def nonDefault(self, attribute):
		return not self._default[attribute] == self._dict[attribute]

	def clone(self):
		return Dummy_ConfigurationProxy( self._dict.copy() )

