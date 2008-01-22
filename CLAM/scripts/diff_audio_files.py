#! /usr/bin/python
# requirements: sox
# optional: wav2png 
# svn checkout http://wav2png.googlecode.com/svn/trunk/ wav2png-read-only

import os
def run(command) :
	print "\033[32m:: ", command, "\033[0m"
	return os.system(command)
	lines = []
	for line in os.popen(command) :
		lines += line
		print line,
		sys.stdout.flush()
	return lines

def norun(command) :
	print "\033[31mXX ", command, "\033[0m"


run('sox -v -1 2.wav /tmp/negative.wav')
run('sox -m -v 1 1.wav -v 1 /tmp/negative.wav result.wav')
sox_volume = run("sox result.wav -n stat 2>&1 | awk '/Volume/ {print $3}'")
print "vol", sox_volume
norun('wav2png --input result.wav --width 700 --linecolor ff0088 --backgroundcolor dddddd --zerocolor 000000')
