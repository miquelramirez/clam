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

#include "SMSSynthesis.hxx"

using namespace CLAM;




/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SMSSYNTHESIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


void SMSSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SMSSynthesisConfig::DefaultValues()
{
	SetSamplingRate(44100);

	/** Default analysis window size corresponds to 512*/
	GetSpectralSynth().SetAnalWindowSize(513);
	GetSpectralSynth().SetAnalWindowType(EWindowType::eBlackmanHarris92);
	GetSpectralSynth().GetAnalWindowGenerator().SetInvert(true);

	/** WindowSize/2*/
	SetHopSize((GetAnalWindowSize()-1)/2);

}

void SMSSynthesisConfig::SetSpectrumSize(TSize specSize)
{
	GetSynthSineSpectrum().SetSpectrumSize(specSize);
}

TSize SMSSynthesisConfig::GetSpectrumSize() const
{
	return GetSynthSineSpectrum().GetSpectrumSize();
}

/** Synthesis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
void SMSSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSpectralSynth().SetAnalWindowSize(w);
	
	GetSynthSineSpectrum().SetSpectrumSize(GetSpectralSynth().GetIFFT().GetAudioSize()/2+1);
/*	TODO:This condition should be checked!!
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);*/
}

TSize SMSSynthesisConfig::GetAnalWindowSize() const
{
	return GetSpectralSynth().GetAnalWindowSize();
}

/** Analysis Window type*/
void SMSSynthesisConfig::SetAnalWindowType(const EWindowType& t)
{
	GetSpectralSynth().SetAnalWindowType(t);
}

const EWindowType& SMSSynthesisConfig::GetAnalWindowType() const
{
	return GetSpectralSynth().GetAnalWindowType();
}


void SMSSynthesisConfig::SetSynthWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSpectralSynth().SetSynthWindowSize(w);
}

TSize SMSSynthesisConfig::GetSynthWindowSize() const
{
	return GetSpectralSynth().GetSynthWindowSize();
}

/** Synthesis Hop size in miliseconds. Must be < (WindowSize-(1/SR))/2*/	
void SMSSynthesisConfig::SetHopSize(TSize h)
{

	//CLAM_ASSERT(GetSynthWindowSize()>=2*h, "SMSSynthesisConfig::SetHopSize: Hop Size is too large compared to window size");
	GetSpectralSynth().SetHopSize(h);
}

void SMSSynthesisConfig::SetFrameSize(TSize f)
{
	GetSpectralSynth().SetFrameSize(f);
}

TSize SMSSynthesisConfig::GetFrameSize()
{
	return GetSpectralSynth().GetFrameSize();
}


TSize SMSSynthesisConfig::GetHopSize() const
{
	return GetSpectralSynth().GetHopSize();
}

/** Sampling rate of the input audio*/
void SMSSynthesisConfig::SetSamplingRate(TData sr)
{
	GetSynthSineSpectrum().SetSamplingRate(sr);
	GetPhaseMan().SetSamplingRate(sr);
	GetSpectralSynth().SetSamplingRate(sr);
}

TData SMSSynthesisConfig::GetSamplingRate() const
{
	return GetSynthSineSpectrum().GetSamplingRate();
}


TInt32 SMSSynthesisConfig::PowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}




//methods for PO

void SMSSynthesis::AttachChildren()
{
	mPO_SpectralSynthesis.SetParent(this);
	mPO_ResSpectralSynthesis.SetParent(this);
	mPO_SinSpectralSynthesis.SetParent(this);
	mPO_SynthSineSpectrum.SetParent(this);
	mPO_PhaseMan.SetParent(this);
	mPO_SpectrumAdder.SetParent(this);
}

SMSSynthesis::SMSSynthesis() 
{
	Configure(SMSSynthesisConfig());
	AttachChildren();
}

SMSSynthesis::SMSSynthesis(const SMSSynthesisConfig& cfg)
{
	Configure(cfg);
	AttachChildren();
}

SMSSynthesis::~SMSSynthesis()
{
	
}

bool SMSSynthesis::ConfigureChildren()
{
	//configure global spectral synthesis
	if(!mPO_SpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure residual spectral synthesis
	if(!mPO_ResSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure sinusoidal spectral synthesis
	if(!mPO_SinSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//SynthSineSpectrum
	if(!mPO_SynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum()))
		return false;

	//Phase Management
	if(!mPO_PhaseMan.Configure(mConfig.GetPhaseMan()))
		return false;

	mPO_PhaseMan.Init();
	
	//Spectrum Adder
	if(!mPO_SpectrumAdder.Configure(SpecAdder2Config()))
		return false;

	return true;
}

void SMSSynthesis::ConfigureData()
{

}


bool SMSSynthesis::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const SMSSynthesisConfig&>(c);

	//CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

bool SMSSynthesis::Do(void)
{
	return false;
}


bool SMSSynthesis::Do(SpectralPeakArray& in,Audio& out)
{
	SpectrumConfig tmpcfg;
	tmpcfg.SetScale(EScale::eLinear);
	Spectrum tmpSpec(tmpcfg);
	return Do(in,tmpSpec,out);
	
}

/** Sinusoidal synthesis, gives also the output spectrum */
bool SMSSynthesis::Do(SpectralPeakArray& in,Spectrum& outSpec,Audio& outAudio)
{
	outSpec.SetSize(mConfig.GetSpectrumSize());
	
	mPO_SynthSineSpectrum.Do(in,outSpec);
	return mPO_SinSpectralSynthesis.Do(outSpec,outAudio);

}

bool SMSSynthesis::Do(Frame& in)
{
	in.AddSinusoidalAudioFrame();
	in.AddResidualAudioFrame();
	in.AddSynthAudioFrame();
	in.UpdateData();

	in.GetSinusoidalAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetResidualAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetSynthAudioFrame().SetSize(mConfig.GetFrameSize());
		
	if(in.GetCenterTime()<0) return false;//such frames should not be synthesized

	mPO_PhaseMan.Do(in);

	SpectrumConfig tmpcfg;
	tmpcfg.SetScale(EScale::eLinear);
	Spectrum tmpSpec(tmpcfg),tmpSpec2(tmpcfg);
	tmpSpec.SetSize(mConfig.GetSpectrumSize());
	tmpSpec2.SetSize(mConfig.GetSpectrumSize());
	
	tmpSpec.SetSpectralRange(in.GetResidualSpec().GetSpectralRange());
	tmpSpec2.SetSpectralRange(in.GetResidualSpec().GetSpectralRange());
	Do(in.GetSpectralPeakArray(),tmpSpec,in.GetSinusoidalAudioFrame());
	
	mPO_SpectrumAdder.Do(tmpSpec,in.GetResidualSpec(),tmpSpec2);
	mPO_SpectralSynthesis.Do(tmpSpec2,in.GetSynthAudioFrame());

	//Synthesizing residual only component
	return mPO_ResSpectralSynthesis.Do(in.GetResidualSpec(),in.GetResidualAudioFrame());
}

bool SMSSynthesis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}

