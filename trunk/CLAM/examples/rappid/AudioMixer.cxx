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
#error You must not compile this file alone. It is included from AudioMixer.hxx
#endif


namespace CLAM {

	void AudioMixerConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);
	}


	template<int N>
	AudioMixer<N>::AudioMixer(const AudioMixerConfig& c)
		: Input(N,"Input Audio",this,1),
		  Output("Output Audio",this,1)
	{
		Configure(c);
	}

	template<int N>
	bool AudioMixer<N>::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mFrameSize = mConfig.GetFrameSize();

		Input.SetParams(mFrameSize);

		Output.SetParams(mFrameSize);

		return true;
	}

	template<int N>
	bool AudioMixer<N>::Do()
	{
		bool res = Do(Input.GetDataArray().GetPtr(),
					  Output.GetData());
		Input.LeaveDataArray();
		Output.LeaveData();	  
		return res;
	}

	template<int N>
	void AudioMixer<N>::Do(Array<TData>* in_array[N], Array<TData>& out_array)
	{
		TData sum;

		for (unsigned i=0; i<mFrameSize; i++) {
			sum=0.0;
			for (unsigned a=0; a<N; a++)
				sum += (*in_array[a])[i];
			out_array[i] = sum / TData(N);
		}
	}


	template<int N>
	bool AudioMixer<N>::Do(Audio* inp[3], Audio& out)
	{
		CLAM_ASSERT(inp[0]->GetSize() == int(mFrameSize) &&
					out.GetSize()     == int(mFrameSize),
					"AudioMixer::Do(...): Size mismatch");
		CLAM_ASSERT(inp[0]->GetSampleRate() == mConfig.GetSampleRate() &&
					out.GetSampleRate()     == mConfig.GetSampleRate(),
					"AudioMixer::Do(...): sample rate mismatch");

		Array<TData>* in_array[N];
		Array<TData> &out_array = out.GetBuffer();

		for (unsigned a=0; a<N; a++)
			in_array[a] = &inp[a]->GetBuffer();

		Do(in_array,out_array);
			
		return true;
	}

	template<int N>
	bool AudioMixer<N>::Do(const Audio inp[3], Audio& out)
	{
		CLAM_ASSERT(inp[0].GetSize() == int(mFrameSize) &&
					out.GetSize()     == int(mFrameSize),
					"AudioMixer::Do(...): Size mismatch");
		CLAM_ASSERT(inp[0].GetSampleRate() == mConfig.GetSampleRate() &&
					out.GetSampleRate()     == mConfig.GetSampleRate(),
					"AudioMixer::Do(...): sample rate mismatch");

		Array<TData>* in_array[N];
		Array<TData> &out_array = out.GetBuffer();

		for (unsigned a=0; a<N; a++)
			in_array[a] = &inp[a].GetBuffer();
			
		Do(in_array,out_array);
			
		return true;
	}

}
