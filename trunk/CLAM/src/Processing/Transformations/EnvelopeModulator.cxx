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

#include "EnvelopeModulator.hxx"


namespace CLAM {

	void EnvModulatorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);
		SetEnvelopeCompression(true);
	}


	EnvelopeModulator::EnvelopeModulator(const EnvModulatorConfig& c)
		: InputEnvelope("Input Envelope",this,1),
		  InputAudio("Input Audio",this,1),
		  Output("Output Audio",this,1)
	{
		Configure(c);
	}

	bool EnvelopeModulator::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mDeltaX = 1000.0/((TTime)mConfig.GetSampleRate());

		mCompress = mConfig.GetEnvelopeCompression();

		InputAudio.SetParams(mConfig.GetFrameSize());
		Output.SetParams(mConfig.GetFrameSize());
		return true;
	}

	TData EnvelopeModulator::Compress(TData x)
	{
		if (x>=0)
			return 1.0 - exp(-x);
		else
			return - (1.0 - exp(x));
	}


	bool EnvelopeModulator::Do()
	{
		bool res = Do(InputEnvelope.GetData(),
					  InputAudio.GetData(),
					  Output.GetData());
		InputEnvelope.LeaveData();
		InputAudio.LeaveData();
		Output.LeaveData();	  
		return res;
	}

	bool EnvelopeModulator::Do(const Envelope& env, const Audio& inp, Audio& out) 
	{
		Array<TData> &inputArray = inp.GetBuffer();
		Array<TData> &outputArray = out.GetBuffer();
		BPFTmpl<TTime,TData> &amplitudeBpf = env.GetAmplitudeBPF();
		TTime pos = 0.0;
		const int size = MIN(inp.GetSize(), out.GetSize());
		if (mCompress)
			for (int i=0;i<size;i++) {
				outputArray[i]=inputArray[i]*amplitudeBpf.GetValue(pos);
				pos += mDeltaX;
			}
		else
			for (int i=0;i<size;i++) {
				outputArray[i]=inputArray[i]*Compress(amplitudeBpf.GetValue(pos));
				pos += mDeltaX;
			}
		return true;
	}

	void EnvelopeModulator::Attach(Envelope& env, Audio& inp, Audio& out)
	{
		InputEnvelope.Attach(env);
		InputAudio.Attach(inp);
		Output.Attach(out);
	}


}
