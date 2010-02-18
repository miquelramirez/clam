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

#include "Audio.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "Fundamental.hxx"
#include "SpectrumConfig.hxx"
#include "SMSAnalysis.hxx"

namespace CLAM
{

SMSAnalysis::SMSAnalysis()
	: mAudioProvider( "Audio provider", 0 )
{
	mAudioProvider.ConnectToIn( mCore.GetInPort( "Input Audio" ));
	AttachChildren();
	Configure(SMSAnalysisConfig());
}

SMSAnalysis::SMSAnalysis(SMSAnalysisConfig& cfg)
	: mAudioProvider( "Audio provider", 0 )
{
	AttachChildren();
	Configure(cfg);
}

SMSAnalysis::~SMSAnalysis()
{
}

bool SMSAnalysis::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SMSAnalysis::ConfigureChildren()
{
	mCore.Configure( mConfig );
	return true;

}

void SMSAnalysis::ConfigureData()
{
	mAudioFrameIndex=0;
	
	mAudioProvider.SetSize( mConfig.GetHopSize() );
	mAudioProvider.SetHop( mConfig.GetHopSize() );
}

void SMSAnalysis::AttachChildren()
{
	mCore.SetParent( this );
}

bool SMSAnalysis::ConcreteStart()
{
	//we have to initialize internal counter
	mAudioFrameIndex=0;
	return ProcessingComposite::ConcreteStart();
}

bool SMSAnalysis::Do()
{
	return mCore.Do();
}

bool SMSAnalysis::Do(Frame& in)
{
	InitFrame(in);

	//we set spectrum size and fundamental number of candidates
	in.GetSpectrum().SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	in.GetFundamental().SetnMaxCandidates(1);

	bool result=false;

	if(mAudioProvider.CanProduce())
	{
		
		mAudioProvider.SetSampleRate( in.GetAudioFrame().GetSampleRate() );
		mAudioProvider.GetAudio().GetBuffer() = in.GetAudioFrame().GetBuffer();
		mAudioProvider.Produce();
	}

	if(mCore.CanConsumeAndProduce())
		result = mCore.Do();
	else
		return false;

	OutPortBase & outSinSpectrum  = mCore.GetOutPort("Sinusoidal Branch Spectrum");
	in.GetSinusoidalAnalSpectrum() =  dynamic_cast<OutPortPublisher<Spectrum>&>(outSinSpectrum).GetLastWrittenData();

	OutPortBase & outResSpectrum  = mCore.GetOutPort("Residual Spectrum");
	in.GetResidualSpec() =  dynamic_cast<OutPortPublisher<Spectrum>&>(outResSpectrum).GetLastWrittenData();

	OutPortBase & outSpectrum  = mCore.GetOutPort("Residual Branch Spectrum");
	in.GetSpectrum() =  dynamic_cast<OutPortPublisher<Spectrum>&>(outSpectrum).GetLastWrittenData();

	OutPortBase & fundamental  = mCore.GetOutPort("Fundamental");
	in.GetFundamental() =  dynamic_cast<OutPortPublisher<Fundamental>&>(fundamental).GetLastWrittenData();

	OutPortBase & peaks  = mCore.GetOutPort("Sinusoidal Peaks");
	in.GetSpectralPeakArray() = dynamic_cast<OutPortPublisher<SpectralPeakArray>&>(peaks).GetLastWrittenData();

	// MRJ: Let's check the poscondition...
	CLAM_DEBUG_ASSERT( in.GetResidualSpec().GetSpectralRange() > 0, 
			   "Residual spectrum is not being properly configured" );

	if (result) // TODO: refactor
		//if we have been able to analyze something we set whether frame is voiced or not
		in.SetIsHarmonic(in.GetFundamental().GetFreq(0)>0);
	return result;
}

bool SMSAnalysis::Do(Segment& in)
{
	//first we compute necessary sizes, indices and parameters
	TIndex frameIndex=in.mCurrentFrameIndex;
	int step=mConfig.GetHopSize();
	TData samplingRate=mConfig.GetSamplingRate();
	TTime frameCenterTime=frameIndex*step/samplingRate;
	//Audio center time is different from frame center time. This index corresponds to
	//the audio that is being written into member stream buffer
	TIndex audioCenterSample=(mAudioFrameIndex)*step;
	//Unused variable: TTime audioCenterTime=audioCenterSample/samplingRate;
	
	/**TODO: miliseconds and seconds are inconsistently used in different places?*/
	//If we have reached end of input audio we return false
	if(frameCenterTime>in.GetAudio().GetDuration()*0.001)
		return false;

	tmpFrame.SetDuration(step/samplingRate);
	tmpFrame.SetCenterTime(TData(frameCenterTime));
	tmpFrame.AddAudioFrame();
	tmpFrame.UpdateData();
	tmpFrame.GetAudioFrame().SetBeginTime(((float)frameIndex - 0.5f)*step/samplingRate);
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

void SMSAnalysis::InitFrame(Frame& in)
{
	//We add necessary attributes to input frame
	if ( !in.HasSpectrum() )
		in.AddSpectrum();
	if ( !in.HasSpectralPeakArray() )
		in.AddSpectralPeakArray();
	if ( !in.HasFundamental() )
		in.AddFundamental();
	if ( !in.HasResidualSpec() )
		in.AddResidualSpec();
	if ( !in.HasIsHarmonic() )
		in.AddIsHarmonic();
	if ( !in.HasSinusoidalAnalSpectrum() )
		in.AddSinusoidalAnalSpectrum();

	in.UpdateData();
}

} // namespace CLAM

