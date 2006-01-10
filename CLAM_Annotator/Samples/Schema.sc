<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<DescriptionScheme>

  <Uri>descriptionScheme:www.iua.upf.edu:clam:dummyTest-0.90</Uri>

  <Attributes>
    <Attribute name="Artist" scope="Song" type="String"/>
    <Attribute name="Title" scope="Song" type="String"/>
    <Attribute name="Genre" scope="Song" type="Enumerated">
      <EnumerationValues>Dance Classic Jazz Rhythm&amp;Blues Folk</EnumerationValues>
    </Attribute>
    <Attribute name="Danceability" scope="Song" type="Float">
      <fRange>
        <Min>0</Min>
        <Max>10</Max>
      </fRange>
    </Attribute>
    <Attribute name="Key" scope="Song" type="Enumerated">
      <EnumerationValues>C C# D D# E F F# G G# A A# B</EnumerationValues>
    </Attribute>
    <Attribute name="Mode" scope="Song" type="Enumerated">
      <EnumerationValues>Minor Major</EnumerationValues>
    </Attribute>
    <Attribute name="DynamicComplexity" scope="Song" type="Float">
      <fRange>
        <Min>0</Min>
        <Max>10</Max>
      </fRange>
    </Attribute>
    <Attribute name="BPM" scope="Song" type="Int">
      <iRange>
        <Min>0</Min>
        <Max>240</Max>
      </iRange>
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
      <fRange>
        <Min>0</Min>
        <Max>10</Max>
      </fRange>
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
      <iRange>
        <Min>1</Min>
        <Max>12</Max>
      </iRange>
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
    <Attribute name="Mean" scope="Frame" type="Float"/>
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
  </Attributes>

</DescriptionScheme>
