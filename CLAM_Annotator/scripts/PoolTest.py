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


from Pool import *
import cStringIO
import unittest
import xml.dom.ext


attributeS1Simple = """\
<AttributePool name='Simple'>7.2</AttributePool>
"""
attributeS1Artist = """\
<AttributePool name='Artist'>
	<Band size='2'>
		<Component>Jerry Wilco</Component>
		<Component>Josh Wilco</Component>
	</Band>
</AttributePool>
"""
attributeS1MoreAttributes = """\
<AttributePool name='AdditionalAttribute' lenght='9'>
	286500 454356 617701 725474 846725 1081347 1278632 1416662 154943
</AttributePool>
"""

document ="""\
<DescriptorsPool>
<ScopePool name='S1' size='1'>\
""" + attributeS1Artist + attributeS1Simple + \
"""\
</ScopePool>
</DescriptorsPool>
"""

document2 ="""\
<DescriptorsPool>
<ScopePool name='S2' size='1'>\
""" + attributeS1MoreAttributes + \
"""\
</ScopePool>
</DescriptorsPool>
"""

documentWithDifferentScopeSize ="""\
<DescriptorsPool>
<ScopePool name='S2' size='3'>\
""" + attributeS1MoreAttributes + \
"""\
</ScopePool>
</DescriptorsPool>
"""


class PoolsTest(unittest.TestCase):
	def setUp(self):
		self.pool = cStringIO.StringIO(document)
		self.pool2 = cStringIO.StringIO(document2)
		self.poolDifferentSize = cStringIO.StringIO(documentWithDifferentScopeSize)

	def serializeXml(self, nodes) :
		output = cStringIO.StringIO()
		for node in nodes :
			xml.dom.ext.Print(node, output)
			print >> output
		return output.getvalue()


	def testGetScopeSize(self) :
		pool = Pool(self.pool)
		scopeSize = pool.GetScopeSize('S1')
		self.assertEqual(1, scopeSize)

	def testGetScopeSize_badScopeName(self) :
		pool = Pool(self.pool)
		try :
			scopeSize = pool.GetScopeSize('BadScope')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual("Scope 'BadScope' not found",e.what)

	def testSelectAttributes(self) :
		pool = Pool(self.pool)

		nodes = pool.SelectAttributes('S1', 'Artist')

		self.assertEquals(attributeS1Artist, self.serializeXml(nodes))

	def testInserAttributeAsNodes(self) :
		pool1 = Pool(self.pool)
		pool2 = Pool(self.pool2)
		nodesToMove = pool2.SelectAttributes('S2','AdditionalAttribute')

		pool1._InsertNode(nodesToMove,'S1')

		nodes = pool1.SelectAttributes('S1', 'AdditionalAttribute')
		self.assertEquals(self.serializeXml(nodesToMove), self.serializeXml(nodes))

	def testInserAttribute(self) :
		pool1 = Pool(self.pool)
		pool2 = Pool(self.pool2)

		pool1.InsertAttribute(pool2,'S2', 'AdditionalAttribute', 'S1')

		result = pool1.SelectAttributes('S1', 'AdditionalAttribute')
		expect = pool2.SelectAttributes('S2', 'AdditionalAttribute')
		self.assertEquals(self.serializeXml(expect), self.serializeXml(result))

	def testInserAttribute_withNonExistingSourceScope(self) :
		pool1 = Pool(self.pool)
		pool2 = Pool(self.pool2)

		try :
			pool1.InsertAttribute(pool2,'BadScope', 'AdditionalAttribute', 'S1')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Attribute 'BadScope::AdditionalAttribute' not found",
				e.what)

	def testInserAttribute_withNonExistingAttribute(self) :
		pool1 = Pool(self.pool)
		pool2 = Pool(self.pool2)

		try :
			pool1.InsertAttribute(pool2,'S2', 'BadAttribute', 'S1')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Attribute 'S2::BadAttribute' not found",
				e.what)

	def testInsertAttribute_differentScopeSizes(self) :
		pool = Pool(self.pool)
		poolDifferentSize = Pool(self.poolDifferentSize)

		try:
			pool.InsertAttribute(poolDifferentSize,'S2', 'AdditionalAttribute', 'S1')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Requested size for scope 'S1' was 3 but it is actually 1",
				e.what)


	def testAssureScopeWithPopulation_existingScopeSameSize(self) :
		pool = Pool(self.pool)
		pool.AssureScopeWithPopulation("S1",1)
		self.assertEqual(1, pool.GetScopeSize("S1"))

	def testAssureScopeWithPopulation_existingScopeDifferentSizes(self) :
		pool = Pool(self.pool)
		try :
			pool.AssureScopeWithPopulation("S1",3)
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Requested size for scope 'S1' was 3 but it is actually 1",
				e.what)

		self.assertEqual(1, pool.GetScopeSize("S1"))

	def testPopulateScope(self) :
		pool = Pool(self.pool)
		pool.PopulateScope("NewScope",4)
		self.assertEqual(4, pool.GetScopeSize("NewScope"))

	def testAssureScopeWithPopulation_newScope(self) :
		pool = Pool(self.pool)
		pool.AssureScopeWithPopulation("NewScope",4)
		self.assertEqual(4, pool.GetScopeSize("NewScope"))

	def testInserAttribute_onANewScope(self) :
		pool1 = Pool(self.pool)
		pool2 = Pool(self.pool2)

		pool1.InsertAttribute(pool2,'S2', 'AdditionalAttribute', 'NewScope')

		result = pool1.SelectAttributes('NewScope', 'AdditionalAttribute')
		expect = pool2.SelectAttributes('S2', 'AdditionalAttribute')
		self.assertEquals(self.serializeXml(expect), self.serializeXml(result))

	def testRemoveAttribute(self) :
		pool1 = Pool(self.pool)

		pool1.RemoveAttribute('S1', 'Simple')

		result = pool1.SelectAttributes('S1', 'Simple')
		self.assertEqual(0,len(result))

	def testRemoveAttribute_scopeDoesNotExists(self) :
		pool1 = Pool(self.pool)

		try :
			pool1.RemoveAttribute('BadScope', 'Simple')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Scope 'BadScope' not found while removing 'BadScope::Simple'",
				e.what)

	def testRemoveAttribute_attributeDoesNotExists(self) :
		pool1 = Pool(self.pool)

		try :
			pool1.RemoveAttribute('S1', 'BadAttribute')
			self.fail("Expected exception was not thrown")
		except Pool.Exception, e:
			self.assertEqual(
				"Attribute 'BadAttribute' not found while removing 'S1::BadAttribute'",
				e.what)

	def testDefaultInit(self) :
		pool = Pool()
		file = cStringIO.StringIO()
		pool.Dump(file)
		self.assertEqual("<?xml version='1.0' encoding='UTF-8'?>\n<DescriptorsPool/>\n", file.getvalue())




if __name__ == "__main__":
	unittest.main()




