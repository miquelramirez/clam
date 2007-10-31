#! /usr/bin/python

import glob, sys, os

def loadSamples(filename):
	return [ float(sample) for time, sample in [line.split() for line in open(filename).readlines() if line[0]!=';' ]]

def maxsample(filename):
	samples = loadSamples(filename)
	largersample = max( [abs(sample) for sample in samples] )
	print "local max:",largersample
	return largersample

def convert(filename, largersample):
	samples = loadSamples(filename)
	normalizedfile = open("temp.norm.dat","w")
	normalizedfile.write("; Sample Rate 44100\n; Channels 1\n")
	normalizedfile.writelines( ["%s\t%.12f\n"%(time/44100, sample/largersample) for time, sample in zip(range(0,len(samples)), samples) ] )
	outputfile = os.path.splitext(file)[0] + ".wav"
	print "Generating", outputfile
	os.system("sox temp.norm.dat %s" % (outputfile) )


datfiles = glob.glob("lalaDatabase/*.dat")
datfiles.sort()
print "datfiles", datfiles

# .data (text files) normalization to norm.dat
globalmax = 2.15820312
if True:
	globalmax = max( map( maxsample, datfiles) )
	print "max:", globalmax

if True :
	for file in datfiles :
		print "converting: ",file
		convert(file, globalmax)



