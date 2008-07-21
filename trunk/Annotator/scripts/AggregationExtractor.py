#!/usr/bin/python

from MetadataSourceAggregator import *
from FileMetadataSource import *
import sys
import os
from optparse import OptionParser

# location of the first extractor's fixed schema file and pool files
path1="D:/devel/Annotator/example-data/fileExample/"
# location of the second extractor's fixed schema file and pool files
path2="D:/devel/Annotator/example-data/fileChord/"
# location of the output aggregated pool files
path3="D:/devel/Annotator/example-data/"

sources = [("example", FileMetadataSource(path1, extractor="D:/devel/Annotator/ClamExtractorExample.exe")),
("chord", FileMetadataSource(path2, extractor="D:/devel/Annotator/ChordExtractor.exe")),]

map = [("Track::Player", "example", "Song::Artist"),("Track::Harte", "chord", "Song::Chords_Harte"),]

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
	provider.QuerySchema(provider.AvailableDescriptors()).Dump(sys.stdout)
	sys.exit()


import sys
for audiofile in args:
	target = open(path3+audiofile+options.suffix,'w')
	provider.QueryDescriptors(audiofile, provider.AvailableDescriptors()).Dump(target)
	target.close()


