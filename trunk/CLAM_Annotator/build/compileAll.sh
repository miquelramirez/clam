touch ../src/Annotator.*
make -f Makefile.images.rules
make depend
make interface
make CONFIG=debug
