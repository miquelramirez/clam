<?xml version='1.0' encoding='UTF-8'?>
<DescriptionScheme>
  <Attributes>
    <Attribute scope='Song' name='Frames' type='FrameDivision'>
      <ChildScope>Frame</ChildScope>
    </Attribute>
    <Attribute scope='Song' name='ChordFrames' type='FrameDivision'>
      <ChildScope>ChordFrame</ChildScope>
    </Attribute>
    <Attribute scope='Song' name='Danceable' type='Float'>
      <Documentation>&lt;p>The &lt;em>Danceability&lt;/em> is a 0 to 10 score that depends on how much defined is the rhythm&lt;/p>
&lt;p>&lt;b>Data for this data is not the one in Simac but randomly generated.&lt;/b>&lt;/p>
</Documentation>
      <MinValue>0</MinValue>
      <MaxValue>10</MaxValue>
    </Attribute>
    <Attribute scope='Frame' name='Energy' type='Float'/>
    <Attribute scope='ChordFrame' name='Energy' type='Float'/>
    <Attribute scope='ChordFrame' name='ChordHartePcp' type='FloatArray'>
      <BinLabels>G G# A Bb B C C# D Eb E F F#</BinLabels>
    </Attribute>
    <Attribute scope='ChordFrame' name='HarteChordCorrelation' type='FloatArray'>
      <BinLabels>
	  	G G#/Ab A A#/Bb B C C#/Db D D#/Eb E F F#/Gb
	  	g g#/ab a a#/bb b c c#/db d d#/eb e f f#/gb</BinLabels>
    </Attribute>
    <Attribute scope='Song' name='Harte' type='Segmentation'>
      <ChildScope>ExtractedChord</ChildScope>
      <SegmentationPolicy>Discontinuous</SegmentationPolicy>
    </Attribute>
    <Attribute scope='ExtractedChord' name='ChordRoot' type='Enumerated'>
      <EnumerationValues>G G# A A# B C C# D D# E F F#</EnumerationValues>
    </Attribute>
    <Attribute scope='ExtractedChord' name='ChordMode' type='Enumerated'>
      <EnumerationValues>
         Major Minor Major7 Minor7 Dominant7 MinorMajor7
		 Suspended2 Suspended4 Major6 Minor6 6/9
		 Diminished Diminished7 Augmented
      </EnumerationValues>
    </Attribute>
  </Attributes>
</DescriptionScheme>
