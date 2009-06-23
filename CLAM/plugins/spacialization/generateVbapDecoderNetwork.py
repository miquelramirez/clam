#!/usr/bin/python

import sys

if len(sys.argv)!=4 :
	print >> sys.stderr, "Usage: %s <layout_file> <triangulation_file> <choreo_file>"%sys.argv[0]
	sys.exit()


layoutFileName = sys.argv[1]
triangulationFileName = sys.argv[2]
choreoFileName = sys.argv[3]

speakers = [ label for label in [" ".join(line.split()[2:]) for line in open(layoutFileName).readlines() if line[0]!='#' ] if label ]
nSpeakers = len(speakers)

print >> sys.stderr, "Speakers:", nSpeakers

print """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.3.1" id="Unnamed">
"""

# TODO use the layout labels for the AudioSink
for speaker, label in enumerate(speakers) :
	print """
  <processing id="%s" position="%i,%i" size="128,111" type="AudioSink"/>""" % (
	"%02i"%(speaker+1), 600+64*(speaker%6), 80+(speaker%6)*6+(speaker//6)*95 )
#	speaker+1, label, 600+64*(speaker%6), 80+(speaker%6)*6+(speaker//6)*95 )

print """
  <processing id="1 Audio Input" position="0,305" size="128,108" type="AudioSource"/>

  <processing id="ChoreoSequencer_1" position="155,2" size="226,59" type="ChoreoSequencer">
    <Filename>%(choreo)s</Filename>
    <SourceIndex>0</SourceIndex>
    <FrameSize>512</FrameSize>
    <SampleRate>48000</SampleRate>
    <ControlsPerSecond>24</ControlsPerSecond>
    <UseSpiralIfNoFilename>0</UseSpiralIfNoFilename>
  </processing>

  <processing id="AbsoluteCoordinates2RelativeAngles" position="155,80" size="239,58" type="AbsoluteCoordinates2RelativeAngles"/>

  <processing id="GainBecauseOfDistance" position="148,194" size="168,58" type="GainBecauseOfDistance">
    <DistanceExponent>1</DistanceExponent>
    <MinimumDistance>1</MinimumDistance>
    <DistanceThreshold>0</DistanceThreshold>
  </processing>

  <processing id="Input Gain" position="163,362" size="114,58" type="AudioAmplifier">
    <MaxGain>3</MaxGain>
    <InitGain>1</InitGain>
    <PortsNumber>1</PortsNumber>
  </processing>

  <processing id="Vbap3D" position="367,150" size="79,270" type="Vbap3D">
    <SpeakerLayout>%(layoutFile)s</SpeakerLayout>
    <Triangulation>%(triangulationFile)s</Triangulation>
    <IgnoreLabels>1</IgnoreLabels>
  </processing>

  <port_connection>
    <out>1 Audio Input.1</out>
    <in>Input Gain.Input Audio</in>
  </port_connection>

  <port_connection>
    <out>Input Gain.Audio Output</out>
    <in>Vbap3D.W</in>
  </port_connection>

  <port_connection>
    <out>1 Audio Input.1</out>
    <in>ChoreoSequencer_1.sync</in>
  </port_connection>
"""% {
	'nSpeakers': nSpeakers,
	'layoutFile': layoutFileName,
	'triangulationFile': triangulationFileName,
	'choreo': choreoFileName,
}

for i, label in enumerate(speakers) :
	print """
  <port_connection>
    <out>Vbap3D.%(speakerLabel)s</out>
    <in>%(speakerLabel)s.1</in>
  </port_connection>
""" % {
	'speakerNumber' : i+1,
	'speakerLabel' : "%02i"%(i+1), #label,
}

print """
  <control_connection>
    <out>AbsoluteCoordinates2RelativeAngles.relative azimuth</out>
    <in>Vbap3D.azimuth</in>
  </control_connection>

  <control_connection>
    <out>AbsoluteCoordinates2RelativeAngles.relative elevation</out>
    <in>Vbap3D.elevation</in>
  </control_connection>

  <control_connection>
    <out>AbsoluteCoordinates2RelativeAngles.distance</out>
    <in>GainBecauseOfDistance.input distance</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener X</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener X</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener Y</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener Y</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener Z</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener Z</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener roll</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener roll</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener azimuth</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener azimuth</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.listener elevation</out>
    <in>AbsoluteCoordinates2RelativeAngles.listener elevation</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.source X</out>
    <in>AbsoluteCoordinates2RelativeAngles.source X</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.source Y</out>
    <in>AbsoluteCoordinates2RelativeAngles.source Y</in>
  </control_connection>

  <control_connection>
    <out>ChoreoSequencer_1.source Z</out>
    <in>AbsoluteCoordinates2RelativeAngles.source Z</in>
  </control_connection>

  <control_connection>
    <out>GainBecauseOfDistance.calculated output gain</out>
    <in>Input Gain.Gain</in>
  </control_connection>

</network>
"""

