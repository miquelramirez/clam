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

#include "SinusoidalSynthesis.hxx"

#include "SpectrumConfig.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, SinusoidalSynthesis> regSinusoidalSynthesis("SinusoidalSynthesis");
}
	

void SinusoidalSynthesis::AttachChildren()
{
	mSinSpectralSynthesis.SetParent(this);
	mSynthSineSpectrum.SetParent(this);
	mPhaseMan.SetParent(this);
	mSpectrumAdder.SetParent(this);
	mOverlapAddSin.SetParent(this);
}

SinusoidalSynthesis::SinusoidalSynthesis(const SMSSynthesisConfig& cfg)
	: mInputSinSpectralPeaks("InputSinPeaks",this)
	, mOutputSpectrum("OutputSpectrum",this)
	, mOutputAudio("OutputAudio",this)
	, mCurrentTimeControl("CurrentTime",this)
	, mCurrentPitch("CurrentPitch",this)
{
	mCurrentFrame = 0;
	Configure(cfg);
	AttachChildren();
}

SinusoidalSynthesis::~SinusoidalSynthesis()
{
	
}

bool SinusoidalSynthesis::ConfigureChildren()
{
//	mConfig.SetSpectrumSize( mAudioFrame.GetSize()/2+1 );

	//configure sinusoidal spectral synthesis
	mConfig.GetSpectralSynth().SetResidual(false);
	if(!mSinSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//SynthSineSpectrum
	if(!mSynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum()))
		return false;

	//Phase Management
	if(!mPhaseMan.Configure(mConfig.GetPhaseMan()))
		return false;

	mPhaseMan.Init();
	
	//Spectrum Adder
	if(!mSpectrumAdder.Configure(SpecAdder2Config()))
		return false;

	//Overlap and add PO
	if(!mOverlapAddSin.Configure(mConfig.GetOverlapAddSin()))
		return false;

	return true;
}

void SinusoidalSynthesis::ConfigureData()
{
	mAudioFrame.SetSize(mConfig.GetHopSize()*2);//audio used as input of the overlap and add
	mOutputAudio.SetSize( mAudioFrame.GetSize()/2 );

	mOutputAudio.SetHop( mConfig.GetHopSize() );

	mOutputSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
	mOutputSinSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
}


bool SinusoidalSynthesis::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	ConfigureChildren();
	ConfigureData();
	mCurrentTimeControl.DoControl(-1.0); 
	return true;
}

bool SinusoidalSynthesis::Do(void)
{
	bool result =  Do(mInputSinSpectralPeaks.GetData(),
						mOutputSpectrum.GetData(),
						mOutputAudio.GetAudio());

	mInputSinSpectralPeaks.Consume();

	mOutputSinSpectrum.Produce();
	mOutputSpectrum.Produce();
	mOutputAudio.Produce();

	return result;
}


bool SinusoidalSynthesis::Do(
		SpectralPeakArray& inputSinusoidalPeaks, 
		Audio& outputAudio)
{
	Spectrum tmpOutputSpec;
	// init its size since we'll operate with these spectrums
//	tmpOutputSinSpec.SetSize( inputResidualSpectrum.GetSize() );
	
	return Do(inputSinusoidalPeaks,tmpOutputSpec,outputAudio);

}

bool SinusoidalSynthesis::Do(
		SpectralPeakArray& inputSinusoidalPeaks,
		Spectrum& outputSpectrum,		//
		Audio& outputAudio)
{
	//First we do the phase managing. Note that if the Do(frame) overload is not used,
	//the time and pitch controls in this processing should be set by hand before this
	//method is used


//	mPhaseMan.mCurrentTime.DoControl(mCurrentTimeControl.GetLastValue()); //TODO used in SMSBase (Synth from segment)
	TData currentTime = 0;	
	if (mCurrentTimeControl.GetLastValue() < -0.9)
	{
		int framesize = outputAudio.GetSize();
		//TData samplerate = inputResidualSpectrum.GetSpectralRange()*2;
		TData samplerate = mConfig.GetSynthSineSpectrum().GetSamplingRate();
		currentTime =  TData( mCurrentFrame*framesize ) / samplerate;
	} 
	else 
	{
		currentTime = mCurrentTimeControl.GetLastValue();
	}
	mPhaseMan.mCurrentTime.DoControl( currentTime );
	mCurrentFrame ++;
	
	mPhaseMan.mCurrentPitch.DoControl(mCurrentPitch.GetLastValue());
	mPhaseMan.Do(inputSinusoidalPeaks);

	outputSpectrum.SetSize(mConfig.GetSpectrumSize());
	mSynthSineSpectrum.Do(inputSinusoidalPeaks,outputSpectrum);

	mSinSpectralSynthesis.Do(outputSpectrum, mAudioFrame);
	//Finally the overlap and add is accomplished
	
	mOverlapAddSin.Do(mAudioFrame, outputAudio);

	return true;
}

bool SinusoidalSynthesis::Do(Frame& in)
{
	if(in.GetCenterTime()<0) return false;//such frames should not be synthesized	
	
//We initialize input frame, adding necessary attributes
	InitFrame(in);
//First we set the controls sa	mCurrentTimeControl.DoControl(in.GetCenterTime());
	mCurrentPitch.DoControl(in.GetFundamental().GetFreq(0));
	

//We make sure that spectrums in input frame has the appropiate size and spectral range.
//Note that the only spectrum we can be sure has the correct spectral range is the residual 
//because it its the only one that has been stored in the analysis process.
	in.GetOutSpec().SetSize(mConfig.GetSpectrumSize());
	in.GetOutSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());
	in.GetSinusoidalSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());

	return Do(in.GetSpectralPeakArray(),in.GetOutSpec(),in.GetSynthAudioFrame());

}

bool SinusoidalSynthesis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}


void SinusoidalSynthesis::InitFrame(Frame& in)
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

} // namespace CLAM

