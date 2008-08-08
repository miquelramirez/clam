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

options_filename = 'options.cache'
if not os.path.exists(options_filename):
	print "\nError. Options file is missing. Run \'scons configure prefix=CLAM_LIBRARY_PATH\' first.\n"
	exit(1)
clam_path = "/usr/local/include"; plugins_path = ''
for line in open(options_filename).readlines():
	(name,value) = line.split(' = ')
	if name=='prefix': clam_path=value[1:-2]+"/include"
	if name=='plugins_prefix':
		if value=='True': plugins_path=clam_path
		else: plugins_path=value[1:-2]
enablePlugins = True if plugins_path!='' else False

#import glob
#clam_clam_file_list = glob.glob('CLAM/*.hxx')

clam_file_list = []

#clam_file_list += ["Processing.hxx", "ProcessingConfig.hxx"] # <- not put here, breaks the parser (FIXME)
clam_file_list += ["DynamicTypeMacros.hxx"] 
clam_file_list += ["Flags.hxx", "SpecTypeFlags.hxx"]
clam_file_list += ["Component.hxx","Storage.hxx", "XMLStorage.hxx"]
clam_file_list += ["OutControl.hxx","InControl.hxx"]

clam_file_list += ["FFT.hxx","FFT_base.hxx","FFT_ooura.hxx"] #"FFT_fftw3.hxx" Needs CLAM::FFTConfig.hxx
#clam_file_list += ["FFTConfig.hxx"] #Problem2 no works by now (see wrap at Definitions.hxx)
#clam_file_list += ["FFT_fftw3.hxx", "FFT_rfftw.hxx"]

# Warning: don't move below lines, breaks the compilation with DynamicType error (Problem3).
clam_file_list += ["Processing.hxx", "ProcessingConfig.hxx","ProcessingData.hxx","ProcessingDataConfig.hxx","ProcessingDataPlugin.hxx"] 

#clam_file_list += ["NullProcessingConfig.hxx"]
clam_file_list += ["FlowControl.hxx"]
clam_file_list += ["FlattenedNetwork.hxx","Network.hxx"]

clam_file_list += ["NetworkPlayer.hxx"] #, "PANetworkPlayer.hxx"
#clam_file_list += ["JACKNetworkPlayer.hxx"]

#clam_file_list += ["MonoAudioFileReader.hxx","MonoAudioFileReaderConfig.hxx"] # Problem2 with MonoAudioFileReaderConfig


""" Usual problems:
== Problem1 ==
"error: call of overloaded 'Xxxxx_wrapper(...)'"

(Note1:could be fixed removing line 850 constructor?)
(Note2:Py++ parse ok, fails with compilation)
clam_bindings.cxx:2361:   instantiated from here
/usr/include/boost/python/object/value_holder.hpp:134: error: call of overloaded 'Spectrum_wrapper(const CLAM::Spectrum&)' is ambiguous
clam_bindings.cxx:864: note: candidates are: Spectrum_wrapper::Spectrum_wrapper(const CLAM::Spectrum&, bool, bool)
clam_bindings.cxx:850: note:                 Spectrum_wrapper::Spectrum_wrapper(const CLAM::Spectrum&)
clam_bindings.cxx:848: note:                 Spectrum_wrapper::Spectrum_wrapper(const Spectrum_wrapper&)

== Problem2 ==
"error: invalid application of 'sizeof' to incomplete type"

Note1:line of clam_bindings.cxx is always end of file
Note2:Py++ parse ok, fails with compilation
Note3:comes fromo DynamicTypeMacros.hxx include
clam_bindings.cxx:3649:   instantiated from here
/usr/include/boost/python/object/make_instance.hpp:24: error: invalid application of 'sizeof' to incomplete type 'boost::STATIC_ASSERTION_FAILURE<false>'
	
== Problem3 ==
'DynamicType'

( KeyError: (('../include/DynamicType.hxx', 311), ('::', 'CLAM', 'DynamicType', 'AttributePositionBase<10u>')) )
"""

#clam_file_list += ["SMSHarmonizer.hxx"]
#clam_file_list += ["DynamicType.hxx"] # here associated with "Problem2"
#clam_file_list += ["Array.hxx"] # here associated with "Problem2"
#clam_file_list += ["Audio.hxx"]#,,"AudioFileHeader.hxx"]
#clam_file_list += ["MonoAudioFileReaderConfig.hxx"]
#clam_file_list += ["Spectrum.hxx"] # here associated with "Problem1" and "Problem2"

# General
clam_file_list += ["DataTypes.hxx","Enum.hxx","CLAM_Math.hxx","Err.hxx"]

# Adds CLAM dir
file_list = ["CLAM/"+item for item in clam_file_list]

# Experimental:
if enablePlugins:
	# FIXME: Most of them parse and compile, but they need NullProcessingConfig (not parsed ATM, Problem1) exposed to python since this construction:
	# PROCESSING_NAME_CONSTRUCTOR(const Config & config=Config()) { Configure(config); ... }
	# PROCESSING_NAME_CONSTRUCTOR() { ... }

	clam_plugins_file_list = []
	#clam_plugins_file_list += ["GuitarEffects/DCRemoval/DCRemoval.hxx"]
	#clam_plugins_file_list += ["GuitarEffects/AutomaticGainControl/AutomaticGainControl.hxx"]
	#clam_plugins_file_list += ["GuitarEffects/AudioSwitch/AudioSwitch.hxx"]
	#clam_plugins_file_list += ["sndfile/SndfilePlayer.hxx"]
	#clam_plugins_file_list += ["MIDI/MIDISource/MIDISource.hxx"]
	#clam_plugins_file_list += ["osc/LibloSource.hxx","osc/LibloSink.hxx"]
	file_list += [ plugins_path+"/"+item for item in clam_plugins_file_list]


# Special definitions
file_list += ["Definitions.hxx"]

clam_include_path = []
clam_include_path.append( clam_path )
if enablePlugins: clam_include_path.append( plugins_path )

# Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t (
					file_list
					, working_directory = r"."
					, include_paths = clam_include_path
		 			, define_symbols = [ 'CLAM_FLOAT','_DEBUG','USE_PTHREADS=1','USE_XERCES=1','CLAM_USE_XML','USE_LADSPA=1','USE_FFTW3=1','USE_SNDFILE=1','USE_OGGVORBIS=1','WITH_VORBIS=1','USE_MAD=1','WITH_MAD=1','USE_ID3=1','USE_ALSA=1','USE_JACK=1','USE_PORTAUDIO=1' ]
					#, cflags=''
					, indexing_suite_version = 2
)

mem_funs = mb.calldefs()

mem_funs.create_with_signature = True
for mem_fun in mem_funs:
	if mem_fun.call_policies:
		continue
	
# only references
	#if (not mem_fun.call_policies) and (declarations.is_reference(mem_fun.return_type)):
# or declarations.is_pointer (mem_fun.return_type)):

# references and pointers
	if (not mem_fun.call_policies) and (declarations.is_reference(mem_fun.return_type)or (declarations.is_pointer (mem_fun.return_type)) ):
		
		mem_fun.call_policies = call_policies.return_value_policy(call_policies.reference_existing_object )

# Well, don't you want to see what is going on?
#mb.print_declarations()

# Creating code creator. After this step you should not modify/customize declarations.
mb.build_code_creator( module_name='clam' )

# Writing code to file.
mb.write_module( 'clam_bindings.cxx' )
