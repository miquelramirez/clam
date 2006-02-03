from Schema import *
import cStringIO
import unittest
import xml.dom.ext

def stringAttribute(scope,name) :
	return """\
<Attribute scope='%s' name='%s' type='String'/>
"""%(scope,name)

def enumAttribute(scope, name, values) :
	return """\
<Attribute scope='%s' name='%s' type='Enum'>
	<Values size='%d'>%s</Values>
</Attribute>
"""%(scope, name, len(values), " ".join(values))

document ="""\
<DescriptionScheme>
<Uri>http://my.uri.com</Uri>
<Attributes>\
""" + \
	stringAttribute("S1","StrA") + \
	stringAttribute("S1","StrB") + \
	stringAttribute("S2","StrA") + \
"""\
</Attributes>
</DescriptionScheme>
"""

document2 ="""\
<DescriptionScheme>
<Uri>http://my.uri.com</Uri>
<Attributes>\
""" + \
	stringAttribute("S1","StrZ") + \
	stringAttribute("S1","EnumX") + \
	stringAttribute("S2","EnumY") + \
"""\
</Attributes>
</DescriptionScheme>
"""

class SchemaTest(unittest.TestCase):
	def setUp(self):
		self.schema = cStringIO.StringIO(document)
		self.schema2 = cStringIO.StringIO(document2)

	def serializeXml(self, nodes) :
		output = cStringIO.StringIO()
		for node in nodes :
			xml.dom.ext.Print(node, output)
			print >> output
		return output.getvalue()

	def testSelectAttribute(self) :
		schema = Schema(self.schema)

		nodes = schema.SelectAttribute('S1', 'StrA')

		self.assertEquals(stringAttribute('S1', 'StrA'), self.serializeXml(nodes))


	def testInserAttribute(self) :
		schema1 = Schema(self.schema)
		schema2 = Schema(self.schema2)

		schema1.InsertAttribute(schema2,'S2', 'EnumY', 'S2')

		result = schema1.SelectAttribute('S2', 'EnumY')
		expect = schema2.SelectAttribute('S2', 'EnumY')
		self.assertEquals(self.serializeXml(expect), self.serializeXml(result))


	def testInserAttribute_withNonExistingSourceScope(self) :
		schema1 = Schema(self.schema)
		schema2 = Schema(self.schema2)

		try :
			schema1.InsertAttribute(schema2,'BadScope', 'BadAttribute', 'S1')
			self.fail("Expected exception was not thrown")
		except Schema.Exception, e:
			self.assertEqual(
				"Attribute 'BadScope::BadAttribute' not found",
				e.what)

	def testDeleteAttribute(self) :
		schema2 = Schema(self.schema2)

		schema2.RemoveAttribute('S2', 'EnumY')

		result = schema2.SelectAttribute('S2', 'EnumY')
		self.assertEqual(0, len(result))

	def testDeleteAttribute_doesNotExist(self) :
		schema2 = Schema(self.schema2)

		try :
			schema2.RemoveAttribute('BadScope', 'BadAttribute')
			self.fail("Expected exception was not thrown")
		except Schema.Exception, e:
			self.assertEqual(
				"Attribute 'BadScope::BadAttribute' not found",
				e.what)


	def testDefaultInit(self) :
		schema = Schema()
		file = cStringIO.StringIO()
		schema.Dump(file)
		self.assertEqual("""\
<?xml version='1.0' encoding='UTF-8'?>
<DescriptionScheme>
  <Attributes/>
</DescriptionScheme>
""",
		file.getvalue())


if __name__ == "__main__":
	unittest.main()




