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

#include "SndPcm.hxx"
#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "AudioDeviceList.hxx"
#include "AudioDevice.hxx"
#include <cstdio>

namespace CLAM {

	class ALSAAudioDevice: public AudioDevice
	{
	private:
		int mNChannelsWritten;
		bool mChannelsWritten[256];

		int mNChannelsRead;
		bool mChannelsRead[256];
		
		int mWriteBufSize;
		Array<short> mWriteBuf;

		int mReadBufSize;
		Array<short> mReadBuf;

		SndPcm* sndpcm;
		std::string mDevice;

		int HighestChannelID();

	public:
		ALSAAudioDevice(const std::string& name,const std::string& device);
		~ALSAAudioDevice();
		
		void Start(void) throw(Err);
		void Stop(void) throw(Err);
		void Read(Audio& audio,const int channelID);
		void Write(const Audio& audio,const int channelID);
	};

	ALSAAudioDevice::ALSAAudioDevice(const std::string& name,const std::string& device): 
		AudioDevice(name)
	{
		//printf("ALSAAudioDevice::ALSAAudioDevice\n");

		int i;
		
		mNChannelsWritten = 0;
		for (i=0;i<256;i++)
			mChannelsWritten[i] = false;

		mNChannelsRead = 0;
		for (i=0;i<256;i++)
			mChannelsRead[i] = false;
			
		sndpcm = 0;
		mDevice = device;
	}

	int ALSAAudioDevice::HighestChannelID(void)
	{
		int max_id = 0;

		std::vector<AudioIn*>::const_iterator in_it;
		for (in_it = mInputs.begin(); in_it != mInputs.end(); in_it++)
		{
			if ( (*in_it)->GetChannelID() > max_id)
				max_id = (*in_it)->GetChannelID();
		}

		std::vector<AudioOut*>::const_iterator out_it;
		for (out_it = mOutputs.begin(); out_it != mOutputs.end(); out_it++)
		{
			if ( (*out_it)->GetChannelID() > max_id)
				max_id = (*out_it)->GetChannelID();
		}

		return max_id;

	}

	void ALSAAudioDevice::Start(void) throw(Err)
	{
		int i;
		bool needs_start = false;

		mNReadChannels = mInputs.size();
		mNWriteChannels = mOutputs.size();
			
		if (mForceNChannels)
		{
			int used_channels = HighestChannelID() + 1;
			if (used_channels > mNChannels)
				throw Err("ALSAAudioDevice::Start(): more inputs or outputs than requested channels.");
				
		}
		else
		{
			if ( HighestChannelID() + 1 != mNChannels)
			{
				if (sndpcm) {
					sndpcm->Stop();
					delete sndpcm;
					sndpcm = 0;
				}
				mNChannels = HighestChannelID() + 1;
			}
		}

		if (sndpcm==0)
		{
			try {
				sndpcm = new ::SndPcm(SampleRate(),mNReadChannels,mNWriteChannels,Latency(),mDevice.c_str(),mDevice.c_str());
			}
			catch (SndPcmError &e) {
				Err ne("ALSAAudioDevice::Start(): Failed to create PCM device.");
				ne.Embed(e);
				throw(ne);
			}
			needs_start = true;
		}

		int bufSize = sndpcm->latency * mNChannels;
		mReadBuf.Resize(bufSize);
		mWriteBuf.Resize(bufSize);
		mReadBuf.SetSize(bufSize);
		mWriteBuf.SetSize(bufSize);

		for (i=0; i<bufSize; i++) {
			mReadBuf[i] = 0;
			mWriteBuf[i] = 0;
		}

		// the following settings will be set at the first Read/Write
		// to the Audio buffer size that is passed.
		mReadBufSize = 0; 
		mWriteBufSize = 0;

		if (needs_start)
			sndpcm->Start();
	}

	void ALSAAudioDevice::Stop(void) throw(Err)
	{
		//printf("ALSAAudioDevice::Stop\n");
		if (sndpcm) {
			sndpcm->Stop();
		}
	}
	ALSAAudioDevice::~ALSAAudioDevice()
	{
		//printf("ALSAAudioDevice::~ALSAAudioDevice\n");
		Stop();
		if (sndpcm) {
			delete sndpcm;
		}
	}

	void ALSAAudioDevice::Read(Audio& audio,const int channelID)
	{
		CLAM_DEBUG_ASSERT(channelID < mNChannels,
		                  "ALSAAudioDevice::Read(): Invalid Channel ID");

		TData* ptrA = audio.GetBuffer().GetPtr();
		short* ptrB = mReadBuf.GetPtr() + channelID;
		int n;

		if (mChannelsRead[channelID]) 
			throw Err("ALSAAudioDevice::Read(): Tried to read "
				  "twice from a channel in a single time frame!");
		if (!sndpcm)
			throw Err("ALSAAudioDevice::Read(): Device not configured.");
		
		if (mReadBufSize==0) mReadBufSize=audio.GetSize();
		else if (mReadBufSize!=audio.GetSize())
			throw Err("Inconsistent Audio size");
		
		if (mReadBufSize>sndpcm->latency) 
			throw Err("You are trying to read audio in blocks bigger than the latency");
		
		if (mNChannelsRead == 0)
		{
			sndpcm->Poll();
			sndpcm->ReadBuf(mReadBuf.GetPtr(),mReadBufSize);
		}

		n = mReadBufSize;
		while (n--)
		{
			*ptrA++ = TData(*ptrB) / 32767.;
			ptrB += mNChannels;
		}
		
		mChannelsRead[channelID] = true;
		mNChannelsRead++;

		if (mNChannelsRead==mNReadChannels)
		{
			mNChannelsRead = 0;
			for (int i=0;i<mNChannels;i++)
				mChannelsRead[i] = false;
		}
	}

	void ALSAAudioDevice::Write(const Audio& audio,const int channelID)
	{
		CLAM_DEBUG_ASSERT(channelID < mNChannels,
		                  "ALSAAudioDevice::Write(): Invalid Channel ID");

		TData* ptrA = audio.GetBuffer().GetPtr();
		short* ptrB = mWriteBuf.GetPtr() + channelID;
		int i,n;

		if (mWriteBufSize==0) mWriteBufSize=audio.GetSize();
		else if (mWriteBufSize!=audio.GetSize())
			throw Err("Inconsistent Audio size");
			
		if (mWriteBufSize>sndpcm->latency) 
			throw Err("You are trying to write audio in blocks bigger than the latency");


		if (mChannelsWritten[channelID])
			throw Err("ALSAAudioDevice::Write(): Tried to write "
					  "twice into a channel in a single time frame.");
		if (!sndpcm)
			throw Err("ALSAAudioDevice::Write(): Device not configured.");

		n = mWriteBufSize;
		while (n--)
		{
			*ptrB = (short) (32767.*(*ptrA++));
			ptrB += mNChannels;
		}

		mChannelsWritten[channelID] = true;
		mNChannelsWritten++;

		if (mNChannelsWritten==mNWriteChannels)
		{
			if (mNReadChannels==0) sndpcm->Poll();
			sndpcm->WriteBuf(mWriteBuf.GetPtr(),mWriteBufSize);
						
			mNChannelsWritten = 0;
			for (i=0;i<mNChannels;i++)
				mChannelsWritten[i] = false;
		}
	}


	class ALSAAudioDeviceList: public AudioDeviceList
	{
		static ALSAAudioDeviceList sDevices;

		ALSAAudioDeviceList()
			:AudioDeviceList(std::string("alsa"))
		{
			char name[64];
			int card, dev;
			snd_ctl_t *handle;
			snd_ctl_card_info_t *info;

			snd_ctl_card_info_alloca(&info);
			card = -1;
			if (snd_card_next(&card) < 0 || card < 0)
				return; // No cards found
			while (card >= 0) {
				snprintf(name, 63,"hw:%d", card);
				if (snd_ctl_open(&handle, name, 0) < 0)
					continue; // Card control open error!
				if (snd_ctl_card_info(handle, info) < 0) {
					snd_ctl_close(handle); // Card control read error!
					continue;
				}
				dev = -1;
				while (1) {
					snd_ctl_pcm_next_device(handle, &dev);
					if (dev < 0)
						break;
					snprintf(name, 63,"hw:%d,%d", card,dev);
					mAvailableDevices.push_back(name);
					snprintf(name, 63,"plughw:%d,%d", card,dev);
					mAvailableDevices.push_back(name);
				}
				snd_ctl_close(handle);
				if (snd_card_next(&card) < 0)
					break;
			}
			
			AddMe();
		}
	public:

		std::string DefaultDevice(void)
		{
			return "plughw:0,0";
		}

		AudioDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new ALSAAudioDevice(name,device);
		}
	};

	ALSAAudioDeviceList ALSAAudioDeviceList::sDevices;
}

