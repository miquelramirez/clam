<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>

  <Description>&lt;h1>Chord extraction review&lt;/h1>
&lt;h2>Description:&lt;/h2>
&lt;p>
The goal of this project is to review the results of the chord extraction algorithm developed by C. Harte at the Queen Mary Univerity of London.
&lt;/p>
&lt;h2>Reference descriptors:&lt;/h2>
&lt;p>
TODO: Explain what the descriptors mean and how this should be reviewed.
&lt;/p>
</Description>

  <Schema>Chords.sc</Schema>

  <Extractor>ChordExtractor</Extractor>

  <PoolSuffix>.chords</PoolSuffix>

  <Songs>
    <Song>
      <SoundFile>SongsTest/Debaser-WoodenHouse.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/Debaser-CoffeeSmell.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-SomethingBetter.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-spunkyfunk.mp3</SoundFile>
    </Song>
  </Songs>

  <Views>
    <View>
      <Type>PcpTorus</Type>
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
