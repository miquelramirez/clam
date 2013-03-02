class ProcessingTypes(object):

	def __init__(self, engine):
		self._engine = engine

	def __getattr__(self, name) :
		types = self._engine.availableTypes()
		if name not in types:
			raise AttributeError(name)
		return types[types.index(name)]

	def __dir__(self) :
		return self._engine.availableTypes()



