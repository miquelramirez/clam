#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
from pyplusplus import module_builder
from pyplusplus.module_builder import call_policies
from pygccxml import declarations
from pygccxml.declarations import custom_matcher_t as c_matcher
from pygccxml.declarations import regex_matcher_t as re_matcher

#import glob
#clam_clam_file_list = glob.glob('CLAM/*.hxx')

clam_file_list = []

# These requires pointer support enabled (see below declarations.is_pointer (mem_fun.return_type)):)
#clam_file_list += ["Processing.hxx", "ProcessingConfig.hxx"] # <- not put here, breaks the parser (FIXME)
clam_file_list += ["Flags.hxx", "SpecTypeFlags.hxx"]
clam_file_list += ["Component.hxx","Storage.hxx", "XMLStorage.hxx"]
clam_file_list += ["FFT.hxx","FFT_base.hxx","FFT_ooura.hxx"]
clam_file_list += ["OutControl.hxx","InControl.hxx"]
#"FFTConfig.hxx" -> no works by now (see wrap at Definitions.hxx)
#"FFT_fftw3.hxx", "FFT_rfftw.hxx"
#clam_file_list += [ "FFT_fftw3.hxx" ] # requires USE_FFTW3 definition (defined below but indeed no works FIXME)

#Warning: don't move below lines, breaks the compilation with DynamicType error.
#( KeyError: (('../include/DynamicType.hxx', 311), ('::', 'CLAM', 'DynamicType', 'AttributePositionBase<10u>')) )
clam_file_list += ["Processing.hxx", "ProcessingConfig.hxx","ProcessingData.hxx","ProcessingDataConfig.hxx","ProcessingDataPlugin.hxx"] 

#clam_file_list += ["SMSHarmonizer.hxx"]
#clam_file_list += ["DynamicType.hxx"] # here associated with "Problem2"
#clam_file_list += ["Array.hxx"] # here associated with "Problem2"
#clam_file_list += ["Audio.hxx"]#,,"AudioFileHeader.hxx"]
#clam_file_list += ["MonoAudioFileReaderConfig.hxx"]
#clam_file_list += ["Spectrum.hxx"] # here associated with "Problem1" and "Problem2"
# Problem1: "error: call of overloaded 'Xxxxx_wrapper(...)'"
#(Note1:could be fixed removing line 850 constructor?)
#(Note2:Py++ parse ok, fails with compilation)
#clam_bindings.cxx:2361:   instantiated from here
#/usr/include/boost/python/object/value_holder.hpp:134: error: call of overloaded 'Spectrum_wrapper(const CLAM::Spectrum&)' is ambiguous
#clam_bindings.cxx:864: note: candidates are: Spectrum_wrapper::Spectrum_wrapper(const CLAM::Spectrum&, bool, bool)
#clam_bindings.cxx:850: note:                 Spectrum_wrapper::Spectrum_wrapper(const CLAM::Spectrum&)
#clam_bindings.cxx:848: note:                 Spectrum_wrapper::Spectrum_wrapper(const Spectrum_wrapper&)
# Problem2: "error: invalid application of 'sizeof' to incomplete type"
#(Note1:line of clam_bindings.cxx is always end of file)
#(Note2:Py++ parse ok, fails with compilation)
#clam_bindings.cxx:3649:   instantiated from here
#/usr/include/boost/python/object/make_instance.hpp:24: error: invalid application of 'sizeof' to incomplete type 'boost::STATIC_ASSERTION_FAILURE<false>'


# general
clam_file_list += ["DataTypes.hxx","Enum.hxx","CLAM_Math.hxx","Err.hxx"]

# Adds CLAM dir
file_list = ["CLAM/"+item for item in clam_file_list]

# Special definitions
file_list += ["Definitions.hxx"]

clam_include_path = "/usr/local/include"
for line in open('options.cache').readlines():
	(name,value) = line.split(' = ')
	if name=='prefix': clam_include_path=value[1:-2]+"/include"

# Creating an instance of class that will help you to expose your declarations
mb = module_builder.module_builder_t (
					file_list
					, working_directory = r"."
					, include_paths = [ clam_include_path ]
		 			, define_symbols = [ "USE_SNDFILE=1", "USE_FFTW3" ]
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
