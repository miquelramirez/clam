#!/usr/bin/python
import os

# will create :
# $HOME/manyclams/10216local/
# $HOME/manyclams/10216clam/
# etc.
for rev in range(10216,10220) :
	vars = {
			"rev": rev, 
			"prefix": "%s/manyclams/%slocal" % (os.environ["HOME"], rev),
			"sandbox": "%s/manyclams/%sclam" % (os.environ["HOME"], rev)
		}
	print vars
	print "\n\n __  Checkout %s\n\n" % rev
	print os.popen("""
		cd ~/manyclams
		svn co http://iua-share.upf.edu/svn/clam/trunk -r%(rev)s %(rev)sclam
		""" % vars ).read()

	print "\n\n __  CLAM compilation %s \n\n"% rev
	print os.popen("""
		mkdir -p %(prefix)s
		cd %(sandbox)s/CLAM
		scons configure prefix=/home/parumi/manyclams/%(rev)slocal
		scons -j3
		scons install
		""" % vars ).read()

	print "\n\n __  NetworkEditor compilation %s \n\n"% rev
	print os.popen("""
		cd %(sandbox)/NetworkEditor
		scons -j3 clam_prefix=%(prefix)s prefix=%(prefix)s
		""" % vars ).read()

