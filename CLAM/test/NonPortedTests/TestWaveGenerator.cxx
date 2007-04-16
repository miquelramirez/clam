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

#include "TestUtils.hxx"
#include "Err.hxx"

// Here you should include the class you want to test!
#include "WaveGenerator.hxx"

// And you may want to provide some feedback.
#include <iostream>

namespace CLAMTest {

	class WaveGeneratorTest {
		CLAM::WaveGenerator *mpObject1;
		CLAM::WaveGenerator *mpObject2;

		int mDataLength;
		int mPos;
		int mSize;

		TData mAmpl1;
		TTime mFrec1;
		TTime mPhase1;
		TTime mSampleRate1;

		TData mAmpl2;
		TTime mFrec2;
		TTime mPhase2;
		TTime mSampleRate2;

		int mData1Length;
		int mData2Length;

		TData mThreshold;

		TData Sine1(int pos);
		TData Sine2(int pos);

		bool CheckSine1Data(Audio &data1, int pos);
		bool CheckSine2Data(Audio &data2, int pos);

		bool TestSine();

		// Basic Test methods. You should make them bigger, and maybe
		// add some more.
		// You can notify a test failure either by throwing an
		// exception inside them or by making them return a false
		// value. In the first case no more checks will be made.  In
		// the second case tests will continue, but at the end a test
		// failure will be reported.
		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		WaveGeneratorTest();
		bool Do();
	};

	WaveGeneratorTest::WaveGeneratorTest()
		: mpObject1(0),
		  mpObject2(0),
		  mDataLength(10000),
		  mPos(0),
		  mSize(222),
		  mAmpl1(2.0),
		  mFrec1(1000.0),
		  mPhase1(0.555),
		  mSampleRate1(8000),
		  mAmpl2(3.0),
		  mFrec2(200.0),
		  mPhase2(1.333),
		  mSampleRate2(48000),
		  mThreshold(0.025)
	{
	}


	bool WaveGeneratorTest::TestConstruction()
	{
		CLAM::WaveGeneratorConfig cfg;
		// You should add more constructions here,
		// using different configuration parameters, or
		// other constructors the class may provide.
		mpObject1 = new CLAM::WaveGenerator();
		mpObject2 = new CLAM::WaveGenerator(cfg);
		return true;
	}

	bool WaveGeneratorTest::TestConfiguration()
	{
		CLAM::WaveGeneratorConfig cfg1,cfg2;

		cfg1.SetFrequency(mFrec1);
		cfg1.SetAmplitude(mAmpl1);
		cfg1.SetPhase(mPhase1);
		cfg1.SetSampleRate(mSampleRate1);

		cfg2.SetFrequency(mFrec2);
		cfg2.SetAmplitude(mAmpl2);
		cfg2.SetPhase(mPhase2);
		cfg2.SetSampleRate(mSampleRate2);

		mpObject1->Configure(cfg2);
		mpObject1->Configure(cfg1);

		mpObject2->Configure(cfg1);
		mpObject2->Configure(cfg2);
		return true;
	}

	TData WaveGeneratorTest::Sine1(int pos)
	{
		return mAmpl1 * sin(mPhase1 + pos * mFrec1 * 2.0 * M_PI/ mSampleRate1);
	}

	TData WaveGeneratorTest::Sine2(int pos)
	{
		return mAmpl2 * sin(mPhase2 + pos * mFrec2 * 2.0 * M_PI/ mSampleRate2);
	}

	bool WaveGeneratorTest::CheckSine1Data(Audio &data1, int pos)
	{
		int i;
		Array<TData> &buff = data1.GetBuffer();

		for (i=0; i<buff.Size(); i++)
		{
			if (fabs (buff[i] - Sine1(pos+i)) > mThreshold)
				return false;
		}
		return true;
	}

	bool WaveGeneratorTest::CheckSine2Data(Audio &data2, int pos)
	{
		int i;
		Array<TData> &buff = data2.GetBuffer();

		for (i=0; i<buff.Size(); i++)
		{
			if (fabs (buff[i] - Sine2(pos+i)) > mThreshold)
				return false;
		}
		return true;
	}

	bool WaveGeneratorTest::TestSine()
	{
		Audio data1,data2;

		data1.SetSize(mSize);
		data2.SetSize(mSize);

		mpObject1->Start();
		mpObject2->Start();

		while (mPos < mDataLength + data1.GetSize())
		{
			mpObject1->Do(data1);
			mpObject2->Do(data2);
			if (!CheckSine1Data(data1,mPos))
				return false;
			if (!CheckSine2Data(data2,mPos))
				return false;
			mPos += data1.GetSize();
		}

		mpObject1->Stop();
		mpObject2->Stop();
		return true;
	}

	bool WaveGeneratorTest::TestExecution()
	{
		bool ok = true;

		ok &= TestSine();

		return ok;
	}

	bool WaveGeneratorTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool WaveGeneratorTest::Do()
	{
		bool res, ok = true;

		res = TestConstruction();
		CLAM_ASSERT( res, "Construction failed" );

		res = TestConfiguration();
		CLAM_ASSERT( res, "Configuration failed." );

		res = TestExecution();
		CLAM_ASSERT( res, "Execution has failed" );

		res = TestDestruction();
		CLAM_ASSERT( res, "Destruction failed." );

		return ok;
	}

}

// main() return values:
//    0:        Test sucessful.
//    non-zero: Test Failed.
int main()
{
	try {
		CLAMTest::WaveGeneratorTest test;
		bool result = test.Do();
		if (result==false) {
			std::cerr << "Failed." << std::endl;
			return 1;
		}
	}
	catch ( CLAM::Err e )
	{
		e.Print();
		std::cerr << "Abnormal Program termination" << std::endl;
		std::cerr << "Failed." << std::endl;
		return 1;
	}
	catch (std::exception &e) {
		std::cerr << "ERROR: Excepton: " << e.what() << std::endl
				  << "Failed." << std::endl;
		return 1;
	}
	std::cerr << "Passed." << std::endl;
	return 0;
}

