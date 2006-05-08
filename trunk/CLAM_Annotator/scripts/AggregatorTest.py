# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


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



