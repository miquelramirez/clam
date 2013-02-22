class Dummy_ConfigurationProxy(object):
	def __init__(self, config):
		self._default = config.copy()
		self._dict = config
		self._holded = None

	def __getitem__(self, name):
		if type(self._dict[name]) == dict:
			import Configuration
			return Configuration.Configuration(
				Dummy_ConfigurationProxy(self._dict[name]) )
		params = self._holded if self._holded else self._dict
		return params[name]

	def __setitem__(self, name, value):
		if value is None: # No type check
			self._dict[name] = None
			return
		if type(value) != type(self._default[name]):
			raise TypeError("%s value expected, got %s"%(
				type(self._default[name]).__name__, type(value).__name__))
		self._dict[name] = value

	def keys(self):
		return self._dict.keys()

	def nonDefault(self, attribute):
		return not self._default[attribute] == self._dict[attribute]

	def clone(self):
		return Dummy_ConfigurationProxy( self._dict.copy() )

	def hold(self) :
		assert self._holded is None, \
			"Configuration is already held"
		self._holded = self._dict.copy()

	def apply(self) :
		self._holded = None

	def discard(self) :
		"""Discards pending changes to apply in a held configuration"""
		assert self._holded is not None, \
			"Discarding a configuration requires to be held"
		self._dict = self._holded
		self._holded = None

