<?xml version='1.0' encoding='UTF-8' standalone='no' ?>
<DescriptionScheme>

 <Uri>descriptionScheme:www.iua.upf.edu:clam:ChordExtraction</Uri>

 <Attributes>
   <Attribute name='Chords_Harte' scope='Song' type='Segmentation'>
     <ChildScope>ExtractedChord</ChildScope>
     <SegmentationPolicy>Discontinuous</SegmentationPolicy>
   </Attribute>
    <Attribute name='DebugFrameSegments' scope='Song' type='Segmentation'>
	  <Documentation>
	  	This segmentation is just to check that the segments and the frames are properly aligned.
	  </Documentation>
      <ChildScope></ChildScope>
      <SegmentationPolicy>Continuous</SegmentationPolicy>
    </Attribute>
    <Attribute name='Frames' scope='Song' type='FrameDivision'>
      <ChildScope>Frame</ChildScope>
    </Attribute>
    <Attribute name='TunningPosition' scope='Frame' type='Float'/>
    <Attribute name='TunningStrength' scope='Frame' type='Float'/>
    <Attribute name='FirstChordCandidateRelevance' scope='Frame' type='Float'/>
    <Attribute name='SecondChordCandidateRelevance' scope='Frame' type='Float'/>
    <Attribute name='FirstChordIndex' scope='Frame' type='Float'/>
    <Attribute name='SecondChordIndex' scope='Frame' type='Float'/>
    <Attribute name='Energy' scope='Frame' type='Float'/>
	<!--
    <Attribute name='PrimaryRoot' scope='Frame'type='Enumerated'>
      <EnumerationValues>A A# B B# C C# D D# E F F# G G#</EnumerationValues>
    </Attribute>
    <Attribute name='PrimaryMode' scope='Frame' type='Enumerated'
      <EnumerationValues>Major Minor Major7 Minor7 Dominant7 MajorMinor7 Diminished Augmented</EnumerationValues>
    </Attribute>
    <Attribute name='SecondaryRoot' scope='Frame' type='Enumerated'>
      <EnumerationValues>A A# B B# C C# D D# E F F# G G#</EnumerationValues>
    </Attribute>
    <Attribute name='SecondaryMode'scope='Frame' type='Enumerated'>
      <EnumerationValues>Major Minor Major7 Minor7 Dominant7 MajorMinor7 Diminished Augmented</EnumerationValues>
    </Attribute>
	-->
    <Attribute name='HartePcp' scope='Frame' type='FloatArray'>
      <BinLabels>G G# A Bb B C C# D Eb E F F#</BinLabels>
    </Attribute>
    <Attribute name='HarteChordCorrelation' scope='Frame' type='FloatArray'>
      <BinLabels>
	  	G G#/Ab A A#/Bb B C C#/Db D D#/Eb E F F#/Gb
	  	g g#/ab a a#/bb b c c#/db d d#/eb e f f#/gb</BinLabels>
    </Attribute>
    <Attribute name='Root' scope='ExtractedChord' type='Enumerated'>
      <EnumerationValues>G G# A A# B C C# D D# E F F#</EnumerationValues>
    </Attribute>
    <Attribute name='Mode' scope='ExtractedChord' type='Enumerated'>
      <EnumerationValues>
         Major Minor Major7 Minor7 Dominant7 MinorMajor7
		 Suspended2 Suspended4 Major6 Minor6 6/9
		 Diminished Diminished7 Augmented
      </EnumerationValues>
    </Attribute>
  </Attributes>

</DescriptionScheme>
