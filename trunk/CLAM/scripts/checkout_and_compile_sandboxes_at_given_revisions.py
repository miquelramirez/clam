#!/usr/bin/python
import os

# will create :
# $HOME/manyclams/10216local/
# $HOME/manyclams/10216clam/
# etc.
print "WARNING: each sandbox can weight around 1GB!!"
for rev in range(10216,10220) :
	vars = {
			"rev": rev, 
			"prefix": "%s/manyclams/%slocal" % (os.environ["HOME"], rev),
			"sandbox": "%s/manyclams/%sclam" % (os.environ["HOME"], rev)
		}

	print "\n\n __  Checkout %s\n\n" % rev
	print os.popen("""
		mkdir -p %(prefix)s
		cd ~/manyclams
		svn export http://clam-project.org/clam/trunk -r%(rev)s %(sandbox)s
		""" % vars ).read()

	print "\n\n __  CLAM compilation %s \n\n"% rev
	print os.popen("""
		cd %(sandbox)s/CLAM
		scons configure prefix=/home/parumi/manyclams/%(rev)slocal
		scons -j3
		scons install
		""" % vars ).read()

	print "\n\n __  NetworkEditor compilation %s \n\n"% rev
	print os.popen("""
		cd %(sandbox)s/NetworkEditor
		scons -j3 clam_prefix=%(prefix)s prefix=%(prefix)s
		""" % vars ).read()

