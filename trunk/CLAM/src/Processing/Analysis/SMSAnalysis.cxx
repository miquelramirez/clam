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
	AddName();
	AddprSamplingRate();
	AddprFFTSize();
	
	AddSinSpectralAnalysis();
	AddResSpectralAnalysis();
	AddPeakDetect();
	AddFundFreqDetect();
	AddSinTracking();
	AddSynthSineSpectrum();

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
	GetSynthSineSpectrum().SetSpectrumSize(GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
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

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*							SMSANALYSIS 							*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

SMSAnalysis::SMSAnalysis()
{
	AttachChildren();
	Configure(SMSAnalysisConfig());
}

SMSAnalysis::SMSAnalysis(SMSAnalysisConfig& cfg)
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

	// Objects used only for initializing a frame
	// Spectrum used for temporary residual analysis, it may be possible to get rid of
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	mSpec.Configure(scfg);
	
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
 	Scfg.SetSize(mSpec.GetSize());
 	Scfg.SetSpectralRange(22050);
 	Spectrum tmpSpecIn(Scfg);    

	mPO_SpecSubstract.SetPrototypes(mSpec,tmpSpecIn,mSpec);

	
	// Fundamental
	mFund.AddCandidatesFreq();
	mFund.AddCandidatesErr();
	mFund.UpdateData();
	mFund.SetnMaxCandidates(1); // number of candidates wanted
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

bool SMSAnalysis::Do(const Audio& in, Spectrum& outGlobalSpec,SpectralPeakArray& outPk,Fundamental& outFn,Spectrum& outResSpec,Spectrum& outSinSpec)
{
	//Analyzing sinusoidal component
	mPO_SinSpectralAnalysis.Do(in,outGlobalSpec);
	Do(outGlobalSpec,outPk,outFn);

	
	//Analyzing residual component
	//First we synthesize Sinusoidal Spectrum
	mPO_SynthSineSpectrum.Do(outPk,outSinSpec);
	//Then we analyze the spectrum of the whole audio using residual config
	    
	outResSpec.SetSize(mSpec.GetSize());

	mPO_ResSpectralAnalysis.Do(in/*Res*/,mSpec);

	//Finally we substract mSpectrum-SinusoidalSpectrum
	mPO_SpecSubstract.Do(mSpec,outSinSpec,outResSpec);

	return true;

}


bool SMSAnalysis::Do(Spectrum& outSp, SpectralPeakArray& pkArray,Fundamental& outFn)
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


bool SMSAnalysis::Do(Frame& in)
{
	return Do(in.GetAudioFrame(),/*in.GetResidualAudioFrame(),*/in.GetSpectrum(),in.GetSpectralPeakArray(),in.GetFundamental(),in.GetResidualSpec(),in.GetSinusoidalSpec());
}

bool SMSAnalysis::Do(Segment& in)
{
	int frameIndex=in.mCurrentFrameIndex;

	int step=mConfig.GetHopSize();
	int sinFrameSize=mConfig.GetSinSpectralAnalysis().GetWindowSize()-1;
	int resFrameSize=mConfig.GetResSpectralAnalysis().GetWindowSize()-1;

	TData samplingRate=mConfig.GetSamplingRate();
	TSize centerSample=(frameIndex*step);
	TTime centerTime=centerSample/samplingRate;
	
	//Adding a new frame to segment, this frame will have the audiochunk as audioframe
	Frame tmpFrame;
	tmpFrame.SetDuration(step/samplingRate);	
	tmpFrame.SetCenterTime(TData(centerTime));
	tmpFrame.AddAudioFrame();
	tmpFrame.AddResidualAudioFrame();
	tmpFrame.AddSpectrum();
	tmpFrame.AddSpectralPeakArray();
	tmpFrame.AddFundamental();
	tmpFrame.AddSinusoidalSpec();
	tmpFrame.AddResidualSpec();
	tmpFrame.UpdateData();
	Audio tmpAudio,tmpAudio2;
	tmpAudio.SetBeginTime(frameIndex*step);
	tmpAudio2.SetBeginTime(frameIndex*step);

	in.GetAudio().GetAudioChunk(centerSample-step/2,centerSample+step/2,tmpAudio,true);

	tmpFrame.SetAudioFrame(tmpAudio);

	Spectrum tmpSpec;
	tmpSpec.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	tmpFrame.SetSpectrum(tmpSpec);
	tmpFrame.SetFundamental(mFund);
	in.AddFrame(tmpFrame);
	
	return Do(in.GetFrame(frameIndex));
}



