#!/usr/bin/python

# milestone 1: multiple sources, fixed parameters (sample rate, origen, sequenced-controls), optional inputs
# milestone 2: configurable networks and warnings, output dirs. monitor duration of files and sequencer.

# USER CONFIG

sources = [
	"pluck_1s.wav",
	"clicks.wav",
	]

# CODE
import os, sys
def run(command) :
	print "\033[32m:: ", command, "\033[0m"
#	return os.system(command)
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print "\033[31mXX ", command, "\033[0m"

run('rm *_surround.wav')
for source in sources:
	basename = source[:-4]
	print "Rendering input:", basename
	run('rm -f /tmp/outraytracing')
	run('./OfflinePlayer offline_networks/on_the_fly_surround-source_1.clamnetwork \
		%s \
		c.wav l.wav r.wav sl.wav sr.wav' % source )
	run('sox -M l.wav r.wav sl.wav sr.wav c.wav %s_surround.wav' % basename)
run('sox -m *_surround.wav surround.wav')
