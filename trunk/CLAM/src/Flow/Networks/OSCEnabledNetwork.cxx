/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "OSCEnabledNetwork.hxx"
#include <string>
#include <iostream>

using namespace std;

//Inner class of the Network the OSC Listener

using namespace osc;

namespace CLAM
{
	OSCEnabledNetwork::OSCEnabledNetwork(const int port)
	{
		// The constructor call is not needed as it is automatically summoned
		//Network::Network();
		
		//Rename the network, as it's OSC enabled!
		SetName("Unnamed OSCEnabledNetwork");
		
		mListener.AttachToNetwork(this);
		SetPort(port);

		mListeningOSC=false;

	}
	
	void OSCEnabledNetwork::StartListeningOSC()
	{
		InitializeNetworking();
		mListenerPort = new UdpPacketListenerPort( GetPort(), &mListener );
		mListeningOSC=true;
	}
	
	void OSCEnabledNetwork::StopListeningOSC()
	{
		delete mListenerPort;
		TerminateNetworking();
		mListeningOSC=false;
	}
	
	const string OSCEnabledNetwork::GetLogMessage(void)
	{
		//Maybe it would be interesting to 'ask' how many messages we want in a single call (param)
		string message="";
		
		if (mMessageLog.size()>0)
		{
			message=mMessageLog.front();
			mMessageLog.pop();
		}
		return message;
	}

	//Adds the specified message to the log queue
	void OSCEnabledNetwork::AddLogMessage(const std::string& message)
	{
		//std::cout << "[LOG]Adding <"<<message<<">\n";
		mMessageLog.push(message);
	}

	void OSCEnabledNetwork::OscReceivePacketListener::ProcessBundle( const osc::ReceivedBundle& b )
	{
		for( ReceivedBundle::const_iterator i = b.ElementsBegin(); i != b.ElementsEnd(); ++i )
		{
			if( i->IsBundle() )
				ProcessBundle( ReceivedBundle(*i) );
			else
				ProcessMessage( ReceivedMessage(*i) );
		}
	}

	void OSCEnabledNetwork::OscReceivePacketListener::ProcessMessage( const osc::ReceivedMessage& m )
	{
		ostringstream log;
		string path;
		float controlvalue=0;

		try {
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
			
			args >> controlvalue >> osc::EndMessage;

			////Message Processing (should be fixed in Network & NetworkEditor soon)
			path=m.AddressPattern();
			path.erase(0,1); //Erase initial slash
			path.replace(path.find_first_of('/'), 1, ".");

			log << "[RECEIVED] "<<path<<" "<<controlvalue;

			string processingname=mParentNetwork->GetProcessingIdentifier(path);
			string controlname=mParentNetwork->GetLastIdentifier(path);

			if (mParentNetwork->HasProcessing(processingname))
			{
				Processing &p=mParentNetwork->GetProcessing(processingname);

				if (p.HasInControl(controlname))
				{
					p.GetInControl(controlname).DoControl(controlvalue);
					log << " - Delivered";
				}
				else
					log << " - No such control in processing";
			}
			else
				log << " - No such processing";

			//std::cout << std::endl<<log.str() << std::endl;			
			mParentNetwork->AddLogMessage(log.str());
			
		}catch( Exception& e ){
			
			log << "[RECEIVED] ERROR Parsing: " << path<<" "<<controlvalue << " : " << e.what();
			mParentNetwork->AddLogMessage(log.str());
			//std::cout << log.str()<< std::endl;
			
		}
	}

	void OSCEnabledNetwork::OscReceivePacketListener::ProcessPacket( const char *data, unsigned long size )
	{
		osc::ReceivedPacket p( data, size );
		if( p.IsBundle() )
			ProcessBundle( ReceivedBundle(p) );
		else
			ProcessMessage( ReceivedMessage(p) );
	} 

	void OSCEnabledNetwork::OscReceivePacketListener::AttachToNetwork(OSCEnabledNetwork* net)
	{
		mParentNetwork=net;
	}
}
