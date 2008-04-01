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


threshold = -80.0 # dB

def diff_files(expected, result) :
	diff = "diff_"+os.path.basename(expected)
	silentrun('sox -m -v 1 %s -v -1 %s %s 2>&1 '%(expected, result, diff))
	max_dbs = float(silentrun("sndfile-info %s | awk '/Signal/ {print $5}'"%(diff))[1:])
	if max_dbs > threshold :
		silentrun('wav2png --input %s --width 700 --linecolor ff0088 --backgroundcolor dddddd --zerocolor 000000'%(diff))
		print "Files are different with threshold", threshold
		print "Max diff is :", max_dbs, "dB"
		print "expected:",expected
		print "result:",result
		return False
	return True


if __name__=="__main__" :
		equals = diff_files(sys.argv[1], sys.argv[2])
		if equals:
			print "\033[32m Equals\033[0m"
		else:
			print "\033[31m Non equals\033[0m"
