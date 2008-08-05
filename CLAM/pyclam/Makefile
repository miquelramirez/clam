CPPC=g++

# clean:
# 	rm *.o
# 	rm clam.so

# FIXME: make generic/use standard path (TODO: read from options.cache?)
clam_prefix = '/home/hordia/dev/local/clam_dev/'
clam_cflags = -I$(clam_prefix)/include -L$(clam_prefix)/lib -D"USE_SNDFILE=1" -lclam_core -lclam_audioio -lclam_processing

boost_cflags = -I/usr/include/python2.5  -I/usr/include/
boost_libs = -L/usr/lib/python2.5/config -lboost_python 


# default:

clam.so: clam_bindings.o Definitions.o indexing_slice.o python_iterator.o
	g++ -o clam.so -module -shared $(clam_cflags) $(boost_libs) clam_bindings.o Definitions.o indexing_slice.o

clam_bindings.o: clam_bindings.cxx
	g++ -c $(clam_cflags) $(boost_cflags) -fpic clam_bindings.cxx

Definitions.o: Definitions.hxx Definitions.cxx
	g++ -c $(clam_cflags) Definitions.cxx

indexing_slice.o: indexing/indexing_slice.cpp
	g++ -c $(clam_cflags) $(boost_cflags) indexing/indexing_slice.cpp

python_iterator.o: indexing/python_iterator.cpp
	g++ -c $(clam_cflags) $(boost_cflags) indexing/python_iterator.cpp
