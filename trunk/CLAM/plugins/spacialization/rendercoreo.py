#!/usr/bin/python

# INSTALL: ln -s bformat2binaural wavs  

# DONE milestone 1: multiple sources, fixed parameters (sample rate, origen, sequenced-controls), optional inputs DONE
# DONE milestone 2: output in b-format + 5.0 + binaural
# DONE milestone 3: configuragle networks.
# milestone 4: warnings, output dirs. monitor duration of files and sequencer.

import os, sys

# USER CONFIG

basedir=os.environ['HOME']+'/acustica/'

params = {
	'path_file' : basedir+'realitzacions/two_moving_sources2.data',
#	'path_file' : basedir+'realitzacions/arrevola_coreo_simplified.data',
	'controls_per_second' : 24 }
sources = [
#	basedir+'wavs/pluck_1s.wav',
#	basedir+'wavs/metronom.wav',
	basedir+'wavs/inlanguage_100s.wav',
	basedir+'wavs/roland_drums_100s_48khz.wav'
]


# CODE
def run(command) :
	print '\033[32m:: ', command, '\033[0m'
#	return os.system(command)
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print '\033[31mXX ', command, '\033[0m'

network_filename = 'offline.clamnetwork'
def write_parametrized_network(params):
	content = file('offline_networks/parametrized_on_the_fly_choreography.clamnetwork').read()
	file(network_filename,'w').write(content % params)


run('rm *_surround.wav')
run('rm *_binaural.wav')
for i, source in enumerate(sources):
	basename = source.split('/')[-1][:-4]
	print 'Source %i: File: %s' % (i, basename)
	params['source_index']=i
	write_parametrized_network(params)
	run('./OfflinePlayer %s \
		%s \
		W.wav X.wav Y.wav Z.wav' % (network_filename,source) )
	run('./OfflinePlayer example-data/bformat2gformat.clamnetwork \
		W.wav X.wav Y.wav\
		l.wav r.wav sl.wav sr.wav c.wav')
	run('sox -M l.wav r.wav sl.wav sr.wav c.wav %s_surround.wav' % basename)
	run('./OfflinePlayer ~/acustica/bformat2binaural/bformat2binaural.clamnetwork \
		W.wav X.wav Y.wav Z.wav\
		lbinaural.wav rbinaural.wav')
	run('sox -M lbinaural.wav rbinaural.wav %s_binaural.wav' % basename)

if len(sources)<=1 : sys.exit()
run('sox -m *_surround.wav surround.wav')
run('sox -m *_binaural.wav binaural.wav')
