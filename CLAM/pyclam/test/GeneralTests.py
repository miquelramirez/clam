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
		self.assertEqual( False, reader.Configure( clam.toProcessingConfig(config) ) )
		config.SetSourceFile( "test4seg.mp3" )
		self.assertEqual( True, reader.Configure( clam.toProcessingConfig(config) ) )
	#testAudioFileReaderConfig()

	def testAudioFileReaderLength(self):
		network = clam.Network()
		reader = network.AddProcessing( "MonoAudioFileReader" )
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "test4seg.mp3" )
		network.ConfigureProcessing(reader,clam.toProcessingConfig(config))
		self.assertEqual( 4, int( network.GetProcessing(reader).GetHeader().GetLength()/1000) )
		
		reader = clam.MonoAudioFileReader()
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "test4seg.mp3" )
		reader.Configure( clam.toProcessingConfig(config) )
		self.assertEqual( 4, int(reader.GetHeader().GetLength()/1000) )
	#testAudioFileReaderLength()

	def testSpectrumFlags(self):
		myspectrum = clam.Spectrum()
		specFlags = clam.SpecTypeFlags()

		specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), 1 )
		self.assertEqual( True, specFlags.IsSetFlag( specFlags.GetFlagPosition("Polar") ) )
		specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), 0 )
		self.assertEqual( False, specFlags.IsSetFlag( specFlags.GetFlagPosition("Polar") ) )
		specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), True )
		self.assertEqual( True, specFlags.IsSetFlag( specFlags.GetFlagPosition("Polar") ) )
		specFlags.SetFlag( specFlags.GetFlagPosition("Polar"), False )
		self.assertEqual( False, specFlags.IsSetFlag( specFlags.GetFlagPosition("Polar") ) )
	#testSpectrumFlags()		
		
#class GeneralTests

if __name__ == '__main__':
    unittest.main()
