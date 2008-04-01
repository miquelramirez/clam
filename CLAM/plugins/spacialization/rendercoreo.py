#!/usr/bin/python

# INSTALL: ln -s bformat2binaural wavs  

# DONE milestone 1: multiple sources, fixed parameters (sample rate, origen, sequenced-controls), optional inputs DONE
# DONE milestone 2: output in b-format + 5.0 + binaural
# DONE milestone 3: configuragle networks.
# milestone 4: warnings, output dirs. monitor duration of files and sequencer.
# TODO export IR histeresis distance to settings
import os, sys

# USER CONFIG

basedir=os.environ['HOME']+'/acustica/'

renderParams = { # parameters for the network template
#	'path_file' : basedir+'realitzacions/two_moving_sources2.data',
#	'path_file' : basedir+'realitzacions/target_90_source_left.data',
	'path_file' : basedir+'realitzacions/Simple_03.coreo',
	'3Dmodel_file' : 'entorns/salo_simplificat.dat',
	'controls_per_second' : 24,
	}
generalParams = {
	'video' : basedir+'realitzacions/Simple_03.flv', # OPTIONAL: if existent puts the binaural output to this video.
	}

sources = [
#	basedir+'wavs/pluck_1s.wav', 
#	basedir+'wavs/metronom.wav',
#	basedir+'wavs/inlanguage_100s.wav',
	basedir+'wavs/roland_drums_100s_48khz.wav'
]


enable_binaural = True
enable_surround = True

# CODE
def run(command) :
	print '\033[32m:: ', command, '\033[0m'
#	return os.system(command)
	for line in os.popen(command) :
		print line,
		sys.stdout.flush()
def norun(command) :
	print '\033[31mXX ', command, '\033[0m'

generated_network_filename = 'generated.clamnetwork'
clam_path = os.path.join(os.environ['HOME'], 'clam')
spacialization_path = os.path.join(clam_path,'CLAM','plugins','spacialization')
parametrized_network_filename = os.path.join(spacialization_path,
	'offline_networks','parametrized_on_the_fly_choreography.clamnetwork')
offlineplayer_bin = os.path.join(clam_path,'NetworkEditor','OfflinePlayer')

def write_parametrized_network(renderParams):
	content = file(parametrized_network_filename).read()
	file(generated_network_filename,'w').write(content % renderParams)


controlsCount = float(os.popen("wc -l "+renderParams['path_file']).read().split()[0])
totalTime = controlsCount / renderParams['controls_per_second']
print "TIME",totalTime

if enable_surround: run('rm -f *_surround.wav')
if enable_binaural: run('rm -f *_binaural.wav')
run('rm -f *_bformat.wav')
run('ln -s ~/acustica/bformat2binaural/Ew.wav')
run('ln -s ~/acustica/bformat2binaural/Ex.wav')
run('ln -s ~/acustica/bformat2binaural/Ey.wav')
run('ln -s ~/acustica/bformat2binaural/Ez.wav')

for i, source in enumerate(sources):
	basename = source.split('/')[-1][:-4]
	print 'Source %i: File: %s' % (i, basename)
	renderParams['source_index']=i
	write_parametrized_network(renderParams)
	run( '%s %s -t %f %s W.wav X.wav Y.wav Z.wav' % 
		(offlineplayer_bin, generated_network_filename, totalTime, source) )
	run('sox -M W.wav X.wav Y.wav Z.wav %s_bformat.wav' % basename)
	if enable_surround:
		run('%s %s/example-data/bformat2gformat.clamnetwork W.wav X.wav Y.wav l.wav r.wav sl.wav sr.wav c.wav' % (offlineplayer_bin, spacialization_path) )
		run('sox  -M l.wav r.wav sl.wav sr.wav c.wav %s_surround.wav' % basename)
	if enable_binaural:
		run( offlineplayer_bin + ' ~/acustica/bformat2binaural/bformat2binaural.clamnetwork W.wav X.wav Y.wav Z.wav lbinaural.wav rbinaural.wav' )
		run('sox -M -v 10 lbinaural.wav -v 10 rbinaural.wav  %s_binaural.wav' % basename)

if len(sources)<=1 : 
	run('cp *_binaural.wav binaural.wav')
	run('cp *_binaural.wav binaural.wav')
else :	
	run('sox -m *_surround.wav surround.wav')
	run('sox -m *_binaural.wav binaural.wav')

if generalParams.has_key('video') and generalParams['video']:
	video_path = generalParams['video']
	video_nopath = os.path.basename( generalParams['video'] )
	video_basename, video_ext = os.path.splitext(video_nopath)
	run('mencoder -ovc copy %s -audiofile binaural.wav -oac copy -o %s_binaural%s' % 
		(video_path, video_basename, video_ext)
		)

#tear down
run('rm E?.wav')
