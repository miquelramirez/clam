
# Top points to the root directory of your project
TOP = ../

$(shell make -f Makefile.images.rules > removeme)

program_and_images : interface program

interface : uic/About.h uic/About.cxx uic/AnnotatorBase.h uic/AnnotatorBase.cxx 



include $(TOP)/build/Makefile.images.rules

include $(TOP)/build/Makefile.rules

# The program name to compile and its settings must be in the 
# settings.cfg at this same directory

