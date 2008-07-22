from FileMetadataSource import *
import unittest
import os

class FileMetadataSourceTest(unittest.TestCase):
	def setUp(self):
		pass

	def testQuerySchema(self) :
		source1 = FileMetadataSource("testdata/FileSource1/")
		expected = cStringIO.StringIO()
		Schema(file("testdata/FileSource1/schema.sc")).Dump(expected)
		result = cStringIO.StringIO()
		source1.QuerySchema().Dump(result)
		self.assertEquals(expected.getvalue(), result.getvalue())

	def testQuerySchema_noSchemaInSource(self) :
		try :
			source1 = FileMetadataSource("testdata/BadSource/")
			self.fail("Should have thrown an exception")
		except FileMetadataSource.InvalidSchemaException, e:
			self.assertEquals(str(e), "Schema file 'testdata/BadSource/schema.sc' not found.")

	def testQueryDescriptors_nonExistingSong(self) :
		source1 = FileMetadataSource("testdata/FileSource1/")
		try :
			source1.QueryDescriptors('BadId').Dump()
			self.fail("Should have thrown an exception")
		except FileMetadataSource.SongNotFoundException, e :
			self.assertEquals(str(e), "No descriptors found for song id 'BadId'.")

	def testQueryDescriptors_existingSong(self) :
		source1 = FileMetadataSource("testdata/FileSource1/")
		expected = cStringIO.StringIO()
		Pool(file("testdata/FileSource1/id1.pool")).Dump(expected)
		result = cStringIO.StringIO()
		source1.QueryDescriptors("id1").Dump(result)
		self.assertEquals(expected.getvalue(), result.getvalue())

	def testQueryDescriptors_differentSong(self) :
		source1 = FileMetadataSource("testdata/FileSource1/")
		expected = cStringIO.StringIO()
		Pool(file("testdata/FileSource1/id2.pool")).Dump(expected)
		result = cStringIO.StringIO()
		source1.QueryDescriptors("id2").Dump(result)
		self.assertEquals(expected.getvalue(), result.getvalue())

	def testQueryDescriptors_differentSource(self) :
		source1 = FileMetadataSource("testdata/FileSource2/")
		expected = cStringIO.StringIO()
		Pool(file("testdata/FileSource2/id1.pool")).Dump(expected)
		result = cStringIO.StringIO()
		source1.QueryDescriptors("id1").Dump(result)
		self.assertEquals(expected.getvalue(), result.getvalue())

	def testUpdateDescriptors_whenNoPreviousPoolExists(self):
		source1 = FileMetadataSource("testdata/FileSource1/")

		poolToInsert = Pool(file("testdata/FileSource1/id1.pool"))
		source1.UpdateDescriptors("newId", poolToInsert)

		expected = cStringIO.StringIO()
		poolToInsert.Dump(expected)
		result = cStringIO.StringIO()
		source1.QueryDescriptors("newId").Dump(result)
		self.assertEquals(expected.getvalue(), result.getvalue())

		os.remove('testdata/FileSource1/newId.pool')

if __name__ == "__main__":
	unittest.main()






