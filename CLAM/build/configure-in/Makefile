all: ../configure 

run: ../configure
	(cd .. ; ./configure $(CONFIGURE_FLAGS) )

clean:
	rm -f ../configure ../configure.ac .depend.configure.ac config.log
	rm -rf ../autom4te.cache

../configure: ../configure.ac
	(cd .. ; autoconf -f )
	
../configure.ac: configure.pl
	perl configure.pl ../configure.ac

depend:
	rm .depend.configure.ac
	
include .depend.configure.ac

.depend.configure.ac:
	echo ../configure.ac: `ls acv/*.acv` > .depend.configure.ac
	echo >> .depend.configure.ac
	echo .depend.configure.ac: `ls acv/*.acv` >> .depend.configure.ac
	echo >> .depend.configure.ac

