#!/usr/bin/python


import sys
import os
from optparse import OptionParser

parser = OptionParser(
	usage="%prog [options] audiofile1 audiofile2 ...",
	version="%prog 1.0")
parser.add_option("-s","--print-schema",
		default=None,
		dest="printSchema",
		metavar="SCHEMAFILE",
		help="Outputs the description schema for the extractor. Use '-' for standard output."
	)
parser.add_option("-w","--writeback",
		action="store_true",
		dest="writeback",
		help="Writes back the descriptors to the source. (Not applied in this extrator)"
	)
parser.add_option("-f","--suffix",
		default=".pool",
		dest="suffix",
		help="Appends SUFFIX to the generated descriptors file (default: '%default')"
	)
parser.add_option("-c","--configuration",
		default='',
		dest="ConfigurationFile",
		help="Specifies a configuration file with the options for the extractor."
	)
(options, args) = parser.parse_args()



if options.printSchema :
	schemaFile = sys.stdout if options.printSchema == "-" else file(options.printSchema,'w')
	print >> schemaFile, """<?xml version='1.0' encoding='UTF-8' standalone='no' ?>
<DescriptionScheme>

 <Uri>descriptionScheme:www.iua.upf.edu:clam:OnsetExtraction</Uri>

 <Attributes>
   <Attribute name='Onsets' scope='Song' type='Segmentation'>
     <ChildScope></ChildScope>
     <SegmentationPolicy>Unsized</SegmentationPolicy>
   </Attribute>
 </Attributes>

</DescriptionScheme>

"""
	sys.exit()

if options.writeback :
	sys.exit()

dataTemplate = """<?xml version='1.0' encoding='UTF-8' standalone='no' ?>
<Description>

  <ScopePool name="Song" size="1">
    <AttributePool name="Onsets" size="%i">
		%s
    </AttributePool>
  </ScopePool>

</Description>
"""

import sys
for audiofile in args:
	if audiofile.endswith(".ogg") :
		os.popen('ogg123 -d wav -f lala.wav %s'%audiofile).read()
		audiofile="lala.wav"
	if audiofile.endswith(".mp3") :
		os.popen('mpg123 -w lala.wav %s'%audiofile).read()
		audiofile="lala.wav"
	f=os.popen('./OnsetExtractor %s'%audiofile,'r')
	onsets = [ str(float(onset)/2) for onset in f ]
	target = open(audiofile+options.suffix,'w')
	print >> target, dataTemplate%(len(onsets), " ".join(onsets))
	target.close()




