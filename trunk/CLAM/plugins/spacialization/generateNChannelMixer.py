#!/usr/bin/python

import sys

nSpeakers = int(sys.argv[1])
gain0 = float(sys.argv[2])
gain1 = float(sys.argv[3])

print >> sys.stderr, "Speakers:", nSpeakers

print """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.3.1" id="Unnamed">
"""

print """
  <processing id="Gain 0" position="129,20" size="190,80" type="OutControlSender">
    <Min>0</Min>
    <Default>%(gain0)s</Default>
    <Max>1</Max>
    <Step>0.01</Step>
    <ControlRepresentation>Horizontal Slider</ControlRepresentation>
    <Mapping>Linear</Mapping>
  </processing>

  <processing id="Gain 1" position="306,19" size="190,80" type="OutControlSender">
    <Min>0</Min>
    <Default>%(gain1)s</Default>
    <Max>1</Max>
    <Step>0.01</Step>
    <ControlRepresentation>Horizontal Slider</ControlRepresentation>
    <Mapping>Linear</Mapping>
  </processing>
"""%dict(gain0=gain0, gain1=gain1)

for i in xrange(nSpeakers) :
	print """\
  <processing id="Input_%(i)02i_0" position="0,%(y1)s" size="128,108" type="AudioSource"/>

  <processing id="Input_%(i)02i_1" position="5,%(y2)s" size="128,108" type="AudioSource"/>

  <processing id="Mixer_%(i)02i" position="213,%(y1)s" size="143,62" type="AudioMixer">
    <NumberOfInPorts>2</NumberOfInPorts>
    <DefaultGains size="2">1 1</DefaultGains>
  </processing>

  <processing id="Ouput_%(i)02i" position="446,%(y1)s" size="128,111" type="AudioSink"/>
""" % dict(i = i, y1=110+i*100, y2=120+(nSpeakers+i)*100)

for i in xrange(nSpeakers) :
	print """\
  <port_connection>
    <out>Input_%(i)02i_0.1</out>
    <in>Mixer_%(i)02i.Input 0</in>
  </port_connection>

  <port_connection>
    <out>Input_%(i)02i_1.1</out>
    <in>Mixer_%(i)02i.Input 1</in>
  </port_connection>

  <port_connection>
    <out>Mixer_%(i)02i.Output Audio</out>
    <in>Ouput_%(i)02i.1</in>
  </port_connection>
"""%dict(i=i)

for i in xrange(nSpeakers) :
	print """\
  <control_connection>
    <out>Gain 0.out</out>
    <in>Mixer_%(i)02i.Gain 0</in>
  </control_connection>

  <control_connection>
    <out>Gain 1.out</out>
    <in>Mixer_%(i)02i.Gain 1</in>
  </control_connection>
"""%dict(i=i)

print """
</network>
"""

