class ProcessingTypes(object):

	def __init__(self, proxy):
		self._proxy = proxy

	def __getattr__(self, name) :
		types = self._proxy.availableTypes()
		if name not in types:
			raise AttributeError(name)
		return types[types.index(name)]

	def __dir__(self) :
		return self._proxy.availableTypes()



