
class ConfigurationResult :
	def __init__(self) :
		self.compilationOk = True
		
class TestResult :
	"info obtained from execution of a test"
	def __init__(self) :
		self.debug = ConfigurationResult()
		self.release = ConfigurationResult()
		self.name = "unnamed test"
		
	def passed(self):
		return self.debug.compilationOk and self.release.compilationOk 
		
	def compilationOk(self, config, ok):
		self.__checkConfig(config)
		if config == "debug" :
			self.debug.compilationOk = ok
		else :
			self.release.compilationOk = ok
	
	# (pseudo) private methods
	def __checkConfig(self, config) :
		if config not in ["debug", "release"] :
			raise Exception, "wrong configuration name (should be [debug|release])" 

class TestResultSet :
	def __init__(self) :
		self.tests = []

	def nTests(self) :
		return len(self.tests)

	def add(self, test) :
		self.tests.append( test )

	def passed(self) :
		result = True
		for t in self.tests :
			if not t.passed() : 
				return False
		return True
