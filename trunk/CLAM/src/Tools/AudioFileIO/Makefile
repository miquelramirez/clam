SOURCES=SoundFileIO.cxx SoundHeader.cxx WaveFileIO.cxx AIFFFileIO.cxx extended.c
TESTSOURCES=SoundFileIOTest.cxx
OBJECTS=$(addsuffix .o,$(basename $(SOURCES)))
TESTOBJECTS=$(addsuffix .o,$(basename $(TESTSOURCES)))
CXXFLAGS=-g -Wall
CFLAGS=-g -Wall

TEST=SoundFileIOTest

LIBRARY=libsoundfileio.a
LIBCOMMAND=ar cr
RANLIB=ranlib

.SUFFIXES: .cxx

all: $(LIBRARY) test

include .depend
	
$(LIBRARY): $(OBJECTS) SoundFileIO.hxx
	$(LIBCOMMAND) $@ $(OBJECTS)
	$(RANLIB) $@

test: $(TEST)

$(TEST): $(LIBRARY) $(TESTOBJECTS)
	g++ -o $@ $(TESTOBJECTS) -L. -lsoundfileio

clean:
	rm -f *.o $(TEST) $(LIBRARY)

.cxx.o:
	g++ $(CXXFLAGS) -c $< -o $@

.c.o:
	gcc $(CFLAGS) -c $< -o $@

install: all
	cp *.a /home/mdeboer/src/sms_public/external/lib/linux
	cp *.hxx /home/mdeboer/src/sms_public/external/include/SoundFileIO

install_mtgclasses: all
	mkdir -p ../MTG-Classes-new/externals/lib/linux/
	mkdir -p ../MTG-Classes-new/externals/SoundFileIO/
	cp *.a ../MTG-Classes-new/externals/lib/linux/
	cp *.hxx ../MTG-Classes-new/externals/SoundFileIO/

.depend:
	touch .depend
	make depend

depend: 
	rm -f .depend
	for a in $(SOURCES) $(TESTSOURCES); do \
		echo -n `dirname $$a`/ >> .depend; \
		$(CXX) -M $(CXXFLAGS) $(INCLDIRS) $$a >> .depend; \
	done	

.FORCE:

tags: .FORCE
	ctags -n -i cdefgmpstuv $(CPPFILES) *.*xx
