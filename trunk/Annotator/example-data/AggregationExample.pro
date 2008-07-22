<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
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

  <Extractor>./scripts/AggregationExtractor.py</Extractor>

  <PoolSuffix>.aggregatedPool</PoolSuffix>

  <Songs>
    <Song>
      <SoundFile>SongsTest/Debaser-CoffeeSmell.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/Debaser-WoodenHouse.mp3</SoundFile>
    </Song>
  </Songs>

  <Views>
    <View>
      <Type>Tonnetz</Type>
      <AttributeScope>ChordFrame</AttributeScope>
      <AttributeName>ChordHartePcp</AttributeName>
    </View>
    <View>
      <Type>KeySpace</Type>
      <AttributeScope>ChordFrame</AttributeScope>
      <AttributeName>HarteChordCorrelation</AttributeName>
    </View>
  </Views>

</Project>
