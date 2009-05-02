#!/usr/bin/python

import math
import os


def run(command) :
	print "\033[32m$ %s\033[0m"%command
	return os.system(command)


localizations = [
	# elevation, azimuth
	(0, 30),
]

wavs = [
	"pinknoise.wav"
]

algorithms = [
	# name, n intermediate channels, encoder, decoder
	("hoa1", 4, "hoa1roomsimulator.clamnetwork", "hoa1to15decoder.clamnetwork"),
	("hoa2", 9, "hoa2roomsimulator.clamnetwork", "hoa2to15decoder.clamnetwork"),
	("hoa3", 16, "hoa3roomsimulator.clamnetwork", "hoa3to15decoder.clamnetwork"),
	("vbap", 1, None, "vbap15decoder.clamnetwork"),
	("hoa1rev", 4, "hoa1revroomsimulator.clamnetwork", "hoa1to15decoder.clamnetwork"),
]

ambients = [
	# name, zr
	("anechoich", 0),
	("littlereverb", 100),
	("fullreverb", 1000000),
]
try:
	os.makedirs("usertest")
except : pass

print "Generate choreography"
for i, (e,a) in enumerate(localizations) :
	print "Dumping coreo for localization", i, "at", (e,a)
	choreoFileName = 'usertest/position_%02i.coreo'%i
	choreo = open(choreoFileName, 'w')
	# frame, fov,la,le,lr,lx,ly,lz,sx,sy,sz
	r = 1.2
	x = r*math.cos(math.radians(a))*math.cos(math.radians(e))
	y = r*math.sin(math.radians(a))*math.cos(math.radians(e))
	z = r*math.sin(math.radians(e))
	choreoLine = "0 0  0 0 0  0 0 0  %f %f %f"%(x,y,z)
	print >> choreo, "#ClamChoreoVersion 1.3"
	for a in xrange(100) :
		print >> choreo, choreoLine
	choreo.close()

print "Generate geometries"
for space_name, z in ambients :
	print "Generate geometry for", space_name
	geometryFileName = 'usertest/geometry_%s.data'%space_name
	geometry = open(geometryFileName, 'w')
	print >> geometry, """<sala_01>
<VERTS>
-2.7 -2.7 -2.7
2.7 -2.7 -2.7
-2.7 2.7 -2.7
2.7 2.7 -2.7
-2.7 -2.7 2.7
2.7 -2.7 2.7
-2.7 2.7 2.7
2.7 2.7 2.7
<FACES>
3 1 4 13.0 %(z)s 0.0 material0
2 4 1 13.0 %(z)s 0.0 material0
6 5 8 13.0 %(z)s 0.0 material0
7 8 5 13.0 %(z)s 0.0 material0
2 1 6 13.0 %(z)s 0.0 material0
5 6 1 13.0 %(z)s 0.0 material0
4 2 8 13.0 %(z)s 0.0 material0
6 8 2 13.0 %(z)s 0.0 material0
3 4 7 13.0 %(z)s 0.0 material0
8 7 4 13.0 %(z)s 0.0 material0
1 3 5 13.0 %(z)s 0.0 material0
7 5 3 13.0 %(z)s 0.0 material0
"""%dict(z=z)
	geometry.close()

for i in 1, 2, 3 :
	run("./generateHoaRoomSimulator.py %(i)i usertest/coreo usertest/geometry > usertest/hoa%(i)iroomsimulator.clamnetwork"%globals())
	run("./generateHoaDecoderNetwork.py layouts/sixteen.layout %(i)i > usertest/hoa%(i)ito15decoder.clamnetwork"%globals())
run("cp usertest/hoa1roomsimulator.clamnetwork usertest/hoa1revroomsimulator.clamnetwork")
run("./generateVbapDecoderNetwork.py layouts/sixteen.layout layouts/sixteen.triangulation > usertest/vbap15decoder.clamnetwork ")
run("clamrefactor.py usertest/hoa1roomsimulator.clamnetwork -c 'setConfig RoomImpulseResponseSimulator SeparateDirectSoundAndReverb 1' > usertest/hoa1revroomsimulator.clamnetwork")

for space_name, z in ambients :
	run("cp usertest/geometry_%s.data usertest/geometry"%space_name)
	for posi, pos in enumerate(localizations) :
		run("cp usertest/position_%02i.coreo usertest/coreo"%posi)
		for algorithm_name, channels, encoder, decoder in algorithms :
			for wav in wavs :
				print "Generating", algorithm_name, "for", wav, "with absortion", z
				if encoder and decoder :
					run("OfflinePlayer usertest/%(encoder)s %(wav)s -o -c %(channels)s usertest/encoded_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))
					run("OfflinePlayer usertest/%(decoder)s usertest/encoded_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s -o -c 15 usertest/15_%(algorithm_name)s_%(space_name)s_%(posi)i_%(wav)s"%dict(globals()))
				elif decoder and not encoder :
					run("OfflinePlayer usertest/%(decoder)s %(wav)s -o -c 15 usertest/15_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))
					



