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


#include "AudioCompressor.hxx"

namespace CLAM {

	Enum::tEnumValue ECompressionType::sEnumValues[] = {
		{ECompressionType::eExponential,"Exponential"},
		{ECompressionType::eArctangent,"ArcTangent"},
		{0,NULL}
	};

	Enum::tValue ECompressionType::sDefault = ECompressionType::eExponential;

	class ExponentialCompressor {
	public:
		static TData Apply(TData x, TData gain)
		{
			if (x>=0)
				return 1.0 - exp(-gain*x);
			else
				return - (1.0 - exp(gain*x));
		}
	};


	class ArctangentCompressor {
	public:
		static TData Apply(TData x, TData gain)
		{
			return 2.0 * atan(2.0*gain*x) / M_PI;
		}
	};


	void AudioCompressorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSampleRate(44100);
		SetFrameSize(512);
		SetGain(1.0);
		SetCompressionType(ECompressionType::eExponential);
	}


	AudioCompressor::AudioCompressor(const AudioCompressorConfig& c)
		: Input("Input Audio",this,1),
		  Output("Output Audio",this,1)
	{
		Configure(c);
	}

	bool AudioCompressor::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mFrameSize = mConfig.GetFrameSize();

		mGain = mConfig.GetGain();

		mType = mConfig.GetCompressionType();

		Input.SetParams(mConfig.GetFrameSize());
		Output.SetParams(mConfig.GetFrameSize());
		return true;
	}

	bool AudioCompressor::Do()
	{
		bool res = Do(Input.GetData(),
					  Output.GetData());
		Input.LeaveData();
		Output.LeaveData();	  
		return res;
	}


	template<class COMPRESSOR>
	void AudioCompressor::Compress(const Array<TData> &inp, Array<TData> &outp)
	{
		int i;
		for (i=0; i<mFrameSize; i++) {
			outp[i] = COMPRESSOR::Apply(inp[i],mGain);
		}
	}

	bool AudioCompressor::Do(const Audio& inp, Audio& out)
	{
		CLAM_ASSERT(inp.GetSize() == out.GetSize() &&
					inp.GetSize() == mFrameSize,
					"AudioCompressor::Do(...): Size mismatch");
		CLAM_ASSERT(inp.GetSampleRate() == out.GetSampleRate() &&
					inp.GetSampleRate() == mConfig.GetSampleRate(),
					"AudioCompressor::Do(...): sample rate mismatch");

		Array<TData> &inpa     = inp.GetBuffer();
		Array<TData> &outa     = out.GetBuffer();

		switch (mType) {
		case ECompressionType::eExponential:
			Compress<ExponentialCompressor>(inpa,outa);
			break;
		case ECompressionType::eArctangent:
			Compress<ArctangentCompressor>(inpa,outa);
			break;
		default:
			CLAM_ASSERT(0,"AudioCompressor::Do(...): "
			              "Inconsistent compression type");
		}

		return true;
	}

}
