#!/usr/bin/python
# -*- coding: UTF-8 -*-

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
