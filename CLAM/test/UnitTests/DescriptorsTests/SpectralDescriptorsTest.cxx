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

#include "SpectralDescriptors.hxx"
#include "Spectrum.hxx"
#include "FFT.hxx"
#include "SpecTypeFlags.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"



/*
#include "similarityHelper.hxx"
#include <algorithm>
*/
#include <iostream>

namespace CLAMTest
{


class SpectralDescriptorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectralDescriptorsTest );

class SpectralDescriptorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralDescriptorsTest );
	CPPUNIT_TEST( testRolloff );
	CPPUNIT_TEST( testFlatness );
	CPPUNIT_TEST( testCentroid );
	CPPUNIT_TEST( testSpread );
	CPPUNIT_TEST( testSlope );

	CPPUNIT_TEST_SUITE_END();

private:
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");

		if ( !pathToTestData )
			mPathToTestData = "../../../../CLAM-TestData/";
		else
			mPathToTestData = pathToTestData;

		mPathToTestData += "descriptorsData/frames";
		mPathToTestData = "../../../../CLAM-TestData/descriptorsData/frames/";

		mDescriptors = new CLAM::SpectralDescriptors();
		mDescriptors->RemoveAll();
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mDescriptors;
	}

private:
	CLAM::SpectralDescriptors *mDescriptors;
	std::string mPathToTestData;

	CLAM::Spectrum ComputeSpectrum(const CLAM::Audio& audioData, CLAM::TSize spectrumSize)
	{
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

	CLAM::Spectrum helperGetData(const std::string & fileName)
	{
		CLAM::AudioFile audioFile;
		audioFile.SetLocation(mPathToTestData+fileName);
		CPPUNIT_ASSERT_MESSAGE( 
			"Unable to load file "+ audioFile.GetLocation(),
			audioFile.IsReadable());
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile(audioFile);
		CLAM::MonoAudioFileReader reader;
		CPPUNIT_ASSERT_MESSAGE(
			"configuration failed " + reader.GetStatus(),
			reader.Configure(cfg));

		CLAM::Audio buf;
		buf.SetSize(512);

		reader.GetOutPorts().GetByNumber(0).Attach( buf );

		reader.Start();

		reader.Do();

		reader.Stop();

		return ComputeSpectrum(buf, buf.GetSize()/2 + 1);
	}

	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
		CLAM::TData tolerance, CLAM::TData & (CLAM::SpectralDescriptors::*getter)() const )
	{
		std::stringstream log;
		bool success = true;
		mDescriptors->UpdateData();
		CLAM::Spectrum spectrum;
		std::map<std::string, CLAM::TData>::const_iterator it;
		for (it = expected.begin(); it != expected.end(); it++) {
			spectrum = helperGetData((*it).first);
			mDescriptors->SetpSpectrum(&spectrum);
			mDescriptors->Compute();
			if ((mDescriptors->*getter)() > (*it).second + tolerance
			    || (mDescriptors->*getter)() < (*it).second - tolerance)
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

	void testRolloff()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 473.268293;
		data["Balance000.600.wav"] = 150.585366;
		data["Balance000.992.wav"] = 236.634146;
		data["Balance001.988.wav"] = 925.024390;
		data["Balance010.910.wav"] = 150.585366;
		data["Cello_A2.wav"] = 1656.439024;
		data["Cello_C2.wav"] = -1.000000;
		data["Disco_Rojo001.008.wav"] = 4904.780488;
		data["Disco_Rojo002.327.wav"] = 537.804878;
		data["Geiger_Counter005.020.wav"] = 258.146341;
		data["SaxBritHorns12.wav"] = 2732.048780;
		data["Time002.624.wav"] = 9874.097561;
		data["bell_A3.wav"] = 2646.000000;
		data["gamelan-gong.wav"] = 139.829268;
		data["gt_E4.wav"] = 86.048780;
		data["pno_Eb1.wav"] = -1.000000;
		data["silence.wav"] = -1.000000;
		data["vln_A3.wav"] = -1.000000;
		data["vln_D5.wav"] = -1.000000;
		data["whitenoise.wav"] = 18672.585366;

		mDescriptors->AddRolloff();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetRolloff);
	}

	void testFlatness()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -5.570786;
		data["Balance000.600.wav"] = -14.812896;
		data["Balance000.992.wav"] = -11.349304;
		data["Balance001.988.wav"] = -10.240200;
		data["Balance010.910.wav"] = -4.842938;
		data["Cello_A2.wav"] = -5.525457;
		data["Cello_C2.wav"] = 0.000000;
		data["Disco_Rojo001.008.wav"] = -5.169985;
		data["Disco_Rojo002.327.wav"] = -6.795831;
		data["Geiger_Counter005.020.wav"] = -7.020690;
		data["SaxBritHorns12.wav"] = -4.667628;
		data["Time002.624.wav"] = -3.651583;
		data["bell_A3.wav"] = -5.276683;
		data["gamelan-gong.wav"] = -5.224524;
		data["gt_E4.wav"] = -3.491369;
		data["pno_Eb1.wav"] = 0.000000;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.000000;
		data["vln_D5.wav"] = 0.000000;
		data["whitenoise.wav"] = -0.795157;

		mDescriptors->AddFlatness();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetFlatness);
	}

	void testCentroid()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 2546.840064;
		data["Balance000.600.wav"] = 416.676904;
		data["Balance000.992.wav"] = 795.851545;
		data["Balance001.988.wav"] = 1588.760453;
		data["Balance010.910.wav"] = 2696.114917;
		data["Cello_A2.wav"] = 3017.010992;
		data["Cello_C2.wav"] = 0.000000;
		data["Disco_Rojo001.008.wav"] = 4169.324045;
		data["Disco_Rojo002.327.wav"] = 2579.081038;
		data["Geiger_Counter005.020.wav"] = 1764.977615;
		data["SaxBritHorns12.wav"] = 3750.426935;
		data["Time002.624.wav"] = 7135.560467;
		data["bell_A3.wav"] = 3686.122991;
		data["gamelan-gong.wav"] = 1263.962139;
		data["gt_E4.wav"] = 4403.107081;
		data["pno_Eb1.wav"] = 0.000000;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.000000;
		data["vln_D5.wav"] = 0.000000;
		data["whitenoise.wav"] = 11072.758057;

		mDescriptors->AddCentroid();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetCentroid);
	}

	void testSpread()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 1.791044;
		data["Balance000.600.wav"] = 4.119432;
		data["Balance000.992.wav"] = 3.158641;
		data["Balance001.988.wav"] = 1.451249;
		data["Balance010.910.wav"] = 1.822904;
		data["Cello_A2.wav"] = 1.322429;
		data["Cello_C2.wav"] = 0.000000;
		data["Disco_Rojo001.008.wav"] = 0.916335;
		data["Disco_Rojo002.327.wav"] = 1.335445;
		data["Geiger_Counter005.020.wav"] = 2.159354;
		data["SaxBritHorns12.wav"] = 1.075443;
		data["Time002.624.wav"] = 0.698290;
		data["bell_A3.wav"] = 1.092978;
		data["gamelan-gong.wav"] = 1.871866;
		data["gt_E4.wav"] = 1.324165;
		data["pno_Eb1.wav"] = 0.000000;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.000000;
		data["vln_D5.wav"] = 0.000000;
		data["whitenoise.wav"] = 0.568572;

		mDescriptors->AddSpread();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetSpread);
	}

	void testSlope()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -4.07655e-07;
		data["Balance000.600.wav"] = -5.10341e-07;
		data["Balance000.992.wav"] = -4.92062e-07;
		data["Balance001.988.wav"] = -4.5384e-07;
		data["Balance010.910.wav"] = -4.00459e-07;
		data["Cello_A2.wav"] = -3.8499e-07;
		data["Cello_C2.wav"] = 0;
		data["Disco_Rojo001.008.wav"] = -3.29443e-07;
		data["Disco_Rojo002.327.wav"] = -4.06101e-07;
		data["Geiger_Counter005.020.wav"] = -4.45345e-07;
		data["SaxBritHorns12.wav"] = -3.49636e-07;
		data["Time002.624.wav"] = -1.86454e-07;
		data["bell_A3.wav"] = -3.52736e-07;
		data["gamelan-gong.wav"] = -8.17134e-07;
		data["gt_E4.wav"] = -3.18173e-07;
		data["pno_Eb1.wav"] = 0;
		data["silence.wav"] = 0;
		data["vln_A3.wav"] = 0;
		data["vln_D5.wav"] = 0;
		data["whitenoise.wav"] = 3.34021e-09;

		mDescriptors->AddSlope();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::SpectralDescriptors::GetSlope);
	}


};


} // namespace CLAMTest
