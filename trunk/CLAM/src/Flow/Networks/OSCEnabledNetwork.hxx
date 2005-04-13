#ifndef _OSC_ENABLED_NETWORK_
#define _OSC_ENABLED_NETWORK_


//CLAM dependencies
#include <Network.hxx>
#include <BasicFlowControl.hxx>


//OSC dependencies
#include <ip/NetworkingUtils.h>
#include <ip/UdpPacketListenerPort.h>
#include <osc/OscReceivedElements.h>

//Other dependencies
#include <queue>
#include <list>
#include <string>



namespace CLAM
{
	class OSCEnabledNetwork : public CLAM::Network
	{
	  //Inner class of OSCEnabledNetwork
		class OscReceivePacketListener : public UdpPacketListener
		{
			void ProcessBundle( const osc::ReceivedBundle& b );
			void ProcessMessage( const osc::ReceivedMessage& m );

			OSCEnabledNetwork* mParentNetwork;
			
		public:
			virtual void ProcessPacket( const char *data, unsigned long size );
			void AttachToNetwork(OSCEnabledNetwork* net);
		};
		
	private:
		/*static*/ UdpPacketListenerPort *mListenerPort;
		OscReceivePacketListener mListener;
		int mPort;
		std::queue<std::string> mMessageLog;
		
		int GetPort(void) const
		{
			return mPort;
		}

	public:
		OSCEnabledNetwork(const int port=7000);

		void Start();
		void Stop();
		//Gets the first message in the log queue
		const std::string GetLogMessage(void);
		void SetPort(const int p)
		{
			mPort=p;
		}

	protected:
		//Adds the specified message to the log queue
		void AddLogMessage(const std::string& message);
};

} // namespace CLAM


#endif // _OSC_ENABLED_NETWORK_
