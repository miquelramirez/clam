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

#include "MIDIIn.hxx"
#include <stdio.h>
#include "MIDIDeviceList.hxx"
#include "MIDIDevice.hxx"

namespace CLAM {

	class TextFileMIDIDevice: public MIDIDevice
	{
	private:
		std::string mDevice;
		FILE* mOut;
		FILE* mIn;
		TControlData mClock;
	public:
		TextFileMIDIDevice(const std::string& name,const std::string& device);
		~TextFileMIDIDevice();

		void ConcreteStart(void) throw(Err);
		void ConcreteStop(void) throw(Err);

		void Read(void) throw(Err);
		void Write(unsigned char* msg,int size) throw(Err);

		void SetClock(TControlData val) { mClock = val; }

	};

	TextFileMIDIDevice::TextFileMIDIDevice(const std::string& name,const std::string& device): 
		MIDIDevice(name)
	{
		mDevice = device;
		mOut = 0;
		mIn = 0;
		mClock = 0;
	}

	void TextFileMIDIDevice::ConcreteStart(void) throw(Err)
	{
		if (mOut) fclose(mOut);
		if (mIn) fclose(mIn);

		mIn = 0;
		mOut = 0;
		
		if (mInputs.size() && mOutputs.size() && mDevice!="-")
		{
			throw Err("TextFileMIDIDevice: Cannot use the same file for reading and writing");
		}
		if (mInputs.size())
		{
			if (mDevice == "-")
				mIn = stdin;
			else
				mIn = fopen(mDevice.c_str(),"r");
		}
		if (mOutputs.size())
		{
			if (mDevice == "-")
				mOut = stdout;
			else
				mOut = fopen(mDevice.c_str(),"w");
		}
	}

	void TextFileMIDIDevice::ConcreteStop(void) throw(Err)
	{
		if (mOut) fclose(mOut);
		if (mIn) fclose(mOut);
		mOut = 0;
		mIn = 0;
	}

	void TextFileMIDIDevice::Write(unsigned char* msg,int size) throw(Err)
	{
		printf("TextFileMIDIDevice::Write:");
		printf("%f",mClock);
		for (int i=0;i<size;i++)
		{
			printf(" 0x%02x",msg[i]);
		}
		printf("\n");
		fflush(stdout);
		
		fprintf(mOut,"%f",mClock);
		for (int i=0;i<size;i++)
		{
			fprintf(mOut," 0x%02x",msg[i]);
		}
		fprintf(mOut,"\n");
	}

	void TextFileMIDIDevice::Read(void) throw(Err)
	{
	}

	TextFileMIDIDevice::~TextFileMIDIDevice()
	{
		Stop();
	}

	class TextFileMIDIDeviceList: public MIDIDeviceList
	{
		static TextFileMIDIDeviceList sDevices;

		TextFileMIDIDeviceList()
			:MIDIDeviceList(std::string("textfile"))
		{
			mAvailableDevices.push_back("-");

			AddMe();
		}
	public:

		std::string DefaultDevice(void)
		{
			return "-";
		}

		MIDIDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new TextFileMIDIDevice(name,device);
		}
	};

	TextFileMIDIDeviceList TextFileMIDIDeviceList::sDevices;
}

