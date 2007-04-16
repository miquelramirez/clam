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

#ifndef _OSC_SENDER_HXX_
#define _OSC_SENDER_HXX_

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include <oscpack/osc/OscOutboundPacketStream.h>
#include <oscpack/ip/NetworkingUtils.h>
#include <oscpack/ip/UdpSocket.h>
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
			SetPort(7000);
			SetPath("/processing/control");
		}
	};

	class OSCSender : public Processing
	{
	private:
		OSCSenderConfig mConf;
		char mBuffer[IP_MTU_SIZE];
		UdpTransmitSocket *mTransmitSocket;
		ExecState mState;
		InControlTmpl<OSCSender> mInput;
		
	public:
		OSCSender() 
		: mInput("input",this,&OSCSender::InputControlCB)
		{
			mTransmitSocket=NULL;
		}
		
		OSCSender(const OSCSenderConfig & c)
		: mInput("input",this,&OSCSender::InputControlCB)
		{
			mTransmitSocket=NULL;
			ConcreteConfigure(c);
		}
		
		~OSCSender()
		{
			if (mTransmitSocket!=NULL)
				delete mTransmitSocket;			
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

