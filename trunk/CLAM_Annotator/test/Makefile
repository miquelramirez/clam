######################################################################
# Generic C++ Project Makefile v2.1
# Copyright (c) 1998-2001 Vokimon/KKEPerian UNLTD
# Copyright (c) 1998-2001 Helly/Patata Productions
######################################################################

# Change the following options as suited to your project

# The name of the executable file
EXEC=./TotsElsTests

# Flags for the linker
LDFLAGS= --relax
# Library flags for the linker
LDLIBS=-lm
# Flags for the C compiler
CFLAGS=-Wall -pedantic -Os -march=i686 -I . -g 

# Change the following options as suited to your system

# The C compiler
CC=gcc
# The C++ compiler
CPPC=g++
# Command used to delete files
RM=rm
# The build counter program (empty if none)
BUILDCOUNTER=./buildnum

# Those variables rarely need changes (pe. When you use .cpp extension for c++)
ALLCPPSOURCES:= \
	$(wildcard *.cxx TestsUnitaris/*.cxx TestsFuncionals/*.cxx)
ALLOBJSCPP:=$(ALLCPPSOURCES:.cxx=.o)

CPPSOURCES:= $(wildcard *.cxx)
OBJSCPP:=$(CPPSOURCES:.cxx=.o)




default: copirrait depend ${EXEC} testspassats
copirrait:
	@echo KKEPerians UNLTD Generic C++ Project Makefile v2.1
#	@echo --- Incrementant el nombre de builds
#	@ ${BUILDCOUNTER}

depend: copirrait .depend
	@echo --- Generant dependencies
	$(CPPC) $(CFLAGS) -MM $(CPPSOURCES) -D_KKEP_BUSCANT_DEPENDENCIES > .depend

.depend:
	touch .depend

clean: copirrait 
	@echo --- Eliminant arxius intermedis
	-$(RM) `find -name '*.o'`
	-$(RM) ${EXEC}

include .depend

${EXEC}: $(ALLOBJSCPP) 
	@echo --- Linkant $@
	${CPPC} $(LDFLAGS) $(ALLOBJSCPP) ${LDLIBS} -o ${EXEC}

$(OBJSCPP) : %.o: %.cxx
	@echo --- Compilant $<
	${CPPC} ${CFLAGS} -c $< -o $@

testspassats: $(EXEC)
	./$(EXEC) 


