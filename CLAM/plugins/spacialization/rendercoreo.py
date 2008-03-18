#!/usr/bin/python

# milestone 1: multiple sources, fixed parameters (sample rate, origen, sequenced-controls), optional inputs DONE
# milestone 2: output in b-format + 5.0 + binaural
# milestone 3: configurable networks and warnings, output dirs. monitor duration of files and sequencer.
# ln -s bformat2binaural wavs  

# USER CONFIG

sources = [
	"pluck_1s.wav",
	"metronom.wav",
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
	run('./OfflinePlayer offline_networks/on_the_fly_surround.clamnetwork \
		~/acustica/wavs/%s \
		W.wav X.wav Y.wav Z.wav' % source )
	run('./OfflinePlayer example-data/bformat2gformat.clamnetwork \
		W.wav X.wav Y.wav\
		l.wav r.wav sl.wav sr.wav c.wav')
	run('sox -M l.wav r.wav sl.wav sr.wav c.wav %s_surround.wav' % basename)
	run('./OfflinePlayer ~/acustica/bformat2binaural/bformat2binaural.clamnetwork \
		W.wav X.wav Y.wav Z.wav\
		lbinaural.wav rbinaural.wav')
	run('sox -M lbinaural.wav rbinaural.wav %s_binaural.wav' % basename)
run('sox -m *_surround.wav surround.wav')
run('sox -m *_binaural.wav binaural.wav')
