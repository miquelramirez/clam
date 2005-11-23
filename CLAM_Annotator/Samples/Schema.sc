<?xml version="1.0" encoding="UTF-8"?>
<DescriptionScheme>
  <Uri>descriptionScheme:www.iua.upf.edu:clam:dummyTest</Uri>
  <Attributes>
    <Attribute name="Artist" scope="Song" type="String"/>
    <Attribute name="Title" scope="Song" type="String"/>
    <Attribute name="Genre" scope="Song" type="RestrictedString">
      <RestrictionValues>Dance Classic Jazz Rhythm&amp;Blues Folk</RestrictionValues>
    </Attribute>
    <Attribute name="Danceability" scope="Song" type="Float">
      <fRange>
        <Min>0</Min>
        <Max>10</Max>
      </fRange>
    </Attribute>
    <Attribute name="Key" scope="Song" type="RestrictedString">
      <RestrictionValues>A A# B C C# D D# E F F# G G#</RestrictionValues>
    </Attribute>
    <Attribute name="Mode" scope="Song" type="RestrictedString">
      <RestrictionValues>Minor Major</RestrictionValues>
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
    <Attribute name="DetectedChange" scope="Onset" type="RestrictedString">
      <RestrictionValues>PitchChange EnergyChange</RestrictionValues>
    </Attribute>
    <Attribute name="Notes" scope="Song" type="Segmentation">
      <ChildScope>Note</ChildScope>
      <SegmentationPolicy>Unsized</SegmentationPolicy>
    </Attribute>
    <Attribute name="Pitch" scope="Note" type="RestrictedString">
      <RestrictionValues>A A# B C C# D D# E F F# G G#</RestrictionValues>
    </Attribute>
    <Attribute name="Octave" scope="Note" type="Int">
      <iRange>
        <Min>1</Min>
        <Max>12</Max>
      </iRange>
    </Attribute>
    <Attribute name="Instrument" scope="Note" type="String"/>
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
