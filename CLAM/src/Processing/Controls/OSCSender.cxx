#include "OSCSender.hxx"

namespace CLAM
{
	int OSCSender::InputControlCB(TControlData val)
	{
		osc::OutboundPacketStream p( mBuffer, IP_MTU_SIZE);
		p.Clear();
		std::ostringstream header;
		header <<mConf.GetPath();

		p << osc::BeginMessage (header.str().c_str() ) 
			<< val
			<< osc::EndMessage;

		mTransmitPort->Send (p.Data(), p.Size() );
		
		return 0;
	}

	bool OSCSender::ConcreteConfigure(const ProcessingConfig &c)
	{
		CopyAsConcreteConfig(mConf,c);

		if (mTransmitPort)
			delete mTransmitPort;
		
		//Use of InitializeNetworking is actually useless under POSIX because
		// it does nothing -- should revise it if things change just in case
		InitializeNetworking();
		unsigned long hostAddress=GetHostByName(mConf.GetHostName().c_str());
		mTransmitPort=new UdpTransmitPort (hostAddress, mConf.GetPort());
		
	
		return true;
	}
}
