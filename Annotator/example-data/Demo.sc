<?xml version='1.0' encoding='UTF-8'?>
<DescriptionScheme>
  <Attributes>
    <Attribute scope='Song' name='ClamMode' type='Enumerated'>
      <EnumerationValues>Minor Major</EnumerationValues>
    </Attribute>
    <Attribute scope='Song' name='DynamicComplexity' type='Float'>
      <MinValue>0</MinValue>
      <MaxValue>10</MaxValue>
    </Attribute>
    <Attribute scope='Song' name='BPM' type='Int'>
      <MinValue>0</MinValue>
      <MaxValue>240</MaxValue>
    </Attribute>
    <Attribute scope='Song' name='ClamFrames' type='FrameDivision'>
      <ChildScope>Frame</ChildScope>
    </Attribute>
    <Attribute scope='Song' name='Frames' type='FrameDivision'>
      <ChildScope>Frame</ChildScope>
    </Attribute>
    <Attribute scope='Frame' name='HartePcp' type='FloatArray'>
      <BinLabels>G G# A Bb B C C# D Eb E F F#</BinLabels>
    </Attribute>
    <Attribute scope='Frame' name='HarteChordCorrelation' type='FloatArray'>
      <BinLabels>G G#/Ab A A#/Bb B C C#/Db D D#/Eb E F F#/Gb g g#/ab a a#/bb b c c#/db d d#/eb e f f#/gb </BinLabels>
    </Attribute>
  </Attributes>
</DescriptionScheme>
