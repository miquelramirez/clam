#! /usr/bin/python

import glob, sys, os

SampleRate = 48000
def loadSamples(filename):
	return [ float(sample) for time, sample in [line.split() for line in open(filename).readlines() if line[0]!=';' ]]

def maxsample(filename):
	samples = loadSamples(filename)
	largersample = max( [abs(sample) for sample in samples] )
	print "local max:",largersample
	return largersample

def convertDatToWav(filename, largersample):
	samples = loadSamples(filename)
	normalizedfile = open("temp.norm.dat","w")
	normalizedfile.write("; Sample Rate %i\n; Channels 1\n" % SampleRate)
	anticlippingFactor = .98
	normalizedfile.writelines( ["%s\t%.20f\n"%(time/SampleRate, sample*anticlippingFactor/largersample) for time, sample in zip(range(0,len(samples)), samples) ] )
	outputfile = os.path.splitext(filename)[0] + ".wav"
	print "Generating", outputfile
	os.system("sox temp.norm.dat %s" % (outputfile) )


def processDir(dir):
	datfiles = glob.glob( dir+"/*.dat")
	datfiles.sort()
	print "datfiles", datfiles

	# .data (text files) normalization to norm.dat
	globalmaxSampler = 2.15820312
	if True:
		globalmaxSampler = max( map( maxsample, datfiles) )
		print "max:", globalmaxSampler

	if True :
		for filename in datfiles :
			print "converting: ",filename
			convertDatToWav(filename, globalmaxSampler)

def main():
	import sys
	if len(sys.argv)<=1 :
		print "Error: pass the dir with *.dat as argument"
		return
		 
	dir = sys.argv[1]
	print "Normalizing and converting to wav this dir: ", dir
	processDir( dir )


if __name__=="__main__" :
	main()
