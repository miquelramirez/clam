#!/usr/bin/python


def readDatFile(filename) :
	wavefile = file(filename, "r")
	return [ float(sample) for time, sample in [ line.split() for line in wavefile.readlines() if line[0]!=";"]]

def writeDatFile(filename, samples) :
	result = []
	result += ["; Sample Rate 44100\n"]
	result += ["; Channels 1\n"]
	result += [ "%.11g %.11g\n"%(timeInSamples/44100., sample) for timeInSamples, sample in  zip( range(0, len(samples)), samples) ]
	wavefile = file(filename, "w")
	wavefile.writelines(result)


NX = 5
NY = 4
sizeX = 100. #meters
sizeY = 70. #meters
recordingDistance = 10
recordingAzimut = 0 #radians
impulseResponseFile = "lala.dat"

import math
print "rec distance ", recordingDistance
print "rec angle ", recordingAzimut

samples = readDatFile(impulseResponseFile)
filepattern =  "lalaDatabase/%s_emissor_%i-%i-%i_receptor_%i-%i-%i.dat"
xs = 2
ys= 2
for xt in range(NX) : 
	for yt in range(NY) :
		presureFilename = filepattern % ("p", 0,0,0, xt,yt,0)
		diff = complex(float(xs)/NX*sizeX, float(ys)/NY*sizeY) - complex(float(xt)/NX*sizeX, float(yt)/NY*sizeY )
		distance = abs(diff)
		if distance<.01 : distance=.01
		print distance, '\t',
		pressures = [ sample*recordingDistance/distance for sample in samples ]
		writeDatFile(presureFilename, samples)
	print
	

def test() :
	samples = readDatFile("lala.dat")
	writeDatFile("out.dat", samples)



