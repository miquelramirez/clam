#! /usr/bin/python

class TestResult :
	"info obtained from execution of a test"
	def __init__(self) :
		self.testname = "unnamed test"
		self.isAutoTest = False
		self.autoTestNFailures = 0
		self.autoTestNErrors = 0
		self.autoTestNTests = 0
		self.debugCompilationOk = True
		self.releaseCompilationOk = True
		self.debugNWarnings = 0
		self.releaseNWarnings = 0
		self.isMandatory = True
		self.reportedError = ""

	def passed(self):
		if not self.debugCompilationOk :
			return False
		if not self.releaseCompilationOk :
			return False
		if self.isAutoTest :
			return not self.autoTestNFailures and not self.autoTestNErrors
		return True
	def __checkConfiguration__(self, configuration):
		if not configuration in ["debug", "release"] :
			raise Exception, 'compilationStatus with a wrong configuration name'
	def compilationStatus(self, configuration, ok):
		self.__checkConfiguration__(configuration)
		if configuration == "debug" :
			self.debugCompilationOk = ok
		else : # release
			self.releaseCompilationOk = ok
		
	def nWarnings(self, configuration, num):
		self.__checkConfiguration__(configuration)
		if configuration == "debug" :
			self.debugNWarnings = num
		else : # release
			self.releaseNWarnings = num

	def reportError(self, configuration, error):
		self.__checkConfiguration__(configuration)
		self.reportedError = error

	def name(self, aName) :
		self.testname = aName
	
	def nFailures(self, n) :
		self.autoTestNFailures = n
	
	def nErrors(self, n) :
		self.autoTestNErrors = n
	
	def nTests(self, n) :
		self.autoTestNTests = n
	
	def getNFailures(self) :
		return self.autoTestNFailures
	
	def getNErrors(self) :
		return self.autoTestNErrors 
	
	def getNTests(self) :
		return self.autoTestNTests 

#--------------------
# Tests

import unittest

class TestTestResult (unittest.TestCase) :
	def setUp(self):
		self.test = TestResult()

	def testConstructor(self):
		self.assertEqual("unnamed test", self.test.testname)

	def testPassed_whenDebugErrors(self): 
		"""passed should return false when debug errors"""
		self.test.debugCompilationOk = False
		self.assertEqual(False, self.test.passed())
	
	def testPassed_whenReleaseErrors(self): 
		"""passed should return false when release errors"""
		self.test.releaseCompilationOk = False
		self.assertEqual(False, self.test.passed())

	def testPassed_whenNoErrorsSet(self):
		assert  self.test.passed()

	def testPassed_whenNoErrorsSetAndIsAutoTest_Failures(self):
		"if autotest and tests not passed, should return false"
		self.test.isAutoTest = True
		self.test.nFailures(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNFailures() )
	
	def testPassed_whenNoErrorsSetAndIsAutoTest_Errors(self):
		"if autotest and tests not passed, should return false"
		self.test.isAutoTest = True
		self.test.nErrors(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNErrors() )
		
	def testPassed_whenNoErrorsSet_IsAutoTest_Passed(self):
		"passed autotests should return true"
		self.test.isAutoTest = True
		assert self.test.passed()
		
	def testConfigurationStatus_whithInvalidConfiguration(self):
		"passing configuration not debug or release should raise"
		self.assertRaises( Exception, 
			self.test.compilationStatus, 'debbug', True)
		
	def testConfigurationStatus_DebugOk(self):
		self.test.debugCompilationOk = False
		self.test.compilationStatus('debug', True)
		assert self.test.debugCompilationOk

	def testConfigurationStatus_ReleaseNotOk(self):
		self.test.debugCompilationOk = True
		self.test.compilationStatus('release', False)
		assert not self.test.releaseCompilationOk
		
	def testNWarnings_Debug(self):
		self.test.nWarnings('debug', 2)
		assert 2 == self.test.debugNWarnings
	
	def testNWarnings_Release(self):
		self.test.nWarnings('release', 1)
		assert 1 == self.test.releaseNWarnings
	
	def testReportedError(self):
		self.test.reportError('debug', 'un error')
		assert 'un error' == self.test.reportedError

	def testName(self):
		self.test.name('a test')
		assert 'a test' == self.test.testname


if __name__ == "__main__":
	unittest.main()
	
	
