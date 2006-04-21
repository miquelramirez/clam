<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Project>

  <Description>&lt;h1&gt;Beats Annotation&lt;/h1&gt;
&lt;h2&gt;Description:&lt;/h2&gt;
&lt;p&gt;
The goal of this annotation task is to annotate the first 2 minutes of the excerpt for the beats.
&lt;/p&gt;
&lt;h2&gt;Reference descriptors:&lt;/h2&gt;
&lt;p&gt;
You can take some descriptors as reference such as the energy.
Consider that such descriptors are computed every 2048 samples so their precission may be not as accurate as it seems.
&lt;/p&gt;
</Description>

  <Schema>Chords.sc</Schema>

  <Extractor>/home/vokimon/CajitasDeArena/CLAM_Annotator/ChordExtractor</Extractor>

  <PoolSuffix>.chords</PoolSuffix>

  <Songs>
    <Song>
      <SoundFile>SongsTest/Defunkid - By the Face (live) - The pain.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-SomethingBetter.mp3</SoundFile>
    </Song>
    <Song>
      <SoundFile>SongsTest/LisaRein-spunkyfunk.mp3</SoundFile>
    </Song>
  </Songs>

</Project>
