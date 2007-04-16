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

#include "AudioManager.hxx"

#define STRICT
#include "CLAM_windows.h"
#undef GetClassName
#include <dxerr8.h>
#include <dsound.h>
#include <dinput.h>

#include "DXFullDuplex.hxx"
#include "AudioDeviceList.hxx"

namespace CLAM {

	class ErrDirectX:public Err
	{
		public:
			ErrDirectX(char* str,HRESULT hr) throw();
	};

	ErrDirectX::ErrDirectX(char* str,HRESULT hr):Err()
	{
		const char* dxErrStr = DXGetErrorString8(hr);
		mMsg = (char*) malloc(strlen(str)+strlen(dxErrStr)+1);
		strcpy(mMsg,str);
		strcat(mMsg,dxErrStr);
	}

	class DirectXAudioDevice: public AudioDevice
	{
	private:
		int mNChannelsWritten;
		bool mChannelsWritten[256];

		int mNChannelsRead;
		bool mChannelsRead[256];
		
		int mWriteBufSize;
		short mWriteBuf[8192];

		int mReadBufSize;
		short mReadBuf[8192];

		DXFullDuplex* mDXFullDuplex;
		LPGUID mGUID;

	public:
		DirectXAudioDevice(const std::string& name,const LPGUID& guid);
		~DirectXAudioDevice(void);
		void Start(void) throw(Err);
		void Stop(void) throw(Err);
		void Read(Audio& audio,const int channelID);
		void Write(const Audio& audio,const int channelID);

		virtual void GetInfo( AudioDevice::TInfo& );
	};

	DirectXAudioDevice::DirectXAudioDevice(const std::string& name,const LPGUID& guid)
		:AudioDevice(name)
	{
		int i;

		mNChannelsWritten = 0;
		for (i=0;i<256;i++)
		{
			mChannelsWritten[i] = false;
		}

		mNChannelsRead = 0;
		for (i=0;i<256;i++)
		{
			mChannelsRead[i] = false;
		}

		mDXFullDuplex = 0;
		mGUID = guid;
	}


	DirectXAudioDevice::~DirectXAudioDevice(void)
	{
		if (mDXFullDuplex)
		{
			delete mDXFullDuplex;
		}
	}

	void DirectXAudioDevice::Start(void) throw(Err)
	{
		if (mOutputs.size() && mInputs.size())
		{
			if (mOutputs.size() != mInputs.size())
			{
				throw(Err("DirectXAudioDevice::Start(): "
					  "need identical number of inputs and outputs "
					  "for full-duplex operation"));
			}
			mNChannels = mOutputs.size();
			if (mDXFullDuplex==0)
			{
				try {
					mDXFullDuplex = new DXFullDuplex(SampleRate(),mNChannels,Latency(),mGUID);
				}
				catch (ErrDXFullDuplex e) {
					Err ne("DirectXAudioDevice::Start(): Failed to create DirectX device.");
					ne.Embed(e);
					throw(ne);
				}

				mWriteBufSize = mReadBufSize = 0;

				mDXFullDuplex->Start();
			}
		}else{
			throw Err("currently only full-duplex implemented");
		}


	}

	void DirectXAudioDevice::Stop()
	{
		// :TODO: Alert! This is a kludge...
		// mDXFullDuplex->Stop();
	}

	void DirectXAudioDevice::GetInfo( AudioDevice::TInfo &info )
	{
		AudioDevice::GetInfo( info );
	}

	void DirectXAudioDevice::Read(Audio& audio,const int channelID)
	{
		TData* ptrA = audio.GetBuffer().GetPtr();
		short* ptrB = mReadBuf + channelID;
		int n;

		if (mChannelsRead[channelID]) 
			throw Err("DirectXAudioDevice::Read(): Tried to read "
				  "twice from a channel in a single time frame!");
		if (!mDXFullDuplex)
			throw Err("DirectXAudioDevice::Read(): Device not configured.");
		
		if (mReadBufSize==0) mReadBufSize=audio.GetSize();
		else if (mReadBufSize!=audio.GetSize())
			throw Err("Inconsistent Audio size");
		if (mReadBufSize>mDXFullDuplex->mLatency) 
			throw Err("You are trying to read audio in blocks bigger than the latency");

		if (mNChannelsRead == 0)
		{
			mDXFullDuplex->Poll();
			mDXFullDuplex->Read(mReadBuf,mReadBufSize);
		}

		n = mReadBufSize;
		while (n--)
		{
			*ptrA++ = TData(*ptrB) / TData(32767.);
			ptrB += mNChannels;
		}
		
		mChannelsRead[channelID] = true;
		mNChannelsRead++;

		if (mNChannelsRead==mNChannels)
		{
			mNChannelsRead = 0;
			for (int i=0;i<mNChannels;i++)
				mChannelsRead[i] = false;
		}
	}

	void DirectXAudioDevice::Write(const Audio& audio,const int channelID)
	{
		TData* ptrA = audio.GetBuffer().GetPtr();
		short* ptrB = mWriteBuf + channelID;
		int i,n;

		if (mWriteBufSize==0) mWriteBufSize=audio.GetSize();
		else if (mWriteBufSize!=audio.GetSize())
			throw Err("Inconsistent Audio size");
			
		
		if (mChannelsWritten[channelID])
			throw Err("DirectXAudioDevice::Write(): Tried to write "
					  "twice into a channel in a single time frame.");
		if (!mDXFullDuplex)
			throw Err("DirectXAudioDevice::Write(): Device not configured.");
		if (mWriteBufSize>mDXFullDuplex->mLatency) 
			throw Err("You are trying to write audio in blocks bigger than the latency");

		n = mWriteBufSize;
		while (n--)
		{
			*ptrB = (short) (32767.*(*ptrA++));
			ptrB += mNChannels;
		}

		mChannelsWritten[channelID] = true;
		mNChannelsWritten++;

		if (mNChannelsWritten==mNChannels)
		{
			mDXFullDuplex->Write(mWriteBuf,mWriteBufSize);
						
			mNChannelsWritten = 0;
			for (i=0;i<mNChannels;i++)
				mChannelsWritten[i] = false;
		}
	}


	

	class DirectXAudioDeviceList: public AudioDeviceList
	{
			static DirectXAudioDeviceList sDevices;
		private:
			DirectXAudioDeviceList()
				:AudioDeviceList(std::string("directx"))
			{
				HRESULT hr;

				if( FAILED( hr = DirectSoundEnumerate(DSEnumCallback,this ) ) )
					throw(ErrDirectX( TEXT("DirectSoundEnumerate"), hr ));

				AddMe();
			}
		public:
			std::vector<LPGUID> mGuids;

			static BOOL CALLBACK DSEnumCallback(
				LPGUID  lpGuid,    
				LPCSTR  lpcstrDescription,  
				LPCSTR  lpcstrModule,   
				LPVOID  lpContext    
			)
			{
				DirectXAudioDeviceList* list = (DirectXAudioDeviceList*) lpContext;
				list->mAvailableDevices.push_back(lpcstrDescription);
				list->mGuids.push_back(lpGuid);
				return true;
			}

			std::string DefaultDevice(void)
			{
				return mAvailableDevices[0];
			}

			AudioDevice* Create(
				const std::string& name, const std::string& device)
			{
				int i = 0;
				for (i=0;i<mAvailableDevices.size();i++)
				{
					if (device == mAvailableDevices[i])
						return new DirectXAudioDevice(name,mGuids[i]);
				}
				return 0;
			}
	};

	DirectXAudioDeviceList DirectXAudioDeviceList::sDevices;

}

