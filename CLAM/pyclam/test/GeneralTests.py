#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
import clam

class GeneralTests(unittest.TestCase):

	def setUp(self):
		pass
	
	def tearDown(self):
		pass

	def testAudioFileReaderConfig(self):
		reader = clam.MonoAudioFileReader()
		config = clam.MonoAudioFileReaderConfig()
		self.assertEqual( False, reader.real().Configure( config.real() ) )
		config.SetSourceFile( "test4seg.mp3" )
		self.assertEqual( True, reader.real().Configure( config.real() ) )
	#testAudioFileReaderConfig()

	def testAudioFileReaderLength(self):
		network = clam.Network()
		reader = network.AddProcessing( "MonoAudioFileReader" )
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "test4seg.mp3" )
		network.ConfigureProcessing(reader,config.real())
		self.assertEqual( 4, clam.MonoAudioFileReader( network.GetProcessing(reader) ).GetLength() )
		
		reader = clam.MonoAudioFileReader()
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "test4seg.mp3" )
		reader.real().Configure( config.real() )
		self.assertEqual( 4, reader.GetLength() )
	#testAudioFileReaderLength()

#class GeneralTests

if __name__ == '__main__':
    unittest.main()
