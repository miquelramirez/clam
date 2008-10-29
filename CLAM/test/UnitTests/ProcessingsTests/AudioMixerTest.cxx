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
#include "AudioMixer.hxx" // CLAM
#include "Processing.hxx" // CLAM
#include "Audio.hxx" // CLAM
#include "OutControl.hxx" // CLAM

namespace CLAMTest{

class AudioMixerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( AudioMixerTest );

class AudioMixerTest : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE( AudioMixerTest );

	CPPUNIT_TEST( testDo_WhenControlsGivesEqualValue );
	CPPUNIT_TEST( testDo_WhenControlsGivesDifferentValue );

	CPPUNIT_TEST_SUITE_END();
	
	//processing
	CLAM::FloatOutControl mOut1;
	CLAM::FloatOutControl mOut2;
	CLAM::AudioMixer<2> mMixer;

	//data
	CLAM::Audio mInAudio1;
	CLAM::Audio mInAudio2;
	CLAM::Audio mOutAudio;
	const double mDelta;

public:
	void setUp()
	{

		//add link to in controls of mixer
		mOut1.AddLink(&mMixer.GetInControl("Input Gain_0"));
		mOut2.AddLink(&mMixer.GetInControl("Input Gain_1"));

		mInAudio1.SetSize(1);
		mInAudio2.SetSize(1);
		mOutAudio.SetSize(1);

		CLAM::AudioMixerConfig mixerCfg;
		mixerCfg.SetFrameSize(1);
		mMixer.Configure(mixerCfg);

		mMixer.GetInPort("Input Audio_0").Attach(mInAudio1);
		mMixer.GetInPort("Input Audio_1").Attach(mInAudio2);
		mMixer.GetOutPort("Output Audio").Attach(mOutAudio);
	}

public:
	AudioMixerTest()
		: mOut1("Sender left"),
		  mOut2("Sender right"),
		  mDelta(0.000001)
	{		
	}

private:
	void testDo_WhenControlsGivesEqualValue()
	{
		mInAudio1.GetBuffer()[0] = 1;
		mInAudio2.GetBuffer()[0] = 2;

		mMixer.Start();
		const CLAM::TControlData val1(0.3f);
		const CLAM::TControlData val2(0.3f);

		mOut1.SendControl( val1 );
		mOut2.SendControl( val2 );

		mMixer.Do();
		mMixer.Do();

		CPPUNIT_ASSERT_EQUAL( CLAM::TControlData( val1 ), 
				      mMixer.GetInControl("Input Gain_0").GetLastValue());
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData( (val1*1 + val2*2)/2 ) , mOutAudio.GetBuffer()[0], mDelta );
	}

	void testDo_WhenControlsGivesDifferentValue()
	{
		mInAudio1.GetBuffer()[0] = 3;
		mInAudio2.GetBuffer()[0] = 2;

		mMixer.Start();

		mOut1.SendControl(0.5f);
		mOut2.SendControl(0.6f);

		mMixer.Do();
		mMixer.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(1.35) , mOutAudio.GetBuffer()[0], mDelta );
	}
};
	
}

