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
#include "BaseLoggable.hxx" // also includes <sstream>
#include "ProcessingController.hxx"
#include "Oscillator.hxx"
#include "PushFlowControl.hxx"
#include "Processing.hxx"
#include "Signalv1.hxx"

#include <iostream>

namespace CLAMTest 
{

class ProcessingControllerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingControllerTest );


class ProcessingControllerTest : public CppUnit::TestFixture, public CLAMVM::ProcessingController
{
	CPPUNIT_TEST_SUITE( ProcessingControllerTest );
		
	CPPUNIT_TEST( testPublishCreatesPortsAndControls );
	CPPUNIT_TEST( testConfigureProcessingExecutesConfigureOfObservedProcessing );
		
	CPPUNIT_TEST_SUITE_END();

	CLAM::Oscillator * mProc;
public:
	ProcessingControllerTest() : mProc(0)		
	{
		mProc = new CLAM::Oscillator;
		BindTo(*mProc);
	}

	~ProcessingControllerTest()
	 {
		 delete mProc;
	 }
	
	void testPublishCreatesPortsAndControls()
	{
		Publish();
		CPPUNIT_ASSERT_EQUAL( 2, (int)mInPortNames.size() );
		CPPUNIT_ASSERT_EQUAL( 1, (int)mOutPortNames.size() );
		CPPUNIT_ASSERT_EQUAL( 3, (int)mInControlNames.size() );
		CPPUNIT_ASSERT_EQUAL( 0, (int)mOutControlNames.size() );

	}
	
	void testConfigureProcessingExecutesConfigureOfObservedProcessing()
	{
		CLAM::OscillatorConfig cfg;
		cfg.SetFrequency(880.0);
		cfg.SetAmplitude(0.5);
		cfg.SetSamplingRate(22050);

		ConfigureProcessing(cfg);

		CLAM::OscillatorConfig & cfg2 ((CLAM::OscillatorConfig&)(mProc->GetConfig()));
		CPPUNIT_ASSERT_EQUAL( 880.0f, cfg2.GetFrequency() );
		CPPUNIT_ASSERT_EQUAL( 0.5f, cfg2.GetAmplitude() );
		CPPUNIT_ASSERT_EQUAL( 22050.0f, cfg2.GetSamplingRate() );
	}
};

} // namespace CLAMTest
