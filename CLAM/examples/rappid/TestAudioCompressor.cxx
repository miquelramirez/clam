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

#include "AudioCompressor.hxx"
#include "WaveGenerator.hxx"
#include "AudioFileOut.hxx"

// And you may want to provide some feedback.
#include <iostream>

#include <NodeTmpl.hxx>
#include <CircularStreamImpl.hxx>

namespace CLAMTest {

	using namespace CLAM;

	class AudioCompressorTest {
		AudioCompressor *mpObject1;
		AudioCompressor *mpObject2;

		WaveGenerator mGenerator;
		AudioFileOut mOutput1,mOutput2;

		int mFrameSize;
		TData mSampleRate;

		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode1;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode2;

		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		AudioCompressorTest();

		bool Do();
	};


	AudioCompressorTest::AudioCompressorTest()
		: mpObject1(0),
		  mpObject2(0),
		  mFrameSize(512),
		  mSampleRate(48000)
	{
		mInputNode.SetSampleRate(mSampleRate);
		mOutputNode1.SetSampleRate(mSampleRate);
		mOutputNode2.SetSampleRate(mSampleRate);

		WaveGeneratorConfig gcfg;
		gcfg.SetFrameSize(mFrameSize);
		gcfg.SetSampleRate(mSampleRate);
		gcfg.SetAmplitude(10.0);
		mGenerator.Configure(gcfg);

		AudioFileConfig fcfg;
		fcfg.SetFrameSize(mFrameSize);
		fcfg.SetSampleRate(mSampleRate);
		fcfg.SetFiletype(EAudioFileType::eWave);
		fcfg.SetFilename("out1.wav");
		mOutput1.Configure(fcfg);
		fcfg.SetFilename("out2.wav");
		mOutput2.Configure(fcfg);
	}

	bool AudioCompressorTest::TestConstruction()
	{
		AudioCompressorConfig cfg;

		mpObject1 = new AudioCompressor();
		mpObject2 = new AudioCompressor(cfg);
		return true;
	}

	bool AudioCompressorTest::TestConfiguration()
	{
		AudioCompressorConfig cfg1,cfg2;
		cfg1.SetFrameSize(mFrameSize);
		cfg1.SetSampleRate(mSampleRate);

		cfg2 = cfg1;

		cfg2.SetCompressionType(ECompressionType::eArctangent);

		mpObject1->Configure(cfg1);
		mpObject2->Configure(cfg1);
		mpObject1->Configure(cfg2);
		mpObject2->Configure(cfg2);
		return true;
	}

	bool AudioCompressorTest::TestExecution()
	{
		int i, iterations = 1000;

		mGenerator.Output.Attach(mInputNode);
		mpObject1->Input.Attach  (mInputNode);
		mpObject2->Input.Attach  (mInputNode);

		mpObject1->Output.Attach(mOutputNode1);
		mOutput1.Input.Attach  (mOutputNode1);

		mpObject2->Output.Attach(mOutputNode2);
		mOutput2.Input.Attach  (mOutputNode2);

		mInputNode.Configure(10*mFrameSize);
		mOutputNode1.Configure(10*mFrameSize);
		mOutputNode2.Configure(10*mFrameSize);

		mGenerator.Start();
		mpObject1->Start();
		mpObject2->Start();
		mOutput1.Start();
		mOutput2.Start();

		for (i=0; i<iterations; i++) {
			mGenerator.Do();

			mpObject1->Do();
			mpObject2->Do();

			mOutput1.Do();
			mOutput2.Do();

		}

		mGenerator.Stop();
		mpObject1->Stop();
		mpObject2->Stop();
		mOutput1.Stop();
		mOutput2.Stop();


		return true;
	}

	bool AudioCompressorTest::TestDestruction()
	{
		delete mpObject1;
		delete mpObject2;
		return true;
	}

	bool AudioCompressorTest::Do()
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
		CLAMTest::AudioCompressorTest test;
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
