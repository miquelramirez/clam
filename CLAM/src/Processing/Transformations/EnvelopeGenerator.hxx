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

#ifndef _EnvelopeGenerator_
#define _EnvelopeGenerator_

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "ProcessingData.hxx"
#include "Envelope.hxx"
#include "AudioOutPort.hxx"
#include "InPort.hxx"

namespace CLAM
{


class EnvelopeGeneratorConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (EnvelopeGeneratorConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData,       Duration);
	DYN_ATTRIBUTE (1, public, TData,       SampleRate);
	DYN_ATTRIBUTE (2, public, bool,        FrameEnvelopes);
	DYN_ATTRIBUTE (3, public, int,         FrameSize);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();

		SetDuration(1.0);
		SetSampleRate(44100.0);
		SetFrameEnvelopes(false);
		SetFrameSize(512);
	}
};
	
class EnvelopeGenerator: public Processing
{
private:
	EnvelopeGeneratorConfig mConfig;
	TData mX;
	TData mDX;
	bool mXFrameReset;

	InControlTmpl<EnvelopeGenerator> mEnvelopePos;

	int UpdateEnvelopePosition(TControlData val)
	{
		mX=val;
		return 0;
	}

public:
	EnvelopeGenerator(const EnvelopeGeneratorConfig& c = EnvelopeGeneratorConfig())
		:
		mEnvelopePos("EnvelopePosition",this, &EnvelopeGenerator::UpdateEnvelopePosition),
		Input("Input",this),
		Output("Output",this)
	{
		Configure(c);
	}

	const char * GetClassName() const { return "EnvelopeGenerator";}

	InPort<Envelope> Input;

	AudioOutPort   Output;

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mXFrameReset = mConfig.GetFrameEnvelopes();
		mX = 0;
		mDX = TData(1000.)/(mConfig.GetSampleRate()*mConfig.GetDuration());

		Output.SetParams(mConfig.GetFrameSize());
				
		return true;
	}

	void Attach(Envelope& in,Audio& out)
	{
		Input.Attach(in);
		Output.Attach(out);
	}

	bool Do(void)
	{
		bool res = Do(Input.GetData(),Output.GetData());
		Input.Consume();
		Output.Produce();
		return res;
	}

	bool Do(Envelope& in,Audio& out)
	{
		BPFTmpl<TTime,TData>   &envelope = in.GetAmplitudeBPF();
		DataArray &audio = out.GetBuffer();
		int size = audio.Size();

		for (int i=0;i<size;i++)
		{
			audio[i] = envelope.GetValue(mX);
			mX += mDX;
			if (mX>1000.0) mX=1000.0;
		}
		if (mXFrameReset)
			mX=0.0;
		return true;
	}

};
	
}

#endif


