<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Schema>

  <HLDSchema>
    <HLDs>
      <HLD>
        <Name>Artist</Name>
        <Type>String</Type>
      </HLD>
      <HLD>
        <Name>Title</Name>
        <Type>String</Type>
      </HLD>
      <HLD>
        <Name>Genre</Name>
        <Type>RestrictedString</Type>
        <RestrictionValues>Dance Classic Jazz Rhythm&amp;Blues Folk</RestrictionValues>
      </HLD>
      <HLD>
        <Name>Danceability</Name>
        <Type>Float</Type>
        <fRange>
          <Max>10</Max>
          <Min>0</Min>
        </fRange>
      </HLD>
      <HLD>
        <Name>Key</Name>
        <Type>RestrictedString</Type>
        <RestrictionValues>A A# B C C# D D# E F F# G G#</RestrictionValues>
      </HLD>
      <HLD>
        <Name>Mode</Name>
        <Type>RestrictedString</Type>
        <RestrictionValues>Minor Major</RestrictionValues>
      </HLD>
      <HLD>
        <Name>DynamicComplexity</Name>
        <Type>Float</Type>
        <fRange>
          <Max>10</Max>
          <Min>0</Min>
        </fRange>
      </HLD>
      <HLD>
        <Name>BPM</Name>
        <Type>Int</Type>
        <iRange>
          <Max>240</Max>
          <Min>0</Min>
        </iRange>
      </HLD>
    </HLDs>
  </HLDSchema>

  <LLDSchema>
    <LLDNames>Mean GeometricMean Energy Centroid Moment2 Moment3 Moment4 Moment5 Moment6 Flatness MagnitudeKurtosis MaxMagFreq LowFreqEnergyRelation Spread MagnitudeSkewness Rolloff Slope HighFrequencyContent</LLDNames>
  </LLDSchema>

</Schema>
