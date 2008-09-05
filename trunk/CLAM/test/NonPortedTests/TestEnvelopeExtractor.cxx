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

#include "EnvelopeExtractor.hxx"
#include "EnvelopeGenerator.hxx"

#if 0
#include "GnuplotSnapshot.hxx"
#endif

#include "NodeTmpl.hxx"
#include "StreamBuffer.hxx"
#include "CircularStreamImpl.hxx"
#include "WaveGenerator.hxx"
#include "AudioFileOut.hxx"
#include "Controller.hxx"

#include <iostream>
#include <cstdlib>

namespace CLAMTest {

	using namespace CLAM;

	class EnvelopeExtractorTest {

		int mDataLength;
		int mSampleRate;
		int mFrameSize;
		int mIterations;

		EnvelopeExtractor *mpExtractor1;
		EnvelopeExtractor *mpExtractor2;
		EnvelopeGenerator *mpGenerator1;
		EnvelopeGenerator *mpGenerator2;

		WaveGenerator mGenerator;
		AudioFileOut  mOutput1;
		AudioFileOut  mOutput2;
		Controller    mController;

		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode1;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode2;

		void SendControls(int controls);
		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();

	public:
		EnvelopeExtractorTest();
		bool Do();
	};


	EnvelopeExtractorTest::EnvelopeExtractorTest()
		: mDataLength(100000),
		  mSampleRate(48000),
		  mFrameSize(512),
		  mIterations(100)
	{
		WaveGeneratorConfig wcfg;
		wcfg.SetSampleRate(mSampleRate);
		wcfg.SetFrequency(50.0);
		mGenerator.Configure(wcfg);

		Audio proto;
		proto.SetSampleRate(mSampleRate);
		mInputNode.SetPrototype(proto);
		mOutputNode1.SetPrototype(proto);
		mOutputNode2.SetPrototype(proto);

		AudioFileConfig fcfg;
		fcfg.SetSampleRate(mSampleRate);
		fcfg.SetFiletype(EAudioFileType::eWave);
		fcfg.SetFilename("test1.wav");
		mOutput1.Configure(fcfg);
		fcfg.SetFilename("test2.wav");
		mOutput2.Configure(fcfg);

	}

	bool EnvelopeExtractorTest::TestConstruction()
	{
		EnvExtractorConfig cfg;
		// You should add more constructions here,
		// using different configuration parameters, or
		// other constructors the class may provide.
		mpExtractor1 = new EnvelopeExtractor();
		mpExtractor2 = new EnvelopeExtractor(cfg);
		mpGenerator1 = new EnvelopeGenerator();
		mpGenerator2 = new EnvelopeGenerator();
		return true;
	}

	bool EnvelopeExtractorTest::TestConfiguration()
	{
		EnvExtractorConfig cfg,cfg2;

		cfg.GetInterpolationPeriod().SetInitValue(2.5);
		cfg.GetIntegrationLength().SetInitValue(5.0);
		cfg.GetNormalLevel().SetInitValue(2.0);
		cfg.SetFrameSize(mFrameSize);
		cfg.SetSampleRate(mSampleRate);
		cfg.SetInterpolationType(EInterpolation::eSpline);

		cfg2 = cfg;

		mpExtractor1->Configure(cfg);
		mpExtractor2->Configure(cfg);
		mpExtractor1->Configure(cfg2);
		mpExtractor2->Configure(cfg2);

		EnvelopeGeneratorConfig ecfg1,ecfg2;
		ecfg1.SetDuration(1.0);
		ecfg1.SetFrameEnvelopes(true);
		ecfg1.SetSampleRate(mSampleRate);
		mpGenerator1->Configure(ecfg1);
		ecfg2 = ecfg1;
		mpGenerator2->Configure(ecfg2);

		return true;
	}

	void EnvelopeExtractorTest::SendControls(int controls)
	{
		if (controls & 0x1) {
			TData interpolation_period = double(rand())/double(RAND_MAX);
			mpExtractor1->cInterpolationPeriod.DoControl(interpolation_period);
			mpExtractor2->cInterpolationPeriod.DoControl(interpolation_period);
		}
		if (controls & 0x2) {
			TData integration_length = double(rand())/double(RAND_MAX);
			mpExtractor1->cIntegrationLength.DoControl(integration_length);
			mpExtractor2->cIntegrationLength.DoControl(integration_length);
			integration_length = integration_length + 10.0;
		}
		if (controls & 0x4) {
			TData normal_level = double(rand())/double(RAND_MAX);
			mpExtractor1->cNormalLevel.DoControl(normal_level);
			mpExtractor2->cNormalLevel.DoControl(normal_level);
		}
		if (controls & 0x8) {
			TData silence_level = double(rand())/double(RAND_MAX);
			mpExtractor1->cSilenceLevel.DoControl(silence_level);
			mpExtractor2->cSilenceLevel.DoControl(silence_level);
		}
		
	}

	bool EnvelopeExtractorTest::TestExecution()
	{
		Envelope envelope1,envelope2;

		mGenerator.Output.Attach(mInputNode);
		mpExtractor1->Input.Attach(mInputNode);
		mpExtractor2->Input.Attach(mInputNode);

		mpExtractor1->Output.Attach(envelope1);
		mpGenerator1->Input.Attach(envelope1);

		mpExtractor2->Output.Attach(envelope2);
		mpGenerator2->Input.Attach(envelope2);

		mpGenerator1->Output.Attach(mOutputNode1);
		mOutput1.GetInPort(0).Attach(mOutputNode1);

		mpGenerator2->Output.Attach(mOutputNode2);
		mOutput2.GetInPort(0).Attach(mOutputNode2);

		mInputNode.Configure(5*mFrameSize);
		mOutputNode1.Configure(5*mFrameSize);
		mOutputNode2.Configure(5*mFrameSize);

		mGenerator.Start();
		mpExtractor1->Start();
		mpExtractor2->Start();
		mpGenerator1->Start();
		mpGenerator2->Start();
		mOutput1.Start();
		mOutput2.Start();

		for (int i=0; i<mIterations; i++)
		{

			mGenerator.Do();

			mpExtractor1->Do();
			mpExtractor2->Do();
			mpGenerator1->Do();
			mpGenerator2->Do();

			mOutput1.Do();
			mOutput2.Do();

			if ( (i & 0x7) == 0x7)
				SendControls(int(4.0*rand()/(RAND_MAX+1.0)));
		}

		mGenerator.Stop();
		mpExtractor1->Stop();
		mpExtractor2->Stop();
		mpGenerator1->Stop();
		mpGenerator2->Stop();
		mOutput1.Stop();
		mOutput2.Stop();

		return true;
	}

	bool EnvelopeExtractorTest::TestDestruction()
	{
		delete mpExtractor1;
		delete mpExtractor2;
		return true;
	}

	bool EnvelopeExtractorTest::Do()
	{
		bool res, ok = true;

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
		CLAMTest::EnvelopeExtractorTest test;
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

