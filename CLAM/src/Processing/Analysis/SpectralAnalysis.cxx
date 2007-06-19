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

#include "Segment.hxx"
#include "Frame.hxx"
#include "SpectrumConfig.hxx"
#include "SpectralAnalysis.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail 
{
	static FactoryRegistrator<ProcessingFactory, SpectralAnalysis> regSpectralAnalysis("SpectralAnalysis");
}

SpectralAnalysis::SpectralAnalysis()
	: mInput("Input",this ),
	  mOutput("Output",this )
{
	AttachChildren();
	Configure(SpectralAnalysisConfig());
}

SpectralAnalysis::SpectralAnalysis(SpectralAnalysisConfig& cfg)
	: mInput("Input",this),
   	  mOutput("Output",this)
{
	AttachChildren();
	Configure(cfg);
}

SpectralAnalysis::~SpectralAnalysis()
{
}

bool SpectralAnalysis::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
 
	mConfig.Sync();
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
	
	mInput.SetSize(mConfig.GetWindowSize());
	mInput.SetHop(mConfig.GetHopSize());

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
	mInput.SetSampleRate( samplingRate );
}

void SpectralAnalysis::AttachChildren()
{
	mPO_WinGen.SetParent(this);
	mPO_AProduct.SetParent(this);
	mPO_CShift.SetParent(this);
	mPO_FFT.SetParent(this);
}

bool SpectralAnalysis::Do(void)
{
	mOutput.GetData().SetSize( mConfig.GetFFT().GetAudioSize()/2+1);
	mOutput.GetData().SetSpectralRange( mInput.GetAudio().GetSampleRate()/2);
	
	bool result =  Do(mInput.GetAudio(),mOutput.GetData());

	mInput.Consume();
	mOutput.Produce();

	return result;
}

bool SpectralAnalysis::Do(const Audio& in,Spectrum& outSp)
{

	/* mAudioFrame is used as a helper audio copy where all windowing is done */
	in.GetAudioChunk(0,in.GetSize()-1 ,mAudioFrame,true );

	// TODO: it is wrong
	mAudioFrame.SetSize(mConfig.GetWindowSize()-1);


	/* Zero padding is added to audioframe */
	mAudioFrame.SetSize(mConfig.GetprFFTSize());
	
	/* Windowing function is now applied */
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


} // namespace CLAM

