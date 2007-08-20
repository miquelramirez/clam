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

#include "OSCSource.hxx"
#include <string>
#include <iostream>

#ifndef WIN32
#include <signal.h>
#endif

namespace CLAM
{

OSCSource::OSCSource()
:	mPort(7000),
	mThread (/*realtime*/false),
	mReceiveSocket(NULL),
	mIsListening(false),
	oscValuesMap()
{
	CreateThread();
}

OSCSource::OSCSource(int argPortNumber) :
	mPort(argPortNumber),
	mThread (/*realtime*/false),
	mReceiveSocket(NULL),
	mIsListening(false),
	oscValuesMap()
{
	CreateThread();
}

OSCSource::~OSCSource()
{
	Stop();
	if ( mReceiveSocket ) delete mReceiveSocket;
}

void OSCSource::CreateThread()
{
	//Init receiver socket
	mReceiveSocket = new UdpListeningReceiveSocket( mPort, this );

	//Init thread
	mThread.SetThreadCode( makeMemberFunctor0( *mReceiveSocket, UdpListeningReceiveSocket, Run ) );
}

void OSCSource::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
	std::ostringstream log;
	std::string path;
	float controlvalue=0;

	try {
		osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
		osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

		// find the map entry for the path we have received
		path = m.AddressPattern();

		std::map<std::string,std::vector<float> >::iterator iter = oscValuesMap.find(path);
		if( iter == oscValuesMap.end() )
		{
			oscValuesMap[ path ] = std::vector<float>();
			iter = oscValuesMap.find(path);
		}
		std::vector<float>& valuesForPath = iter->second;
		valuesForPath.clear();

		// read the values for message into the float vector from the map
		bool isOutOfValues;
		log << "[RECEIVED] "<<path;
		while ( (isOutOfValues = args.Eos()) != true)
		{
			float nextValue;
			args >> nextValue;
			log << " " << nextValue;
			valuesForPath.push_back(nextValue);
		}
		args >> osc::EndMessage;

		// record this message in the log
		log << " - Delivered";
		//std::cout << log.str() << std::endl;
		AddLogMessage(log.str());
	}
	catch( osc::Exception& e )
	{

		log << "[RECEIVED] ERROR Parsing: " << path<<" "<<controlvalue << " : " << e.what();
		AddLogMessage(log.str());
	}
}

std::vector<float>* OSCSource::GetLastValuesForTarget(std::string& targetName)
{
	// TODO synchronize access to map
	std::map<std::string,std::vector<float> >::iterator iter = oscValuesMap.find(targetName);

	if( iter == oscValuesMap.end() )
	{
		return NULL;
	}
	else
	{
		std::vector<float>& valuesForPath = iter->second;
		return &valuesForPath;
	}
}

void OSCSource::AddLogMessage(const std::string& message)
{
	mMessageLog.push(message);
}

const std::string OSCSource::GetLogMessage(void)
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

} // end namespace CLAM

