/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "SpectralPeakArray.hxx" // CLAM
#include "SpectralPeakDetect.hxx" // CLAM
#include "Spectrum.hxx" // CLAM
#include "FFT.hxx" // CLAM
#include "SpecTypeFlags.hxx" // CLAM
#include "AudioFile.hxx" // CLAM
#include "MonoAudioFileReader.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM



#include <iostream>

namespace CLAMTest
{


class SpectralPeakDetectTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDetectTest );

class SpectralPeakDetectTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeakDetectTest );
	CPPUNIT_TEST( test5kSin );
	CPPUNIT_TEST_SUITE_END();

private:
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		mPathToTestData = GetTestDataDirectory("");

	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
	}

private:
	std::string mPathToTestData;

	CLAM::Spectrum ComputeSpectrum(const CLAM::Audio& audioData, int audioBlockSize=1024)
	{
		const CLAM::TSize spectrumSize = audioBlockSize/2 + 1;
		// Configure and create the spectrum
		CLAM::SpecTypeFlags specFlags;
		specFlags.bMagPhase = 1;
			
		CLAM::Spectrum mySpectrum;
		mySpectrum.SetType(specFlags);
		mySpectrum.SetSize(spectrumSize);

		// Configure the transform
		CLAM::FFTConfig FFTCfg;
		FFTCfg.SetAudioSize(audioBlockSize);

		// Compute the transform
		CLAM::FFT myFFT;
		myFFT.Configure(FFTCfg);
		myFFT.Start();
		myFFT.Do(audioData, mySpectrum);
		myFFT.Stop();

		// Return the result
		return mySpectrum;
	}

	CLAM::SpectralPeakArray ComputeSpectralPeaks(const CLAM::Spectrum& spectrum)
	{
		CLAM::SpectralPeakDetect peakDetect;
		CLAM::SpectralPeakDetectConfig peakDetectCfg;
		CLAM::SpectralPeakArray  myPeakArray;
	
		CLAM::Spectrum dbSpectrum(spectrum);
		dbSpectrum.ToDB();
		myPeakArray.SetScale(CLAM::EScale::eLog);
		peakDetect.Do(dbSpectrum,myPeakArray);
		myPeakArray.ToLinear();

		return myPeakArray;
	}

	CLAM::Audio ReadAudio(const std::string & fileName, int audioBlockSize=1024)
	{
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(fileName);
		CLAM::MonoAudioFileReader reader;
		bool configOk = reader.Configure(cfg);
		CPPUNIT_ASSERT_MESSAGE(
			"configuration failed " + reader.GetConfigErrorMessage(),
			configOk);

		CLAM::Audio buf;
		buf.SetSize(audioBlockSize);


		reader.Start();
		reader.Do( buf );
		reader.Stop();

		return buf;

	}

	CLAM::SpectralPeakArray helperGetData(const std::string & fileName)
	{
		std::string extension = fileName.substr(fileName.size()-4,fileName.size());
		std::string fullPath = mPathToTestData+fileName;
		if (extension!=".xml")
			return ComputeSpectralPeaks(ComputeSpectrum(ReadAudio(fullPath, 512), 512));
		CLAM::SpectralPeakArray peaks;
		CLAM::XMLStorage::Restore(peaks,fullPath); 
		return peaks;
	}

private:
	void test5kSin()
	{
		CLAM::SpectralPeakArray peaks;
		peaks = helperGetData("sin5000.wav");
		CLAM::XMLStorage::Dump(peaks, "Peaks",  "sin5000-Peaks.xml");
		CPPUNIT_ASSERT( peaks.GetFreqBuffer()[0] > 4995);
		CPPUNIT_ASSERT( peaks.GetFreqBuffer()[0] < 5001);
	}	
};


} // namespace CLAMTest
