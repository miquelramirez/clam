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

#include "OSCControlDispatcher.hxx"
#include <string>
#include <iostream>

#ifndef WIN32
#include <signal.h>
#endif

namespace CLAM
{
	OSCControlDispatcher::OSCControlDispatcher(Network * network)
		: mParentNetwork(network)
		, mPort(7000)
		, mThread (/*realtime*/false)
		, mReceiveSocket(NULL)
		, mIsListening(false)
	{
		//Init receiver socket
		mReceiveSocket = new UdpListeningReceiveSocket( mPort, this );

		//Init thread
		mThread.SetThreadCode( makeMemberFunctor0( *mReceiveSocket, UdpListeningReceiveSocket, Run ) );
	}
	OSCControlDispatcher::~OSCControlDispatcher()
	{
		Stop();
		if ( mReceiveSocket ) delete mReceiveSocket;
	}
	void OSCControlDispatcher::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
	{
		std::ostringstream log;
		std::string path;
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

			std::string processingname=mParentNetwork->GetProcessingIdentifier(path);
			std::string controlname=mParentNetwork->GetConnectorIdentifier(path);

			if (not mParentNetwork->HasProcessing(processingname))
			{
				log << " - No such processing";
				AddLogMessage(log.str());
				return;
			}

			Processing &p=mParentNetwork->GetProcessing(processingname);
			if (not p.HasInControl(controlname))
			{
				log << " - No such control in processing";
				AddLogMessage(log.str());
				return;
			}
			p.GetInControl(controlname).DoControl(controlvalue);
			log << " - Delivered";
			AddLogMessage(log.str());
		}
		catch( osc::Exception& e )
		{
			
			log << "[RECEIVED] ERROR Parsing: " << path<<" "<<controlvalue << " : " << e.what();
			AddLogMessage(log.str());
		}
	}
	void OSCControlDispatcher::AddLogMessage(const std::string& message)
	{
		mMessageLog.push(message);
	}
	const std::string OSCControlDispatcher::GetLogMessage(void)
	{
		//Maybe it would be interesting to 'ask' how many messages we want in a single call (param)
		std::string message="";
		
		if (mMessageLog.size()>0)
		{
			message=mMessageLog.front();
			mMessageLog.pop();
		}
		return message;
	}

}

