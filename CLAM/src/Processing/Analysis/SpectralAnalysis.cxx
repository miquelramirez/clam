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

#include "Segment.hxx"
#include "Frame.hxx"
#include "SpectrumConfig.hxx"
#include "SpectralAnalysis.hxx"

using namespace CLAM;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SpectralANALYSIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////




	
void SpectralAnalysisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SpectralAnalysisConfig::DefaultValues()
{

	SetprSamplingRate(44100);
/** prFFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);

	SetprHopSize(0);//for preventing reading uninitialized memory

	/** Default window size */
	SetWindowSize(513);
	SetWindowType(EWindowType::eHamming);
	
	/** WindowSize/2*/
	SetHopSize((GetWindowSize()-1)/2);

	GetCircularShift().SetAmount(-256);

}


void SpectralAnalysisConfig::SetWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetWindowGenerator().SetSize(w);
	SetprFFTSize(int(PowerOfTwo((w-1)*pow(TData(2),TData(GetZeroPadding())))));
	GetCircularShift().SetAmount(-((w-1)/TData(2))); 
	GetFFT().SetAudioSize(GetprFFTSize());
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);
}

TSize SpectralAnalysisConfig::GetWindowSize() const
{
	return GetWindowGenerator().GetSize();
}

/** Analysis Window type*/
void SpectralAnalysisConfig::SetWindowType(const EWindowType& t)
{
	GetWindowGenerator().SetType(t);
}

const EWindowType& SpectralAnalysisConfig::GetWindowType() const
{
	return GetWindowGenerator().GetType();
}

void SpectralAnalysisConfig::SetFFTSize(int s)
{
	SetprFFTSize(s);
	GetFFT().SetAudioSize(s);
}


/** Zero padding factor*/
void SpectralAnalysisConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	SetprFFTSize(int(PowerOfTwo((GetWindowSize()-1)*pow(TData(2),TData(GetZeroPadding())))));
	GetFFT().SetAudioSize(GetprFFTSize());
}

int SpectralAnalysisConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}



void SpectralAnalysisConfig::SetHopSize(TSize h)
{
	SetprHopSize(h);
}

TSize SpectralAnalysisConfig::GetHopSize() const
{
		return GetprHopSize();
}

/** Sampling rate of the input audio*/
void SpectralAnalysisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));
	/** for sychronizing existing config*/
	SetWindowSize(GetWindowSize());
}

TData SpectralAnalysisConfig::GetSamplingRate() const
{
	return GetprSamplingRate();
}


/* TODO: this functionality should be elsewhere */
TInt32 SpectralAnalysisConfig::PowerOfTwo(TInt32 size)
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
/*							SpectralANALYSIS 							*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

SpectralAnalysis::SpectralAnalysis():mInput("Input",this,1),
		mOutput("Output",this,1)
{
	AttachChildren();
	Configure(SpectralAnalysisConfig());
}

SpectralAnalysis::SpectralAnalysis(SpectralAnalysisConfig& cfg):mInput("Input",this,1),
		mOutput("Output",this,1)
{
	AttachChildren();
	Configure(cfg);
}

SpectralAnalysis::~SpectralAnalysis()
{
}

void SpectralAnalysis::Attach(Audio& in, Spectrum &out)
{
	mInput.Attach(in);
	mOutput.Attach(out);
}

bool SpectralAnalysis::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SpectralAnalysis::ConfigureChildren()
{
	
	mPO_WinGen.Configure(mConfig.GetWindowGenerator());
	mPO_CShift.Configure(mConfig.GetCircularShift());
	mPO_FFT.Configure(mConfig.GetFFT());
	return true;

}

void SpectralAnalysis::ConfigureData()
{
	TData samplingRate=mConfig.GetSamplingRate();
		  
	mAudioFrame.SetSize(mConfig.GetprFFTSize());
	mAudioFrame.SetSampleRate(mConfig.GetSamplingRate());
	mWindow.SetSize(mConfig.GetWindowSize());
	
	/*Window is generated and data is kept in internal member mWindow*/
	mPO_WinGen.Do(mWindow);
	
	/*Leaving out last sample of odd-sized window*/
	mWindow.SetSize(mWindow.GetSize()-1);

	/* Adding zero padding to windowing function */
	mWindow.SetSize(mConfig.GetprFFTSize());

	/* Spectrum used only for initializing a frame */
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetprFFTSize()/2+1); 
	mSpec.Configure(scfg);
	
	
	/*Setting prototypes in the FFT*/
	mPO_FFT.SetPrototypes (mWindow, mSpec);
}

void SpectralAnalysis::AttachChildren()
{
	mPO_WinGen.SetParent(this);
	mPO_AProduct.SetParent(this);
	mPO_CShift.SetParent(this);
	mPO_FFT.SetParent(this);
}

bool SpectralAnalysis::Do(void){return Do(mInput.GetData(),mOutput.GetData());}

bool SpectralAnalysis::Do(const Audio& in,Spectrum& outSp)
{
	/* mAudioFrame is used as a helper audio copy where all windowing is done */
	
	in.GetAudioChunk(0,in.GetSize()-1 ,mAudioFrame,true );

	mAudioFrame.SetSize(mConfig.GetWindowSize()-1);

	/* Zero padding is added to audioframe */
	mAudioFrame.SetSize(mConfig.GetprFFTSize());
	
	/* Windowing funcition is now applied */
	mPO_AProduct.Do(mAudioFrame, mWindow, mAudioFrame);
	
	/* Finally, we do the circular shift */
	mPO_CShift.Do(mAudioFrame,mAudioFrame);

	
	/* and now the FFT can be performed */
	mPO_FFT.Do(mAudioFrame, outSp);

	return true;
}


bool SpectralAnalysis::Do(Frame& in)
{
	return Do(in.GetAudioFrame(),in.GetSpectrum());
}

bool SpectralAnalysis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}
