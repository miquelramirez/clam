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
#include <string>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "Network.hxx"
#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AutoPanner.hxx"
#include "FFT_rfftw.hxx"
#include "XMLStorage.hxx"
#include "BasicFlowControl.hxx"

namespace CLAMTest {

class NetworkSerializationTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkSerializationTest );


class NetworkSerializationTest : public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE( NetworkSerializationTest );

	CPPUNIT_TEST( TestLoadEmptyNetwork );
	CPPUNIT_TEST( TestSaveEmptyNetwork );
	CPPUNIT_TEST( TestLoadNetworkWithOneProcessing );
	CPPUNIT_TEST( TestSaveNetworkWithOneProcessing );
	CPPUNIT_TEST( TestLoadNetworkWithMoreThanOneProcessing );
	CPPUNIT_TEST( TestSaveNetworkWithMoreThanOneProcessing );
	CPPUNIT_TEST( TestLoadNetworkWithPortsConnection );
	CPPUNIT_TEST( TestSaveNetworkWithPortsConnection );
	CPPUNIT_TEST( TestSaveNetworkWithControlsConnection );
	CPPUNIT_TEST( TestLoadNetworkWithControlsConnection );
	CPPUNIT_TEST_SUITE_END();

	std::string mPathToTestData;

public: // TestFixture interface
	void setUp()
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../CLAM-TestData"; 
		else
			mPathToTestData = pathToTestData;
		mPathToTestData += "/networkTestsData/";
	}
private:
	void TestLoadEmptyNetwork()
	{
		CLAM::Network foo;
		std::string name("FooNetwork");

		CLAM::XMLStorage::Restore(foo, mPathToTestData+"emptynetwork.xml");

		CPPUNIT_ASSERT_EQUAL( name , foo.GetName() );
		
	}

	void TestSaveEmptyNetwork()
	{
		CLAM::Network foo;
		foo.SetName( "FooNetwork" );

		std::stringstream output("");
		std::string result( "<network id=\"FooNetwork\"/>" );

		CLAM::XMLStorage::Dump(foo, "network", output);
		std::string outputString(output.str());
		CPPUNIT_ASSERT_EQUAL( result, outputString );
		

	}
	
	void TestLoadNetworkWithOneProcessing()
	{	
		CLAM::Network foo;
		std::string procName("oscillator");
		foo.AddFlowControl( new CLAM::BasicFlowControl );

		CLAM::XMLStorage::Restore(foo, mPathToTestData+"networkwithoneproc.xml");
		
		CPPUNIT_ASSERT_EQUAL( true, foo.HasProcessing( procName ) );

	}

	void TestSaveNetworkWithOneProcessing()
	{
		CLAM::Network foo;
		std::string name("FooNetworkWithOneProcessing");
		std::string procName("oscillator");
		CLAM::Oscillator* osc = new CLAM::Oscillator;

		foo.SetName(name);
		foo.AddFlowControl( new CLAM::BasicFlowControl );
		foo.AddProcessing("oscillator", osc);

		std::stringstream output("");
		CLAM::XMLStorage::Dump(foo, "network",output);
		std::string result(
			"<network id=\"FooNetworkWithOneProcessing\">"
			"<processing id=\"oscillator\" type=\"Oscillator\">"
			"<Frequency>440</Frequency>"
			"<Amplitude>1</Amplitude>"
			"<ModIndex>1</ModIndex>"
			"<Phase>0</Phase>"
			"<SamplingRate>44100</SamplingRate>"
			"</processing>"
			"</network>"
			);
		std::string outputString(output.str());

		CPPUNIT_ASSERT_EQUAL( result, outputString );
	}

	void TestLoadNetworkWithMoreThanOneProcessing()
	{
		CLAM::Network foo;
		std::string procName("oscillator");
		std::string procName2("fftw");
		std::string procName3("multiplier");
		foo.AddFlowControl( new CLAM::BasicFlowControl );

		CLAM::XMLStorage::Restore(foo, mPathToTestData+"networkwithmorethanoneproc.xml");
		
		CPPUNIT_ASSERT_EQUAL( true, foo.HasProcessing( procName ) );
		CPPUNIT_ASSERT_EQUAL( true, foo.HasProcessing( procName2 ) );
		CPPUNIT_ASSERT_EQUAL( true, foo.HasProcessing( procName3 ) );
	}

	void TestSaveNetworkWithMoreThanOneProcessing()
	{		
		CLAM::Network foo;
		std::string name("FooNetworkWithMoreThanOneProcessing");
		std::stringstream output("");

		std::string result(
			"<network id=\"FooNetworkWithMoreThanOneProcessing\">"
			"<processing id=\"fftw\" type=\"FFT_rfftw\">"
			"<AudioSize>0</AudioSize>"
			"</processing>"
			"<processing id=\"multiplier\" type=\"AudioMultiplier\"/>"
			"<processing id=\"oscillator\" type=\"Oscillator\">"
			"<Frequency>440</Frequency>"
			"<Amplitude>1</Amplitude>"
			"<ModIndex>1</ModIndex>"
			"<Phase>0</Phase>"
			"<SamplingRate>44100</SamplingRate>"
			"</processing>"
			"</network>" 
			);
		
		foo.SetName(name);
		foo.AddFlowControl( new CLAM::BasicFlowControl );
		foo.AddProcessing("oscillator", new CLAM::Oscillator );
		foo.AddProcessing("multiplier", new CLAM::AudioMultiplier );
		foo.AddProcessing("fftw", new CLAM::FFT_rfftw );

		CLAM::XMLStorage::Dump(foo, "network", output );
		std::string outputString(output.str());
		
		CPPUNIT_ASSERT_EQUAL( result, outputString );

	}

	void TestLoadNetworkWithPortsConnection()
	{		
		CLAM::Network foo;
		CLAM::XMLStorage storage;
		foo.AddFlowControl( new CLAM::BasicFlowControl );

		storage.Restore(foo, mPathToTestData+"networkwithportconnections.xml");
		
		CPPUNIT_ASSERT_EQUAL( true, foo.GetProcessing("oscillator").GetOutPorts().Get("Audio Output").IsConnectedTo( 
					      foo.GetProcessing("multiplier").GetInPorts().Get("First Audio Input")));
	}

	void TestSaveNetworkWithPortsConnection()
	{
		CLAM::Network foo;
		std::string name("FooNetworkWithPortsConnection");
		std::stringstream output("");
		
		std::string result(
			"<network id=\"FooNetworkWithPortsConnection\">"
			"<processing id=\"multiplier\" type=\"AudioMultiplier\"/>"
			"<processing id=\"oscillator\" type=\"Oscillator\">"
			"<Frequency>440</Frequency>"
			"<Amplitude>1</Amplitude>"
			"<ModIndex>1</ModIndex>"
			"<Phase>0</Phase>"
			"<SamplingRate>44100</SamplingRate>"
			"</processing>"
			"<port_connection>"
			"<out>oscillator.Audio Output</out>"
			"<in>multiplier.First Audio Input</in>"
			"</port_connection>"
			"</network>" 
			);
		
		foo.SetName(name);
		foo.AddFlowControl( new CLAM::BasicFlowControl );
		foo.AddProcessing("oscillator", new CLAM::Oscillator );
		foo.AddProcessing("multiplier", new CLAM::AudioMultiplier );
		foo.ConnectPorts("oscillator.Audio Output", "multiplier.First Audio Input");
		CLAM::XMLStorage::Dump(foo, "network", output );
		std::string outputString(output.str());
		
		CPPUNIT_ASSERT_EQUAL( result, outputString );
	
	}

	void TestLoadNetworkWithControlsConnection()
	{		
		CLAM::Network foo;
		foo.AddFlowControl( new CLAM::BasicFlowControl );

		CLAM::XMLStorage::Restore(foo, mPathToTestData+"networkwithcontrolconnections.xml");
		
		CPPUNIT_ASSERT_EQUAL( true, foo.GetProcessing("panner").GetOutControls().Get("Left Control").IsConnectedTo( 
					      foo.GetProcessing("oscillator").GetInControls().Get("ModIndex")));
		CPPUNIT_ASSERT_EQUAL( true, foo.GetProcessing("panner").GetOutControls().Get("Right Control").IsConnectedTo( 
					      foo.GetProcessing("oscillator").GetInControls().Get("Amplitude")));

	}


	void TestSaveNetworkWithControlsConnection()
	{		
		CLAM::Network foo;
		std::string name("FooNetworkWithControlsConnection");
		std::stringstream output("");
		
		std::string result(
			"<network id=\"FooNetworkWithControlsConnection\">"
			"<processing id=\"oscillator\" type=\"Oscillator\">"
			"<Frequency>440</Frequency>"
			"<Amplitude>1</Amplitude>"
			"<ModIndex>1</ModIndex>"
			"<Phase>0</Phase>"
			"<SamplingRate>44100</SamplingRate>"
			"</processing>"
			"<processing id=\"panner\" type=\"AutoPanner\">"
			"<Frequency>440</Frequency>"
			"<SamplingRate>44100</SamplingRate>"
			"<Phase>0</Phase>"
			"<FrameSize>512</FrameSize>"
			"</processing>"
			"<control_connection>"
			"<out>panner.Left Control</out>"
			"<in>oscillator.Amplitude</in>"
			"</control_connection>"
			"</network>" 
			);
		
		foo.SetName(name);
		foo.AddFlowControl( new CLAM::BasicFlowControl );
		foo.AddProcessing("oscillator", new CLAM::Oscillator );
		foo.AddProcessing("panner", new CLAM::AutoPanner );
		foo.ConnectControls("panner.Left Control","oscillator.Amplitude");
		CLAM::XMLStorage::Dump(foo, "network", output );
		std::string outputString(output.str());
		
		CPPUNIT_ASSERT_EQUAL( result, outputString );

	}
};

} // namespace CLAMTest
