#! /usr/bin/python

class TestResult :
	"info obtained from execution of a test"
	def __init__(self) :
		self._currentConfig = "debug"
		self._testname = "unnamed test"
		self._isAutoTest = False
		self._autoTestNFailures = 0
		self._autoTestNErrors = 0
		self._autoTestNTests = 0
		self._debugCompilationOk = True
		self._releaseCompilationOk = True
		self._debugNWarnings = 0
		self._releaseNWarnings = 0
		self._isMandatory = True # TODO useit
		self._reportedError = ""

	def currentConfig(self, config) :
		self._currentConfig = config
		
	def passed(self):
		if not self._debugCompilationOk :
			return False
		if not self._releaseCompilationOk :
			return False
		if self._isAutoTest :
			return not self._autoTestNFailures and not self._autoTestNErrors
		return True
	def __checkConfiguration__(self, configuration):
		if not configuration in ["debug", "release"] :
			raise Exception, 'compilationStatus with a wrong configuration name'
	def compilationStatus(self, configuration, ok):
		self.__checkConfiguration__(configuration)
		if configuration == "debug" :
			self._debugCompilationOk = ok
		else : # release
			self._releaseCompilationOk = ok
		
	def nWarnings(self, configuration, num):
		self.__checkConfiguration__(configuration)
		if configuration == "debug" :
			self._debugNWarnings = num
		else : # release
			self._releaseNWarnings = num

	def reportError(self, configuration, error):
		self.__checkConfiguration__(configuration)
		self._reportedError = error

	def name(self, aName) :
		self._testname = aName
	
	def nFailures(self, n) :
		self._autoTestNFailures = n
	
	def nErrors(self, n) :
		self._autoTestNErrors = n
	
	def nTests(self, n) :
		self._autoTestNTests = n
	
	def getNFailures(self) :
		return self._autoTestNFailures
	
	def getNErrors(self) :
		return self._autoTestNErrors 
	
	def getNTests(self) :
		return self._autoTestNTests 

#--------------------
# Tests

import unittest

class TestTestResult (unittest.TestCase) :
	def setUp(self):
		self.test = TestResult()

	def testConstructor(self):
		self.assertEqual("unnamed test", self.test._testname)

	def testPassed_whenDebugErrors(self): 
		"""passed should return false when debug errors"""
		self.test._debugCompilationOk = False
		self.assertEqual(False, self.test.passed())
	
	def testPassed_whenReleaseErrors(self): 
		"""passed should return false when release errors"""
		self.test._releaseCompilationOk = False
		self.assertEqual(False, self.test.passed())

	def testPassed_whenNoErrorsSet(self):
		assert  self.test.passed()

	def testPassed_whenNoErrorsSetAndIsAutoTest_Failures(self):
		"if autotest and tests not passed, should return false"
		self.test._isAutoTest = True
		self.test.nFailures(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNFailures() )
	
	def testPassed_whenNoErrorsSetAndIsAutoTest_Errors(self):
		"if autotest and tests not passed, should return false"
		self.test._isAutoTest = True
		self.test.nErrors(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNErrors() )
		
	def testPassed_whenNoErrorsSet_IsAutoTest_Passed(self):
		"passed autotests should return true"
		self.test._isAutoTest = True
		assert self.test.passed()
		
	def testConfigurationStatus_whithInvalidConfiguration(self):
		"passing configuration not debug or release should raise"
		self.assertRaises( Exception, 
			self.test.compilationStatus, 'debbug', True)
		
	def testConfigurationStatus_DebugOk(self):
		self.test._debugCompilationOk = False
		self.test.compilationStatus('debug', True)
		assert self.test._debugCompilationOk

	def testConfigurationStatus_ReleaseNotOk(self):
		self.test._debugCompilationOk = True
		self.test.compilationStatus('release', False)
		assert not self.test._releaseCompilationOk
		
	def testNWarnings_Debug(self):
		self.test.nWarnings('debug', 2)
		assert 2 == self.test._debugNWarnings
	
	def testNWarnings_Release(self):
		self.test.nWarnings('release', 1)
		assert 1 == self.test._releaseNWarnings
	
	def testReportedError(self):
		self.test.reportError('debug', 'un error')
		assert 'un error' == self.test._reportedError

	def testName(self):
		self.test.name('a test')
		assert 'a test' == self.test._testname


if __name__ == "__main__":
	unittest.main()
	
	
