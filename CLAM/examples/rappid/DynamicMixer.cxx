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
#error You must not compile this file alone. It is included from DynamicMixer.hxx
#endif

#include "mtgsstream.h"

namespace CLAM {

	void DynamicMixerConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);
		SetTransitionTime(1.0);
		SetFadeType(EFadeType::eLinear);
		SetEnabledAtStart(true);
		SetEnableCompression(true);
		SetCompressionType(ECompressionType::eExponential);
	}


	template<unsigned int N>
	DynamicMixer<N>::DynamicMixer(const DynamicMixerConfig& c)
		: Input(N,"Input Audio",this,1),
		  Output("Output Audio",this,1),
		  Enable(N,"EnableInputs",this)
	{
		Configure(c);
		AdoptChildren();
	}

	template<unsigned int N>
	void DynamicMixer<N>::AdoptChildren()
	{
		for (unsigned i=0; i<N; i++)
			mSwitch[i].SetParent(this);
		mMixer.SetParent(this);
		mCompressor.SetParent(this);
	}

	template<unsigned int N>
	void DynamicMixer<N>::ConfigureChildren()
	{
		SoftSwitchConfig scfg;
		scfg.SetFrameSize(mFrameSize);
		scfg.SetSampleRate(mSampleRate);
		scfg.SetFadeType(mConfig.GetFadeType());
		scfg.SetTransitionTime(mConfig.GetTransitionTime());

		for (unsigned i=0; i<N; i++) {
			std::stringstream name;
			name << "SoftSwitch_" << i;
			scfg.SetName(name.str());
			mSwitch[i].Configure(scfg);
		}

		AudioMixerConfig mcfg;
		mcfg.SetName("Mixer");
		mcfg.SetFrameSize(mFrameSize);
		mcfg.SetSampleRate(mSampleRate);

		mMixer.Configure(mcfg);

		AudioCompressorConfig ccfg;
		ccfg.SetName("Compressor");
		ccfg.SetFrameSize(mFrameSize);
		ccfg.SetSampleRate(mSampleRate);
		ccfg.SetCompressionType(mConfig.GetCompressionType());

		mCompressor.Configure(ccfg);
	}

	template<unsigned int N>
	void DynamicMixer<N>::ConfigureData()
	{
		mFrameSize  = mConfig.GetFrameSize();
		mSampleRate = mConfig.GetSampleRate();
		for (unsigned i=0; i<N; i++) {
			mSwitchedAudio[i].SetSize(mFrameSize);
			mSwitchedAudio[i].SetSampleRate(mSampleRate);
		}
		mMixedAudio.SetSize(mFrameSize);
		mMixedAudio.SetSampleRate(mSampleRate);
	}

	template<unsigned int N>
	bool DynamicMixer<N>::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		ConfigureData();

		ConfigureChildren();

		Input.SetParams(mFrameSize);

		Output.SetParams(mFrameSize);

		InitializeControls();

		return true;
	}

	template<unsigned int N>
	bool DynamicMixer<N>::ConcreteStart()
	{
		ProcessingComposite::ConcreteStart();
		InitializeControls();
		return true;
	}

	template<unsigned int N>
	void DynamicMixer<N>::CheckControls(void)
	{
		for (unsigned i=0; i<N; i++) {
			TData val = Enable[i].GetLastValue();
			mSwitch[i].Enable.DoControl(val);
		}
	}

	template<unsigned int N>
	void DynamicMixer<N>::InitializeControls(void)
	{
		TData val = TData(mConfig.GetEnabledAtStart());
		for (unsigned i=0; i<N; i++)
			Enable[i].DoControl(val);
	}

	template<unsigned int N>
	bool DynamicMixer<N>::Do()
	{
		bool res = Do(Input.GetDataArray(),
					  Output.GetData());
		Input.LeaveDataArray();
		Output.LeaveData();	  
		return res;
	}

	template<unsigned int N>
	bool DynamicMixer<N>::Do(const Array<Audio*> &inp, Audio& out)
	{
		CheckControls();

		for (unsigned i=0; i<N; i++)
			mSwitch[i].Do(*inp[i],mSwitchedAudio[i]);

		mMixer.Do(mSwitchedAudio,mMixedAudio);

		mCompressor.Do(mMixedAudio,out);

		return true;
	}

}
