#!/usr/bin/python

import math
import os
import sys



interTestSilence = 5 # seconds
locations = [
	# elevation, azimuth
	(0, 60),
]

wavs = [
	"metronom.wav",
]

ambients = [
	# name, zr, d, reverbGain
#	("anechoich", 1, 0.0 ),
	("littlereverb", 100., 0.7),
#	("fullreverb", 1000000, 0.7),
]

reverbGain = 0.0224
vbapGain = 0.1122
orders = 1, # 2, 3,
speakers = 15


def die(message) :
	print "\033[31m%s\033[0m"%message
	sys.exit(-1)

def run(command) :
	print "\033[32m$ %s\033[0m"%command
	os.system(command)==0 or die("Command failed: "+command)

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
for space_name, z, d in ambients :
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
3 1 4 %(z)s 0.0 %(d)s material0
2 4 1 %(z)s 0.0 %(d)s material0
6 5 8 %(z)s 0.0 %(d)s material0
7 8 5 %(z)s 0.0 %(d)s material0
2 1 6 %(z)s 0.0 %(d)s material0
5 6 1 %(z)s 0.0 %(d)s material0
4 2 8 %(z)s 0.0 %(d)s material0
6 8 2 %(z)s 0.0 %(d)s material0
3 4 7 %(z)s 0.0 %(d)s material0
8 7 4 %(z)s 0.0 %(d)s material0
1 3 5 %(z)s 0.0 %(d)s material0
7 5 3 %(z)s 0.0 %(d)s material0
"""%dict(z=z, d=d)
	geometry.close()

run("./generateNChannelMixer.py %(speakers)s 1 %(reverbGain)s > usertest/mixer_hoa.clamnetwork"%globals())
run("./generateNChannelMixer.py %(speakers)s %(vbapGain)s %(reverbGain)s > usertest/mixer_vbap.clamnetwork"%globals())


for i in 1, 2, 3 :
	run("./generateHoaRoomSimulator.py %(i)i usertest/coreo usertest/geometry > usertest/hoa%(i)i_room.clamnetwork"%globals())
	run("./generateHoaDecoderNetwork.py layouts/sixteen.layout %(i)i > usertest/hoa%(i)i_15decoder.clamnetwork"%globals())
	run("clamrefactor.py usertest/hoa%(i)i_room.clamnetwork -c 'setConfig RoomImpulseResponseSimulator NRebounds 0' > usertest/hoa%(i)i_room_ds.clamnetwork"%globals())
	run("clamrefactor.py usertest/hoa%(i)i_room.clamnetwork -c 'setConfig RoomImpulseResponseSimulator SeparateDirectSoundAndReverb 1' > usertest/hoa%(i)i_room_rev.clamnetwork"%globals())

run("./generateVbapDecoderNetwork.py layouts/sixteen.layout layouts/sixteen.triangulation usertest/coreo > usertest/vbap_ds.clamnetwork ")

# Generating direct sounds
for space_name, z, d in ambients :
	run("cp usertest/geometry_%s.data usertest/geometry"%space_name)
	for posi, pos in enumerate(locations) :
		run("cp usertest/position_%02i.coreo usertest/coreo"%posi)
		for order in orders :
			channels = (order+1)**2
			encoder = "usertest/hoa%(order)i_room_ds.clamnetwork"%globals()
			decoder = "usertest/hoa%(order)i_15decoder.clamnetwork"%globals()
			for wav in wavs :
				encoded = "usertest/encoded_hoa%(order)i_ds_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				decoded = "usertest/decoded_hoa%(order)i_ds_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				run("OfflinePlayer %(encoder)s %(wav)s -o -c %(channels)s %(encoded)s"%globals())
				run("OfflinePlayer %(decoder)s %(encoded)s -o -c %(speakers)i %(decoded)s"%globals())
		for wav in wavs :
			decoder = "usertest/vbap_ds.clamnetwork"%globals()
			decoded = "usertest/decoded_vbap_ds_%(space_name)s_%(posi)02i_%(wav)s"%globals()
			run("OfflinePlayer %(decoder)s %(wav)s -o -c %(speakers)s %(decoded)s"%globals())

# Generating reverbs
for space_name, z, d in ambients :
	run("cp usertest/geometry_%s.data usertest/geometry"%space_name)
	for posi, pos in enumerate(locations) :
		run("cp usertest/position_%02i.coreo usertest/coreo"%posi)
		for order in orders :
			channels = (order+1)**2
			encoder = "usertest/hoa%(order)i_room_rev.clamnetwork"%globals()
			decoder = "usertest/hoa%(order)i_15decoder.clamnetwork"%globals()
			for wav in wavs :
				encoded = "usertest/encoded_hoa%(order)i_rev_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				decoded = "usertest/decoded_hoa%(order)i_rev_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				run("OfflinePlayer %(encoder)s %(wav)s -o -c %(channels)s %(encoded)s"%globals())
				run("OfflinePlayer %(decoder)s %(encoded)s -o -c %(speakers)i %(decoded)s"%globals())

# Mixing reverb and direct sound
for space_name, z, d in ambients :
	for posi, pos in enumerate(locations) :
		mixer = "usertest/mixer_hoa.clamnetwork"%globals()
		for order in orders :
			channels = (order+1)**2
			decoder = "usertest/hoa%(order)i_15decoder.clamnetwork"%globals()
			for wav in wavs :
				reverb = "usertest/decoded_hoa%(order)i_rev_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				direct_sound = "usertest/decoded_hoa%(order)i_ds_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				mixed = "usertest/mixed_hoa%(order)i_%(space_name)s_%(posi)02i_%(wav)s"%globals()
				run("OfflinePlayer %(mixer)s %(direct_sound)s %(reverb)s -o -c %(speakers)s %(mixed)s"%globals())
		mixer = "usertest/mixer_vbap.clamnetwork"%globals()
		for wav in wavs :
			order = 1
			reverb       = "usertest/decoded_hoa%(order)i_rev_%(space_name)s_%(posi)02i_%(wav)s"%globals()
			direct_sound = "usertest/decoded_vbap_ds_%(space_name)s_%(posi)02i_%(wav)s"%globals()
			mixed        = "usertest/mixed_vbap_%(space_name)s_%(posi)02i_%(wav)s"%globals()
			run("OfflinePlayer %(mixer)s %(direct_sound)s %(reverb)s -o -c %(speakers)s %(mixed)s"%globals())

# Normalizing
for space_name, z, d in ambients :
	for posi, pos in enumerate(locations) :
		for algorithm_name in ['hoa%i'%o for o in orders] + ['vbap'] :
			for wav in wavs :
				run("soxsucks -N usertest/mixed_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s usertest/tocat_%(algorithm_name)s_%(space_name)s_%(posi)02i_%(wav)s"%globals())

# Shuffling and save the shuffled order for later reference
import random
import glob
testFiles = glob.glob('usertest/tocat_*wav')
random.shuffle(testFiles)

f=file("usertest/usertest.txt",'w')
print >> f, "Order:"
print >> f, "\n".join(testFiles)
print >> f, "\nPoints (index: elevation, azimuth):"
print >> f, "\n".join(["\t%02i: %s %s"%(i,e,a) for i,(e,a) in enumerate(locations)])
print >> f, "\nAmbients (name: Re(Z)):"
print >> f, "\n".join(["\t%s: %f"%(name,z) for name,z,d in ambients])
print >> f
f.close()

# Concatenating them
run("sox -c %(speakers)s -r 48000 -n -c %(speakers)s usertest/silence.wav trim 0 %(interTestSilence)s"%globals())
run("sox "+" usertest/silence.wav ".join(testFiles)+" usertest/usertest.wav ")


