<?xml version="1.0" encoding="UTF-8"?>
<Project>
  <Description>&lt;h1&gt;Aggregation extraction review&lt;/h1&gt;
&lt;h2&gt;Description:&lt;/h2&gt;
&lt;p&gt;
The goal of this project is to review the results of the aggregation extraction results
&lt;/p&gt;
&lt;p&gt;
TODO: Explain how this aggregation extractor should be reviewed.
&lt;/p&gt;
</Description>
  <Schema>AggregationExample.sc</Schema>
  <Extractor>C:/Jun/Devel/Annotator/scripts/dist/AggregationExtractor.exe</Extractor>
  <PoolSuffix>.aggPool</PoolSuffix>
  <Configuration>sources = [
	("example", FileMetadataSource(path=".",
		schemaFile="CLAMDescriptors.sc",
		poolSuffix=".example",
		extractor="ClamExtractorExample")),
	("chord", FileMetadataSource(path=".",
		schemaFile="Chords.sc",
		poolSuffix=".chord",
		extractor="ChordExtractor")),
]

map = [
	# ('TargetScope::TargetAttribute', 'sourceId', 'SourceScope::SourceAttribute'),
	("Song::Frames", "example", "Song::Frames"),
	("Song::ChordFrames", "chord", "Song::Frames"),
	("Song::Danceable", "example", "Song::Danceability"),
	("Frame::Energy", "example", "Frame::Energy"),
	("ChordFrame::Energy", "chord", "Frame::Energy"),
	("ChordFrame::ChordHartePcp", "chord", "Frame::HartePcp"),
	("ChordFrame::HarteChordCorrelation", "chord", "Frame::HarteChordCorrelation"),
	("Song::Harte", "chord", "Song::Chords_Harte"),
	("ExtractedChord::ChordRoot", "chord", "ExtractedChord::Root"),
	("ExtractedChord::ChordMode", "chord", "ExtractedChord::Mode"),
]</Configuration>
  <Songs>
    <Song>
      <SoundFile>SongsTest/Debaser-CoffeeSmell.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/Debaser-WoodenHouse.mp3</SoundFile>
    </Song>
  </Songs>
  <Views/>
</Project>
