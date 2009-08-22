#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

import os
from pyplusplus import module_builder
from pyplusplus.module_builder import call_policies
from pygccxml import declarations
from pygccxml.declarations import custom_matcher_t as c_matcher
from pygccxml.declarations import regex_matcher_t as re_matcher
from pygccxml.declarations.matchers import access_type_matcher_t

from doxygen import doxygen_doc_extractor

options_filename = 'options.cache'
if not os.path.exists(options_filename):
	print "\nError. Options file is missing. Run \'scons configure clam_prefix=CLAM_LIBRARY_PATH\' first.\n"
	exit(1)
clam_path = "/usr/local/include"; plugins_path = ''
for line in open(options_filename).readlines():
	try:
		(name,value) = line.split(' = ')
		if name=='clam_prefix':
			clam_prefix = value[1:-2]
			clam_path=value[1:-2]+"/include"
		if name=='plugins_prefix':
			if value=='True': plugins_path=clam_path
			else: plugins_path=value[1:-2]
	except:
		print "Bad config line"
enablePlugins = True if plugins_path!='' else False


clam_file_list = [] # Files going to be automatically parsed
exported_manually_file_list = [] # Files already (manually) exposed / not going to be parsed


clam_file_list += [
	'DynamicTypeMacros.hxx',
	'Flags.hxx',
	'SpecTypeFlags.hxx',
	'Component.hxx',
	'Storage.hxx',
	'XMLStorage.hxx',
	'OutControl.hxx',
	'InControl.hxx',
	'FFT.hxx',
	'FFT_base.hxx',
	'FFT_ooura.hxx',
	'FFT_fftw3.hxx',
]

exported_manually_file_list += ['FFTConfig.hxx']

clam_file_list += [
	'Processing.hxx',
	'ProcessingConfig.hxx',
	'ProcessingData.hxx',
	'ProcessingDataConfig.hxx',
	'ProcessingDataPlugin.hxx',
	'FlowControl.hxx',
	'FlattenedNetwork.hxx',
	'Network.hxx',
	'NetworkPlayer.hxx',
	'DataTypes.hxx',
	'Enum.hxx',
	'CLAM_Math.hxx',
	'Err.hxx',
	'MonoAudioFileReader.hxx',
	'AudioInFilename.hxx',
	'AudioFile.hxx',
	'Text.hxx',
	'Filename.hxx',
]

exported_manually_file_list += [ 'AudioTextDescriptors.hxx' ]
exported_manually_file_list += [ 'EAudioFileCodec.hxx','EAudioFileEncoding.hxx','EAudioFileEndianess.hxx','EAudioFileFormat.hxx' ] # Original CLAM file for all these classes is 'AudioFileFormats.hxx'
exported_manually_file_list += [ 'DataArray.hxx' ] # Original CLAM file declaring this class/type is 'Array.hxx'
exported_manually_file_list += [ 'AudioFileHeader.hxx','Audio.hxx','MonoAudioFileReaderConfig.hxx','SpectrumConfig.hxx','Spectrum.hxx' ]

# Removes class names that could be added by accident to the list to be parsed (they're already exposed manually)
for classname in exported_manually_file_list:
	try:
		clam_file_list.remove( classname )
	except:
		pass

file_list = ['CLAM/'+item for item in clam_file_list]

# Experimental:
if enablePlugins:
	# FIXME: Most of them parse and compile, but they need NullProcessingConfig (not parsed ATM, Problem1) exposed to python since this construction:
	# PROCESSING_NAME_CONSTRUCTOR(const Config & config=Config()) { Configure(config); ... }
	# PROCESSING_NAME_CONSTRUCTOR() { ... }
	clam_plugins_file_list = []
	#clam_plugins_file_list += ['GuitarEffects/DCRemoval/DCRemoval.hxx']
	#clam_plugins_file_list += ['GuitarEffects/AutomaticGainControl/AutomaticGainControl.hxx']
	#clam_plugins_file_list += ['GuitarEffects/AudioSwitch/AudioSwitch.hxx']
	#clam_plugins_file_list += ['sndfile/SndfilePlayer.hxx']
	#clam_plugins_file_list += ['MIDI/MIDISource/MIDISource.hxx']
	#clam_plugins_file_list += ['osc/LibloSource.hxx','osc/LibloSink.hxx']
	file_list += [ plugins_path+'/'+item for item in clam_plugins_file_list]

# Special definitions and extra-wrappers
file_list += ['src/Definitions.hxx']

clam_include_path = [ '.' ]
clam_include_path.append( clam_path )
if enablePlugins: clam_include_path.append( plugins_path )

clam_modules = [ 'clam_core', 'clam_audioio', 'clam_processing' ]
cflags_list = os.popen( 'PKG_CONFIG_PATH=%s/lib/pkgconfig pkg-config --cflags-only-other %s'%(clam_prefix," ".join(clam_modules))  ).read()[:-1].split(' ')
define_list = [ item[2:] for item in cflags_list if item[:2]=='-D' ]

flags_list = ""

# Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t (
	files = file_list,
	working_directory = r".",
	include_paths = clam_include_path,
	define_symbols = define_list,
	indexing_suite_version = 2,
	cflags = flags_list,
)

mem_funs = mb.calldefs()

mem_funs.create_with_signature = True
for mem_fun in mem_funs:
	if mem_fun.call_policies:
		continue
	
	# References and pointers
	if (not mem_fun.call_policies) and (declarations.is_reference(mem_fun.return_type)or (declarations.is_pointer (mem_fun.return_type)) ):
		mem_fun.call_policies = call_policies.return_value_policy(call_policies.reference_existing_object )

# Well, don't you want to see what is going on?
#mb.print_declarations()

mb.classes().always_expose_using_scope = True

# Manuals exclusions
try:
	#mb.class_("FFTConfig").member_function("StoreAudioSize").exclude()
	#mb.class_('FFTConfig').exclude()
	pass
except:
	print "Error excluding members functions or classes."

# Register of manually exposed classes
for classname in exported_manually_file_list:
	mb.add_declaration_code( "#include  \"src/manual/%s.pypp.hpp\""%(classname[:-4]) )
	mb.add_registration_code( "register_%s_class();"%(classname[:-4]), tail=False )

# Extra registrations
mb.add_registration_code( """bp::implicitly_convertible< int, CLAM::EAudioFileCodec >();
bp::implicitly_convertible< std::string, CLAM::EAudioFileCodec >();
bp::implicitly_convertible< int, CLAM::EAudioFileCodec >();
bp::implicitly_convertible< std::string, CLAM::EAudioFileCodec >();
bp::implicitly_convertible< int, CLAM::EAudioFileEncoding >();
bp::implicitly_convertible< std::string, CLAM::EAudioFileEncoding >();
bp::implicitly_convertible< int, CLAM::EAudioFileEndianess >();
bp::implicitly_convertible< std::string, CLAM::EAudioFileEndianess >();
bp::implicitly_convertible< int, CLAM::EAudioFileFormat >();
bp::implicitly_convertible< std::string, CLAM::EAudioFileFormat >();""" )

# Classes that need to be registered before others
mb.add_registration_code("register_Enum_class();", tail=False)
mb.add_registration_code("register_Component_class();", tail=False)

#TODO: avoid duplicated classes registering

# Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='clam', doc_extractor=doxygen_doc_extractor() )


#// This file has been generated by Py++ from CLAM Library include files 
mb.code_creator.license = """/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
 """

# Writing code to files
mb.split_module( './src/automatic' )

