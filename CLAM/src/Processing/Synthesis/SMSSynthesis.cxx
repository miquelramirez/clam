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

	/* Default frame size is 256*/
	SetFrameSize(GetHopSize());


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
	GetOverlapAddSin().SetHopSize(h);
	GetOverlapAddSin().SetBufferSize(GetFrameSize()+h);
	GetOverlapAddRes().SetHopSize(h);
	GetOverlapAddRes().SetBufferSize(GetFrameSize()+h);
	GetOverlapAddGlobal().SetHopSize(h);
	GetOverlapAddGlobal().SetBufferSize(GetFrameSize()+h);
}

void SMSSynthesisConfig::SetFrameSize(TSize f)
{
//	GetSpectralSynth().SetFrameSize(f);
	GetOverlapAddSin().SetFrameSize(f);
	GetOverlapAddSin().SetBufferSize(f+GetHopSize());
	GetOverlapAddRes().SetFrameSize(f);
	GetOverlapAddRes().SetBufferSize(f+GetHopSize());
	GetOverlapAddGlobal().SetFrameSize(f);
	GetOverlapAddGlobal().SetBufferSize(f+GetHopSize());
}

TSize SMSSynthesisConfig::GetFrameSize()
{
	return GetOverlapAddSin().GetFrameSize();
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
	mPO_OverlapAddSin.SetParent(this);
	mPO_OverlapAddRes.SetParent(this);
	mPO_OverlapAddGlobal.SetParent(this);
}

SMSSynthesis::SMSSynthesis():
mInputSinSpectralPeaks("InputSinPeaks",this,1),
mInputResSpectrum("InputResSpectrum",this,1),
mOutputSinSpectrum("OutputSinSpectrum",this,1),
mOutputSpectrum("OutputSpectrum",this,1),
mOutputAudio("OutputAudio",this,1),
mOutputResAudio("OutputResAudio",this,1),
mOutputSinAudio("OutputSinAudio",this,1),
mCurrentTime("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
{
	Configure(SMSSynthesisConfig());
	AttachChildren();
}

SMSSynthesis::SMSSynthesis(const SMSSynthesisConfig& cfg):
mInputSinSpectralPeaks("InputSinPeaks",this,1),
mInputResSpectrum("InputResSpectrum",this,1),
mOutputSinSpectrum("OutputSinSpectrum",this,1),
mOutputSpectrum("OutputSpectrum",this,1),
mOutputAudio("OutputAudio",this,1),
mOutputResAudio("OutputResAudio",this,1),
mOutputSinAudio("OutputSinAudio",this,1),
mCurrentTime("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
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
	mConfig.GetSpectralSynth().SetResidual(true);
	if(!mPO_ResSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure sinusoidal spectral synthesis
	mConfig.GetSpectralSynth().SetResidual(false);
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

	//Overlap and add PO
	if(!mPO_OverlapAddSin.Configure(mConfig.GetOverlapAddSin()))
		return false;
	if(!mPO_OverlapAddRes.Configure(mConfig.GetOverlapAddRes()))
		return false;
	if(!mPO_OverlapAddGlobal.Configure(mConfig.GetOverlapAddGlobal()))
		return false;

	return true;
}

void SMSSynthesis::ConfigureData()
{
		mAudioFrame.SetSize(mConfig.GetHopSize()*2);//audio used as input of the overlap and add
}


bool SMSSynthesis::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	//CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

void SMSSynthesis::Attach(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum,
			Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
			Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio)
{
	mInputSinSpectralPeaks.Attach(inputSinusoidalPeaks);
	mInputResSpectrum.Attach(inputResidualSpectrum);
	mOutputSinSpectrum.Attach(outputSinusoidalSpectrum);
	mOutputSpectrum.Attach(outputSpectrum);
	mOutputAudio.Attach(outputAudio);
	mOutputResAudio.Attach(outputSinusoidalAudio);
	mOutputSinAudio.Attach(outputResidualAudio);
}


bool SMSSynthesis::SinusoidalSynthesis(const SpectralPeakArray& in,Audio& out)
{
	SpectrumConfig tmpcfg;
	tmpcfg.SetScale(EScale::eLinear);
	Spectrum tmpSpec(tmpcfg);
	return SinusoidalSynthesis(in,tmpSpec,out);
	
}

/** Sinusoidal synthesis, gives also the output spectrum */
bool SMSSynthesis::SinusoidalSynthesis(const SpectralPeakArray& in,Spectrum& outSpec,Audio& outAudio)
{
	outSpec.SetSize(mConfig.GetSpectrumSize());
	
	mPO_SynthSineSpectrum.Do(in,outSpec);
	mPO_SinSpectralSynthesis.Do(outSpec,mAudioFrame);
	//Finally the overlap and add is accomplished
	return mPO_OverlapAddSin.Do(mAudioFrame, outAudio);

}


bool SMSSynthesis::Do(void)
{
	return Do(mInputSinSpectralPeaks.GetData(),mInputResSpectrum.GetData(),
		mOutputSinSpectrum.GetData(),mOutputSpectrum.GetData(),
		mOutputAudio.GetData(),mOutputSinAudio.GetData(),mOutputResAudio.GetData());
}


bool SMSSynthesis::Do(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum, 
			Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio)
{
	//This may need to be initialized?
	Spectrum tmpOutputSinSpec;
	Spectrum tmpOutputSpec;
	
	return Do(inputSinusoidalPeaks,inputResidualSpectrum,tmpOutputSinSpec,tmpOutputSpec,
		outputAudio,outputSinusoidalAudio,outputResidualAudio);

}

bool SMSSynthesis::Do(SpectralPeakArray& inputSinusoidalPeaks,Spectrum& inputResidualSpectrum,
		Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
		Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio)
{
	
	//First we do the phase managing. Note that if the Do(frame) overload is not used,
	//the time and pitch controls in this processing should be set by hand before this
	//method is used
	mPO_PhaseMan.mCurrentTime.DoControl(mCurrentTime.GetLastValue());
	mPO_PhaseMan.mCurrentPitch.DoControl(mCurrentPitch.GetLastValue());
	mPO_PhaseMan.Do(inputSinusoidalPeaks);
	
	//We synthesize the sinusoidal component 	
	SinusoidalSynthesis(inputSinusoidalPeaks,outputSinusoidalSpectrum,outputSinusoidalAudio);
	
	//We add Residual spectrum in the input frame plus the synthesized sinusoidal spectrum
	mPO_SpectrumAdder.Do(outputSinusoidalSpectrum,inputResidualSpectrum,outputSpectrum);
	
	//We synthesize to audio the resulting summed spectrum
	mPO_SpectralSynthesis.Do(outputSpectrum,mAudioFrame);
	//We do the overlap and add
	mPO_OverlapAddGlobal.Do(mAudioFrame, outputAudio);


	//Now we synthesize only the residual spectrum
	mPO_ResSpectralSynthesis.Do(inputResidualSpectrum,mAudioFrame);
	//And we do the overlap and add process for the residual
	return mPO_OverlapAddRes.Do(mAudioFrame, outputResidualAudio);

	/* Note: although sinusoidal spectrum is already available from the analysis phase, we 
	need to store it again in the frame because the original peak array may have been
	transformed
	*/

}

bool SMSSynthesis::Do(Frame& in)
{
	if(in.GetCenterTime()<0) return false;//such frames should not be synthesized	
	
//We initialize input frame, adding necessary attributes
	InitFrame(in);
//First we set the controls 
	mCurrentTime.DoControl(in.GetCenterTime());
	mCurrentPitch.DoControl(in.GetFundamental().GetFreq(0));
	

//We make sure that spectrums in input frame has the appropiate size and spectral range.
//Note that the only spectrum we can be sure has the correct spectral range is the residual 
//because it its the only one that has been stored in the analysis process.
	in.GetOutSpec().SetSize(mConfig.GetSpectrumSize());
	in.GetOutSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());
	in.GetSinusoidalSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());

	return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),in.GetSinusoidalSpec(),in.GetOutSpec(),
		in.GetSynthAudioFrame(),in.GetSinusoidalAudioFrame(),in.GetResidualAudioFrame());

}

bool SMSSynthesis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}


void SMSSynthesis::InitFrame(Frame& in)
{
	in.AddOutSpec();
	in.AddSinusoidalSpec();
	in.AddSinusoidalAudioFrame();
	in.AddResidualAudioFrame();
	in.AddSynthAudioFrame();
	in.UpdateData();

	in.GetSinusoidalAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetResidualAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetSynthAudioFrame().SetSize(mConfig.GetFrameSize());
	
}