#!/usr/bin/python

import sys

nSpeakers = int(sys.argv[1])

print """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.3.1" id="Unnamed">
"""
for speaker in xrange(nSpeakers) :
	print """
  <processing id="%02i" position="%i,%i" size="128,111" type="AudioSink"/>""" % (
	speaker+1, 400+64*(speaker%6), (speaker%6)*6+(speaker//6)*95 )

print """
  <processing id="1" position="25,480" size="128,108" type="AudioSource"/>

  <processing id="2" position="130,498" size="128,108" type="AudioSource"/>

  <processing id="3" position="237,516" size="128,108" type="AudioSource"/>

  <processing id="4" position="343,535" size="128,108" type="AudioSource"/>

  <processing id="LoopBack" position="450,555" size="128,108" type="AudioSource"/>

  <processing id="AudioAmplifier" position="200,64" size="114,414" type="AudioAmplifier">
    <MaxGain>1</MaxGain>
    <InitGain>.04</InitGain>
    <PortsNumber>%(nSpeakers)s</PortsNumber>
  </processing>

  <processing id="Oscilloscope" position="850,156" size="196,226" type="Oscilloscope"/>

  <processing id="SndfilePlayer" position="41,64" size="107,414" type="SndfilePlayer">
    <SourceFile>multiSweep%(nSpeakers)s.wav</SourceFile>
    <Loop>0</Loop>
    <SavedNumberOfChannels>%(nSpeakers)s</SavedNumberOfChannels>
  </processing>

  <processing id="SndfileWriter" position="596,482" size="104,110" type="SndfileWriter">
    <TargetFile>calibrationMicsOutput.wav</TargetFile>
    <Format>Wav Integer 16 bit</Format>
    <SampleRate>48000</SampleRate>
    <NumberChannels>5</NumberChannels>
  </processing>

  <port_connection>
    <out>1.1</out>
    <in>SndfileWriter.in1</in>
  </port_connection>

  <port_connection>
    <out>2.1</out>
    <in>SndfileWriter.in2</in>
  </port_connection>

  <port_connection>
    <out>3.1</out>
    <in>SndfileWriter.in3</in>
  </port_connection>

  <port_connection>
    <out>4.1</out>
    <in>SndfileWriter.in4</in>
  </port_connection>

  <port_connection>
    <out>LoopBack.1</out>
    <in>SndfileWriter.in5</in>
  </port_connection>

  <port_connection>
    <out>SndfilePlayer.out0</out>
    <in>Oscilloscope.Input</in>
  </port_connection>
"""% { 'nSpeakers': nSpeakers }

for speaker in xrange(nSpeakers) :
	print """
  <port_connection>
    <out>SndfilePlayer.out%(playerPort)s</out>
    <in>AudioAmplifier.Input Audio%(mixerPort)s</in>
  </port_connection>

  <port_connection>
    <out>AudioAmplifier.Audio Output%(mixerPort)s</out>
    <in>%(sourceNumber)s.1</in>
  </port_connection>""" % {
	'mixerPort': (" %i"%speaker) if speaker else '',
	'playerPort': str(speaker),
	'sourceNumber': "%02i"%(speaker+1),
	}

print """

</network>
"""






