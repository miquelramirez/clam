#! /usr/bin/python
# requirements: sox
# optional: wav2png 
# svn checkout http://wav2png.googlecode.com/svn/trunk/ wav2png-read-only

import os, sys
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


def diff_files(expected, result) :

	silentrun('sox -m -v 1 %s -v -1 %s diff.wav 2>&1 '%(expected, result))
	max_sample = float(silentrun("sndfile-info diff.wav | awk '/Signal/ {print $4}'"))
	print "max", max_sample

	threshold = 0.00001

	if max_sample > threshold :
		silentrun('wav2png --input diff.wav --width 700 --linecolor ff0088 --backgroundcolor dddddd --zerocolor 000000')
		print "Files are different with threshold", threshold
		print "Maxi diff is :", max_sample
		print "expected:",expected
		print "result:",result
		return False
	return True

equals = diff_files(sys.argv[1], sys.argv[2])
print "equals?", equals
