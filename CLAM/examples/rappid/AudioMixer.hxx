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

#ifndef _AUDIO_MIXER_HXX
#define _AUDIO_MIXER_HXX

#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "Audio.hxx"

namespace CLAM
{

	class AudioMixerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioMixerConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TData,       SampleRate);
		DYN_ATTRIBUTE (2, public, int,         FrameSize);

	protected:

		inline void DefaultInit(void);
	};

	template<unsigned int N>
	class AudioMixer: public Processing
	{
	public:

		InPortArrayTmpl<Audio> Input;
		OutPortTmpl<Audio>     Output;

	private:

		AudioMixerConfig  mConfig;

		unsigned int mFrameSize;

		inline bool ConcreteConfigure(const ProcessingConfig& c);

		inline void Do(Array<TData>* inp[N], Array<TData>& out);

	public:

		inline AudioMixer(const AudioMixerConfig& c = AudioMixerConfig());

		inline const ProcessingConfig &GetConfig() const { return mConfig;}

		const char * GetClassName() const {return "AudioMixer";}

		inline bool Do(void);

		inline bool Do(Audio* inp[N], Audio& out);

		inline bool Do(const Audio  inp[N], Audio& out);
	};
	
}

#include "AudioMixer.cxx"

#endif
