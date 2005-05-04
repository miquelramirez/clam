#ifndef _OSC_SENDER_HXX_
#define _OSC_SENDER_HXX_

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include <osc/OscOutboundPacketStream.h>
#include <ip/NetworkingUtils.h>
#include <ip/UdpTransmitPort.h>
#include <string>

#define IP_MTU_SIZE 1536

namespace CLAM{

	class OSCSenderConfig : public ProcessingConfig
	{
	public:
	DYNAMIC_TYPE_USING_INTERFACE (OSCSenderConfig,3,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string, HostName);
	DYN_ATTRIBUTE(1,public,int, Port);
	DYN_ATTRIBUTE(2,public,std::string, Path);
	protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetHostName("localhost");
		SetPort(6000);
		SetPath("/processing/control");
	}
	};

	class OSCSender : public Processing
	{
	private:
		OSCSenderConfig mConf;
		char mBuffer[IP_MTU_SIZE];
		UdpTransmitPort *mTransmitPort;
		ExecState mState;
		InControlTmpl<OSCSender> mInput;
		
	public:
		OSCSender() 
		: mInput("input",this,&OSCSender::InputControlCB)
		{
			mTransmitPort=NULL;
		}
		
		OSCSender(const OSCSenderConfig & c)
		: mInput("input",this,&OSCSender::InputControlCB)
		{
			mTransmitPort=NULL;
			ConcreteConfigure(c);
		}
		
		~OSCSender()
		{
			delete mTransmitPort;			
			TerminateNetworking();
		}
		
		bool Do()
		{
			return true;
		}
		
		const char* GetClassName() const { return "OSCSender";}
		
		bool ConcreteConfigure(const ProcessingConfig &c);
		
		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
		
		int InputControlCB (TControlData val);
	};
} //namespace CLAM

#endif
