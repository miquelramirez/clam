import cStringIO
import unittest
from Aggregator import *

class AggregatorTest(unittest.TestCase):

	def setUp(self) :
		pass

	def helperTestParser(self, input) :
		aggregator = Aggregator(cStringIO.StringIO(input))
		sink = cStringIO.StringIO()
		aggregator.dump(sink)
		return sink.getvalue()

	def testComments(self) :
		input = \
			"# Informative Comment\n" + \
			"copy 1 scope1::attribute1 scope1::attribute1\n" + \
			"copy 2 scope1::attribute1 scope2::attribute2\n" + \
			"copy 3 scope1::attribute1 scope3::attribute3\n" + \
			"#copy 1 scope5::attribute4 scope3::attribute4\n"

		output = self.helperTestParser(input)
		self.assertEqual( \
			"copy 1 scope1::attribute1 scope1::attribute1\n" + \
			"copy 2 scope1::attribute1 scope2::attribute2\n" + \
			"copy 3 scope1::attribute1 scope3::attribute3\n"
			, output)


	def testTrailingSpaces(self) :
		input = \
			"\n" + \
			"   copy 1 scope1::attribute1 scope1::attribute1\n" + \
			"copy 2 scope1::attribute1    scope2::attribute2\n" + \
			"copy 3 scope1::attribute1 scope3::attribute3   \n" + \
			"\n"

		output = self.helperTestParser(input)
		self.assertEqual( \
			"copy 1 scope1::attribute1 scope1::attribute1\n" + \
			"copy 2 scope1::attribute1 scope2::attribute2\n" + \
			"copy 3 scope1::attribute1 scope3::attribute3\n"
			, output)

	def testSyntaxError(self) :
		input = \
			"copy 1 scope1::attribute1 scope1::attribute1\n" + \
			"cpy 2 scope1::attribute1 scope2::attribute2\n" + \
			"copy 3 scope1::attribute1 scope3::attribute3   \n" + \
			"\n"
		try :
			output = self.helperTestParser(input)
			self.fail("Expected exception was not thrown")
		except Aggregator.Exception, e:
			self.assertEqual("Parser error at line 2\n"+ \
				">cpy 2 scope1::attribute1 scope2::attribute2\n",e.what)

	# TODO: Testing run
	# TODO: Testing run with less sources than required



if __name__ == "__main__":
	unittest.main()



