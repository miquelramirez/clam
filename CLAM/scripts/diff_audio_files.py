#! /usr/bin/python
# requirements: sox
# optional: wav2png 
# svn checkout http://wav2png.googlecode.com/svn/trunk/ wav2png-read-only

import os, sys, numpy
def run(command) :
	print "\033[32m:: ", command, "\033[0m"
	lines = []
	for line in os.popen(command) :
		lines += line
		print line,
		sys.stdout.flush()
	return "".join(lines)

def silentrun(command) :
	lines = []
	for line in os.popen(command) :
		lines += line
		sys.stdout.flush()
	return "".join(lines)

def norun(command) :
	print "\033[31mXX ", command, "\033[0m"


threshold_dBs = -80.0 # dB
threshold_amplitude = 10**(threshold_dBs/20)

import math
def diff_files(expected, result, diffbase) :
	if not os.access(result, os.R_OK):
		print "Result file not found: ", result
		return "Result was not generated: '%s'"%result
	if not os.access(expected, os.R_OK):
		print "Expected file not found: ", result
		return "No expectation for the output. Check the results and accept them with the --accept option."
	return diff_files_wav(expected, result, diffbase)

def diff_files_wav(expected, result, diffbase) :
	diffwav = diffbase+'.wav'
	substractResult = silentrun('soxsucks --compare %f %s %s 2>&1 && echo OK '%(threshold_amplitude, expected, result))
	if 'OK' in substractResult : return None
	substractResult= [line for line in substractResult.split("\n") if line.find("there is a different sample (")!=-1]
	if substractResult==[]:
		print "files substraction with sox failed. They might not be comparable: (different lenght or sample-rate: %s %s)" % (expected, result)
		return "Non comparable waves (different lenght, channels or sample-rate)"
	substractResult="".join(substractResult)
# TODO: REFACTOR, and use wrapper python of soxsucks compare function!!
	def extract(content, start, stop) :
		return content[content.find(start)+len(start):].split(stop)[0]
	errorString = extract(substractResult, "amplitude diff: ", ")" )
	timeErrorString = extract(substractResult, "on second ", "\n" )
	sampleString = extract(substractResult, "in the sample ", "," )
	silentrun('sox -m -v 1 %s -v -1 %s %s 2>&1 '%(expected, result, diffwav))
#####
	max_amplitude = abs(float(errorString))
	if not numpy.isfinite(max_amplitude) :
		print "One of the files contains infinite values or NaN's"
		return "One of the files contains infinite values or NaN's"
	if not max_amplitude : return None
	max_dBs = 20*math.log10(max_amplitude)
	if max_dBs > threshold_dBs :
		silentrun('wav2png --input %s --width 700 --linecolor ff0088 --backgroundcolor dddddd --zerocolor 000000'%(diffwav))
		print "Files are different with threshold %f dB (amplitude: %f)" % (threshold_dBs, threshold_amplitude)
		print "\tMax diff is %f dB (amplitude: %f)" % ( max_dBs, max_amplitude)
		print "\tOn sample: %s" % sampleString
		return "Differences of %s dB (threshold: %s dB) at %s seconds (sample %s)" % (
			max_dBs, threshold_dBs, timeErrorString, sampleString)
	return None


if __name__=="__main__" :
		equals = diff_files(*sys.argv[1:])
		if equals:
			print "\033[32m Equals\033[0m"
		else:
			print "\033[31m Non equals\033[0m"
