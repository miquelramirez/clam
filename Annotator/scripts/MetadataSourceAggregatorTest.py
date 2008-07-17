from MetadataSourceAggregator import *
from FileMetadataSource import *
import unittest
import os

sources = [
	("source1", FileMetadataSource("data/FileSource1/")),
	("source2", FileMetadataSource("data/FileSource2/")),
	("source3", FileMetadataSource("data/FileSource3/")),
]

map = [
	("Song::Artist", "source1", "File::Singer"),
	("Song::Title", "source1", "File::Name"),
	("Song::Genre", "source1", "File::Style"),
	("Song::DynamicComplexity", "source2", "WaveFile::Complexity"),
	("Frame::Energy", "source3", "Frame::SpectralEnergy"),
]

badMap = map + [("BadSourceScope::At", "badSource", "BadSourceScope::At")]

class MetadataSourceAggregatorTest(unittest.TestCase):

	def dumpXml(self, dom) :
		output = StringIO()
		dom.Dump(output)
		return output.getvalue()

	def setUp(self) :
		files=[
			"data/FileSource1/data-newId.pool",
			"data/FileSource2/data-newId.pool",
			"data/FileSource3/data-newId.pool",
		]
		for file in files :
			if os.access(file, os.F_OK) :
				os.remove(file)
	def tearDown(self):
		files=[
			"data/FileSource1/data-newId.pool",
			"data/FileSource2/data-newId.pool",
			"data/FileSource3/data-newId.pool",
		]
		for file in files :
			if os.access(file, os.F_OK) :
				os.remove(file)

	def testAggregatorScriptBuilding_partialFirstSource(self) :
		provider = MetadataSourceAggregator(sources, map)
		(script, sourceIds) = provider._AggregatorScriptFor(["Song::Artist"])
		self.assertEquals( 
			"copy 1 File::Singer Song::Artist\n" +
			"", script)
		self.assertEquals(["source1"], sourceIds)

	def testAggregatorScriptBuilding_partialSecondSource(self) :
		provider = MetadataSourceAggregator(sources, map)
		(script, sourceIds) = provider._AggregatorScriptFor(["Song::DynamicComplexity"])
		self.assertEquals( 
			"copy 1 WaveFile::Complexity Song::DynamicComplexity\n" +
			"", script)
		self.assertEquals(["source2"], sourceIds)

	def testAggregatorScriptBuilding_partialThirdSource(self) :
		provider = MetadataSourceAggregator(sources, map)
		(script, sourceIds) = provider._AggregatorScriptFor(["Frame::Energy"])
		self.assertEquals( 
			"copy 1 Frame::SpectralEnergy Frame::Energy\n" +
			"", script)
		self.assertEquals(["source3"], sourceIds)

	def testAggregatorScriptBuilding_severalSources(self) :
		provider = MetadataSourceAggregator(sources, map)
		(script, sourceIds) = provider._AggregatorScriptFor(
			[
			"Song::Artist",
			"Song::Title",
			"Song::DynamicComplexity",
			])
		self.assertEquals( 
			"copy 1 File::Singer Song::Artist\n" +
			"copy 1 File::Name Song::Title\n" +
			"copy 2 WaveFile::Complexity Song::DynamicComplexity\n" +
			"", script)
		self.assertEquals(["source1","source2"], sourceIds)

	def testQuerySchema_noDescriptor(self) :
		provider = MetadataSourceAggregator(sources, map)
		schema = provider.QuerySchema([])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptionScheme>\n"+
			"  <Attributes/>\n"+
			"</DescriptionScheme>\n" +
			"", self.dumpXml(schema))

	def testQuerySchema_singleSource(self) :
		provider = MetadataSourceAggregator(sources, map)
		schema = provider.QuerySchema(["Song::Artist"])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptionScheme>\n"+
			"  <Attributes>\n"+
			"    <Attribute scope='Song' name='Artist' type='String'/>\n"+
			"  </Attributes>\n"+
			"</DescriptionScheme>\n" +
			"", self.dumpXml(schema))

	def testQuerySchema_otherSource(self) :
		provider = MetadataSourceAggregator(sources, map)
		schema = provider.QuerySchema(
			[
			"Song::DynamicComplexity",
			])
		self.assertEquals(
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptionScheme>\n"+
			"  <Attributes>\n"+
			"    <Attribute scope='Song' name='DynamicComplexity' type='Float'>\n"+
			"      <fRange>\n"+
			"        <Min>0</Min>\n"+
			"        <Max>10</Max>\n"+
			"      </fRange>\n"+
			"    </Attribute>\n"+
			"  </Attributes>\n"+
			"</DescriptionScheme>\n" +
			"", self.dumpXml(schema))

	def testQuerySchema_severalSources(self) :
		provider = MetadataSourceAggregator(sources, map)
		schema = provider.QuerySchema(
			[
			"Song::Artist",
			"Song::Title",
			"Song::DynamicComplexity"
			])
		self.assertEquals(
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptionScheme>\n"+
			"  <Attributes>\n"+
			"    <Attribute scope='Song' name='Artist' type='String'/>\n"+
			"    <Attribute scope='Song' name='Title' type='String'/>\n"+
			"    <Attribute scope='Song' name='DynamicComplexity' type='Float'>\n"+
			"      <fRange>\n"+
			"        <Min>0</Min>\n"+
			"        <Max>10</Max>\n"+
			"      </fRange>\n"+
			"    </Attribute>\n"+
			"  </Attributes>\n"+
			"</DescriptionScheme>\n" +
			"", self.dumpXml(schema))

	def testQueryDescriptors_noDescriptor(self) :
		provider = MetadataSourceAggregator(sources, map)
		descriptors = provider.QueryDescriptors('id1', [])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool/>\n" +
			"", self.dumpXml(descriptors))

	def testQueryDescriptors_singleDescriptor(self) :
		provider = MetadataSourceAggregator(sources, map)
		descriptors = provider.QueryDescriptors('id1', ["Song::Artist"])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testQueryDescriptors_multipleDescriptors(self) :
		provider = MetadataSourceAggregator(sources, map)
		descriptors = provider.QueryDescriptors('id1',
			[
			"Song::Artist",
			"Song::Title",
			"Song::DynamicComplexity",
			])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 1</AttributePool>\n"+
			"    <AttributePool name='Title'>Title 1</AttributePool>\n"+
			"    <AttributePool name='DynamicComplexity'>8.1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testDisgregatorScriptsBuilding_severalSources(self) :
		provider = MetadataSourceAggregator(sources, map)
		script = provider._DisgregatorScripts(
			[
			"Song::Artist",
			"Song::Title",
			"Song::DynamicComplexity",
			])
		self.assertEquals( {
			"source1":
			"copy 1 Song::Artist File::Singer\n" +
			"copy 1 Song::Title File::Name\n",
			"source2":
			"copy 1 Song::DynamicComplexity WaveFile::Complexity\n", 
			"source3":
			"" 
			} , script)

	def testUpdateDescriptors_noDescriptor(self) :
		provider = MetadataSourceAggregator(sources, map)
		descriptors = provider.QueryDescriptors('id1', [])
		provider.UpdateDescriptors('newId', descriptors)

		descriptors = provider.QueryDescriptors('newId', [])
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool/>\n" +
			"", self.dumpXml(descriptors))

	def testUpdateDescriptors_singleDescriptor(self) :
		attributeIds = [ "Song::Artist" ]
		provider = MetadataSourceAggregator(sources, map)
		poolToCopy = provider.QueryDescriptors('id1', attributeIds)
		provider.UpdateDescriptors('newId', poolToCopy)
		descriptors = provider.QueryDescriptors('newId', attributeIds)
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testUpdateDescriptors_multipleSources(self) :
		attributeIds = [
			"Song::Artist",
			"Song::Title",
			"Song::DynamicComplexity",
			]
		provider = MetadataSourceAggregator(sources, map)
		poolToCopy = provider.QueryDescriptors('id1', attributeIds)
		provider.UpdateDescriptors('newId', poolToCopy)
		descriptors = provider.QueryDescriptors('newId', attributeIds)
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 1</AttributePool>\n"+
			"    <AttributePool name='Title'>Title 1</AttributePool>\n"+
			"    <AttributePool name='DynamicComplexity'>8.1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testUpdateDescriptors_mergesExistingData(self) :
		provider = MetadataSourceAggregator(sources, map)

		# Init newId just with some attributes of id1
		attributeIds = [
			"Song::Artist",
			"Song::Title",
			]
		poolToCopy = provider.QueryDescriptors('id1', attributeIds)
		provider.UpdateDescriptors('newId', poolToCopy)
		# Merge them with some others from id2
		attributeIds = [
			"Song::Artist",
			]
		poolToCopy = provider.QueryDescriptors('id2', attributeIds)
		provider.UpdateDescriptors('newId', poolToCopy)

		attributeIds = [
			"Song::Artist",
			"Song::Title",
			]
		descriptors = provider.QueryDescriptors('newId', attributeIds)
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 2</AttributePool>\n"+
			"    <AttributePool name='Title'>Title 1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testUpdateDescriptors_partialPool(self) :
		provider = MetadataSourceAggregator(sources, map)

		# Init newId just with some attributes of id1
		attributeIds = [
			"Song::Artist",
			"Song::Title",
			]
		poolToCopy = provider.QueryDescriptors('id1', attributeIds)
		provider.UpdateDescriptors('newId', poolToCopy)
		# Merge them with some others from id2
		poolToCopy = provider.QueryDescriptors('id2', attributeIds)
		attributeIds = [
			"Song::Artist",
			]
		provider.UpdateDescriptors('newId', poolToCopy, attributeIds)

		attributeIds = [
			"Song::Artist",
			"Song::Title",
			]
		descriptors = provider.QueryDescriptors('newId', attributeIds)
		self.assertEquals( 
			"<?xml version='1.0' encoding='UTF-8'?>\n"+
			"<DescriptorsPool>\n" +
			"  <ScopePool name='Song' size='1'>\n"+
			"    <AttributePool name='Artist'>Artist 2</AttributePool>\n"+
			"    <AttributePool name='Title'>Title 1</AttributePool>\n"+
			"  </ScopePool>\n"+
			"</DescriptorsPool>\n" +
			"", self.dumpXml(descriptors))

	def testAvailableDescriptors(self) :
		provider = MetadataSourceAggregator(sources, map)
		attributeIds = provider.AvailableDescriptors()
		expectedAttributeIds = [
			"Song::Artist",
			"Song::Title",
			"Song::Genre",
			"Song::DynamicComplexity",
			"Frame::Energy",
			]
		self.assertEquals( expectedAttributeIds, attributeIds)

if __name__ == "__main__":
	unittest.main()


