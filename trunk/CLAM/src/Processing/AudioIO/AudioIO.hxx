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

#ifndef __AudioIO__
#define __AudioIO__

#include "Audio.hxx"
#include "Processing.hxx"

#ifdef WIN32
//#define DEFAULT_AUDIO_ARCH "directx"
//#define DEFAULT_AUDIO_ARCH "portaudio"
#define DEFAULT_AUDIO_ARCH "rtaudio"
#else
#ifdef linux
#define DEFAULT_AUDIO_ARCH "alsa"
#else
#define DEFAULT_AUDIO_ARCH "rtaudio"
#endif
#endif

namespace CLAM {

/** Configuration of the AudioIn and AudioOut classes. You can specify
 * a device (string), which will be used by the AudioManager, when creating
 * the AudioDevice objects. When you don't specify a concrete device, or
 * specify "default", the AudioManager will choose the default device for your
 * setup. For multichannel Audio, you need to configure each in/out with it's
 * own channel id, typically 0 for left, and 1 for right.
 * @see AudioIn, AudioOut, AudioDevice, AudioManager, ProcessingConfig
*/
	class AudioIOConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioIOConfig, 4, ProcessingConfig);
		/** The Device that will be used by this AudioIn or AudioOut instantiation, if it is not indicated, or its value is "default:default", system will choose the best device for this object*/
		DYN_ATTRIBUTE (0, public, std::string, Device);
		/** Channel attached to object; in case of stereo configuration 0 will be left channel and 1 right channel*/
		DYN_ATTRIBUTE (1, public, int, ChannelID);
		/** FrameSize of the AudioIn or AudioOut audio port */
		DYN_ATTRIBUTE (2, public, int, FrameSize);
		/** Sample Rate of the AudioIn or AudioOut class*/
		DYN_ATTRIBUTE (3, public, int, SampleRate);
	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();

			SetDevice("default:default");
			SetChannelID(0);
			SetSampleRate(0); /* leave it to the device */
			SetFrameSize(512);
		}
	};


}

#endif

