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
		_controlSender.mLeft.AddLink(&_receiverLeft);
		_controlSender.mRight.AddLink(&_receiverRight);		
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
		: _receiverLeft("Receiver Left"),
		  _receiverRight("Receiver Right"),
		  _controlSender(44100, 44100, 0),
		  _delta(0.00001)
		// AutoPanner needs a constructor but we won't use
		// this configuration
	{
	}

private:
	//fixture attributes
	CLAM::InControl _receiverLeft;
	CLAM::InControl _receiverRight;
	CLAM::AutoPanner _controlSender;
	const double _delta;

	void testDo_WhenFreqEqualSamplingRateAndNoPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 1;

		_controlSender.Configure( freq , samplingRate, phase, frameSize );

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);
	}

	void testDo_WhenFreqEqualSamplingRateWithPhase()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = CLAM::TData(M_PI/2);
		int frameSize = 1;

		_controlSender.Configure( freq , samplingRate, phase, frameSize );

		_controlSender.Do();


		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta);

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta);
	}

	void testDo_WhenFreqIsQuarterOfSamplingRate()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = 0;
		int frameSize = 1;

		_controlSender.Configure( freq , samplingRate, phase, frameSize );

		_controlSender.Do();
		//first Do gives the initial state, already tested
		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta); 
	}

	void testDo_WhenFreqEqualSamplingRateAndFrameSizeIs10()
	{
		CLAM::TData freq = 44100;
		CLAM::TData samplingRate = 44100;
		CLAM::TData phase = 0.0;
		int frameSize = 10;

		_controlSender.Configure( freq , samplingRate, phase, frameSize );

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta);
	}

	void testDo_WhenFreqIsQuarterOfSamplingRateAndPhaseAndFrameSizeIs7()
	{
		CLAM::TData samplingRate = 44100;
		CLAM::TData freq = samplingRate/4;
		CLAM::TData phase = CLAM::TData(M_PI);
		int frameSize = 7;

		_controlSender.Configure( freq , samplingRate, phase, frameSize );

		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta); 
		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverRight.GetLastValue(),_delta); 
		_controlSender.Do();

		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(0.0) , _receiverLeft.GetLastValue(),_delta);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TControlData(1.0) , _receiverRight.GetLastValue(),_delta); 
	}





};

} //namespace CLAMTest


