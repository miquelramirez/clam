/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#ifndef __PORTMIDIDevice__
#define __PORTMIDIDevice__

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

	class PORTMIDIINDevice: public MIDIDevice
	{
	private:
		std::string mDevice;   
		PmStream *mHandleIn ;  
	public:
		PORTMIDIINDevice(const std::string& name,const std::string& device);

		~PORTMIDIINDevice();

		void ConcreteStart(void) throw(Err);
		void ConcreteStop(void) throw(Err);
		void Read(void) throw(Err);
	};


	PORTMIDIINDevice::PORTMIDIINDevice(const std::string& name,const std::string& device):
			MIDIDevice(name)
	{
		mDevice = device;
	}

 	void PORTMIDIINDevice::ConcreteStart(void) throw(Err)
	{
		PmDeviceID deviceID = atoi( mDevice.c_str() );

		PmError err = Pm_OpenInput( &mHandleIn,  /** data stream */
                deviceID,                        /** Input device */
                NULL,                            /** device info */
                100,                             /** stream size */
                NULL,
                NULL,
                0 );
		if (err)
		{
			std::string str("Could not open PORTMIDIDevice ");
			str += mDevice;
			throw Err(str.c_str());
		}

	}

	void PORTMIDIINDevice::ConcreteStop(void) throw(Err)
	{

		if ( mHandleIn )
		{
			Pm_Close(mHandleIn);
			mHandleIn = 0 ;
		}
	}

	void PORTMIDIINDevice::Read(void) throw(Err)
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

	PORTMIDIINDevice::~PORTMIDIINDevice()
	{
		Stop();
	}

	class PORTMIDIINDeviceList: public MIDIDeviceList
	{
		static PORTMIDIINDeviceList sDevices;

		PORTMIDIINDeviceList()
			:MIDIDeviceList(std::string("portmidi"))
		{
			char name[64];
			int dev = 0 ;

			Pm_Initialize() ;

			/** Only gets input devices */
			for ( dev = 0; dev < Pm_CountDevices(); dev++) {
				const PmDeviceInfo *info = Pm_GetDeviceInfo(dev) ;

				if ( info->input )
				{
					snprintf(name, 63,"%d",dev); 
					mAvailableDevices.push_back(name);
				}
			}

			AddMe();
		}

public:
		~PORTMIDIINDeviceList()
		{
			Pm_Terminate() ;
		}


		std::string DefaultDevice(void)
		{
			char name[64] ;

			PmDeviceID	id_input = Pm_GetDefaultInputDeviceID() ;
			snprintf( name, 63 , "%d", id_input );

			return name ;
	    }

		MIDIDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new PORTMIDIINDevice(name,device);
		}
	};

	PORTMIDIINDeviceList PORTMIDIINDeviceList::sDevices;


}

#endif
