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
#include "MIDIDeviceList.hxx"
#include "MIDIDevice.hxx"

#include "MIDIEvent.hxx"
#include "MIDITrack.hxx"
#include "MIDISong.hxx"
#include "MIDITempo.hxx"
#include "MIDIReader.hxx"
#include "MIDISongPlayer.hxx"

namespace CLAM {

	class FileMIDIDevice: public MIDIDevice
	{
	private:
		std::string mFilename;
		::MIDI::Song mSong;
		::MIDI::SongPlayer mSongPlayer;
		::MIDI::Tempo mTempo;
		bool  mHavePendingEvent;
		::MIDI::Event mPendingEvent;
		int   mPendingTrackId;
		::MIDI::Milliseconds mPendingTime;
		::MIDI::Milliseconds mCurTime;
		bool mReadDone;
	public:
		FileMIDIDevice(const std::string& name,const std::string& device);
		~FileMIDIDevice();

		void ConcreteStart(void) throw(Err);
		void ConcreteStop(void) throw(Err);

		void Read(void) throw(Err);
		void Write(unsigned char* msg,int size) throw(Err);
		void SetClock(TControlData val);
	};

	FileMIDIDevice::FileMIDIDevice(const std::string& name,const std::string& fname): 
		MIDIDevice(name)
	{
		mFilename = fname;
		mHavePendingEvent = false;
		mReadDone = false;
		mCurTime = 0;
	}

	void FileMIDIDevice::ConcreteStart(void) throw(Err)
	{
		if (!mReadDone)
		{
			
			mReadDone = true;
			::MIDI::Reader reader(mFilename.c_str());
			
			if (!reader.Ok())
			{
					std::string str("Could not open FileMIDIDevice with file ");
					str += mFilename;
					throw Err(str.c_str());
			}
			try{
				reader.Read(mSong);
			}
			catch(::MIDI::Reader::Error err)
			{
				throw Err(err.mStr);
			}
			mSongPlayer.Init(&mSong);
			mTempo.Init(&mSong);

		}
	}

	void FileMIDIDevice::ConcreteStop(void) throw(Err)
	{
	}

	void FileMIDIDevice::SetClock(TControlData val)
	{
		mCurTime = (::MIDI::Milliseconds) val;
	}

	void FileMIDIDevice::Write(unsigned char* msg,int size) throw(Err)
	{
		throw Err("FileMIDIDevice::Write not implemented yet");
	}

	void FileMIDIDevice::Read(void) throw(Err)
	{
		bool flag;
		static int nbytesPerChnMsg[7] =
		{ 3,3,3,3,2,3,3 };
		do
		{
			flag = false;
			if (!mHavePendingEvent)
			{
				mHavePendingEvent = 
					mSongPlayer.GetEvent(mPendingEvent,mPendingTrackId);
				if (mHavePendingEvent)
				{
					mPendingTime = mTempo.TicksToTime(mPendingEvent.GetTicks());
				}else{
				  HandleRawByte(0xF0|int(MIDI::eStop)); // system stop
				}
			}
			if (mHavePendingEvent) {
				if (mCurTime >= mPendingTime)
				{
					if (
						mPendingEvent[0] != 0xF0 && 
						mPendingEvent[0] != 0xF7 && 
						mPendingEvent[0] != 0xFF)
					// skip meta events
					{
						int nbytes = nbytesPerChnMsg[((mPendingEvent[0]&0xF0)>>4)-8];
						if ((mPendingEvent[0]&0xF0)==0x80)
						{
							HandleRawByte((mPendingEvent[0]&0x0F)|0x90);				
							HandleRawByte(mPendingEvent[1]);				
							HandleRawByte(0);										
						}else
						for (int i = 0; i < nbytes; i++)
						{
							HandleRawByte(mPendingEvent[i]);				
						}
					}
					mHavePendingEvent = false;
					flag = true;
				}
			}
		} while (flag);
	}

	FileMIDIDevice::~FileMIDIDevice()
	{
	}

	class FileMIDIDeviceList: public MIDIDeviceList
	{
		static FileMIDIDeviceList sDevices;

		FileMIDIDeviceList()
			:MIDIDeviceList(std::string("file"))
		{
			AddMe();
			mAvailableDevices.push_back("*.mid");
		}
	public:

		std::string DefaultDevice(void)
		{
			return "file:unknown.mid";
		}

		MIDIDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new FileMIDIDevice(name,device);
		}
	};

	FileMIDIDeviceList FileMIDIDeviceList::sDevices;
}







