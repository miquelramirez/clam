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
#include "AutoPanner.hxx" // CLAM
#include "InControl.hxx" // CLAM
#include "OSDefines.hxx" // CLAM
#include "Processing.hxx" // CLAM

namespace CLAMTest {

class AutoPannerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( AutoPannerTest );

class AutoPannerTest : public CppUnit::TestFixture, public CLAM::Processing
{
public:

	void setUp(){}
	void tearDown(){}

private:

	CPPUNIT_TEST_SUITE( AutoPannerTest );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateAndNoPhase );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateWithPhase );
	CPPUNIT_TEST( testDo_WhenFreqIsQuarterOfSamplingRate );
	CPPUNIT_TEST( testDo_WhenFreqEqualSamplingRateAndFrameSizeIs10 );
	CPPUNIT_TEST( testDo_WhenFreqIsQuarterOfSamplingRateAndPhaseAndFrameSizeIs7 );



	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }

public:
	AutoPannerTest()
		: mReceiverLeft("Receiver Left",this),
		  mReceiverRight("Receiver Right",this),
		  mDelta(0.00001f)
	{}

private:
	//fixture attributes
	CLAM::FloatInControl mReceiverLeft;
	CLAM::FloatInControl mReceiverRight;
	double mDelta;

	void testDo_WhenFreqEqualSamplingRateAndNoPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 1;

		CLAM::AutoPanner controlSender;
		controlSender.GetOutControl("Left Control").AddLink(mReceiverLeft);
		controlSender.GetOutControl("Right Control").AddLink(mReceiverRight);		

		CLAM::AutoPannerConfig cfg;
		cfg.SetFrequency( freq );
		cfg.SetSamplingRate( samplingRate );
		cfg.SetPhase( phase );
		cfg.SetFrameSize( frameSize );

		controlSender.Configure(cfg);
		controlSender.Start();
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);

		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);
		controlSender.Stop();
	}

	void testDo_WhenFreqEqualSamplingRateWithPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = CLAM::TData(M_PI/2);
		int frameSize = 1;

		CLAM::AutoPanner controlSender;
		controlSender.GetOutControl("Left Control").AddLink(mReceiverLeft);
		controlSender.GetOutControl("Right Control").AddLink(mReceiverRight);		

		CLAM::AutoPannerConfig cfg;
		cfg.SetFrequency( freq );
		cfg.SetSamplingRate( samplingRate );
		cfg.SetPhase( phase );
		cfg.SetFrameSize( frameSize );

		controlSender.Configure(cfg);
		controlSender.Start();
		controlSender.Do();


		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta);

		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta);

		controlSender.Stop();
	}

	void testDo_WhenFreqIsQuarterOfSamplingRate()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = 0;
		int frameSize = 1;

		CLAM::AutoPanner controlSender;
		controlSender.GetOutControl("Left Control").AddLink(mReceiverLeft);
		controlSender.GetOutControl("Right Control").AddLink(mReceiverRight);		

		CLAM::AutoPannerConfig cfg;
		cfg.SetFrequency( freq );
		cfg.SetSamplingRate( samplingRate );
		cfg.SetPhase( phase );
		cfg.SetFrameSize( frameSize );

		controlSender.Configure(cfg);
		controlSender.Start();
		controlSender.Do();
		//first Do gives the initial state, already tested
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta); 

		controlSender.Stop();
	}

	void testDo_WhenFreqEqualSamplingRateAndFrameSizeIs10()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 10;

		CLAM::AutoPanner controlSender;
		controlSender.GetOutControl("Left Control").AddLink(mReceiverLeft);
		controlSender.GetOutControl("Right Control").AddLink(mReceiverRight);		

		CLAM::AutoPannerConfig cfg;
		cfg.SetFrequency( freq );
		cfg.SetSamplingRate( samplingRate );
		cfg.SetPhase( phase );
		cfg.SetFrameSize( frameSize );

		controlSender.Configure(cfg);
		controlSender.Start();
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);

		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta);

		controlSender.Stop();
	}

	void testDo_WhenFreqIsQuarterOfSamplingRateAndPhaseAndFrameSizeIs7()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = CLAM::TData(M_PI);
		int frameSize = 7;

		CLAM::AutoPanner controlSender;
		controlSender.GetOutControl("Left Control").AddLink(mReceiverLeft);
		controlSender.GetOutControl("Right Control").AddLink(mReceiverRight);		

		CLAM::AutoPannerConfig cfg;
		cfg.SetFrequency( freq );
		cfg.SetSamplingRate( samplingRate );
		cfg.SetPhase( phase );
		cfg.SetFrameSize( frameSize );

		controlSender.Configure(cfg);
		controlSender.Start();
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta); 
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverRight.GetLastValue(),mDelta); 
		controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , mReceiverLeft.GetLastValue(),mDelta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , mReceiverRight.GetLastValue(),mDelta); 

		controlSender.Stop();
	}
};

} //namespace CLAMTest


