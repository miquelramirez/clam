#! /usr/bin/python

from testResult import *
import unittest

class TestTestResultSet (unittest.TestCase) :
	def setUp(self):
		self.set = TestResultSet()

	def testConstructor(self) :
		self.assertEqual( 0, self.set.nTests() )
		
	def testnTests_withTwo(self) :
		t1 = t2 = TestResult()
		self.set.add(t1)
		self.set.add(t2)
		self.assertEqual( 2, self.set.nTests() )
		
	def testPassed_default(self) :
		self.assert_( self.set.passed() )
		
	def testPassed_withOneTestNotPassed(self) :
		t = TestResult()
		t.compilationOk( "debug", False )
		self.assert_( not t.passed() )
		self.set.add( t )
		self.assert_ ( not self.set.passed() )
		
	
		
class TestTestResult (unittest.TestCase) :
	def setUp(self):
		self.test = TestResult()

	def testConstructor(self):
		self.assertEqual("unnamed test", self.test.name)

	def testassed_default(self):
		self.assert_( self.test.passed() )

	def testPassed_whenDebugErrors(self): 
		"""passed should return false when debug errors"""
		self.test.compilationOk("debug", False)
		self.assert_(not self.test.passed() )
	
	def testPassed_whenReleaseErrors(self): 
		"""passed should return false when release errors"""
		self.test.compilationOk("release", False)
		self.assert_(not self.test.passed() )


	def XtestPassed_whenNoErrorsSetAndIsAutoTest_Failures(self):
		"if autotest and tests not passed, should return false"
		self.test._isAutoTest = True
		self.test.nFailures(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNFailures() )
	
	def XtestPassed_whenNoErrorsSetAndIsAutoTest_Errors(self):
		"if autotest and tests not passed, should return false"
		self.test._isAutoTest = True
		self.test.nErrors(1)
		assert not self.test.passed()
		self.assertEqual( 1, self.test.getNErrors() )
		
	def XtestPassed_whenNoErrorsSet_IsAutoTest_Passed(self):
		"passed autotests should return true"
		self.test._isAutoTest = True
		assert self.test.passed()
		
	def XtestConfigurationStatus_whithInvalidConfiguration(self):
		"passing configuration not debug or release should raise"
		self.assertRaises( Exception, 
			self.test.compilationStatus, 'debbug', True)
		
	def XtestConfigurationStatus_DebugOk(self):
		self.test._debugCompilationOk = False
		self.test.compilationStatus('debug', True)
		assert self.test._debugCompilationOk

	def XtestConfigurationStatus_ReleaseNotOk(self):
		self.test._debugCompilationOk = True
		self.test.compilationStatus('release', False)
		assert not self.test._releaseCompilationOk
		
	def XtestNWarnings_Debug(self):
		self.test.nWarnings('debug', 2)
		assert 2 == self.test._debugNWarnings
	
	def XtestNWarnings_Release(self):
		self.test.nWarnings('release', 1)
		assert 1 == self.test._releaseNWarnings
	
	def XtestReportedError(self):
		self.test.reportError('debug', 'un error')
		assert 'un error' == self.test._reportedError

	def XtestName(self):
		self.test.name('a test')
		assert 'a test' == self.test._testname


if __name__ == "__main__":
	unittest.main()
	
	
