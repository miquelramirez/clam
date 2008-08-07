#!/usr/bin/python

from MetadataSourceAggregator import *
from FileMetadataSource import *
import sys
import os
from optparse import OptionParser


parser = OptionParser(
	usage="%prog [options] audiofile1 audiofile2 ...",
	version="%prog 1.0")
parser.add_option("-s","--print-schema",
		type='string',
		default=None,
		dest="printSchema",
		metavar="SCHEMAFILE",
		help="Outputs the description schema for the extractor. Use '-' for standard output."
	)
parser.add_option("-f",# "--suffix",
		default=".pool",
		dest="suffix",
		help="Appends SUFFIX to the generated descriptors file (default: '%default')"
	)
parser.add_option("-c",# "--print-configuration",
		type='string',
		default=None,
		dest="ConfigurationFile",
		help=" the Configuration file for the Aggregator."
	)
parser.add_option("-w",# "--write flag of writing back"
		help=" the flag for the write back."
	)
(options, args) = parser.parse_args()


class config :
	#Here define the default configuration
	verbose = True
	sources = [
		("example", FileMetadataSource(path=".",
			schemaFile="CLAMDescriptors.sc",
			poolSuffix=".example",
			extractor="ClamExtractorExample")),
		("chord", FileMetadataSource(path=".",
			schemaFile="Chords.sc",
			poolSuffix=".chord",
			extractor="ChordExtractor")),
	]

	map = [
		# ('TargetScope::TargetAttribute', 'sourceId', 'SourceScope::SourceAttribute'),
		("Song::Frames", "example", "Song::Frames"),
		("Song::ChordFrames", "chord", "Song::Frames"),
		("Song::Danceable", "example", "Song::Danceability"),
		("Frame::Energy", "example", "Frame::Energy"),
		("ChordFrame::Energy", "chord", "Frame::Energy"),
		("ChordFrame::ChordHartePcp", "chord", "Frame::HartePcp"),
		("ChordFrame::HarteChordCorrelation", "chord", "Frame::HarteChordCorrelation"),
		("Song::Harte", "chord", "Song::Chords_Harte"),
		("ExtractedChord::ChordRoot", "chord", "ExtractedChord::Root"),
		("ExtractedChord::ChordMode", "chord", "ExtractedChord::Mode"),
	]
	
	#Here we load new values from configuration file, if it exists
	if options.ConfigurationFile is not None:

		if not os.access(options.ConfigurationFile,os.R_OK) :
			print >> sys.stderr, "Bad Configuration File!"
			sys.exit(-1)

		execfile(options.ConfigurationFile)

provider = MetadataSourceAggregator(config.sources, config.map, verbose=config.verbose)


if options.printSchema is not None:
	target = sys.stdout if options.printSchema == "-" else open(options.printSchema,'w')
	provider.QuerySchema(provider.AvailableDescriptors()).Dump(target)
	sys.exit()

if not args :
	parser.print_help()
	sys.exit()

for audiofile in args:
	print "Processing %s..."%audiofile
	target = open(audiofile+options.suffix,'w')
	poolToCopy=provider.QueryDescriptors(audiofile, provider.AvailableDescriptors())
	poolToCopy.Dump(target)
	if options.w is not None:
		provider.UpdateDescriptors(audiofile,poolToCopy)
	target.close()


