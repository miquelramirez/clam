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
		
	def testStabilityLevel_default(self) :
		self.assert_( self.set.stabilityLevel() == DoesntCompile )
		
	def testPassed_withOneTestNotPassed(self) :
		t = TestResult()
		t.compilationOk( "debug", False )
		assert( not t.compiles() )
		self.set.add( t )
		assert ( not self.set.compiles() )

	def testPassUnitTests_default(self):
		assert( self.set.passUnitTests() )

	def testPassUnitTests_withOneTestWithFailures(self) :
		t = TestResult()
		t.unitTestsFailures("debug", 1)
		self.set.add( t )
		assert (not self.set.passUnitTests() )

	def testStabilityLevel(self) :
		t = TestResult()
		t.unitTestsFailures("debug", 1)
		self.set.add( t )
		expected = "Reaches level 1 (Everything compiles), but don't reach level 2 (Pass unit tests)"
		assert expected == self.set.stabilityLevelString(PassUnitTests), self.set.stabilityLevelString(PassUnitTests)
	
	def testStabilityLevelReachesPassFunctionalTests(self) :
		t = TestResult()
		self.set.add( t )
		expected = "Reaches level 3 (Pass functional tests), but don't reach level 4 (Clean of warnings)"
		assert expected == self.set.stabilityLevelString(CleanOfWarnings), self.set.stabilityLevelString(CleanOfWarnings)
		
class TestTestResult (unittest.TestCase) :
	def setUp(self):
		self.test = TestResult()

	def testConstructor(self):
		self.assertEqual("unnamed test", self.test.name)

	def testCompiles_default(self):
		self.assert_( self.test.compiles() )

	def testCompiles_whenDebugErrors(self): 
		"""compiles should return false when debug errors"""
		self.test.compilationOk("debug", False)
		self.assert_(not self.test.compiles() )
	
	def testCompiles_whenReleaseErrors(self): 
		"""compiles should return false when release errors"""
		self.test.compilationOk("release", False)
		self.assert_(not self.test.compiles() )

	def testPassUnitTests_default(self):
		self.assert_( self.test.passUnitTests() )
		
	def testPassUnitTests_whenDebugFailures(self):
		self.test.unitTestsFailures("debug", 1)
		self.assert_( not self.test.passUnitTests() )

	def testPassUnitTests_whenReleaseFailures(self):
		self.test.unitTestsFailures("release", 1)
		self.assert_( not self.test.passUnitTests() )

	def testPassFuncTests_trueByDefault(self):
		self.assert_( self.test.passFunctionalTests() )

	def testPassFuncTests_whenDebugFailures(self):
		self.test.functionalTestsFailures("debug", 1)
		self.assert_( not self.test.passFunctionalTests() )

if __name__ == "__main__":
	unittest.main()
	
	
