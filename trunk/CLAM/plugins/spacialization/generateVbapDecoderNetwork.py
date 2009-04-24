#!/usr/bin/python

import sys

if len(sys.argv)!=3 :
	print >> sys.stderr, "Usage: %s <layout_file> <triangulation_file>"%sys.argv[0]
	sys.exit()


layoutFileName = sys.argv[1]
triangulationFileName = sys.argv[2]

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

  <processing id="Azimuth" position="57,9" size="113,58" type="ControlSource">
    <MinValue>0</MinValue>
    <MaxValue>360</MaxValue>
    <Step>0.1</Step>
    <UnitName>degrees</UnitName>
  </processing>

  <processing id="Distance" position="158,10" size="113,58" type="ControlSource">
    <MinValue>0</MinValue>
    <MaxValue>1000</MaxValue>
    <Step>0.1</Step>
    <UnitName>meters</UnitName>
  </processing>

  <processing id="Elevation" position="270,9" size="113,58" type="ControlSource">
    <MinValue>-89.99</MinValue>
    <MaxValue>89.99</MaxValue>
    <Step>0.1</Step>
    <UnitName>degrees</UnitName>
  </processing>

  <processing id="ExponentForDistance" position="386,11" size="152,58" type="ControlSource">
    <MinValue>0</MinValue>
    <MaxValue>2</MaxValue>
    <Step>0.1</Step>
    <UnitName>u</UnitName>
  </processing>

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
    <out>1 Audio Input.AudioOut</out>
    <in>Input Gain.Input Audio</in>
  </port_connection>

  <port_connection>
    <out>Input Gain.Audio Output</out>
    <in>Vbap3D.W</in>
  </port_connection>
"""% {
	'nSpeakers': nSpeakers,
	'layoutFile': layoutFileName,
	'triangulationFile': triangulationFileName,
}

for i, label in enumerate(speakers) :
	print """
  <port_connection>
    <out>Vbap3D.%(speakerLabel)s</out>
    <in>%(speakerLabel)s.AudioIn</in>
  </port_connection>
""" % {
	'speakerNumber' : i+1,
	'speakerLabel' : "%02i"%(i+1), #label,
}

print """
  <control_connection>
    <out>Azimuth.output</out>
    <in>Vbap3D.azimuth</in>
  </control_connection>

  <control_connection>
    <out>Distance.output</out>
    <in>GainBecauseOfDistance.input distance</in>
  </control_connection>

  <control_connection>
    <out>Elevation.output</out>
    <in>Vbap3D.elevation</in>
  </control_connection>

  <control_connection>
    <out>ExponentForDistance.output</out>
    <in>GainBecauseOfDistance.inverse exponent to calculate gain</in>
  </control_connection>

  <control_connection>
    <out>GainBecauseOfDistance.calculated output gain</out>
    <in>Input Gain.Gain</in>
  </control_connection>

</network>
"""

