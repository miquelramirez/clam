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

#include "SpectrumConfig.hxx"
#include "SMSSynthesis.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SMSSynthesis",
		"category", "Synthesis",
		"description", "SMSSynthesis",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SMSSynthesis> reg = metadata;
}
	

void SMSSynthesis::AttachChildren()
{
	mSpectralSynthesis.SetParent(this);
	mResSpectralSynthesis.SetParent(this);
	mSinSpectralSynthesis.SetParent(this);
	mSynthSineSpectrum.SetParent(this);
	mPhaseMan.SetParent(this);
	mSpectrumAdder.SetParent(this);
	mOverlapAddSin.SetParent(this);
	mOverlapAddRes.SetParent(this);
	mOverlapAddGlobal.SetParent(this);
}

SMSSynthesis::SMSSynthesis():
mInputSinSpectralPeaks("InputSinPeaks",this),
mInputResSpectrum("InputResSpectrum",this),
mOutputSinSpectrum("OutputSinSpectrum",this),
mOutputSpectrum("OutputSpectrum",this),
mOutputAudio("OutputAudio",this),
mOutputResAudio("OutputResAudio",this),
mOutputSinAudio("OutputSinAudio",this),
mCurrentTimeControl("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
{
	mCurrentFrame = 0;
	Configure(SMSSynthesisConfig());
	AttachChildren();
}

SMSSynthesis::SMSSynthesis(const SMSSynthesisConfig& cfg):
mInputSinSpectralPeaks("InputSinPeaks",this),
mInputResSpectrum("InputResSpectrum",this),
mOutputSinSpectrum("OutputSinSpectrum",this),
mOutputSpectrum("OutputSpectrum",this),
mOutputAudio("OutputAudio",this),
mOutputResAudio("OutputResAudio",this),
mOutputSinAudio("OutputSinAudio",this),
mCurrentTimeControl("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
{
	mCurrentFrame = 0;
	Configure(cfg);
	AttachChildren();
}

SMSSynthesis::~SMSSynthesis()
{
	
}

bool SMSSynthesis::ConfigureChildren()
{
//	mConfig.SetSpectrumSize( mAudioFrame.GetSize()/2+1 );

	//configure global spectral synthesis
	if(!mSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure residual spectral synthesis
	mConfig.GetSpectralSynth().SetResidual(true);
	if(!mResSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

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
	if(!mOverlapAddRes.Configure(mConfig.GetOverlapAddRes()))
		return false;
	if(!mOverlapAddGlobal.Configure(mConfig.GetOverlapAddGlobal()))
		return false;

	return true;
}

void SMSSynthesis::ConfigureData()
{
	mAudioFrame.SetSize(mConfig.GetHopSize()*2);//audio used as input of the overlap and add
	mOutputAudio.SetSize( mAudioFrame.GetSize()/2 );
	mOutputSinAudio.SetSize( mAudioFrame.GetSize()/2 );
	mOutputResAudio.SetSize( mAudioFrame.GetSize()/2 );

	mOutputAudio.SetHop( mConfig.GetHopSize() );
	mOutputSinAudio.SetHop( mConfig.GetHopSize() );
	mOutputResAudio.SetHop( mConfig.GetHopSize() );

	mOutputSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
	mOutputSinSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
}


bool SMSSynthesis::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	ConfigureChildren();
	ConfigureData();
	mCurrentTimeControl.DoControl(-1.0); 
	return true;
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
//	outSpec.SetSize(mConfig.GetSpectrumSize());
	mSynthSineSpectrum.Do(in,outSpec);

	mSinSpectralSynthesis.Do(outSpec,mAudioFrame);
	//Finally the overlap and add is accomplished
	
	return mOverlapAddSin.Do(mAudioFrame, outAudio);

}


bool SMSSynthesis::Do(void)
{
	bool result;
	if (mOutputSinAudio.HasConnections() || mOutputResAudio.HasConnections())
	{
		result =  Do(mInputSinSpectralPeaks.GetData(),mInputResSpectrum.GetData(),
					mOutputSinSpectrum.GetData(),mOutputSpectrum.GetData(),
					mOutputAudio.GetAudio(),mOutputSinAudio.GetAudio(),mOutputResAudio.GetAudio());
	}
	else
	{
		result =  Do(mInputSinSpectralPeaks.GetData(),mInputResSpectrum.GetData(),
					mOutputSinSpectrum.GetData(),mOutputSpectrum.GetData(),
					mOutputAudio.GetAudio());
	}
	
	mInputSinSpectralPeaks.Consume();
	mInputResSpectrum.Consume();

	mOutputSinSpectrum.Produce();
	mOutputSpectrum.Produce();
	mOutputAudio.Produce();
	mOutputSinAudio.Produce();
	mOutputResAudio.Produce();

	return result;
}


bool SMSSynthesis::Do(
		SpectralPeakArray& inputSinusoidalPeaks, 
		Spectrum& inputResidualSpectrum, 
		Audio& outputAudio, 
		Audio& outputSinusoidalAudio, 
		Audio& outputResidualAudio)
{
	Spectrum tmpOutputSinSpec;
	Spectrum tmpOutputSpec;
	// init its size since we'll operate with these spectrums
	tmpOutputSinSpec.SetSize( inputResidualSpectrum.GetSize() );
	tmpOutputSinSpec.SetSize( inputResidualSpectrum.GetSize() );
	
	return Do(inputSinusoidalPeaks,inputResidualSpectrum,tmpOutputSinSpec,tmpOutputSpec,
		outputAudio,outputSinusoidalAudio,outputResidualAudio);

}

bool SMSSynthesis::Do(
		SpectralPeakArray& inputSinusoidalPeaks,
		Spectrum& inputResidualSpectrum,
		Spectrum& outputSinusoidalSpectrum,	//
		Spectrum& outputSpectrum,		//
		Audio& outputAudio, 
		Audio& outputSinusoidalAudio, 
		Audio& outputResidualAudio)
{
	//First we do the phase managing. Note that if the Do(frame) overload is not used,
	//the time and pitch controls in this processing should be set by hand before this
	//method is used


//	mPhaseMan.mCurrentTime.DoControl(mCurrentTimeControl.GetLastValue()); //TODO used in SMSBase (Synth from segment)
	TData currentTime = 0;	
	if (mCurrentTimeControl.GetLastValue() < -0.9)
	{
		int framesize = outputAudio.GetSize();
		TData samplerate = inputResidualSpectrum.GetSpectralRange()*2;
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

	//We synthesize the sinusoidal component 	
	SinusoidalSynthesis(inputSinusoidalPeaks,outputSinusoidalSpectrum,outputSinusoidalAudio);
	
	outputSpectrum.SetSize( inputResidualSpectrum.GetSize() );


	//We add Residual spectrum in the input frame plus the synthesized sinusoidal spectrum
	mSpectrumAdder.Do(outputSinusoidalSpectrum, inputResidualSpectrum, outputSpectrum);
	
	//We synthesize to audio the resulting summed spectrum
	mSpectralSynthesis.Do(outputSpectrum,mAudioFrame);


	//We do the overlap and add
	mOverlapAddGlobal.Do(mAudioFrame, outputAudio);

	//Now we synthesize only the residual spectrum
	mResSpectralSynthesis.Do(inputResidualSpectrum,mAudioFrame);
	//And we do the overlap and add process for the residual
	
	mOverlapAddRes.Do(mAudioFrame, outputResidualAudio);

	/* Note: although sinusoidal spectrum is already available from the analysis phase, we 
	need to store it again in the frame because the original peak array may have been
	transformed
	*/
	return true;
}

bool SMSSynthesis::Do(
		SpectralPeakArray& inputSinusoidalPeaks,
		Spectrum& inputResidualSpectrum,
		Spectrum& outputSinusoidalSpectrum,	//
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
		TData samplerate = inputResidualSpectrum.GetSpectralRange()*2;
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

	// We create a spectrum from the sinusoidal peaks 	
	outputSinusoidalSpectrum.SetSize(mConfig.GetSpectrumSize());
	mSynthSineSpectrum.Do(inputSinusoidalPeaks,outputSinusoidalSpectrum);
	
	outputSpectrum.SetSize( inputResidualSpectrum.GetSize() );

	//We add Residual spectrum in the input frame plus the synthesized sinusoidal spectrum
	mSpectrumAdder.Do(outputSinusoidalSpectrum, inputResidualSpectrum, outputSpectrum);
	
	//We synthesize to audio the resulting summed spectrum
	mSpectralSynthesis.Do(outputSpectrum,mAudioFrame);


	//We do the overlap and add
	mOverlapAddGlobal.Do(mAudioFrame, outputAudio);

	/* Note: although sinusoidal spectrum is already available from the analysis phase, we 
	need to store it again in the frame because the original peak array may have been
	transformed
	*/
	return true;
}

bool SMSSynthesis::Do(Frame& in)
{
	bool isSynthesizeSinusoidsAndResidual = true;
	return Do(in, isSynthesizeSinusoidsAndResidual);
}

bool SMSSynthesis::Do(Frame& in, bool isSynthesizeSinusoidsAndResidual)
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

	if ( isSynthesizeSinusoidsAndResidual )
	{
		return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),in.GetSinusoidalSpec(),in.GetOutSpec(),
					in.GetSynthAudioFrame(),in.GetSinusoidalAudioFrame(),in.GetResidualAudioFrame());
	}
	else
	{
		return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),in.GetSinusoidalSpec(),in.GetOutSpec(),
					in.GetSynthAudioFrame());
	}
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

} // namespace CLAM

