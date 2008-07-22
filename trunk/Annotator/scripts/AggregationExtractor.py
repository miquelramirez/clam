#!/usr/bin/python

from MetadataSourceAggregator import *
from FileMetadataSource import *
import sys
import os
from optparse import OptionParser


sources = [
	("example", FileMetadataSource(path="./",
		schemaTitle="CLAMDescriptors.sc",
		poolSuffix=".pool",
		extractor="D:/devel/Annotator/ClamExtractorExample.exe")),
	("chord", FileMetadataSource(path="./",
		schemaTitle="Chords.sc",
		poolSuffix=".chords",
		extractor="D:/devel/Annotator/ChordExtractor.exe")),
]

map = [
	("Song::Frames", "example", "Song::Frames"),
	("Song::Danceable", "example", "Song::Danceability"),
	#("TrackFrame::ClamMean", "example", "Frame::Mean"),  # there is a bug or something.
	("Frame::Energy", "chord", "Frame::Energy"),
	("Frame::ChordHartePcp", "chord", "Frame::HartePcp"),
	("Frame::HarteChordCorrelation", "chord", "Frame::HarteChordCorrelation"),
	("Song::Harte", "chord", "Song::Chords_Harte"),
	# How to remain the childscope relationship while changing the scope names?
	("ExtractedChord::ChordRoot", "chord", "ExtractedChord::Root"),
	("ExtractedChord::ChordMode", "chord", "ExtractedChord::Mode"),
]


provider = MetadataSourceAggregator(sources, map)


parser = OptionParser(
	usage="%prog [options] audiofile1 audiofile2 ...",
	version="%prog 1.0")
parser.add_option("-s","--print-schema",
		action="store_true",
		default=False,
		dest="printSchema",
		help="Outputs the description schema for the extractor"
	)
parser.add_option("-f",# "--suffix",
		default=".pool",
		dest="suffix",
		help="Appends SUFFIX to the generated descriptors file (default: '%default')"
	)
(options, args) = parser.parse_args()


if options.printSchema :
	target= open("./Aggregation.sc",'w')
	provider.QuerySchema(provider.AvailableDescriptors()).Dump(target)
	sys.exit()


import sys
for audiofile in args:
	target = open(audiofile+options.suffix,'w')
	provider.QueryDescriptors(audiofile, provider.AvailableDescriptors()).Dump(target)
	target.close()

