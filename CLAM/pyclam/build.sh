rm *.o & rm clam.so

LANG=C ./generate_bindings.py && make
