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

#include <CLAM/AudioDescriptors.hxx>
#include <CLAM/MonoAudioFileReaderConfig.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/XMLStorage.hxx>



#include <iostream>
#include <iomanip>

namespace CLAMTest
{


class AudioDescriptorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AudioDescriptorsTest );

class AudioDescriptorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AudioDescriptorsTest );
	CPPUNIT_TEST( testLogAttackTime );
	CPPUNIT_TEST( testTemporalCentroid );
	CPPUNIT_TEST( testZeroCrossingRate );
	CPPUNIT_TEST( testEnergy );
//	CPPUNIT_TEST( testDecrease );


	CPPUNIT_TEST_SUITE_END();

private:
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		mPathToTestData = GetTestDataDirectory() + "descriptorsData/";

		mDescriptors = new CLAM::AudioDescriptors();
		mDescriptors->RemoveAll();
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
		delete mDescriptors;
	}

private:
	CLAM::AudioDescriptors *mDescriptors;
	std::string mPathToTestData;

	CLAM::Audio ReadAudio(const std::string & fileName)
	{
		CLAM::MonoAudioFileReaderConfig infilecfg;
		infilecfg.SetSourceFile(fileName);
		CLAM::MonoAudioFileReader reader(infilecfg);

		CLAM::Audio buf;
		buf.SetSize(reader.GetHeader().GetSamples());
		buf.SetSampleRate(reader.GetHeader().GetSampleRate());

		reader.Start();
		reader.Do(buf);
		reader.Stop();

		return buf;

	}

	CLAM::Audio helperGetData(const std::string & fileName)
	{
		std::string extension = fileName.substr(fileName.size()-4,fileName.size());
		std::string fullPath = mPathToTestData+fileName;
		if (extension!=".xml")
			return ReadAudio(fullPath);
		CLAM::Audio audio;
		CLAM::XMLStorage::Restore(audio,fullPath); 
		return audio;
	}

	void assertDescriptorExtractionInsideTolerance(const std::map<std::string, CLAM::TData> & expected, 
		CLAM::TData tolerance, CLAM::TData & (CLAM::AudioDescriptors::*getter)() const )
	{
		std::stringstream log;
		bool success = true;
		mDescriptors->UpdateData();
		CLAM::Audio audio;
		std::map<std::string, CLAM::TData>::const_iterator it;
		for (it = expected.begin(); it != expected.end(); it++) {
			audio = helperGetData((*it).first);
			mDescriptors->SetpAudio(&audio);
			mDescriptors->Compute();
			if (
				(std::isnan((mDescriptors->*getter)()) != std::isnan(it->second)) ||
				(std::isinf((mDescriptors->*getter)()) != std::isinf(it->second)) ||
				(mDescriptors->*getter)() > (*it).second + tolerance ||
				(mDescriptors->*getter)() < (*it).second - tolerance
				)
			{
				log << (*it).first
				<< ": expected " << std::setprecision(15) << it->second
				<< ", received " << std::setprecision(15) << (mDescriptors->*getter)()
				<< ", difference " << std::setprecision(15) << (it->second - (mDescriptors->*getter)())
				<< std::endl;

				success = false;
			}
		}

		CPPUNIT_ASSERT_MESSAGE(std::string("Error margins surpassed:\n")+log.str()
			,success);
	}

private:

	void testLogAttackTime()
	{
		CLAM::TData tolerance = 0.0001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -1.380329;
		data["Balance000.600.wav"] = -1.691646;
		data["Balance000.992.wav"] = -1.687790;
		data["Balance001.988.wav"] = -1.501736;
		data["Balance010.910.wav"] = -1.672699;
		data["Cello_A2.wav"] = -1.114367;
		data["Cello_C2.wav"] = -0.166912;
		data["Disco_Rojo001.008.wav"] = -1.864842;
		data["Disco_Rojo002.327.wav"] = -1.605818;
		data["Geiger_Counter005.020.wav"] = -1.787106;
		data["SaxBritHorns12.wav"] = -1.697495;
		data["Time002.624.wav"] = -1.895476;
		data["bell_A3.wav"] = -1.703922;
		data["gamelan-gong.wav"] = -1.331838;
		data["gt_E4.wav"] = -2.216304;
		data["pno_Eb1.wav"] = -1.725360;
		data["silence.wav"] = -5.000000;
		data["vln_A3.wav"] = -0.572336;
		data["vln_D5.wav"] = -0.451927;
		data["whitenoise.wav"] = -1.897804;

		mDescriptors->AddLogAttackTime();

		assertDescriptorExtractionInsideTolerance(data, tolerance, &CLAM::AudioDescriptors::GetLogAttackTime);
	}

	void testTemporalCentroid()
	{
		CLAM::TData tolerance = 0.001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 1.10702383518219;
		data["Balance000.600.wav"] = 0.045051;
		data["Balance000.992.wav"] = 0.044318;
		data["Balance001.988.wav"] = 0.051773;
		data["Balance010.910.wav"] = 0.042667;
		data["Cello_A2.wav"] = 0.207102;
		data["Cello_C2.wav"] = 2.455419;
		data["Disco_Rojo001.008.wav"] = 0.049909;
		data["Disco_Rojo002.327.wav"] = 0.052137;
		data["Geiger_Counter005.020.wav"] = 0.053245;
		data["SaxBritHorns12.wav"] = 0.075114;
		data["Time002.624.wav"] = 0.050065;
		data["bell_A3.wav"] = 1.198206;
		data["gamelan-gong.wav"] = 1.803790;
		data["gt_E4.wav"] = 0.646689;
		data["pno_Eb1.wav"] = 2.3413622379303;
		data["silence.wav"] = 0.05;  // Avoids NaN
		data["vln_A3.wav"] = 0.500487;
		data["vln_D5.wav"] = 0.456816;
		data["whitenoise.wav"] = 0.049801;

		mDescriptors->AddTemporalCentroid();

		assertDescriptorExtractionInsideTolerance(data,tolerance, &CLAM::AudioDescriptors::GetTemporalCentroid);

	}

	void testZeroCrossingRate()
	{
		CLAM::TData tolerance = 0.006;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 0.126168;
		data["Balance000.600.wav"] = 0.006349;
		data["Balance000.992.wav"] = 0.011791;
		data["Balance001.988.wav"] = 0.027891;
		data["Balance010.910.wav"] = 0.004989;
		data["Cello_A2.wav"] = 0.032868;
		data["Cello_C2.wav"] = 0.019433;
		data["Disco_Rojo001.008.wav"] = 0.123129;
		data["Disco_Rojo002.327.wav"] = 0.018594;
		data["Geiger_Counter005.020.wav"] = 0.029252;
		data["SaxBritHorns12.wav"] = 0.062320;
		data["Time002.624.wav"] = 0.287755;
		data["bell_A3.wav"] = 0.045547;
		data["gamelan-gong.wav"] = 0.006900;
		data["gt_E4.wav"] = 0.035136;
		data["pno_Eb1.wav"] = 0.011902;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 0.021366;
		data["vln_D5.wav"] = 0.060226;
		data["whitenoise.wav"] = 0.504535;

		mDescriptors->AddZeroCrossingRate();

		assertDescriptorExtractionInsideTolerance(data,tolerance, &CLAM::AudioDescriptors::GetZeroCrossingRate);
	}

	void testEnergy()
	{
		CLAM::TData tolerance = 5.0;   // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = 1371.542406;
		data["Balance000.600.wav"] = 64.598414;
		data["Balance000.992.wav"] = 28.735406;
		data["Balance001.988.wav"] = 25.132553;
		data["Balance010.910.wav"] = 107.347657;
		data["Cello_A2.wav"] = 390.249943;
		data["Cello_C2.wav"] = 6398.441127;
		data["Disco_Rojo001.008.wav"] = 156.370021;
		data["Disco_Rojo002.327.wav"] = 160.895829;
		data["Geiger_Counter005.020.wav"] = 36.972497;
		data["SaxBritHorns12.wav"] = 363.679432;
		data["Time002.624.wav"] = 42.958806;
		data["bell_A3.wav"] = 316.499392;
		data["gamelan-gong.wav"] = 4236.226573;
		data["gt_E4.wav"] = 395.857142;
		data["pno_Eb1.wav"] = 4870.099537;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = 5176.937709;
		data["vln_D5.wav"] = 3730.424440;
		data["whitenoise.wav"] = 1482.633870;

		mDescriptors->AddEnergy();

		assertDescriptorExtractionInsideTolerance(data,tolerance, &CLAM::AudioDescriptors::GetEnergy);

	}


	void testDecrease()
	{
		CLAM::TData tolerance = 0.001;  // Due to numerical inaccuracies

		std::map<std::string, CLAM::TData> data;
		data["AltoSax-Iowa-ff-Db3B3-Region 012.wav"] = -1.229580;
		data["Balance000.600.wav"] = -4.107201;
		data["Balance000.992.wav"] = -7.823889;
		data["Balance001.988.wav"] = -3.630962;
		data["Balance010.910.wav"] = -6.836714;
		data["Cello_A2.wav"] = -1.477162;
		data["Cello_C2.wav"] = -0.466605;
		data["Disco_Rojo001.008.wav"] = -0.347480;
		data["Disco_Rojo002.327.wav"] = -1.109347;
		data["Geiger_Counter005.020.wav"] = -8.500433;
		data["SaxBritHorns12.wav"] = -23.568533;
		data["Time002.624.wav"] = -1.077993;
		data["bell_A3.wav"] = -0.239008;
		data["gamelan-gong.wav"] = -0.224273;
		data["gt_E4.wav"] = -0.830142;
		data["pno_Eb1.wav"] = -0.187336;
		data["silence.wav"] = 0.000000;
		data["vln_A3.wav"] = -4.458854;
		data["vln_D5.wav"] = -8.369821;
		data["whitenoise.wav"] = -0.189161;

		mDescriptors->AddDecrease();

		assertDescriptorExtractionInsideTolerance(data,tolerance, &CLAM::AudioDescriptors::GetDecrease);

	}

};


} // namespace CLAMTest

