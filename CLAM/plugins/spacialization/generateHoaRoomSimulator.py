#!/usr/bin/python

import sys

if len(sys.argv)!=4 :
	print >> sys.stderr, "Usage: %s <order> <choreo> <geometry>"%sys.argv[0]
	print >> sys.stderr, """
Generates a network for a Hoa encoder from a given order 
of ambisonics using room simulation and source-listener choreography.
"""
	sys.exit()


order = sys.argv[1]
if order not in '0123':
	print >> sys.stderr, "Order",order,"is not supported. It must be either 0, 1, 2 or 3."
	sys.exit(-1)
order=int(order)
channels="".join(['W','XYZ','RSTUV','KLMNOPQ'][0:order+1])

choreo = sys.argv[2]
geometry = sys.argv[3]

print """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.3.1" id="Unnamed">
"""

print """
  <processing id="AudioInput" position="31,332" size="128,108" type="AudioSource" />

  <processing id="AudioMixer" position="176,540" size="107,62" type="AudioMixer">
    <NumberOfInPorts>1</NumberOfInPorts>
  </processing>

  <processing id="AudioWindowing" position="330,542" size="143,59" type="AudioWindowing">
    <HopSize>512</HopSize>
    <SamplingRate>44100</SamplingRate>
    <FFTSize>1024</FFTSize>
    <WindowSize>513</WindowSize>
    <WindowType>None</WindowType>
    <DoHalfWindowShift>0</DoHalfWindowShift>
  </processing>

  <processing id="ChoreoSequencer" position="101,14" size="226,59" type="ChoreoSequencer">
    <Filename>%(choreo)s</Filename>
    <SourceIndex>0</SourceIndex>
    <FrameSize>512</FrameSize>
    <SampleRate>48000</SampleRate>
    <ControlsPerSecond>24</ControlsPerSecond>
    <UseSpiralIfNoFilename>0</UseSpiralIfNoFilename>
  </processing>

  <processing id="MyFFT" position="494,538" size="75,59" type="MyFFT">
    <AudioSize>1024</AudioSize>
  </processing>

  <processing id="Gain 0" position="192,30" size="262,78" type="OutControlSender">
    <Min>0</Min>
    <Default>1</Default>
    <Max>1</Max>
    <Step>0.01</Step>
    <ControlRepresentation>Horizontal Slider</ControlRepresentation>
  </processing>

  <processing id="RoomImpulseResponseSimulator" position="58,164" size="267,100" type="RoomImpulseResponseSimulator">
    <FrameSize>512</FrameSize>
    <Model3DFile>%(geometry)s</Model3DFile>
    <GridDivisions>500</GridDivisions>
    <NRays>20000</NRays>
    <NRebounds>500</NRebounds>
    <IrLength>10.</IrLength>
    <Order>%(order)s</Order>
    <ExtraOptions>--ignore-sources</ExtraOptions>
    <SeparateDirectSoundAndReverb>0</SeparateDirectSoundAndReverb>
    <SupressInitialDelay>1</SupressInitialDelay>
  </processing>
""" % dict(
	order=order,
	choreo=choreo,
	geometry=geometry,
	)

for i, channel in enumerate(channels) :
	print """\
  <processing id="Convolution_%(channel)s_1" position="521,%(y1)s" size="127,62" type="LowLatencyConvolution">
    <MaxFrames>0</MaxFrames>
    <FrameSize>512</FrameSize>
  </processing>

  <processing id="Convolution_%(channel)s_2" position="521,%(y2)s" size="127,62" type="LowLatencyConvolution">
    <MaxFrames>0</MaxFrames>
    <FrameSize>512</FrameSize>
  </processing>

  <processing id="Delay %(channel)s" position="378,%(y1)s" size="84,62" type="IRSwappingDelay" />

  <processing id="MyIFFT_%(channel)s_1" position="690,%(y1)s" size="79,59" type="MyIFFT">
    <AudioSize>1024</AudioSize>
  </processing>

  <processing id="MyIFFT_%(channel)s_2" position="690,%(y2)s" size="79,59" type="MyIFFT">
    <AudioSize>1024</AudioSize>
  </processing>

  <processing id="%(channel)s" position="1085,%(y1)s" size="128,111" type="AudioSink" />

  <processing id="XFade %(channel)s" position="974,%(y1)s" size="85,94" type="XFadeAndSwitch" />

  <processing id="toStream %(channel)s 1" position="816,%(y1)s" size="106,59" type="AudioBuffer2Stream">
    <HopSize>512</HopSize>
    <SamplingRate>48000</SamplingRate>
    <FFTSize>1024</FFTSize>
    <WindowSize>513</WindowSize>
    <WindowType>None</WindowType>
    <DoHalfWindowShift>0</DoHalfWindowShift>
  </processing>

  <processing id="toStream %(channel)s 2" position="816,%(y2)s" size="106,59" type="AudioBuffer2Stream">
    <HopSize>512</HopSize>
    <SamplingRate>48000</SamplingRate>
    <FFTSize>1024</FFTSize>
    <WindowSize>513</WindowSize>
    <WindowType>None</WindowType>
    <DoHalfWindowShift>0</DoHalfWindowShift>
  </processing>

  <processing id="Oscilloscope %(channel)s" position="1234,%(y1)s" size="162,109" type="Oscilloscope" />
""" % dict(
	channel=channel,
	y1=80+i*100,
	y2=124+i*100,
	)
print """
  <port_connection>
    <out>AudioInput.1</out>
    <in>AudioMixer.Input 0</in>
  </port_connection>

  <port_connection>
    <out>AudioInput.1</out>
    <in>ChoreoSequencer.sync</in>
  </port_connection>

  <port_connection>
    <out>AudioInput.1</out>
    <in>RoomImpulseResponseSimulator.synchronization</in>
  </port_connection>

  <port_connection>
    <out>AudioMixer.Output Audio</out>
    <in>AudioWindowing.Input</in>
  </port_connection>

  <port_connection>
    <out>AudioWindowing.Output</out>
    <in>MyFFT.Audio Buffer</in>
  </port_connection>
"""
for channel in channels :
	print """\
<!--
  <port_connection>
    <out>BFormatRotation.%(channel)s</out>
    <in>%(channel)s.AudioIn</in>
  </port_connection>

  <port_connection>
    <out>BFormatRotation.%(channel)s</out>
    <in>Oscilloscope %(channel)s.Input</in>
  </port_connection>
-->
  <port_connection>
    <out>MyFFT.Complex Spectrum</out>
    <in>Convolution_%(channel)s_1.Input</in>
  </port_connection>

  <port_connection>
    <out>MyFFT.Complex Spectrum</out>
    <in>Convolution_%(channel)s_2.Input</in>
  </port_connection>

  <port_connection>
    <out>Delay %(channel)s.Current IR</out>
    <in>Convolution_%(channel)s_1.ImpulseResponse</in>
  </port_connection>

  <port_connection>
    <out>Delay %(channel)s.Previous IR</out>
    <in>Convolution_%(channel)s_2.ImpulseResponse</in>
  </port_connection>

  <port_connection>
    <out>Convolution_%(channel)s_1.Output</out>
    <in>MyIFFT_%(channel)s_1.Complex Spectrum</in>
  </port_connection>

  <port_connection>
    <out>Convolution_%(channel)s_2.Output</out>
    <in>MyIFFT_%(channel)s_2.Complex Spectrum</in>
  </port_connection>

  <port_connection>
    <out>MyIFFT_%(channel)s_1.Audio Buffer</out>
    <in>toStream %(channel)s 1.Audio buffer</in>
  </port_connection>

  <port_connection>
    <out>MyIFFT_%(channel)s_2.Audio Buffer</out>
    <in>toStream %(channel)s 2.Audio buffer</in>
  </port_connection>

  <port_connection>
    <out>toStream %(channel)s 1.Audio stream</out>
    <in>XFade %(channel)s.Audio1</in>
  </port_connection>

  <port_connection>
    <out>toStream %(channel)s 2.Audio stream</out>
    <in>XFade %(channel)s.Audio2</in>
  </port_connection>

  <port_connection>
    <out>Delay %(channel)s.Current IR</out>
    <in>XFade %(channel)s.ImpulseResponse1</in>
  </port_connection>

  <port_connection>
    <out>Delay %(channel)s.Previous IR</out>
    <in>XFade %(channel)s.ImpulseResponse2</in>
  </port_connection>

  <port_connection>
    <out>RoomImpulseResponseSimulator.%(channel)s IR</out>
    <in>Delay %(channel)s.Input IR</in>
  </port_connection>

  <port_connection>
    <out>XFade %(channel)s.CrossFadedBuffer</out>
    <in>%(channel)s.1</in>
  </port_connection>

  <port_connection>
    <out>XFade %(channel)s.CrossFadedBuffer</out>
    <in>Oscilloscope %(channel)s.Input</in>
  </port_connection>
"""%dict(channel=channel)

print """\
  <control_connection>
    <out>ChoreoSequencer.listener X</out>
    <in>RoomImpulseResponseSimulator.listener X</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.listener Y</out>
    <in>RoomImpulseResponseSimulator.listener Y</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.listener Z</out>
    <in>RoomImpulseResponseSimulator.listener Z</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source X</out>
    <in>RoomImpulseResponseSimulator.source X</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source Y</out>
    <in>RoomImpulseResponseSimulator.source Y</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer.source Z</out>
    <in>RoomImpulseResponseSimulator.source Z</in>
  </control_connection>

  <control_connection>
    <out>Gain 0.out</out>
    <in>AudioMixer.Gain 0</in>
  </control_connection>

</network>
"""


