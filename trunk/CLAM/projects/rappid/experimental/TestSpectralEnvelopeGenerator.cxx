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

#include "SpectralEnvelopeGenerator.hxx"
#include "FunkyPresentation.hxx"
#include "FunctionWindow.hxx"

#include <iostream>
#include <unistd.h>

using namespace CLAM;
using namespace CLAMGUI;

namespace CLAMTest {

	class SpectralEnvelopeGeneratorTest {
		SpectralEnvelopeGenerator *mpObject1;
		SpectralEnvelopeGenerator *mpObject2;

		int mMaxPeaks;
		int   mSpectralSize;
		TData mSpectralRange;
		GlutFunctionWindow mWindow;
		FunkyPresentation mFunky;
		
		void NewData(SpectralPeakArray&);

		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		SpectralEnvelopeGeneratorTest() 
			: mFunky(MTPresentation::eStepByStep)
			{}
		bool Do();
	};

	void SpectralEnvelopeGeneratorTest::NewData(SpectralPeakArray& peaks)
	{
		int npeaks = int(50.0*(double(rand())/double(RAND_MAX)));

		TData freq = mSpectralRange /  double(npeaks) * double(rand()) / double(RAND_MAX) ;

		for (int i = 0; i<npeaks; i++)
		{
			freq += mSpectralRange /  double(npeaks) * double(rand()) / double(RAND_MAX) ;
			peaks.GetFreqBuffer()[i] = freq;
			peaks.GetMagBuffer()[i]  = pow(double(rand())/double(RAND_MAX),4);
		}
		peaks.SetnPeaks(npeaks);
	}

	bool SpectralEnvelopeGeneratorTest::TestConstruction()
	{
		SpectralEnvelopeGeneratorConfig cfg;

		mpObject1 = new SpectralEnvelopeGenerator();
		mpObject2 = new SpectralEnvelopeGenerator(cfg);
		return true;
	}

	bool SpectralEnvelopeGeneratorTest::TestConfiguration()
	{
		SpectralEnvelopeGeneratorConfig cfg,cfg2;
		cfg.SetName("EGenerator1");
		cfg.SetSpectralRange(mSpectralRange);
		cfg.SetSpectralSize(mSpectralSize);
		cfg.GetMemoryFactor().SetInitValue(0.0);

		cfg2 = cfg;
		cfg2.SetName("EGenerator2");

		mpObject1->Configure(cfg2);
		mpObject1->Configure(cfg);
		mpObject2->Configure(cfg2);
		return true;
	}

	bool SpectralEnvelopeGeneratorTest::TestExecution()
	{
//		int iterations = 100000;

		SpectralPeakArray data1;
		Spectrum data2;
		Spectrum data3;
		SpectrumConfig cfg;
		cfg.SetSpectralRange(mSpectralRange);
		cfg.SetSize(mSpectralSize);
		data2.Configure(cfg);
		data3.Configure(cfg);

		data1.AddMagBuffer();
		data1.AddFreqBuffer();
		data1.AddIndexArray();
		data1.AddIsIndexUpToDate();
		data1.UpdateData();
		data1.GetMagBuffer().Resize(mMaxPeaks);
		data1.GetFreqBuffer().Resize(mMaxPeaks);
		data1.GetIndexArray().Resize(mMaxPeaks);
		data1.GetMagBuffer().SetSize(mMaxPeaks);
		data1.GetFreqBuffer().SetSize(mMaxPeaks);
		data1.GetIndexArray().SetSize(mMaxPeaks);


		mFunky.BindSpectrum(data2);
		mFunky.BindSpectralEnvelope(data3);
		mFunky.BindPeaks(data1);
		mFunky.SetDisplay(mWindow);

		mWindow.RunInBackground();


		mpObject1->Start();
		mpObject2->Start();

//		while (iterations--) {

		TData memory=0.0;
		TData phase = 0.0;
		while (1) {

			NewData(data1);
			
			mpObject1->Do(data1,data2);
			mpObject2->Do(data1,data3);

			mFunky.Refresh();

			memory = 0.5 + 0.5*sin(phase);
			mpObject1->MemoryFactor.DoControl(memory);
			mpObject2->MemoryFactor.DoControl(memory);
			phase+=0.01;
		}

		mpObject1->Stop();
		mpObject2->Stop();

		return true;
	}

	bool SpectralEnvelopeGeneratorTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool SpectralEnvelopeGeneratorTest::Do()
	{
		bool res, ok = true;

		mSpectralRange = 48000/2;
		mSpectralSize = 1024;
		mMaxPeaks = 100;

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

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);

	try {
		CLAMTest::SpectralEnvelopeGeneratorTest test;
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
