program_name=TestAudioFile

TOP=../../../..

UNIX_TOOLS=.

CLAM_INCDIR=../../includes
CLAM_LIBDIR=../../libs

VPATH=.

sourcefiles=CLAM-audiofile_main.cxx
objectfiles=$(sourcefiles:.cxx=.o)

targetdir_release = ./release_bin
targetdir_debug   = ./debug_bin
objdir_release    = ./release_obj
objdir_debug      = ./debug_obj

include $(UNIX_TOOLS)/makefile.default

defines_release=-DHAVE_STANDARD_SSTREAM 
defines_debug=-DHAVE_STANDARD_SSTREAM 
includedirs=-I$(CLAM_INCDIR)
libdirs=-L$(CLAM_LIBDIR)
libs_release=-lCLAM_core -lCLAM_audiofile -lxerces-c
libs_debug= -lCLAM_core -lCLAM_audiofile -lxerces-c

include $(UNIX_TOOLS)/makefile.env
