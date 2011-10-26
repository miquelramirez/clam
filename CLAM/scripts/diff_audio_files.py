#! /usr/bin/python
# requirements: sox
# optional: wav2png 
# svn checkout http://wav2png.googlecode.com/svn/trunk/ wav2png-read-only

import os, sys, numpy
import math

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

def diff_files_txt(expected, result, diffbase) :
	extension = os.path.splitext(result)[-1]
	difftxt = diffbase+extension
	are_equal = os.system("diff %s %s > %s" % (expected, result, difftxt) ) == 0
	if are_equal: return None
	return "The result file \033[31m%s\033[0m is different to the expected \033[31m%s\033[0m"%(result,expected)

def diff_files_wav(expected, result, diffbase) :
	extension = os.path.splitext(result)[-1]
	diffwav = diffbase+extension
	substractResult = silentrun('soxsucks --compare %f %s %s 2>&1 && echo OK '%(threshold_amplitude, expected, result))
	if 'OK' in substractResult : return None
	substractResult= [line for line in substractResult.split("\n") if line.find("there is a different sample (")!=-1]
	if substractResult==[]:
		print "Files substraction with soxsucks failed. Check that soxsucks is in the path and that the 2 wav files are comparable: (different lenght or sample-rate: %s %s)" % (expected, result)
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



diff_for_type = {
	".wav" : diff_files_wav,
	".txt" : diff_files_txt,
	".clamnetwork" : diff_files_txt,
	".xml" : diff_files_txt,
	".ttl" : diff_files_txt,
}
def diff_files(expected, result, diffbase) :
	if not os.access(result, os.R_OK):
		print "Result file not found: ", result
		return "Result was not generated: '%s'"%result
	if not os.access(expected, os.R_OK):
		print "Expectation file not found for: ", result
		return "No expectation for the output. Check the results and accept them with the --accept option."
	extension = os.path.splitext(result)[-1]

	if extension in diff_for_type:
		return diff_for_type[extension](expected, result, diffbase)
	else:
		extension = '.txt'
		return diff_for_type[extension](expected, result, diffbase)

if __name__=="__main__" :
		equals = diff_files(*sys.argv[1:])
		if equals:
			print "\033[32m Equals\033[0m"
		else:
			print "\033[31m Non equals\033[0m"
