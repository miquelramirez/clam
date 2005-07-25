
class ConfigurationResult :
	def __init__(self) :
		self.compilationOk = True
		self.unitTestsFailures = 0 
		self.functionalTestsFailures = 0 
		
class TestResult :
	"info obtained from execution of a test"
	def __init__(self) :
		self.debug = ConfigurationResult()
		self.release = ConfigurationResult()
		self.name = "unnamed test"
		
	def compiles(self):
		return self.debug.compilationOk and self.release.compilationOk 
	
	def passUnitTests(self): 
		return self.debug.unitTestsFailures == 0 and self.release.unitTestsFailures==0

	def passFunctionalTests(self):
		return self.debug.functionalTestsFailures == 0 and self.release.functionalTestsFailures==0
	
	def compilationOk(self, config, ok):
		self.__checkConfig(config)
		if config == "debug" :
			self.debug.compilationOk = ok
		else :
			self.release.compilationOk = ok

	def unitTestsFailures(self, config, nfailures):
		self.__checkConfig(config)
		if config == "debug" :
			self.debug.unitTestsFailures = nfailures
		else :
			self.release.unitTestsFailures = nfailures

	def functionalTestsFailures(self, config, nfailures):
		self.__checkConfig(config)
		if config == "debug" :
			self.debug.functionalTestsFailures = nfailures
		else :
			self.release.functionalTestsFailures = nfailures
	
	# (pseudo) private methods
	def __checkConfig(self, config) :
		if config not in ["debug", "release"] :
			raise Exception, "wrong configuration name (should be [debug|release])" 

# Stability levels constants definitions:
DoesntCompile = 0
Compiles = 1
PassUnitTests = 2
PassFunctionalTests = 3
CleanOfWarnings = 4
# etc: AppsExecutionOk, valgrind-related

class TestResultSet :
	def __init__(self) :
		self.tests = []
		self.levelDic = { DoesntCompile:"Ground level",
			Compiles:"Everything compiles",
			PassUnitTests:"Pass unit tests",
			PassFunctionalTests:"Pass functional tests",
			CleanOfWarnings:"Clean of warnings" }

	def nTests(self) :
		return len(self.tests)

	def add(self, test) :
		self.tests.append( test )

	def compiles(self) :
		result = True
		if len(self.tests)==0 :
			return False
		for t in self.tests :
			if not t.compiles() : 
				return False
		return True
		
	def passUnitTests(self) :
		for t in self.tests :
			if not t.passUnitTests():
				return False
		return True

	def passFunctionalTests(self) :
		for t in self.tests :
			if not t.passFunctionalTests():
				return False
		return True
	
	def stabilityLevel(self) :
		assert DoesntCompile < Compiles, "bad order in stability levels"
		assert Compiles < PassUnitTests, "bad order in stability levels"
		assert PassUnitTests < PassFunctionalTests, "bad order in stability levels"

		if not self.compiles() :
			return DoesntCompile
		elif not self.passUnitTests() :
			return Compiles
		elif not self.passFunctionalTests() :
			return PassUnitTests
		else :
			return PassFunctionalTests


	def stabilityLevelList(self) :
		return "Levels list = [%s]\n" % ", ".join(self.levelDic.values() )

	def stabilityLevelString(self, levelNeeded) :
		currentLevel = self.stabilityLevel()
		if currentLevel >= levelNeeded :
			msg = "Stable level! : Reaches level %i (%s), and stability level is fixed on %i (%s)"
		else :
			msg = "UNSTABLE level! : Reaches level %i (%s), but doesn't reach stability level fixed on %i (%s)"
	
		return msg % (currentLevel, self.levelDic[currentLevel], levelNeeded, self.levelDic[levelNeeded])
