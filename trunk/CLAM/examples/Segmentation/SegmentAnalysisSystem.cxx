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
#include "SegmentAnalysisSystem.hxx"

using namespace CLAM;

Flags<2>::tFlagValue SegmentAnalysisSnapshotsFlags::sFlagValues[] = {
	{SegmentAnalysisSnapshotsFlags::eShowAudioFrame, "Complex"},
	{SegmentAnalysisSnapshotsFlags::eShowFrameSpectrum, "Polar"},
	{0,NULL}
}; 

void SegmentAnalysisSystemConfig::Init()
{
	AddName();
	AddAudioFrameSize();
	AddSamplingRate();
	AddDisplayFlags();
	UpdateData();
	//Default values
	SetName("SegmentAnalysisSystemConfig");
	SetAudioFrameSize(1024);
	SetSamplingRate(44100);
	SegmentAnalysisSnapshotsFlags tmpFlags;
	SetDisplayFlags(tmpFlags);
}

SegmentAnalysisSystem::SegmentAnalysisSystem()
{
	AttachChildren();
	Configure(SegmentAnalysisSystemConfig());
}

SegmentAnalysisSystem::SegmentAnalysisSystem(SegmentAnalysisSystemConfig& cfg)
{
	AttachChildren();
	Configure(cfg);
}


bool SegmentAnalysisSystem::ConcreteConfigure(const ProcessingConfig& cfg)
{
	mConfig=dynamic_cast<const SegmentAnalysisSystemConfig&> (cfg);
	ConfigureData();
	ConfigureChildren();
	return true;
}


bool SegmentAnalysisSystem::ConfigureChildren()
{
	//
	//  Processing Objects Configuration


	////////////////////////////////////////////////////////////////
	// FFT
	FFTConfig fcfg;
	fcfg.SetAudioSize(mConfig.GetAudioFrameSize());
	fcfg.SetName("_FFT_");
	mFFT.Configure(fcfg);
	mFFT.SetPrototypes (mAudioFrame, mSpec);

	////////////////////////////////////////////////////////////////
	// PeakDetect
	PeakDetectConfig peakCfg;
	peakCfg.SetName("Peak");
	peakCfg.SetNumBands(mConfig.GetAudioFrameSize()/2+1);
	peakCfg.SetMaxPeaks(100);
	peakCfg.SetMagThreshold(20);
	mPeakDetect.Configure(peakCfg);

	////////////////////////////////////////////////////////////////
	// Fundamental Detection

	FundFreqDetectConfig FundCfg;
	mFundDetect.Configure(FundCfg);

	////////////////////////////////////////////////////////////////
	// GUI objects initialization:

//	Builder*		GUIBuilder = CLAM::Builder::GetInstance("FLTK");
//	mGUI = GUIBuilder->GetGUI();
	return true;

}

void SegmentAnalysisSystem::ConfigureData()
{
	TData samplingRate=mConfig.GetSamplingRate();
		  
	mAudioFrame.SetSize(mConfig.GetAudioFrameSize());

	// Objects used only for initializing a frame
	// Spectrum
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetAudioFrameSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	mSpec.Configure(scfg);
	mSpec.AddDescriptors();
	mSpec.UpdateData();
	SpectralDescriptors spdesc;
	spdesc.AddAll();
	spdesc.UpdateData();
	
	mSpec.SetDescriptors(spdesc);
	  
	// Fundamental

	FundFreqDetectConfig tmpPOcfg;//just for using default values
//	mFund.Configure(fCfg);
	mFund.SetnMaxCandidates( tmpPOcfg.GetnMaxCandidates() );
	mFund.AddCandidatesFreq();
	mFund.AddCandidatesErr();
	mFund.UpdateData();

	//Frame
	mFrame.AddSpectrum();
	mFrame.AddSpectralPeakArray();
	mFrame.AddFundamental();
	mFrame.UpdateData();
	mFrame.SetSpectrum(mSpec);
	mFrame.SetFundamental(mFund);

}

void SegmentAnalysisSystem::AttachChildren()
{
	mFFT.SetParent(this);
	mPeakDetect.SetParent(this);
	mFundDetect.SetParent(this);
}

bool SegmentAnalysisSystem::Do(Segment& in,int frameIndex)
{
	int step=mConfig.GetAudioFrameSize();
	TData samplingRate=mConfig.GetSamplingRate();
	
	TTime centerTime=(frameIndex*step+step/2)/samplingRate;
		
	mFrame.SetDuration ( step / samplingRate );
	mFrame.SetCenterTime( centerTime );
	

	in.AddFrame(mFrame);
	in.GetAudio().GetAudioChunk(frameIndex*step,frameIndex*step+step,mAudioFrame);
//	if(mConfig.GetDisplayFlags().bShowAudioFrame)
//		mGUI->ShowSnapshot( mAudioFrame, " Audio Frame ");

	/////////////////////////////////////////////////////////////////////
	// Processing
	// FFT
	mFFT.Do(mAudioFrame, in.GetFrame(frameIndex).GetSpectrum());
//	if(mConfig.GetDisplayFlags().bShowFrameSpectrum)
//		mGUI->ShowSnapshot( in.GetFrame(frameIndex).GetSpectrum(), " Frame Spectrum" );

	// Energy Computation, note that this should be done inside a PO
	TData aux = 0;
	for(int j=0; j<in.GetFrame(frameIndex).GetSpectrum().GetSize(); j++)
		aux += (in.GetFrame(frameIndex).GetSpectrum().GetMag(j))*(in.GetFrame(frameIndex).GetSpectrum().GetMag(j));
	in.GetFrame(frameIndex).GetSpectrum().GetDescriptors().SetEnergy(aux);

	// Convert Spectrum to dB
	Lin2dB(in.GetFrame(frameIndex).GetSpectrum());

	// Peak Detection
	mPeakDetect.Do(in.GetFrame(frameIndex).GetSpectrum(),in.GetFrame(frameIndex).GetSpectralPeakArray());
	// If not possible to detect anything with this peak information, FundDetect will return a false
	bool fundFreqFound = mFundDetect.Do(in.GetFrame(frameIndex).GetSpectralPeakArray(),in.GetFrame(frameIndex).GetFundamental());
	return true;	



}

// ADHOC function for converting filters from lin to log.
void SegmentAnalysisSystem::Lin2dB(Spectrum& spec)
{
	unsigned int i;
	if (spec.HasMagBuffer())
	{
		DataArray &Mag = spec.GetMagBuffer();
		for (i=0; i<spec.GetSize(); i++)
		{
			// if(Mag[i]==0) Mag[i]=0.0001;
			// Mag[i]= 20*log10(Mag[i]); 
			Mag[i] = DB( Mag[i], 20 );
		}
	}

	if (spec.HasComplexArray())  // WARNING: computational expensive operation
	{
		Array<Complex> &Complex = spec.GetComplexArray();
		for (i=0; i<spec.GetSize(); i++)
		{
			TData re = Complex[i].Real();
			TData im = Complex[i].Imag();
			TData magLin = sqrt(pow(re,2) + pow(im,2));
			if(magLin==0) magLin=0.0001;
			TData magLog = 20*log10(magLin);
			Complex[i].SetReal(magLog * re / magLin);
			Complex[i].SetImag(magLog * im / magLin);
		}
	}
	spec.SetScale(EScale(EScale::eLog));
}
