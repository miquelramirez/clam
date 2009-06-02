#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os

options_filename = 'options.cache'
if not os.path.exists(options_filename):
	print "\nError. Options file is missing. Run \'scons configure test_data_path=CLAM-TEST-DATA\' first.\n"
	exit(1)
test_data_path = ''
for line in open(options_filename).readlines():
	(name,value) = line.split(' = ')
	if name=='test_data_path': test_data_path=value[1:-2]
if test_data_path=='':
	print "\nError. test_data_path is missing at options file. Run \'scons configure test_data_path=CLAM-TEST-DATA\' first.\n"
	exit(1)

#from sys import path
#path.append( test_data_path )
os.chdir( test_data_path )

import unittest
from NetworkTest import NetworkTest
from StandardTests import StandardTests
from GeneralTests import GeneralTests

def suite():
	suite = unittest.TestSuite()

	suite.addTest( unittest.makeSuite(NetworkTest) )
	suite.addTest( unittest.makeSuite(StandardTests) )
	suite.addTest( unittest.makeSuite(GeneralTests) )
	return suite

runner = unittest.TextTestRunner( verbosity=2 )
runner.run( suite() )
