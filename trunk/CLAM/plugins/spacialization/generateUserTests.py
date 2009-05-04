#!/usr/bin/python

import math
import os
import sys

locations = [
	# elevation, azimuth
	(0, 30),
]

wavs = [
	"pinknoise.wav",
]

ambients = [
	# name, zr
	("anechoich", 0),
	("littlereverb", 100),
	("fullreverb", 1000000),
]


interTestSilence = 5 # seconds
reverbGain = 1.5

def die(message) :
	print "\033[31m%s\033[0m"%message
	sys.exit(-1)

def run(command) :
	print "\033[32m$ %s\033[0m"%command
	os.system(command)==0 or die("Command failed: "+command)


algorithms = [
	# name, n intermediate channels, encoder, decoder
	("hoa1", 4, "hoa1roomsimulator.clamnetwork", "hoa1to15decoder.clamnetwork"),
	("hoa2", 9, "hoa2roomsimulator.clamnetwork", "hoa2to15decoder.clamnetwork"),
	("hoa3", 16, "hoa3roomsimulator.clamnetwork", "hoa3to15decoder.clamnetwork"),
	("hoa1rev", 4, "hoa1revroomsimulator.clamnetwork", "hoa1to15decoder.clamnetwork"),
	("vbap", 15, "vbap15decoder.clamnetwork", "vbaphoa1mixer.clamnetwork"),
]

for wav in wavs :
	if os.path.basename(wav) != wav :
		die("Wavs should be in the working dir. Use simbolic links.")

try:
	os.makedirs("usertest")
except : pass

print "Generate choreography"
for i, (e,a) in enumerate(locations) :
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
3 1 4 %(z)s 0.0 0.5 material0
2 4 1 %(z)s 0.0 0.5 material0
6 5 8 %(z)s 0.0 0.5 material0
7 8 5 %(z)s 0.0 0.5 material0
2 1 6 %(z)s 0.0 0.5 material0
5 6 1 %(z)s 0.0 0.5 material0
4 2 8 %(z)s 0.0 0.5 material0
6 8 2 %(z)s 0.0 0.5 material0
3 4 7 %(z)s 0.0 0.5 material0
8 7 4 %(z)s 0.0 0.5 material0
1 3 5 %(z)s 0.0 0.5 material0
7 5 3 %(z)s 0.0 0.5 material0
"""%dict(z=z)
	geometry.close()


for i in 1, 2, 3 :
	run("./generateHoaRoomSimulator.py %(i)i usertest/coreo usertest/geometry > usertest/hoa%(i)iroomsimulator.clamnetwork"%globals())
	run("./generateHoaDecoderNetwork.py layouts/sixteen.layout %(i)i > usertest/hoa%(i)ito15decoder.clamnetwork"%globals())
run("cp usertest/hoa1roomsimulator.clamnetwork usertest/hoa1revroomsimulator.clamnetwork")
run("./generateVbapDecoderNetwork.py layouts/sixteen.layout layouts/sixteen.triangulation > usertest/vbap15decoder.clamnetwork ")
run("clamrefactor.py usertest/hoa1roomsimulator.clamnetwork -c 'setConfig RoomImpulseResponseSimulator SeparateDirectSoundAndReverb 1' > usertest/hoa1revroomsimulator.clamnetwork")

directSoundGain = 1 if  reverbGain<1 else 1./reverbGain
reverbGain = reverbGain if reverbGain<1. else 1

run("./generateNChannelMixer.py 15 %(directSoundGain)s %(reverbGain)s > usertest/vbaphoa1mixer.clamnetwork"%dict(globals()))

for space_name, z in ambients :
	run("cp usertest/geometry_%s.data usertest/geometry"%space_name)
	for posi, pos in enumerate(locations) :
		run("cp usertest/position_%02i.coreo usertest/coreo"%posi)
		for algorithm_name, channels, encoder, decoder in algorithms :
			for wav in wavs :
				print "Generating", algorithm_name, "for", wav, "with absortion", z
				run("OfflinePlayer usertest/%(encoder)s %(wav)s -o -c %(channels)s usertest/encoded_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))
				if algorithm_name != "vbap" :
					run("OfflinePlayer usertest/%(decoder)s usertest/encoded_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s -o -c 15 usertest/15_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))
				else :
					run("OfflinePlayer usertest/%(decoder)s usertest/encoded_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s usertest/15_hoa1rev_%(space_name)s_%(posi)02i_%(wav)s -o -c 15 usertest/15_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))
					

for space_name, z in ambients :
	for posi, pos in enumerate(locations) :
		for algorithm_name, channels, encoder, decoder in algorithms :
			if algorithm_name == "hoa1rev" : continue
			for wav in wavs :
				run("soxsucks -N usertest/15_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s usertest/tocat_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%dict(globals()))

import glob, random
run("sox -c 15 -n -c 15 usertest/silence.wav trim 0 %(interTestSilence)s"%dict(globals()))

testFiles = glob.glob('usertest/tocat_*wav')
random.shuffle(testFiles)
print testFiles
f=file("usertest/usertest.txt",'w')
print >> f, "Order:"
print >> f, "\n".join(testFiles)
print >> f, "\nPoints (index: elevation, azimuth):"
print >> f, "\n".join(["\t%02i: %s %s"%(i,e,a) for i,(e,a) in enumerate(locations)])
print >> f, "\nAmbients (name: Re(Z)):"
print >> f, "\n".join(["\t%s: %f"%(name,z) for name,z in ambients])
print >> f
f.close()

run("sox "+" usertest/silence.wav ".join(testFiles)+" usertest/usertest.wav ")



