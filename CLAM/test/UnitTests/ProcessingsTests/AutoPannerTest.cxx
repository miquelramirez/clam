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
#include "AutoPanner.hxx"
#include "InControl.hxx"
#include "OSDefines.hxx" // some  tests use M_PI

namespace CLAMTest {

class AutoPannerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( AutoPannerTest );

class AutoPannerTest : public CppUnit::TestFixture
{
public:

	void setUp()
	{
		mControlSender.mLeft.AddLink(&mReceiverLeft);
		mControlSender.mRight.AddLink(&mReceiverRight);		
	}
	void tearDown(){}

private:

	CPPUNIT_TEST_SUITE( AutoPannerTest );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateAndNoPhase );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateWithPhase );
	CPPUNIT_TEST( testDo_WhenFreqIsQuarterOfSamplingRate );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateAndFrameSizeIs10 );
	CPPUNIT_TEST( testDo_WhenFreqIsQuarterOfSamplingRateAndPhaseAndFrameSizeIs7 );



	CPPUNIT_TEST_SUITE_END();
public:
	AutoPannerTest()
		: mReceiverLeft("Receiver Left"),
		  mReceiverRight("Receiver Right"),
		  mControlSender(44100, 44100, 0),
		  mDelta(0.00001)
		// AutoPanner needs a constructor but we won't use
		// this configuration
	{
	}

private:
	//fixture attributes
	CLAM::InControl mReceiverLeft;
	CLAM::InControl mReceiverRight;
	CLAM::AutoPanner mControlSender;
	const double mDelta;

	void testDo_WhenFreqEqualSamplingRateAndNoPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 1;

		mControlSender.Configure( freq , samplingRate, phase, frameSize );

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);
	}

	void testDo_WhenFreqEqualSamplingRateWithPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = CLAM::TData(M_PI/2);
		int frameSize = 1;

		mControlSender.Configure( freq , samplingRate, phase, frameSize );

		mControlSender.Do();


		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta);

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta);
	}

	void testDo_WhenFreqIsQuarterOfSamplingRate()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = 0;
		int frameSize = 1;

		mControlSender.Configure( freq , samplingRate, phase, frameSize );

		mControlSender.Do();
		//first Do gives the initial state, already tested
		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta); 
	}

	void testDo_WhenFreqEqualSamplingRateAndFrameSizeIs10()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 10;

		mControlSender.Configure( freq , samplingRate, phase, frameSize );

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);
	}

	void testDo_WhenFreqIsQuarterOfSamplingRateAndPhaseAndFrameSizeIs7()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = CLAM::TData(M_PI);
		int frameSize = 7;

		mControlSender.Configure( freq , samplingRate, phase, frameSize );

		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta); 
		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta); 
		mControlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta); 
	}





};

} //namespace CLAMTest


