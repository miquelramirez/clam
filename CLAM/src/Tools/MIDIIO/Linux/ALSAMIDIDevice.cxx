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

#include "MIDIIn.hxx"
#include <alsa/asoundlib.h>
#include "MIDIDeviceList.hxx"
#include "MIDIDevice.hxx"

namespace CLAM {

	class ALSAMIDIDevice: public MIDIDevice
	{
	private:
		std::string mDevice;
		snd_rawmidi_t *mHandleIn;
		snd_rawmidi_status_t *mStatusIn;
	public:
		ALSAMIDIDevice(const std::string& name,const std::string& device);
		~ALSAMIDIDevice();

		void ConcreteStart(void) throw(Err);
		void ConcreteStop(void) throw(Err);

		void Read(void) throw(Err);
	};

	ALSAMIDIDevice::ALSAMIDIDevice(const std::string& name,const std::string& device): 
		MIDIDevice(name)
	{
		mDevice = device;
	}

	void ALSAMIDIDevice::ConcreteStart(void) throw(Err)
	{
			int err = snd_rawmidi_open(&mHandleIn,NULL,mDevice.c_str(),
									   SND_RAWMIDI_NONBLOCK);
			if (err)
			{
					std::string str("Could not open ALSAMIDIDevice ");
					str += mDevice;
					throw Err(str.c_str());
			}
			snd_rawmidi_status_malloc(&mStatusIn);

	}

	void ALSAMIDIDevice::ConcreteStop(void) throw(Err)
	{
		if (mHandleIn) {
			snd_rawmidi_drain(mHandleIn); 
			snd_rawmidi_close(mHandleIn);
			snd_rawmidi_status_free(mStatusIn);
		}
	}

	void ALSAMIDIDevice::Read(void) throw(Err)
	{
		unsigned char ch;
		size_t n;
		do
		{
			int err = snd_rawmidi_read(mHandleIn,&ch,1);
			if (err!=-EAGAIN)
			{
				HandleRawByte(ch);
			}
			snd_rawmidi_status(mHandleIn,mStatusIn);
                        n = snd_rawmidi_status_get_avail(mStatusIn);
		} while (n);
	}

	ALSAMIDIDevice::~ALSAMIDIDevice()
	{
		Stop();
	}

	class ALSAMIDIDeviceList: public MIDIDeviceList
	{
		static ALSAMIDIDeviceList sDevices;

		ALSAMIDIDeviceList()
			:MIDIDeviceList(std::string("alsa"))
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
					snd_ctl_rawmidi_next_device(handle, &dev);
					if (dev < 0)
						break;
					snprintf(name, 63,"hw:%d,%d", card,dev);
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
			return "hw:0,0";
		}

		MIDIDevice* Create(
			const std::string& name,const std::string& device)
		{
			return new ALSAMIDIDevice(name,device);
		}
	};

	ALSAMIDIDeviceList ALSAMIDIDeviceList::sDevices;
}

