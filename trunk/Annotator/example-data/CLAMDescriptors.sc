<?xml version="1.0" encoding="UTF-8"?>
<DescriptionScheme>
  <Uri>descriptionScheme:www.iua.upf.edu:clam:dummyTest-0.90</Uri>
  <Attributes>
    <Attribute name="Artist" scope="Song" type="String"/>
    <Attribute name="Title" scope="Song" type="String"/>
    <Attribute name="Genre" scope="Song" type="Enumerated">
      <EnumerationValues>Dance Classic Jazz Rhythm&amp;Blues Folk</EnumerationValues>
    </Attribute>
    <Attribute name="Danceability" scope="Song" type="Float">
      <Documentation>&lt;p&gt;The &lt;em&gt;Danceability&lt;/em&gt; is a 0 to 10 score that depends on how much defined is the rhythm&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Data for this data is not the one in Simac but randomly generated.&lt;/b&gt;&lt;/p&gt;
</Documentation>
      <MinValue>0</MinValue>
      <MaxValue>10</MaxValue>
    </Attribute>
    <Attribute name="Key" scope="Song" type="Enumerated">
      <EnumerationValues>C C# D D# E F F# G G# A A# B</EnumerationValues>
    </Attribute>
    <Attribute name="Mode" scope="Song" type="Enumerated">
      <EnumerationValues>Minor Major</EnumerationValues>
    </Attribute>
    <Attribute name="DynamicComplexity" scope="Song" type="Float">
      <MinValue>0</MinValue>
      <MaxValue>10</MaxValue>
    </Attribute>
    <Attribute name="BPM" scope="Song" type="Int">
      <MinValue>0</MinValue>
      <MaxValue>240</MaxValue>
    </Attribute>
    <Attribute name="RandomSegments" scope="Song" type="Segmentation">
      <ChildScope></ChildScope>
      <SegmentationPolicy>Unsized</SegmentationPolicy>
    </Attribute>
    <Attribute name="Onsets" scope="Song" type="Segmentation">
      <ChildScope>Onset</ChildScope>
      <SegmentationPolicy>Unsized</SegmentationPolicy>
    </Attribute>
    <Attribute name="Relevance" scope="Onset" type="Float">
      <MinValue>0</MinValue>
      <MaxValue>10</MaxValue>
    </Attribute>
    <Attribute name="DetectedChange" scope="Onset" type="Enumerated">
      <EnumerationValues>PitchChange EnergyChange</EnumerationValues>
    </Attribute>
    <Attribute name="Notes" scope="Song" type="Segmentation">
      <ChildScope>Note</ChildScope>
      <SegmentationPolicy>Overlapping</SegmentationPolicy>
    </Attribute>
    <Attribute name="Pitch" scope="Note" type="Enumerated">
      <EnumerationValues>C C# D D# E F F# G G# A A# B</EnumerationValues>
    </Attribute>
    <Attribute name="Octave" scope="Note" type="Int">
      <MinValue>1</MinValue>
      <MaxValue>12</MaxValue>
    </Attribute>
    <Attribute name="Instrument" scope="Note" type="String"/>
    <Attribute name="Chords" scope="Song" type="Segmentation">
      <ChildScope>Chord</ChildScope>
      <SegmentationPolicy>Continuous</SegmentationPolicy>
    </Attribute>
    <Attribute name="Root" scope="Chord" type="Enumerated">
      <EnumerationValues>C C# D D# E F F# G G# A A# B</EnumerationValues>
    </Attribute>
    <Attribute name="Mode" scope="Chord" type="Enumerated">
      <EnumerationValues>Major Minor Diminished Augmented</EnumerationValues>
    </Attribute>
    <Attribute name="Structure" scope="Song" type="Segmentation">
      <ChildScope>StructuralPart</ChildScope>
      <SegmentationPolicy>Discontinuous</SegmentationPolicy>
    </Attribute>
    <Attribute name="Description" scope="StructuralPart" type="Enumerated">
      <EnumerationValues>Versus Chorus Solo Accapella</EnumerationValues>
    </Attribute>
    <Attribute name="SimilarityGroup" scope="StructuralPart" type="Enumerated">
      <EnumerationValues>A B C D E F G H I J</EnumerationValues>
    </Attribute>
    <Attribute name="Frames" scope="Song" type="FrameDivision">
      <ChildScope>Frame</ChildScope>
    </Attribute>
    <Attribute name="Mean" scope="Frame" type="Float">
      <Documentation>&lt;p&gt;The spectral power mean value.
The unit of this measure can be dB or none, depending on the scale set for the measured Spectrum object.&lt;/p&gt;
&lt;p&gt;\f[ Mean(X) = \frac{\sum x_i }{Size(X)} \f]&lt;/p&gt;
&lt;p&gt;Being X the spectrum magnitude array.&lt;/p&gt;
</Documentation>
    </Attribute>
    <Attribute name="GeometricMean" scope="Frame" type="Float"/>
    <Attribute name="Energy" scope="Frame" type="Float"/>
    <Attribute name="Centroid" scope="Frame" type="Float"/>
    <Attribute name="Moment2" scope="Frame" type="Float"/>
    <Attribute name="Moment3" scope="Frame" type="Float"/>
    <Attribute name="Moment4" scope="Frame" type="Float"/>
    <Attribute name="Moment5" scope="Frame" type="Float"/>
    <Attribute name="Moment6" scope="Frame" type="Float"/>
    <Attribute name="Flatness" scope="Frame" type="Float"/>
    <Attribute name="MagnitudeKurtosis" scope="Frame" type="Float"/>
    <Attribute name="MaxMagFreq" scope="Frame" type="Float"/>
    <Attribute name="LowFreqEnergyRelation" scope="Frame" type="Float"/>
    <Attribute name="Spread" scope="Frame" type="Float"/>
    <Attribute name="MagnitudeSkewness" scope="Frame" type="Float"/>
    <Attribute name="Rolloff" scope="Frame" type="Float"/>
    <Attribute name="Slope" scope="Frame" type="Float"/>
    <Attribute name="HighFrequencyContent" scope="Frame" type="Float"/>
    <Attribute name="MelFrequencyCepstrumCoefficients" scope="Frame" type="FloatArray">
      <BinLabels>MFCC1 MFCC2 MFCC3 MFCC4 MFCC5 MFCC6 MFCC7 MFCC8 MFCC9 MFCC10 MFCC11 MFCC12</BinLabels>
    </Attribute>
    <Attribute name="SpectrumMagnitude" scope="Frame" type="FloatArray">
      <FirstBinOffset>0</FirstBinOffset>
      <BinGap>2205</BinGap>
      <NBins>10</NBins>
    </Attribute>
  </Attributes>
</DescriptionScheme>
