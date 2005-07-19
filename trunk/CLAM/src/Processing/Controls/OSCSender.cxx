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

#include "OSCSender.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<OSCSender> regtOSCSender( "OSCSender" );
	}
	
	
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
