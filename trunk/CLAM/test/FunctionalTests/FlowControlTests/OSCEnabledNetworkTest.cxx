#include <cppunit/extensions/HelperMacros.h>

#include "OSCEnabledNetwork.hxx"
#include "Processing.hxx"
#include "PushFlowControl.hxx"
#include "PrintControl.hxx"
#include <iostream>
#include <string>

#include <oscpack/osc/OscOutboundPacketStream.h>
#include <oscpack/ip/NetworkingUtils.h>
#include <oscpack/ip/UdpTransmitPort.h>

#define IP_MTU_SIZE 1536

namespace CLAMTest {
	
class OSCEnabledNetworkTest;
CPPUNIT_TEST_SUITE_REGISTRATION( OSCEnabledNetworkTest );
	
class OSCEnabledNetworkTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( OSCEnabledNetworkTest );
	
	CPPUNIT_TEST( testEmptyLog );
	CPPUNIT_TEST( testReceivedPacket_WhenNoSuchProcessing );
	CPPUNIT_TEST( testReceivedPacket_WhenProcessingHasNoSuchControl );
	CPPUNIT_TEST( testReceivedPacket_EverythingOK );
	CPPUNIT_TEST( testReceivedPacket_MalformedPacket );

	CPPUNIT_TEST_SUITE_END();

	char buffer[IP_MTU_SIZE];
	osc::OutboundPacketStream *p;
	UdpTransmitPort *transmitPort;
	CLAM::OSCEnabledNetwork *mNet;
 
public: 

	void setUp()
	{
		InitializeNetworking();
	
		p= new osc::OutboundPacketStream(buffer, IP_MTU_SIZE );
		transmitPort = new UdpTransmitPort( GetHostByName("localhost"), 7000 );
		
		mNet = new CLAM::OSCEnabledNetwork(7000);
		mNet->AddFlowControl(new CLAM::PushFlowControl(512));
		mNet->Start();
	}
	
	void tearDown(void)
	{
		mNet->Stop();
		delete mNet;

		delete transmitPort;
		TerminateNetworking();
		sleep(1);
	}

private:

	//IMPORTANT: if value==0 then this method will send a parameter-free OSC message,
	// basically for testing issues.
	void Send(const std::string& proc, const std::string& control, const float& value)
	{
		p->Clear();
		std::ostringstream header;
		header << "/"<<proc<<"/"<<control;

		if (value==0)
		(*p) << osc::BeginMessage( header.str().c_str() )
			 << osc::EndMessage;
		else
		(*p) << osc::BeginMessage( header.str().c_str() )
			 << value << osc::EndMessage;

		transmitPort->Send( p->Data(), p->Size() );
	}

	void testEmptyLog()
	{
		CPPUNIT_ASSERT_EQUAL ( std::string(""), mNet->GetLogMessage() );
	}
	
	void testReceivedPacket_WhenNoSuchProcessing()
	{
		Send("processing1","input",1983);
		sleep(1);
		CPPUNIT_ASSERT_EQUAL ( std::string("[RECEIVED] processing1.input 1983 - No such processing"), mNet->GetLogMessage() );
	}

	void testReceivedPacket_WhenProcessingHasNoSuchControl()
	{
		CLAM::PrintControlConfig conf;
		conf.AddAll();
		conf.UpdateData();
		conf.SetName("processing1");
		conf.SetMessage("This is a message");
	
		mNet->AddProcessing("processing1",new CLAM::PrintControl(conf));	

		Send("processing1","wronginput",1983);
		sleep(1);
		
		std::string expected("[RECEIVED] processing1.wronginput 1983 - No such control in processing");
		CPPUNIT_ASSERT_EQUAL (expected, mNet->GetLogMessage() );

	}

	void testReceivedPacket_EverythingOK()
	{
		CLAM::PrintControlConfig conf;
		conf.AddAll();
		conf.UpdateData();
		conf.SetName("processing1");
		conf.SetMessage("This is a message");
		mNet->AddProcessing("processing1",new CLAM::PrintControl(conf));	

		Send("processing1","input",1983);
		sleep(1);
		
		std::string expectedLog("[RECEIVED] processing1.input 1983 - Delivered");
		CPPUNIT_ASSERT_EQUAL ( expectedLog, mNet->GetLogMessage() );

		CLAM::TControlData resultCtl = mNet->GetProcessing("processing1").GetInControl("input").GetLastValue();
		CPPUNIT_ASSERT_EQUAL (  CLAM::TControlData(1983), resultCtl );
	}

	//Case regarding packet without control parameter
	void testReceivedPacket_MalformedPacket()
	{
		//Creates special parameter-free osc packet
		Send("processing1","input",0);
		sleep(1);
		std::string expected("[RECEIVED] ERROR Parsing:  0 : missing argument");
		CPPUNIT_ASSERT_EQUAL ( expected, mNet->GetLogMessage() );

	}

};
	
} // namespace 
