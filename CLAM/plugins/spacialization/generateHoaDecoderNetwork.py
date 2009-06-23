#!/usr/bin/python



import sys

if len(sys.argv)!=3 :
	print >> sys.stderr, "Usage: %s <layout_file> <order>"%sys.argv[0]
	print >> sys.stderr, """
Generates a network for a Hoa decoder from a given order 
of ambisonics to a given regular speaker layout.
Each row of the layout file contains the elevation 
and azimuth in degrees of each speaker separated by spaces.
Lines with # as the first character are ignored as comments.
"""
	sys.exit()


layoutFileName = sys.argv[1]
order = sys.argv[2]
if order not in '0123':
	print >> sys.stderr, "Order",order,"is not supported. It must be either 0, 1, 2 or 3."
	sys.exit(-1)
order=int(order)
channels="".join(['W','XYZ','RSTUV','KLMNOPQ'][0:order+1])
speakers = [ label for label in [" ".join(line.split()[2:]) for line in open(layoutFileName).readlines() if line[0]!='#' ] if label ]
nSpeakers = len(speakers)

print >> sys.stderr, "Speakers:", nSpeakers

print """<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.3.1" id="Unnamed">
"""

for speaker, label in enumerate(speakers) :
	print """
  <processing id="%s" position="%i,%i" size="128,111" type="AudioSink"/>""" % (
	"%02i"%(speaker+1), 600+64*(speaker%6), 80+(speaker%6)*6+(speaker//6)*95 )
#	speaker+1, label, 600+64*(speaker%6), 80+(speaker%6)*6+(speaker//6)*95 )

for i, channel in enumerate(channels) :
	print """
  <processing id="%i %s in" position="%i,%i" size="128,108" type="AudioSource"/>\
""" % (i+1, channel, 10, 10+i*110)

print """\
  <processing id="Hoa2RegularSpeakerArray" position="257,290" size="128,270" type="Hoa2RegularSpeakerArray">
    <Order>%(order)i</Order>
    <SpeakerLayout>%(layoutFile)s</SpeakerLayout>
    <IgnoreLabels>1</IgnoreLabels>
  </processing>
""" % dict(order=order, layoutFile=layoutFileName)

for i, channel in enumerate(channels) :
	print """\
  <port_connection>
    <out>%i %s in.1</out>
    <in>Hoa2RegularSpeakerArray.%s</in>
  </port_connection>
""" % (i+1, channel, channel)

for speaker, label in enumerate(speakers) :
	print """\
  <port_connection>
    <out>Hoa2RegularSpeakerArray.%(speakerName)s</out>
    <in>%(speakerName)s.1</in>
  </port_connection>
""" % dict(
#	speakerNumber=speaker+1,
	speakerName="%02i"%(speaker+1), #label,
	)

print """
</network>
"""

