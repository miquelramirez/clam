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

#include "SpectralEnvelopeExtractor.hxx"
#include "FunkyPresentation.hxx"
#include "FunctionWindow.hxx"

#include <iostream>
#include <cstdlib>

using namespace CLAM;
using namespace CLAMGUI;

namespace CLAMTest {

	class SpectralEnvelopeExtractorTest {
		SpectralEnvelopeExtractor *mpObject1;
		SpectralEnvelopeExtractor *mpObject2;

		int   mSpectralSize;
		int   mMaxPeaks;
		TData mSpectralRange;

		void NewData(Spectrum &s);
		void ConfigureData(Spectrum &data);
		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		bool Do();
	};


	bool SpectralEnvelopeExtractorTest::TestConstruction()
	{
		SpectralEnvelopeExtractorConfig cfg;

		mpObject1 = new SpectralEnvelopeExtractor();
		mpObject2 = new SpectralEnvelopeExtractor(cfg);
		return true;
	}

	bool SpectralEnvelopeExtractorTest::TestConfiguration()
	{
		SpectralEnvelopeExtractorConfig cfg,cfg2;

		cfg.SetSpectralSize(mSpectralSize);
		cfg.SetSpectralRange(mSpectralRange);
		cfg.SetnMaxPeaks(mMaxPeaks);
		cfg.GetDetectConfig().SetMaskingShape(EMaskingShape::eLinear);

		cfg2 = cfg;

		mpObject1->Configure(cfg);
		mpObject2->Configure(cfg);
		mpObject1->Configure(cfg2);
		mpObject2->Configure(cfg2);
		return true;
	}

	void SpectralEnvelopeExtractorTest::NewData(Spectrum &s)
	{
		for (int i=0; i < mSpectralSize; i++)
			s.GetMagBuffer()[i] = pow(double(rand())/double(RAND_MAX),5);
	}

	void SpectralEnvelopeExtractorTest::ConfigureData(Spectrum &data)
	{
		SpectrumConfig cfg;
		cfg.SetSize(mSpectralSize);
		cfg.SetSpectralRange(mSpectralRange);
		data.Configure(cfg);
	}


	bool SpectralEnvelopeExtractorTest::TestExecution()
	{
		Spectrum data1;
		Spectrum data2;
		Spectrum data3;

		ConfigureData(data1);
		ConfigureData(data2);
		ConfigureData(data3);

		GlutFunctionWindow mWindow;
		FunkyPresentation mFunky(FunkyPresentation::eRealTime);

		mFunky.BindSpectrum(data1);
		mFunky.BindSpectralEnvelope(data2);
		mFunky.BindPeaks(mpObject1->DebugGetPeaks());

		mFunky.SetDisplay(mWindow);

		mWindow.RunInBackground();

		mpObject1->Start();
		mpObject2->Start();


		while (1) {

			NewData(data1);

			mpObject1->Do(data1,data2);
			mpObject2->Do(data1,data2);

			mFunky.Refresh();
		}

		mpObject1->Stop();
		mpObject2->Stop();

		return true;
	}

	bool SpectralEnvelopeExtractorTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool SpectralEnvelopeExtractorTest::Do()
	{
		bool res, ok = true;

		mSpectralSize  = 1024;
		mSpectralRange = 48000/2;
		mMaxPeaks      = 100;

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
int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	try {
		CLAMTest::SpectralEnvelopeExtractorTest test;
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
