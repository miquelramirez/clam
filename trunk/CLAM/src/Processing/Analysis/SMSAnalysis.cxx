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

#include "SMSAnalysis.hxx"

using namespace CLAM;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SMSANALYSIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



void SMSAnalysisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SMSAnalysisConfig::DefaultValues()
{

	SetprSamplingRate(44100);

	GetSinSpectralAnalysis().SetWindowType(EWindowType::eHamming);
	GetResSpectralAnalysis().SetWindowType(EWindowType::eBlackmanHarris92);

	GetPeakDetect().SetNumBands((GetSinWindowSize()-1)/2+1);

	GetPeakDetect().SetMaxPeaks(50);
	GetPeakDetect().SetMagThreshold(-60);
	
}


void SMSAnalysisConfig::SetSinWindowSize(TSize w)
{
	GetSinSpectralAnalysis().SetWindowSize(w);
	GetPeakDetect().SetNumBands(GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);
}

TSize SMSAnalysisConfig::GetSinWindowSize() const
{
	return GetSinSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetSinWindowType(const EWindowType& t)
{
	GetSinSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetSinWindowType() const
{
	return GetSinSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetSinZeroPadding(int z)
{
	GetSinSpectralAnalysis().SetZeroPadding(z);
	GetPeakDetect().SetNumBands(GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
}

int SMSAnalysisConfig::GetSinZeroPadding() const
{
	return GetSinSpectralAnalysis().GetZeroPadding();
}

void SMSAnalysisConfig::SetHopSize(TSize h)
{
	GetSinSpectralAnalysis().SetHopSize(h);
	GetResSpectralAnalysis().SetHopSize(h);
	TSize w;
	if (GetSinWindowSize()>GetResWindowSize()) w=GetSinWindowSize();
	else w=GetResWindowSize();

 }

TSize SMSAnalysisConfig::GetHopSize() const
{
	return GetSinSpectralAnalysis().GetHopSize();
}

/*****Configuration for residual component analysis*****/

void SMSAnalysisConfig::SetResWindowSize(TSize w)
{
	GetResSpectralAnalysis().SetWindowSize(w);
	GetSynthSineSpectrum().SetSpectrumSize(GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);
}

TSize SMSAnalysisConfig::GetResWindowSize() const
{
	return GetResSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetResWindowType(const EWindowType& t)
{
	GetResSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetResWindowType() const
{
	return GetResSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetResZeroPadding(int z)
{
	GetResSpectralAnalysis().SetZeroPadding(z);
}

int SMSAnalysisConfig::GetResZeroPadding() const
{
	return GetResSpectralAnalysis().GetZeroPadding();
}

/** Sampling rate of the input audio*/
void SMSAnalysisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(sr);
	GetSinSpectralAnalysis().SetSamplingRate(sr);
	GetResSpectralAnalysis().SetSamplingRate(sr);
	GetSynthSineSpectrum().SetSamplingRate(sr);
}

TData SMSAnalysisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SMSAnalysisConfig::PowerOfTwo(TInt32 size)
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

TSize SMSAnalysisConfig::GetInitialOffset() const
{
	TSize largerWindowSize;
	if(GetSinWindowSize()>GetResWindowSize()) largerWindowSize=GetSinWindowSize();
	else largerWindowSize=GetResWindowSize();
			
	return -(largerWindowSize-1)/2+GetHopSize();
}

TSize SMSAnalysisConfig::GetHopsInBiggerWindow() const
{
	if(GetSinWindowSize()>GetResWindowSize()) return (GetSinWindowSize()-1)/GetHopSize();
	else return (GetResWindowSize()-1)/GetHopSize();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*							SMSANALYSIS 							*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

SMSAnalysis::SMSAnalysis():
mInputAudio("InputAudio",this,1),
mOutputSpectrum("OutputSpectrum",this,1),
mOutputSpectralPeaks("OutputSpectralPeaks",this,1),
mOutputFundamental("Fundamental",this,1),
mOutputResSpectrum("OutputResSpectrum",this,1),
mOutputSinSpectrum("OutputSinSpectrum",this,1)
{
	AttachChildren();
	Configure(SMSAnalysisConfig());
}

SMSAnalysis::SMSAnalysis(SMSAnalysisConfig& cfg):
mInputAudio("InputAudio",this,1),
mOutputSpectrum("OutputSpectrum",this,1),
mOutputSpectralPeaks("OutputSpectralPeaks",this,1),
mOutputFundamental("Fundamental",this,1),
mOutputResSpectrum("OutputResSpectrum",this,1),
mOutputSinSpectrum("OutputSinSpectrum",this,1)
{
	AttachChildren();
	Configure(cfg);
}

SMSAnalysis::~SMSAnalysis()
{
}

bool SMSAnalysis::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
{
	mConfig=dynamic_cast<const SMSAnalysisConfig&> (cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SMSAnalysis::ConfigureChildren()
{
	mPO_SinSpectralAnalysis.Configure(mConfig.GetSinSpectralAnalysis());
	mPO_ResSpectralAnalysis.Configure(mConfig.GetResSpectralAnalysis());
	mPO_PeakDetect.Configure(mConfig.GetPeakDetect());
	mPO_FundDetect.Configure(mConfig.GetFundFreqDetect());
	mPO_SinTracking.Configure(mConfig.GetSinTracking());
	mPO_SynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum());
	
	return true;

}

void SMSAnalysis::ConfigureData()
{
	TData samplingRate=mConfig.GetSamplingRate();

	// Spectrum used for temporary residual analysis
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
	mResSpec.Configure(scfg);
	// Spectrum used for temporary sinusoidal analysis
	scfg.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	mSinSpec.Configure(scfg);
	
	/* Now we set prototype of SpectrumSubstracter: we want to substract two spectrums: 
	the first on in MagPhase format, the second in Complex format and get the result back
	in Mag Phase.*/

 	SpectrumConfig Scfg; 
 	SpecTypeFlags sflags;
   	sflags.bComplex = 1;
 		sflags.bPolar = 0;
 		sflags.bMagPhase = 0;
 		sflags.bMagPhaseBPF = 0;
 	Scfg.SetType(sflags);
 	Scfg.SetSize(mResSpec.GetSize());
 	Scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
 	Spectrum tmpSpecIn(Scfg);    

	mPO_SpecSubstract.SetPrototypes(mResSpec,tmpSpecIn,mResSpec);

	
	

	/*Initializing and configuring member circular buffers*/
		
	TSize hopSize=mConfig.GetHopSize();
	TSize sinWindowSize=mConfig.GetSinWindowSize();
	TSize resWindowSize=mConfig.GetResWindowSize();
	
		
	/* Configuring member stream buffers. We have one writer region (size and hop=hopsize) and
	two readers, one for sinusoidal and the other for residual spectral analysis.*/
	mWriter=mStreamBuffer.NewWriter(hopSize,hopSize);
	mSinReader=mStreamBuffer.NewReader(hopSize,sinWindowSize-1);
	mResReader=mStreamBuffer.NewReader(hopSize,resWindowSize-1);
	mStreamBuffer.Configure(sinWindowSize*2);

	//configure internal audio members used for convinience
	mSinAudioFrame.SetSampleRate(mConfig.GetSamplingRate());
	mResAudioFrame.SetSampleRate(mConfig.GetSamplingRate());

	//now we will Attach input and output ports of Spectral Analysis
	mPO_SinSpectralAnalysis.Attach(mSinAudioFrame,mSinSpec);
	mPO_ResSpectralAnalysis.Attach(mResAudioFrame,mResSpec);

	mAudioFrameIndex=0;
}

void SMSAnalysis::AttachChildren()
{
	mPO_SinSpectralAnalysis.SetParent(this);
	mPO_ResSpectralAnalysis.SetParent(this);
	mPO_PeakDetect.SetParent(this);
	mPO_FundDetect.SetParent(this);
	mPO_SinTracking.SetParent(this);
	mPO_SpecSubstract.SetParent(this);
}

void SMSAnalysis::Start()
{
	//we have to initialize internal counter
	mAudioFrameIndex=0;
	ProcessingComposite::Start();
}

void SMSAnalysis::Attach(Audio& inputAudio, Spectrum& outSpectrum,SpectralPeakArray& outPk,
						 Fundamental& outFn,Spectrum& outResSpec,Spectrum& outSinSpec)
{
	mInputAudio.Attach(inputAudio);
	mOutputSpectrum.Attach(outSpectrum);
	mOutputSpectralPeaks.Attach(outPk);
	mOutputFundamental.Attach(outFn);
	mOutputResSpectrum.Attach(outResSpec);
	mOutputSinSpectrum.Attach(outSinSpec);
}

bool SMSAnalysis::Do()
{
	return Do(mInputAudio.GetData(),mOutputSpectrum.GetData(),mOutputSinSpectrum.GetData(),mOutputSpectralPeaks.GetData(),
		mOutputFundamental.GetData(),mOutputResSpectrum.GetData());

}

bool SMSAnalysis::Do(Audio& in, Spectrum& outGlobalSpec,Spectrum& sinGlobalSpec,SpectralPeakArray& outPk,Fundamental& outFn,Spectrum& outResSpec)
{
	/* First we write new samples into stream buffer*/
	Audio tmpAudio;
	mStreamBuffer.GetAndActivate(mWriter,tmpAudio);
	tmpAudio.GetBuffer()=in.GetBuffer();
	mStreamBuffer.LeaveAndAdvance(mWriter);
	
	//Temporal Sinusoidal spectrum used for substracting from the original to compute residual
	//Note: we do not need to keep it here because it will have to be synthesized in the synthesis
	//process anyway.
	Spectrum tmpSpec;
	
	//Synchronizing spectral ranges of other spectrums
	outGlobalSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	outResSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	sinGlobalSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	
	//first we try to get and activate both readers
	if(!mStreamBuffer.GetAndActivate(mSinReader,mSinAudioFrame)||
		!mStreamBuffer.GetAndActivate(mResReader,mResAudioFrame))
	{
		//it means that stream buffer is not ready to be read and needs more input data
		mStreamBuffer.Leave(mSinReader);
		mStreamBuffer.Leave(mResReader);
		return false;
	}


	//we first analysize sinusoidal spectrum
	mPO_SinSpectralAnalysis.Do();

	//we can now leave and advance sinusoidal reader
	mStreamBuffer.LeaveAndAdvance(mSinReader);
	
	//we call auxiliary method to compute sinusoidal peaks and fundamental frequency
	SinusoidalAnalysis(mSinSpec,outPk,outFn);
	
	
	//We are now ready to analyze residual component
	
	//First we synthesize Sinusoidal Spectrum
	mPO_SynthSineSpectrum.Do(outPk,tmpSpec);
	
	//Then we analyze the spectrum of the whole audio using residual config
	 mPO_ResSpectralAnalysis.Do();
	
	//we can now leave residual reader and advance it
	mStreamBuffer.LeaveAndAdvance(mResReader);
	
	//Output global spectrum is that of the residual branch
	outGlobalSpec=mResSpec;
	sinGlobalSpec=mSinSpec;

	//Finally we substract mResSpec-SinusoidalSpectrum
	outResSpec.SetSize(mResSpec.GetSize());
	mPO_SpecSubstract.Do(mResSpec,tmpSpec,outResSpec);

	return true;

}


bool SMSAnalysis::Do(Frame& in)
{
	InitFrame(in);

	//we set spectrum size and fundamental number of candidates
	in.GetSpectrum().SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	in.GetFundamental().SetnMaxCandidates(1);
	

	

	bool result=false;
	//If we have written enough samples as to do the first processing result will be true
	result=Do(in.GetAudioFrame(),in.GetSpectrum(),in.GetSinusoidalAnalSpectrum(),in.GetSpectralPeakArray(),in.GetFundamental(),in.GetResidualSpec());
	if (result)
		//if we have been able to analyze something we set whether frame is voiced or not
		in.SetIsHarmonic(in.GetFundamental().GetFreq(0)>0);
	return result;
}

bool SMSAnalysis::Do(Segment& in)
{
//first we compute necessary sizes, indices and parameters
	int frameIndex=in.mCurrentFrameIndex;
	int step=mConfig.GetHopSize();
	int sinFrameSize=mConfig.GetSinSpectralAnalysis().GetWindowSize()-1;
	int resFrameSize=mConfig.GetResSpectralAnalysis().GetWindowSize()-1;
	TData samplingRate=mConfig.GetSamplingRate();
	TTime frameCenterTime=frameIndex*step/samplingRate;
	//Audio center time is different from frame center time. This index corresponds to
	//the audio that is being written into member stream buffer
	TSize audioCenterSample=(mAudioFrameIndex)*step;
	TTime audioCenterTime=audioCenterSample/samplingRate;
	
	
	/**TODO: miliseconds and seconds are inconsistently used in different places?*/
	//If we have reached end of input audio we return false
	if(frameCenterTime>in.GetAudio().GetDuration()*0.001)
		return false;

	//We instantiate a temporal frame where all analysis will be performed
	Frame tmpFrame;
	tmpFrame.SetDuration(step/samplingRate);
	tmpFrame.SetCenterTime(TData(frameCenterTime));
	tmpFrame.AddAudioFrame();
	tmpFrame.UpdateData();
	tmpFrame.GetAudioFrame().SetBeginTime(frameIndex*step);
	tmpFrame.GetAudioFrame().SetSampleRate(in.GetAudio().GetSampleRate());
	
	/*	Note: Here we are just taking the "new" audio belonging to each frame. That is, the
	HopSize samples centered around CenterTime */
	in.GetAudio().GetAudioChunk(audioCenterSample-step/2,audioCenterSample+step/2,
		tmpFrame.GetAudioFrame(),true);
	
	//we have taken a new audio chunk and must increment internal counter
	mAudioFrameIndex++;
	
	//tmpFrame.SetAudioFrame(tmpAudio);

	bool hasProcessed=Do(tmpFrame);
	if(hasProcessed)
	{//we have been able to do an analysis and write the result into tmpFrame's attributes
		in.mCurrentFrameIndex++;
		in.AddFrame(tmpFrame);
	}

	return true;
}


bool SMSAnalysis::SinusoidalAnalysis(Spectrum& outSp, SpectralPeakArray& pkArray,Fundamental& outFn)
{
	      
	// Convert Spectrum to dB
	outSp.ToDB();
	
	// Peak Detection
	SpectralPeakArray tmpPk;
	mPO_PeakDetect.Do(outSp,tmpPk);

	// If not possible to detect anything with this peak information, FundDetect will return a false
	bool fundFreqFound = mPO_FundDetect.Do(tmpPk,outFn);

	// Sinusoidal Tracking
	mPO_SinTracking.Do(tmpPk,pkArray,outFn.GetFreq(0));
	
	// Convert Spectrum back to linear
	outSp.ToLinear();
	return true;
}

void SMSAnalysis::InitFrame(Frame& in)
{
	//We add necessary attributes to input frame
	in.AddSpectrum();
	in.AddSpectralPeakArray();
	in.AddFundamental();
	in.AddResidualSpec();
	in.AddIsHarmonic();
	in.AddSinusoidalAnalSpectrum();
	in.UpdateData();
}