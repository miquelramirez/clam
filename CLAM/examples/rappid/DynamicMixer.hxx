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

#ifndef _DYNAMIC_MIXER_HXX
#define _DYNAMIC_MIXER_HXX

#include "ProcessingComposite.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include "Audio.hxx"
#include "SoftSwitch.hxx"
#include "AudioMixer.hxx"
#include "AudioCompressor.hxx"

namespace CLAM
{

	class DynamicMixerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (DynamicMixerConfig, 8, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string,      Name);
		DYN_ATTRIBUTE (1, public, TData,            SampleRate);
		DYN_ATTRIBUTE (2, public, int,              FrameSize);
		DYN_ATTRIBUTE (3, public, TTime,            TransitionTime);
		DYN_ATTRIBUTE (4, public, EFadeType,        FadeType);
		DYN_ATTRIBUTE (5, public, bool,             EnabledAtStart);
		DYN_ATTRIBUTE (6, public, bool,             EnableCompression);
		DYN_ATTRIBUTE (7, public, ECompressionType, CompressionType);

	protected:

		inline void DefaultInit(void);
	};

	template<unsigned int N>
	class DynamicMixer: public ProcessingComposite
	{
	public:

		InPortArrayTmpl<Audio> Input;
		OutPortTmpl<Audio>     Output;

		InControlArray     Enable;

		SoftSwitch         mSwitch[N];
		Audio              mSwitchedAudio[N];

		AudioMixer<N>      mMixer;
		Audio              mMixedAudio;

		AudioCompressor    mCompressor;

		unsigned int mFrameSize;
		TData mSampleRate;

	private:

		DynamicMixerConfig  mConfig;

		void InitializeControls();

		void CheckControls();

		void AdoptChildren();

		void ConfigureChildren();

		void ConfigureData();

		bool ConcreteConfigure(const ProcessingConfig& c);

		bool ConcreteStart();

	public:

		DynamicMixer(const DynamicMixerConfig& c = DynamicMixerConfig());

		const char * GetClassName() const {return "DynamicMixer";}

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(const Array<Audio*>& inp, Audio& out);

		void StoreOn(Storage &s) {};
	};
	
}

#include "DynamicMixer.cxx"

#endif
