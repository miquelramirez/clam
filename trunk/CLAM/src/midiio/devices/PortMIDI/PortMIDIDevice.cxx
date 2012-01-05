/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#ifndef __PortMIDIDevice__
#define __PortMIDIDevice__

#include "MIDIManager.hxx"
#include <portmidi.h>
#include <porttime.h>
#include <iostream>

namespace CLAM {

	std::ostream& operator<<( std::ostream& out_str, const PmDeviceInfo* dev_info )
	{
	
		out_str << "Device Name: " << dev_info->name << std::endl;
		out_str << "Interface Name: " << dev_info->interf << std::endl;
		if ( dev_info->output == 1 ) out_str << "Output device" << std::endl ;
		else						 out_str << "Input device" << std::endl ;

		return out_str;
	}

	class PortMIDIDevice: public MIDIDevice
	{
	private:
		std::string mDevice;   
		PmStream *mHandleIn;  
		///// NEW ///////////
		PmStream *mHandleOut;
		///// NEW ///////////
	public:
		PortMIDIDevice(const std::string& name,const std::string& device);

		~PortMIDIDevice();

		void ConcreteStart(void) throw(Err);
		void ConcreteStop(void) throw(Err);
		void Read(void) throw(Err);
		void Write(unsigned char* msg,int size) throw(Err);
		void OpenInput(int dev) throw(Err);
		void OpenOutput(int dev) throw(Err);
	};


	PortMIDIDevice::PortMIDIDevice(const std::string& name,const std::string& device):
			MIDIDevice(name)
	{
		mDevice = device;
		////// NEW /////////
		mHandleIn = NULL;  
		mHandleOut = NULL;
	}

	void PortMIDIDevice::OpenInput(int dev) throw(Err)
	{
		PmError err = Pm_OpenInput( &mHandleIn,  /** data stream */
									dev,                       /** Input device */
									NULL,                      /** device info */
									100,                       /** stream size */
									NULL,
									0 );

		if (err)
		{
			std::string str("Could not open PortMIDIDevice ");
			str += mDevice;
			throw Err(str.c_str());
		}
	}

	void PortMIDIDevice::OpenOutput(int dev) throw(Err)
	{
		PmError err = Pm_OpenOutput(&mHandleOut,
									dev,
									NULL,
									0,
									NULL,
									NULL,
									0);
		if (err)
		{
			std::string str("Could not open PortMIDIDevice ");
			str += mDevice;
			throw Err(str.c_str());
		}
	}

 	void PortMIDIDevice::ConcreteStart(void) throw(Err)
	{
		int dev;
		
		if (mDevice == "default")
		{
			if (mInputs.size())
			{
				OpenInput(Pm_GetDefaultInputDeviceID());
			}
			if (mOutputs.size())
			{
				OpenOutput(Pm_GetDefaultOutputDeviceID());
			}
			return;
		}

		std::string type = mDevice.substr(0,mDevice.find(":",0));
    std::string name = mDevice.substr(mDevice.find(":",0)+1,mDevice.size());

		if (mInputs.size())
		{
			if (type == "input")
			{
				for ( dev = 0; dev < Pm_CountDevices(); dev++) 
				{
					const PmDeviceInfo *info = Pm_GetDeviceInfo(dev);
					if (info->input && name == info->name)
					{
						OpenInput(dev);
						return;
					}
				}
			}
			std::string str("Could not find PortMIDIDevice ");
			str += mDevice;
			throw Err(str.c_str());
		}

		if (mOutputs.size())
		{
			if (type == "output")
			{
				for ( dev = 0; dev < Pm_CountDevices(); dev++) 
				{
					const PmDeviceInfo *info = Pm_GetDeviceInfo(dev);
					if (info->output && name == info->name)
					{
						OpenOutput(dev);
						return;
					}
				}
			}
			std::string str("Could not find PortMIDIDevice ");
			str += mDevice;
			throw Err(str.c_str());
		}
	}

	void PortMIDIDevice::ConcreteStop(void) throw(Err)
	{

		if ( mHandleIn )
		{
			Pm_Close(mHandleIn);
			mHandleIn = 0 ;
		}

		if ( mHandleOut )
		{
			Pm_Close(mHandleOut);
			mHandleOut = 0 ;
		}

	}

	void PortMIDIDevice::Write(unsigned char* msg,int size) throw(Err)
	{
		int m = msg[0]&0xff;

		if(m == 0xf0) // SysEx
		{
#ifdef WIN32
			Pm_WriteSysEx(mHandleOut,0,(char*) msg);
#else
			Pm_WriteSysEx(mHandleOut,0,msg);
#endif

		}
		else
		{
			if(size == 3)
			{
				Pm_WriteShort(mHandleOut,0,Pm_Message(msg[0],msg[1],msg[2]));
			}
			else
			{
				Pm_WriteShort(mHandleOut,0,Pm_Message(msg[0],msg[1],0));
			}
		}
	}

	void PortMIDIDevice::Read(void) throw(Err)
	{
		PmEvent buffer; /** one message at a time */
		unsigned char status, data1, data2;

		/** Is there any midi input to process ? */
	    while (Pm_Poll(mHandleIn) == TRUE) 
		{
			/** Reading a message */
			if (Pm_Read(mHandleIn, &buffer, 1) == pmBufferOverflow) continue;
			/** If an overflow ocurs, data will be lost */

			status = ( unsigned char ) Pm_MessageStatus(buffer.message);

			if ( GetMessageLength( status ) == 3 )
			{
				data1 = ( unsigned char ) Pm_MessageData1(buffer.message);
				data2 = ( unsigned char ) Pm_MessageData2(buffer.message);
				
				HandleRawByte(status);
				HandleRawByte(data1);
				HandleRawByte(data2);
			}
			else
			{
				data1 = ( unsigned char ) Pm_MessageData1(buffer.message);

				HandleRawByte(status);
				HandleRawByte(data1);
			}

		}

	}

	PortMIDIDevice::~PortMIDIDevice()
	{
		Stop();
	}

	class PortMIDIDeviceList: public MIDIDeviceList
	{
		static PortMIDIDeviceList sDevices;

		PortMIDIDeviceList()
			:MIDIDeviceList(std::string("portmidi"))
		{
			//char name[64];
			int dev = 0 ;

			Pm_Initialize() ;

			/** get input and output devices */
			for ( dev = 0; dev < Pm_CountDevices(); dev++) 
			{
				const PmDeviceInfo *info = Pm_GetDeviceInfo(dev);

				std::string name(info->name);

				if (info->input)
					mAvailableDevices.push_back(std::string("input:")+name);

				if (info->output)
					mAvailableDevices.push_back(std::string("output:")+name);
			}

			AddMe();
		}

public:
		~PortMIDIDeviceList()
		{
			Pm_Terminate() ;
		}

		std::string DefaultDevice(void)
		{
			return std::string("default");
		}

		MIDIDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new PortMIDIDevice(name,device);
		}
	};

	PortMIDIDeviceList PortMIDIDeviceList::sDevices;


}

#endif


