class IpyclamRTException(Exception) :
	def __str__(self) :
		return self.message

class NameAlreadyExists(IpyclamRTException):
	def __init__(self, name) :
		self.processingName = name
		self.message = "Name '{0}' already exists".format(name)

class BadProcessingType(IpyclamRTException):
	def __init__(self, type):
		self.type = type
		self.message = "Type '{0}' is not available".format(type)

class ProcessingNotFound(IpyclamRTException):
	def __init__(self, name) :
		self.processingName = name
		self.message = "Processing '{0}' not found".format(name)


