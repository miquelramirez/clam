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

#include "SpectralPeakDescriptors.hxx" // CLAM
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


class SpectralPeakDescriptorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDescriptorsTest );

class SpectralPeakDescriptorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeakDescriptorsTest );
	CPPUNIT_TEST( testDataAttachment_whenLogDataFails );
	CPPUNIT_TEST( testConstructionDataAttachment_whenLogDataFails );
	CPPUNIT_TEST( testMagnitudeMean );
	CPPUNIT_TEST( testHarmonicCentroid );
	CPPUNIT_TEST( testHarmonicDeviation );
	CPPUNIT_TEST( testFirstTristimulus );
	CPPUNIT_TEST( testSecondTristimulus );
	CPPUNIT_TEST( testThirdTristimulus );
	CPPUNIT_TEST( testOddHarmonics );
	CPPUNIT_TEST( testEvenHarmonics );
	CPPUNIT_TEST( testOddToEvenRatio );


	CPPUNIT_TEST_SUITE_END();

private:
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		mPathToTestData = GetTestDataDirectory("descriptorsData/frames/SpectralPeaks/");

		mDescriptors = new CLAM::SpectralPeakDescriptors();
		mDescriptors->RemoveAll();
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mDescriptors;
	}

private:
	CLAM::SpectralPeakDescriptors *mDescriptors;
	std::string mPathToTestData;

	CLAM::Spectrum ComputeSpectrum(const CLAM::Audio& audioData)
	{
		const CLAM::TSize spectrumSize = audioData.GetSize()/2 + 1;
		// Configure and create the spectrum
		CLAM::SpecTypeFlags specFlags;
		specFlags.bMagPhase = 1;
			
		CLAM::Spectrum mySpectrum;
		mySpectrum.SetType(specFlags);
		mySpectrum.SetSize(spectrumSize);

		// Configure the transform
		CLAM::FFTConfig FFTCfg;
		FFTCfg.SetAudioSize(audioData.GetSize());

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
		CLAM::SpectralPeakArray  myPeakArray;
	
		CLAM::Spectrum dbSpectrum(spectrum);
		dbSpectrum.ToDB();

		myPeakArray.SetScale(CLAM::EScale::eLog);

		peakDetect.Do(dbSpectrum,myPeakArray);

		myPeakArray.ToLinear();

		return myPeakArray;
	}

	CLAM::Audio ReadAudio(const std::string & fileName)
	{
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(fileName);
		CLAM::MonoAudioFileReader reader;
		bool configOk = reader.Configure(cfg);
		CPPUNIT_ASSERT_MESSAGE(
			"configuration failed " + reader.GetConfigErrorMessage(),
			configOk);

		CLAM::Audio buf;
		buf.SetSize(1025);

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
			return ComputeSpectralPeaks(ComputeSpectrum(ReadAudio(fullPath)));
		CLAM::SpectralPeakArray peaks;
		CLAM::XMLStorage::Restore(peaks,fullPath); 
		return peaks;
	}

	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
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

private:
	void testDataAttachment_whenLogDataFails()
	{
		CLAM::SpectralPeakArray peaks;
		peaks = helperGetData("bell_A3.wav-Peaks.xml");
		peaks.SetScale(CLAM::EScale::eLog);
		try 
		{
			mDescriptors->SetpSpectralPeakArray(&peaks);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (const CLAM::ErrAssertionFailed & e)
		{
			std::string msg = e.what();
			CPPUNIT_ASSERT_EQUAL(std::string("Spectral Peak Descriptors require a linear magnitude SpectralPeakArray"),
					std::string(e.what()));
		}
	}

	void testConstructionDataAttachment_whenLogDataFails()
	{
		CLAM::SpectralPeakArray peaks;
		peaks = helperGetData("bell_A3.wav-Peaks.xml");
		peaks.SetScale(CLAM::EScale::eLog);
		try 
		{
			CLAM::SpectralPeakDescriptors peakDescriptors(&peaks);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (const CLAM::ErrAssertionFailed & e)
		{
			std::string msg = e.what();
			CPPUNIT_ASSERT_EQUAL(std::string("Spectral Peak Descriptors require a linear magnitude SpectralPeakArray"),
					std::string(e.what()));
		}
	}

	void testMagnitudeMean()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		
		data["Serie1To5-Peaks.xml"] = 3;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] = 4;
		data["5FlatSloped-Peaks.xml"] = 5;
		data["bell_A3.wav-Peaks.xml"] = 4.69634;
		data["5LinearSloped-Peaks.xml"] = 6;

		

		mDescriptors->AddMagnitudeMean();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetMagnitudeMean);
	}



	void testHarmonicCentroid()
	{
		CLAM::TData tolerance = 0.01;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		
		data["Serie1To5-Peaks.xml"] = 366.6667;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] = 1500;
		data["5FlatSloped-Peaks.xml"] = 940;
		data["bell_A3.wav-Peaks.xml"] = 2463.38;
		data["5LinearSloped-Peaks.xml"] = 580;
		

		mDescriptors->AddHarmonicCentroid();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetHarmonicCentroid);
	}

	
	void testHarmonicDeviation()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		
		data["Serie1To5-Peaks.xml"] =  0.1067;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] = 0;
		data["5FlatSloped-Peaks.xml"] = 0;
		data["bell_A3.wav-Peaks.xml"] = 2.02832;
		data["5LinearSloped-Peaks.xml"] = 0.061894;
		data["Serie5To1-Peaks.xml"] = 0.1067;
		
		mDescriptors->AddHarmonicDeviation ();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetHarmonicDeviation);
	}

	void testFirstTristimulus()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.0181818;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] = 1;
		data["5FlatSloped-Peaks.xml"] = 0.2;
		data["bell_A3.wav-Peaks.xml"] = 4.5164e-005;
		data["5LinearSloped-Peaks.xml"] = 0.4545;
		

		mDescriptors->AddFirstTristimulus();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetFirstTristimulus);
	}


	void testSecondTristimulus()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.527273;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] = 0;
		data["5FlatSloped-Peaks.xml"] = 0.6;
		data["bell_A3.wav-Peaks.xml"] = 0.00486879;
		data["5LinearSloped-Peaks.xml"] = 0.5273;
		
	

		mDescriptors->AddSecondTristimulus();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetSecondTristimulus);
	}

	void testThirdTristimulus()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.454545;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] =0;
		data["5FlatSloped-Peaks.xml"] = 0.2;
		data["bell_A3.wav-Peaks.xml"] = 0.995086;
		data["5LinearSloped-Peaks.xml"] = 0.0182;
	
	

		mDescriptors->AddThirdTristimulus();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetThirdTristimulus);
	}

	
	void testOddHarmonics()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.618182;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] =0;
		data["5FlatSloped-Peaks.xml"] = 0.4;
		data["bell_A3.wav-Peaks.xml"] =0.365391 ;
		data["5LinearSloped-Peaks.xml"] = 0.1818;
		
		mDescriptors->AddOddHarmonics();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetOddHarmonics);
	}

	
	void testEvenHarmonics()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.363636;
		data["No-Peaks.xml"] = 0;
		data["Single-Peaks.xml"] =0;
		data["5FlatSloped-Peaks.xml"] = 0.4;
		data["bell_A3.wav-Peaks.xml"] = 0.634564;
		data["5LinearSloped-Peaks.xml"] = 0.3636;
		
	
		mDescriptors->AddEvenHarmonics();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetEvenHarmonics);
	}

	
	void testOddToEvenRatio()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["Serie1To5-Peaks.xml"] = 0.6296;
		data["No-Peaks.xml"] = 0.5;
		data["Single-Peaks.xml"] =0.5;
		data["5FlatSloped-Peaks.xml"] = 0.5;
		data["bell_A3.wav-Peaks.xml"] = 0.365407;
		data["5LinearSloped-Peaks.xml"] = 0.3333;
		
		
		mDescriptors->AddOddToEvenRatio();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralPeakDescriptors::GetOddToEvenRatio);
	}
		
};


} // namespace CLAMTest

