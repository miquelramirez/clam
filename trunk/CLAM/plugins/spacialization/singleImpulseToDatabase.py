#!/usr/bin/python


import numpy
import math
import cmath

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

def shiftSamples(audio, deltaSamples) :
	if deltaSamples == 0 : return
	if deltaSamples > 0 :
		audio[deltaSamples:] = audio[:-deltaSamples]
		audio[0:deltaSamples] = 0
	else:
		deltaSamples = -deltaSamples # make it positive
		audio[0:-deltaSamples] = audio[deltaSamples:]
		audio[-deltaSamples:] = 0

# Configurable parameters. (TODO make them arguments with sane defaults)

NX = 5
NY = 5
sizeX = 30. #meters
sizeY = 30. #meters
recordingDistance = 15
recordingAzimut = 0 #radians
recordedFilesPattern = "lala%s.dat"
filepattern =  "lalaDatabase/%s_emissor_%i-%i-%i_receptor_%i-%i-%i.dat"
xs = 2
ys = 2


print "Loading recorded impulse responses..."
recordedComponents = dict( [ 
	( prefix, numpy.array(readDatFile(recordedFilesPattern % prefix)) ) 
		for prefix in ("P", "X", "Y") ] )
scale = complex(sizeX/NX, sizeY/NY)
minDistance = min(scale.real, scale.imag)
for xt in range(NX) : 
	for yt in range(NY) :
		gridWayToSource = (complex(xs, ys) - complex(xt, yt))
		wayToSource = complex(gridWayToSource.real*scale.real, gridWayToSource.imag*scale.imag)
		distanceToSource = abs(wayToSource) or minDistance
		azimuthToSource = math.atan2(wayToSource.real, wayToSource.imag)
		distanceFactor = recordingDistance/distanceToSource
		azimuthRotation = azimuthToSource - recordingAzimut
		deltaSamples = int((distanceToSource - recordingDistance)*44100/340)
		print distanceToSource, wayToSource
		assert(deltaSamples >= -2000)

		P,X,Y = [distanceFactor * component for component in 
			[recordedComponents[c] for c in ["P","X","Y"] ] ]
		for component in P,X,Y:
			shiftSamples(component, deltaSamples)
		X, Y = \
			(X * math.cos(azimuthRotation) + Y * math.sin(azimuthRotation), 
			-X * math.sin(azimuthRotation) + Y * math.cos(azimuthRotation))

		print "Writing data...", xt, yt
		#continue
		writeDatFile(filepattern % ("p", 0,0,0, xt,yt,0) , P)
		writeDatFile(filepattern % ("x", 0,0,0, xt,yt,0) , X)
		writeDatFile(filepattern % ("y", 0,0,0, xt,yt,0) , Y)
	print



