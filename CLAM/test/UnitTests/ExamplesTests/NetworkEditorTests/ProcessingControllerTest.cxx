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
#include "ProcessingPresentation.hxx"
#include "ProcessingController.hxx"
#include "Oscillator.hxx"
#include "AutoPanner.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "AudioFile.hxx"
#include <string>
#include <stdlib.h>

namespace CLAMTest 
{

class ProcessingControllerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingControllerTest );


class ProcessingControllerTest : public CppUnit::TestFixture, public NetworkGUI::ProcessingPresentation
{
protected:
	void SetObservedClassName( const std::string& )
	{
	}
	
	void SetInPort( const std::string & )
	{
	}
	
	void SetOutPort( const std::string & )
	{
	}
	
	void SetInControl( const std::string & )
	{
	}
	
	void SetOutControl( const std::string & )
	{
	}
	
	void UpdatePresentation()
	{
	}
	
	void Show()
	{
	}
	
	void Hide()
	{
	}

		
	CPPUNIT_TEST_SUITE( ProcessingControllerTest );
		
	CPPUNIT_TEST( testPublishThrowsAssert_whenControllerNotBinded );
	CPPUNIT_TEST( testBindToProcessingAttachesControllerToProcessing );
	CPPUNIT_TEST( testUpdateListOfPortsAndControlsRegistersItsNames );
	CPPUNIT_TEST( testSlotSendOutControlValueSendsValue_whenSignalEmittedFromPresentation );
	CPPUNIT_TEST_SUITE_END();

public:
	void testPublishThrowsAssert_whenControllerNotBinded()
	{
		CLAMVM::ProcessingController proc;
		try
		{
			proc.Publish();
			CPPUNIT_FAIL(  "Assert expected, but no exception was thrown" );
		}
		catch(...)
		{
		}
	}
	
	void testBindToProcessingAttachesControllerToProcessing()
	{
		CLAMVM::ProcessingController proc;
		CLAM::Oscillator osc;
		proc.BindTo( osc );
		try
		{
			proc.Publish();
		}
		catch(...)
		{
			CPPUNIT_FAIL( "exception should not be thrown, processing not binded correctly to controller" );
		}
		CPPUNIT_ASSERT_EQUAL( std::string("Oscillator"), proc.GetObservedClassName() );

		CLAMVM::ProcessingController::NamesList::iterator it;
		it = proc.BeginInPortNames();
		CPPUNIT_ASSERT_EQUAL( std::string("Input Phase Modulation"), *(it++) );
		CPPUNIT_ASSERT_EQUAL( std::string("Input Frequency Modulation"), *(it++) );
		CPPUNIT_ASSERT_MESSAGE("Can't be more than two in ports", (it==proc.EndInPortNames()) );

		it = proc.BeginOutPortNames();
		CPPUNIT_ASSERT_EQUAL( std::string("Audio Output"), *(it++) );
		CPPUNIT_ASSERT_MESSAGE("Can't be more than one out port", (it==proc.EndOutPortNames()) );
		
		it = proc.BeginInControlNames();
		CPPUNIT_ASSERT_EQUAL( std::string("Pitch"), *(it++) );
		CPPUNIT_ASSERT_EQUAL( std::string("Amplitude"), *(it++) );
		CPPUNIT_ASSERT_EQUAL( std::string("ModIndex"), *(it++) );
		CPPUNIT_ASSERT_MESSAGE("Can't be more than three in controls", (it==proc.EndInControlNames()) );
	
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any out control", (proc.BeginOutControlNames()==proc.EndOutControlNames()) );

	}
	
	void testUpdateListOfPortsAndControlsRegistersItsNames()
	{
		// get name of file to read
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		std::string fileToRead("");
		if ( !pathToTestData )
			fileToRead = "../../../../../CLAM-TestData/"; 
		else
			fileToRead = pathToTestData;
		fileToRead += "ElvisStereo.ogg";

		// create the processing and configure it
		CLAM::AudioFile audioFile;
		audioFile.SetLocation( fileToRead );
		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.AddSourceFile();
		cfg.UpdateData();
		cfg.SetSourceFile( audioFile );
		CLAM::MultiChannelAudioFileReader reader;
		
		// bind to a controller
		CLAMVM::ProcessingController proc;
		proc.BindTo( reader );
		proc.Publish();

		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any in port", (proc.BeginInPortNames()==proc.EndInPortNames()) );
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any out port", (proc.BeginOutPortNames()==proc.EndOutPortNames()) );
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any in control", (proc.BeginInControlNames()==proc.EndInControlNames()) );
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any out control", (proc.BeginOutControlNames()==proc.EndOutControlNames()) );
		

		reader.Configure( cfg );
		proc.UpdateListOfPortsAndControls();

		CLAMVM::ProcessingController::NamesList::iterator it;

		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any in port", (proc.BeginInPortNames()==proc.EndInPortNames()) );
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any in control", (proc.BeginInControlNames()==proc.EndInControlNames()) );
		CPPUNIT_ASSERT_MESSAGE("Shouldn't exist any out control", (proc.BeginOutControlNames()==proc.EndOutControlNames()) );
		
		it = proc.BeginOutPortNames();
		CPPUNIT_ASSERT_EQUAL( std::string("Channel #0"), *(it++) );
		CPPUNIT_ASSERT_EQUAL( std::string("Channel #1"), *(it++) );
		CPPUNIT_ASSERT_MESSAGE("Can't be more than two out ports", (it==proc.EndOutPortNames()) );
	}
	
	void testSlotSendOutControlValueSendsValue_whenSignalEmittedFromPresentation()
	{
		CLAMVM::ProcessingController proc;
		CLAM::Oscillator osc;
		CLAM::AutoPanner panner;
		panner.GetOutControls().Get( "Left Control" ).AddLink( &(osc.GetInControls().Get( "Pitch")) );
		proc.BindTo( panner );
		proc.Publish();
		CLAM::TControlData newValue = 24.3f;
		SignalSendOutControlValue.Connect( proc.SlotSendOutControlValue );
		SignalSendOutControlValue.Emit( "Left Control", newValue );
		CPPUNIT_ASSERT_EQUAL( osc.GetInControls().Get("Pitch").GetLastValue(), 24.3f );
		
	}
};

} // namespace CLAMTest
