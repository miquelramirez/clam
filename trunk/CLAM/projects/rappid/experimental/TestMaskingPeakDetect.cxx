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

#include "MaskingPeakDetect.hxx"

#include <iostream>

using namespace CLAM;

namespace CLAMTest {

	class MaskingPeakDetectTest {
		MaskingPeakDetect *mpObject1;
		MaskingPeakDetect *mpObject2;


		TSize            mMaxPeaks;
		TData            mMagThreshold;
		TSize            mSpectralSize;
		EFrequencyWeight mFrequencyWeight;
		EMaskingShape    mMaskingShape;
		TData            mMaskingBandwidth;

		// Basic Test methods. You should make them bigger, and maybe
		// add some more.
		// You can notify a test failure either by throwing an
		// exception inside them or by making them return a false
		// value. In the first case no more checks will be made.  In
		// the second case tests will continue, but at the end a test
		// failure will be reported.
		bool TestConstruction();
		bool TestConfiguration();
		bool ConfigureData(Spectrum &data);
		bool ConfigureData(SpectralPeakArray &data);
		bool TestExecution();
		void NewData(Spectrum &s);
		bool TestDestruction();
	public:
		bool Do();
	};


	bool MaskingPeakDetectTest::TestConstruction()
	{
		MaskingPeakDetectConfig cfg;
		// You should add more constructions here,
		// using different configuration parameters, or
		// other constructors the class may provide.
		mpObject1 = new MaskingPeakDetect();
		mpObject2 = new MaskingPeakDetect(cfg);
		return true;
	}

	bool MaskingPeakDetectTest::TestConfiguration()
	{
		MaskingPeakDetectConfig cfg,cfg2;

		cfg.SetMaxPeaks(mMaxPeaks);
		cfg.SetMagThreshold(mMagThreshold);
		cfg.SetSpectralSize(mSpectralSize);
		cfg.SetFrequencyWeight(mFrequencyWeight);
		cfg.SetMaskingShape(mMaskingShape);
		cfg.SetMaskingBandwidth(mMaskingBandwidth);

		cfg2 = cfg;

		mpObject1->Configure(cfg);
		mpObject2->Configure(cfg);
		mpObject1->Configure(cfg2);
		mpObject2->Configure(cfg2);
		return true;
	}

	bool MaskingPeakDetectTest::ConfigureData(SpectralPeakArray &data)
	{
		data.AddMagBuffer();
		data.AddFreqBuffer();
		data.AddIndexArray();
		data.AddIsIndexUpToDate();
		data.UpdateData();
		data.GetMagBuffer().Resize(mMaxPeaks);
		data.GetFreqBuffer().Resize(mMaxPeaks);
		data.GetIndexArray().Resize(mMaxPeaks);
		data.GetMagBuffer().SetSize(mMaxPeaks);
		data.GetFreqBuffer().SetSize(mMaxPeaks);
		data.GetIndexArray().SetSize(mMaxPeaks);
		return true;
	}

	bool MaskingPeakDetectTest::ConfigureData(Spectrum &data)
											  
	{
		data.SetSize(mSpectralSize);
		return true;
	}

	void MaskingPeakDetectTest::NewData(Spectrum &s)
	{
		for (int i=0; i < mSpectralSize; i++)
			s.GetMagBuffer()[i] = pow(double(rand())/double(RAND_MAX),5);
	}

	bool MaskingPeakDetectTest::TestExecution()
	{
		int iterations = 1000;

		Spectrum  data1;
		SpectralPeakArray data2;
		SpectralPeakArray data3;

		ConfigureData(data1);
		ConfigureData(data2);
		ConfigureData(data3);

		mpObject1->Start();
		mpObject2->Start();

		while (iterations--) {
			NewData(data1);
			mpObject1->Do(data1,data2);
			mpObject2->Do(data1,data3);
			std::cout << "Peaks detected: " << data2.GetnPeaks()
				      << std::endl;
		}

		mpObject1->Stop();
		mpObject2->Stop();

		return true;
	}

	bool MaskingPeakDetectTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool MaskingPeakDetectTest::Do()
	{
		bool res, ok = true;

		mMaxPeaks = 50;
		mMagThreshold = -60;
		mSpectralSize = 1024;
		mFrequencyWeight = EFrequencyWeight::eNone;
		mMaskingShape = EMaskingShape::eNone;
		mMaskingBandwidth = 200.0;

		res = TestConstruction();
		if (!res) {
			std::cerr << "Construction failed." << std::endl;
			ok = false;
		}
		res = TestConfiguration();
		if (!res) {
			std::cerr << "Configuration failed." << std::endl;
			ok = false;
		}
		res = TestExecution();
		if (!res) {
			std::cerr << "Execution failed." << std::endl;
			ok = false;
		}
		res = TestDestruction();
		if (!res) {
			std::cerr << "Destruction failed." << std::endl;
			ok = false;
		}
		return ok;
	}

}

// main() return values:
//    0:        Test sucessful.
//    non-zero: Test Failed.
int main()
{
	try {
		CLAMTest::MaskingPeakDetectTest test;
		bool result = test.Do();
		if (result==false) {
			std::cerr << "Failed." << std::endl;
			return 1;
		}
	}
	catch (std::exception &e) {
		std::cerr << "ERROR: Excepton: " << e.what() << std::endl
				  << "Failed." << std::endl;
		return 1;
	}
	std::cerr << "Passed." << std::endl;
	return 0;
}
