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
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "SpectralPeakArray.hxx"
#include "SpectralPeakDetect.hxx"
#include "Spectrum.hxx"
#include "FFT.hxx"
#include "SpecTypeFlags.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "XMLStorage.hxx"



#include <iostream>

namespace CLAMTest
{


class SpectralPeakDetectTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDetectTest );

class SpectralPeakDetectTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeakDetectTest );
	CPPUNIT_TEST( test );
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
		CLAM::AudioFile audioFile;
		audioFile.OpenExisting(fileName);
		CPPUNIT_ASSERT_MESSAGE( 
			"Unable to load file "+ audioFile.GetLocation(),
			audioFile.IsReadable());
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(audioFile);
		CLAM::MonoAudioFileReader reader;
		CPPUNIT_ASSERT_MESSAGE(
			"configuration failed " + reader.GetConfigErrorMessage(),
			reader.Configure(cfg));

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

/*	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
		CLAM::TData tolerance, CLAM::TData & (CLAM::SpectralPeakDescriptors::*getter)() const )
	{
		std::stringstream log;
		bool success = true;
		mDescriptors->UpdateData();
		CLAM::SpectralPeakArray peaks;
		std::map<std::string, CLAM::TData>::const_iterator it;
		for (it = expected.begin(); it != expected.end(); it++) {
			peaks = helperGetData(it->first);
//			CLAM::XMLStorage::Dump(peaks, "Peaks", mPathToTestData + "/SpectralPeaks/" + it->first + "-Peaks.xml");
			mDescriptors->SetpSpectralPeakArray(&peaks);
			mDescriptors->Compute();
			if (
				(std::isnan((mDescriptors->*getter)()) != std::isnan(it->second)) ||
				(std::isinf((mDescriptors->*getter)()) != std::isinf(it->second)) ||
				(mDescriptors->*getter)() > (*it).second + tolerance ||
				(mDescriptors->*getter)() < (*it).second - tolerance
				)
			{
				log << (*it).first
				<< ": expected " << (*it).second
				<< ", received " << (mDescriptors->*getter)()
				<< std::endl;

				success = false;
			}
		}

		CPPUNIT_ASSERT_MESSAGE(std::string("Error margins surpassed:\n")+log.str()
			,success);
	}
*/
private:
	void test()
	{
		CLAM::SpectralPeakArray peaks;
		peaks = helperGetData("sin5000.wav");
//		CLAM::XMLStorage::Dump(peaks, "Peaks", mPathToTestData + "/SpectralPeaks/" + it->first + "-Peaks.xml");
		CLAM::XMLStorage::Dump(peaks, "Peaks",  "sin5000-Peaks.xml");
	}	
};


} // namespace CLAMTest
