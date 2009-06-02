#!/usr/bin/python
# -*- coding: UTF-8 -*-

import unittest
import clam
from math import sin

class GeneralTests(unittest.TestCase):

	def setUp(self):
		self.samples = 1024
		self.sampleRate = 8000.0
		self.resolution = 0.0000001 # expected max resolution
	#setUp()

	def tearDown(self):
		pass

	def testAudioBuffer(self):
		myaudio = clam.Audio()
		myaudio.SetSize( self.samples )
		myaudio.SetSampleRate( self.sampleRate )
		vector = myaudio.GetBuffer()
		
		
		sineFreq = 880
		for i in range(self.samples):
			vector[i] = 0.625+0.5*sin(2.0*sineFreq*400.0*((float(i))/self.sampleRate))
			self.assertEqual( abs( 0.625+0.5*sin(2.0*sineFreq*400.0*((float(i))/self.sampleRate))-vector[i] ) < self.resolution, True )
		
		vector.Resize( self.samples+1 )
		vector.SetSize( self.samples+1 )
		vector[self.samples] = 20
		self.assertEqual( 20, vector[self.samples] )
	#testAudioBuffer()
		
	def testAudioFileReaderConfig(self):
		reader = clam.MonoAudioFileReader()
		config = clam.MonoAudioFileReaderConfig()
		self.assertEqual( False, reader.Configure( clam.toProcessingConfig(config) ) )
		config.SetSourceFile( "trumpet.mp3" )
		self.assertEqual( True, reader.Configure( clam.toProcessingConfig(config) ) )
	#testAudioFileReaderConfig()

	def testAudioFileReaderLength(self):
		network = clam.Network()
		reader = network.AddProcessing( "MonoAudioFileReader" )
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "trumpet.mp3" )
		network.ConfigureProcessing(reader,clam.toProcessingConfig(config))
		self.assertEqual( 2, int( network.GetProcessing(reader).GetHeader().GetLength()/1000) )
		
		reader = clam.MonoAudioFileReader()
		config = clam.MonoAudioFileReaderConfig()
		config.SetSourceFile( "trumpet.mp3" )
		reader.Configure( clam.toProcessingConfig(config) )
		self.assertEqual( 2, int(reader.GetHeader().GetLength()/1000) )
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
	
	def testAudioFileHeader(self):
		file = clam.AudioFileSource()
		file.OpenExisting( "trumpet.mp3" )
		self.assertEqual( "Mpeg Audio Layer 3", str(file.GetHeader().GetFormat()) )
		self.assertEqual( 1, file.GetHeader().GetChannels() )
		self.assertEqual( 22050.0, file.GetHeader().GetSampleRate() )
		
		durationMs = file.GetHeader().GetLength()
		numberSeconds = durationMs / 1000.
		numberMinutes = (int)(numberSeconds / 60.)
		numberSeconds = numberSeconds - numberMinutes * 60.
		numberHours = int(numberMinutes / 60.)
		numberMinutes = numberMinutes - numberHours * 60.
		self.assertEqual( 0, numberHours )
		self.assertEqual( 0.0, numberMinutes )
		self.assertEqual( 2.43, numberSeconds )
	#testAudioFileHeader()

	def testAudioFileTextDescriptors(self):
		file = clam.AudioFileSource()
		file.OpenExisting( "trumpet.mp3" )
		txtDesc = file.GetTextDescriptors()
		self.assertEqual( "The Trumpeteers", clam.toString(txtDesc.GetArtist()) )
		self.assertEqual( "A Trumpet", clam.toString(txtDesc.GetTitle()) )
		self.assertEqual( "Trumpets, Trumpets, Trumpets", clam.toString(txtDesc.GetAlbum()) )
		self.assertEqual( "1", clam.toString(txtDesc.GetTrackNumber()) )
	#testAudioFileDescriptors()

	
#class GeneralTests

if __name__ == '__main__':
    unittest.main()
