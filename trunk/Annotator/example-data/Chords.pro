<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>

  <Description>&lt;h1&gt;Chord extraction review&lt;/h1&gt;
&lt;h2&gt;Description:&lt;/h2&gt;
&lt;p&gt;
The goal of this project is to review the results of the chord extraction algorithm developed by C. Harte at the Queen Mary Univerity of London.
&lt;/p&gt;
&lt;h2&gt;Reference descriptors:&lt;/h2&gt;
&lt;p&gt;
TODO: Explain what the descriptors mean and how this should be reviewed.
&lt;/p&gt;
</Description>

  <Schema>Chords.sc</Schema>

  <Extractor>ChordExtractor</Extractor>

  <PoolSuffix>.chords</PoolSuffix>
  <Configuration></Configuration>

  <Songs>
    <Song>
      <SoundFile>SongsTest/Debaser-WoodenHouse.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/Debaser-CoffeeSmell.mp3</SoundFile>
    </Song>
  </Songs>

  <Views>
    <View>
      <Type>Tonnetz</Type>
      <AttributeScope>Frame</AttributeScope>
      <AttributeName>HartePcp</AttributeName>
    </View>
    <View>
      <Type>KeySpace</Type>
      <AttributeScope>Frame</AttributeScope>
      <AttributeName>HarteChordCorrelation</AttributeName>
    </View>
  </Views>

</Project>
