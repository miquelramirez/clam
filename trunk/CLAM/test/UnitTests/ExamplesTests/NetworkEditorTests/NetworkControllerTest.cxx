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
#include "Network.hxx"
#include "NetworkController.hxx"
#include "SimpleOscillator.hxx"
#include "AutoPanner.hxx"
#include "AudioMultiplier.hxx"
#include "PushFlowControl.hxx"
#include "Processing.hxx"
#include "Signalv1.hxx"

#include <iostream>
namespace CLAMTest 
{

class NetworkControllerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkControllerTest );


class NetworkControllerTest : public CppUnit::TestFixture, public CLAMVM::NetworkController
{
	CPPUNIT_TEST_SUITE( NetworkControllerTest );
		
	CPPUNIT_TEST( testNetworkControllerStopsNetwork_WhenIsStopped );
	CPPUNIT_TEST( testAddProcessingCreatesProcessingController );
	CPPUNIT_TEST( testAddProcessingThrowsException_WhenDuplicatedName );
	CPPUNIT_TEST( testRemoveProcessingDeletesProcessingController );
	CPPUNIT_TEST( testRemoveProcessingThrowsException_WhenProcessingControllerDoesntExist );
	CPPUNIT_TEST( testProcessingNameChangedModifiesProcessingControllerMap );
	CPPUNIT_TEST( testProcessingNameChangedModifiesPortConnectionControllerMap );
	CPPUNIT_TEST( testProcessingNameChangedModifiesControlConnectionControllerMap );
	CPPUNIT_TEST( testChangeKeyMapDoesntModify_WhenRepeatedKey );

	
	CPPUNIT_TEST_SUITE_END();

	CLAM::Network * mNetwork;
	
public:
	void setUp() 
	{	
		mNetwork = new CLAM::Network;
		const int nodeSize=1;
		mNetwork->AddFlowControl( new CLAM::PushFlowControl(nodeSize) );
		
 		BindTo(*mNetwork);	
	}
	
	void tearDown() 
	{
		mLoopCondition = false;
		
		mThread.Stop();
		if (mObserved)
			mObserved->Stop();

		Clear();
		mObserved = 0;
		delete mNetwork;

	}
	
	void testNetworkControllerStopsNetwork_WhenIsStopped()
	{
		SigSlot::Signalv1<bool> SignalChangeState;
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		mNetwork->AddProcessing( "oscillator", oscillator) ;

		SignalChangeState.Connect( SlotChangeState );
		
		SignalChangeState.Emit(true);
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		
		SignalChangeState.Emit(false);
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );
		SlotChangeState.Unbind();
	}

	void testAddProcessingCreatesProcessingController()
	{
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		AddProcessing( "oscillator", oscillator );
				
		CPPUNIT_ASSERT_EQUAL( 1, (int)mProcessingControllers.size() );
	}

	void testAddProcessingThrowsException_WhenDuplicatedName()
	{
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::SimpleOscillator * oscillator2 = new CLAM::SimpleOscillator;

		AddProcessing( "oscillator", oscillator );
		try
		{
			AddProcessing( "oscillator", oscillator2 );
			CPPUNIT_FAIL("exception should be thrown when duplicating key");
		}
		catch(CLAM::ErrAssertionFailed &)
		{			
		}	
	}

	void testRemoveProcessingDeletesProcessingController()
	{
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		AddProcessing( "oscillator", oscillator );
		RemoveProcessing( "oscillator" ); // TODO:: its name should be RemoveProcessing

		CPPUNIT_ASSERT_EQUAL( 0, (int)mProcessingControllers.size() );
	}

	void testRemoveProcessingThrowsException_WhenProcessingControllerDoesntExist()
	{
		try
		{
			RemoveProcessing("oscillator");
			CPPUNIT_FAIL("exception should be thrown when trying to remove a nonexistent key");
		}
		catch(CLAM::ErrAssertionFailed &)
		{
		}
	}

	void testProcessingNameChangedModifiesProcessingControllerMap()
	{	
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		AddProcessing( "oscillator", oscillator );
		ProcessingControllersMapIterator it = mProcessingControllers.find("oscillator");
		ProcessingNameChanged( "mynewname", it->second );
		CPPUNIT_ASSERT_EQUAL( it->second, mProcessingControllers.find( "mynewname" )->second );
	}

	void testProcessingNameChangedModifiesPortConnectionControllerMap()
	{
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AudioMultiplier * multiplier = new CLAM::AudioMultiplier;
		
		AddProcessing( "oscillator", oscillator );
		AddProcessing( "multiplier", multiplier );
		
		CreatePortConnection( "oscillator.Audio Output", "multiplier.First Audio Input" );

		ProcessingControllersMapIterator it = mProcessingControllers.find("oscillator");
		ProcessingNameChanged( "mynewname", it->second );

		ConnectionAdapterIterator itc;
		itc = mConnectionAdapters.begin();
		std::string newName( "mynewname.Audio Output" );
		CPPUNIT_ASSERT_EQUAL( newName, (*itc)->GetOutName() );
	}
	void testProcessingNameChangedModifiesControlConnectionControllerMap()
	{
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AutoPanner * panner = new CLAM::AutoPanner;
		
		AddProcessing( "oscillator", oscillator );
		AddProcessing( "panner", panner );
		
		CreateControlConnection( "panner.Left Control", "oscillator.Amplitude" );

		ProcessingControllersMapIterator it = mProcessingControllers.find("oscillator");
		ProcessingNameChanged( "mynewname", it->second );

		ConnectionAdapterIterator itc;
		itc = mConnectionAdapters.begin();
		std::string newName ("mynewname.Amplitude");
		CPPUNIT_ASSERT_EQUAL( newName, (*itc)->GetInName() );
	}



	void testChangeKeyMapDoesntModify_WhenRepeatedKey()
	{	
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::SimpleOscillator * oscillator2 = new CLAM::SimpleOscillator;
		AddProcessing( "oscillator", oscillator );
		AddProcessing( "repeated name", oscillator2 );
		
		CPPUNIT_ASSERT_EQUAL( false, ChangeKeyMap( "oscillator", "repeated name") );
	}


			

};

} // namespace CLAMTest
