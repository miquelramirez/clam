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

#include "RappidInput.hxx"
#include "RappidOutput.hxx"

#include "AudioMixer.hxx"
//#include "clam_su_wrapper.h"

// And you may want to provide some feedback.
#include <iostream>

#include <NodeTmpl.hxx>
#include <CircularStreamImpl.hxx>

#include "sched.h"

namespace CLAMTest {

	using namespace CLAM;

	class RappidIOTest {
		RappidInput *mpInput1;
		RappidInput *mpInput2;
		RappidInput *mpInput3;
		RappidInput *mpInput4;

		RappidOutput *mpOutput1;
		RappidOutput *mpOutput2;
		RappidOutput *mpOutput3;

		AudioMixer<4> mMixer;

		int mFrameSize;
		TData mSampleRate;

		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode1;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode2;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode3;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode4;
		NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode;

		bool TestConstruction();
		bool TestConfiguration();
		bool TestExecution();
		bool TestDestruction();
	public:
		RappidIOTest();

		bool Do();
	};


	RappidIOTest::RappidIOTest()
		: mpInput1(0),
		  mpInput2(0),
		  mFrameSize(512),
		  mSampleRate(48000)
	{
		AudioMixerConfig mcfg;
		mcfg.SetFrameSize(mFrameSize);
		mcfg.SetSampleRate(mSampleRate);
		mMixer.Configure(mcfg);

		mInputNode1.SetSampleRate(mSampleRate);
		mInputNode2.SetSampleRate(mSampleRate);
		mInputNode3.SetSampleRate(mSampleRate);
		mInputNode4.SetSampleRate(mSampleRate);
		mOutputNode.SetSampleRate(mSampleRate);
	}

	bool RappidIOTest::TestConstruction()
	{
		RappidIOConfig cfg;

		mpInput1 = new RappidInput();
		mpInput2 = new RappidInput();
		mpInput3 = new RappidInput();
		mpInput4 = new RappidInput();
		mpOutput1 = new RappidOutput();
		mpOutput2 = new RappidOutput();
		mpOutput3 = new RappidOutput();
		return true;
	}

	bool RappidIOTest::TestConfiguration()
	{
		RappidIOConfig cfg1,cfg2,cfg3,cfg4;
		cfg1.GetGlobalConfig().SetFrameSize(mFrameSize);
		cfg1.GetGlobalConfig().SetSampleRate(mSampleRate);
		cfg1.GetGlobalConfig().SetVerbose(true);
		cfg1.SetAudioDeviceChannelID(0);
		cfg1.SetFileName("in.wav");

		cfg4 = cfg3 = cfg2 = cfg1;

		cfg2.SetAudioDeviceChannelID(1);
		cfg3.SetIOType(ERappidIOType::eFile);
		cfg4.SetIOType(ERappidIOType::eWaveform);
		

		cfg1.SetName("Input1");
		cfg1.Dump();
		mpInput1->Configure(cfg2);
		mpInput1->Configure(cfg1);

		cfg2.SetName("Input2");
		cfg2.Dump();
		mpInput2->Configure(cfg3);
		mpInput2->Configure(cfg2);

		cfg3.SetName("Input3");
		cfg3.Dump();
		mpInput3->Configure(cfg4);
		mpInput3->Configure(cfg3);

		cfg4.SetName("Input4");
		cfg4.Dump();
		mpInput4->Configure(cfg4);

		cfg1.SetName("Output1");
		cfg1.Dump();
		mpOutput1->Configure(cfg1);

		cfg2.SetName("Output2");
		cfg2.Dump();
		mpOutput2->Configure(cfg2);

		cfg3.SetName("Output3");
		cfg3.SetFileName("out.wav");
		cfg3.Dump();
		mpOutput3->Configure(cfg3);

		RappidIOBase::DumpCommon();

		return true;
	}

	bool RappidIOTest::TestExecution()
	{
		int i, iterations = 1000;

		mpInput1->Output.Attach(mInputNode1);
		mpInput2->Output.Attach(mInputNode2);
		mpInput3->Output.Attach(mInputNode3);
		mpInput4->Output.Attach(mInputNode4);

		mMixer.Input[0].Attach(mInputNode1);
		mMixer.Input[1].Attach(mInputNode2);
		mMixer.Input[2].Attach(mInputNode3);
		mMixer.Input[3].Attach(mInputNode4);

		mMixer.Output.Attach(mOutputNode);

		mpOutput1->Input.Attach(mOutputNode);
		mpOutput2->Input.Attach(mOutputNode);
		mpOutput3->Input.Attach(mOutputNode);

		mInputNode1.Configure(10*mFrameSize);
		mInputNode2.Configure(10*mFrameSize);
		mInputNode3.Configure(10*mFrameSize);
		mInputNode4.Configure(10*mFrameSize);
		mOutputNode.Configure(10*mFrameSize);

		mpInput1->Start();
		mpInput2->Start();
		mpInput3->Start();
		mpInput4->Start();

		mMixer.Start();

		mpOutput1->Start();
		mpOutput2->Start();
		mpOutput3->Start();

		for (i=0; i<iterations; i++) {

			mpInput1->Do();
			mpInput2->Do();
			mpInput3->Do();
			mpInput4->Do();

			mMixer.Do();

			mpOutput1->Do();
			mpOutput2->Do();
			mpOutput3->Do();
		}

		mpInput1->Stop();
		mpInput2->Stop();
		mpInput3->Stop();
		mpInput4->Stop();
		mMixer.Stop();

		mpOutput1->Stop();
		mpOutput2->Stop();
		mpOutput3->Stop();

		return true;
	}

	bool RappidIOTest::TestDestruction()
	{
		delete mpInput1;
		delete mpInput2;
		delete mpInput3;
		delete mpInput4;
		delete mpOutput1;
		delete mpOutput2;
		delete mpOutput3;
		return true;
	}

	bool RappidIOTest::Do()
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
int main(int, char**)
{
	sched_param param;
	param.sched_priority = 50;
	sched_setscheduler(0,SCHED_FIFO,&param);

	try {
		CLAMTest::RappidIOTest test;
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
