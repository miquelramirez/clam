/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "SpecTypeFlags.hxx"
#include "SynthSineSpectrum.hxx"
#include "BlackHarrisLobe.hxx"

using namespace CLAM;


/******************************************************************/
/************************ SynthSineSpectrum ***********************/



SynthSineSpectrum::SynthSineSpectrum()
	: mInput( "Input", this ),
	  mOutput("Output", this )
{
	Configure(SynthSineSpectrumConfig());
}

SynthSineSpectrum::SynthSineSpectrum(SynthSineSpectrumConfig& cfg)
	: mInput( "Input", this ),
	  mOutput("Output", this )
{
	Configure(cfg);
}


bool SynthSineSpectrum::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	WindowGeneratorConfig wcfg;
	wcfg.SetNormalize(EWindowNormalize::eNone);
	wcfg.SetSize(MAINLOBE_TABLE_SIZE);
	wcfg.SetType(EWindowType::eBlackmanHarris92);
	mWndGen.Configure(wcfg);
	//@TODO: there is some kind of bug in the window generator: when generating the 
	//BlackmannHarris transform it does not yield the same results as the above table!
	//mBlackHarris92TransMainLobe.SetSize(MAINLOBE_TABLE_SIZE);
	//mWndGen.Do(mBlackHarris92TransMainLobe);
	return true;
}


SynthSineSpectrum::~SynthSineSpectrum()
{
}

bool SynthSineSpectrum::Do()
{
	bool result = Do( mInput.GetData(), mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return result;
}


//-------------------------------------------------------------------//
// note : for now we first fill the mSynthsineSpectrum. Then we convert this 
// to a  Spectrum. Its more cpu expensive, but as we dont know which processes
// has to be applied the mSynthSineSpectrum, we use it like this
// COULD BE OPTIMIZED LATER !!!! JO 
bool SynthSineSpectrum::Do(const SpectralPeakArray& peakArray,Spectrum& residualSpectrumOut, double gain)
{
	CLAM_DEBUG_ASSERT( AbleToExecute(), "SynthSineSpectrum::Do - processing is not running" );

	InitSynthSpec(mConfig.GetSpectrumSize());		// could be optimised with memset
	FillSynthSineSpectrum(peakArray,gain);

 	SpectrumConfig Scfg; 
 	Scfg.SetScale(EScale::eLinear);
	SpecTypeFlags sflags;
   	sflags.bComplex = 1;
	sflags.bPolar = 0;
	sflags.bMagPhase = 0;
	sflags.bMagPhaseBPF = 0;
	int spectralSize;
	spectralSize = mSynthSineSpectrum.Size();
	Scfg.SetType(sflags);
 	Scfg.SetSize(spectralSize);
 	Scfg.SetSpectralRange(residualSpectrumOut.GetSpectralRange());
 	residualSpectrumOut.Configure(Scfg);    
	residualSpectrumOut.SetComplexArray(mSynthSineSpectrum); 
	return true;
}

void SynthSineSpectrum::FillSynthSineSpectrum	(	const SpectralPeakArray& peakArray, double gain)
{
	TSize 		mainLobeBins = TSize ( TData(8)*CLAM_pow(2.0,(double)mConfig.GetZeroPadding()) ) ; // Number of bins in the mainlope of a transformed BHarris92 window 
                           
	TIndex 		b,i,k,l,Index,numPeaks = peakArray.GetnPeaks();
  					
	SpectralPeak currPeak;
	double currMag,currBinPos,Sin,Cos,approxMag,fIndex,currFreq,phase;
	Array< Complex >  SpecPeakEnvelope(mainLobeBins);
	SpecPeakEnvelope.SetSize(mainLobeBins);
	DataArray& peakFreqBuffer=peakArray.GetFreqBuffer();
	DataArray& peakMagBuffer=peakArray.GetMagBuffer();
	DataArray& peakPhaseBuffer=peakArray.GetPhaseBuffer();

	TData samplingRate=mConfig.GetSamplingRate();
	TSize spectrumSize=mConfig.GetSpectrumSize();
	TSize zeroPadding=TSize( TData(mConfig.GetZeroPadding()) );

	TData binPosFactor=2*(spectrumSize-1)/samplingRate;
	TData firstBinFactor= 3* CLAM_pow((TData)2,(TData)zeroPadding);

	int incr=int(CLAM_pow(2.0,9.0-mConfig.GetZeroPadding()));
	int lastBin=4096-incr;
	/* loop thru all the peaks ...*/
	for(i=0;i<numPeaks;i++)
	{
		if(peakArray.GetScale()==EScale::eLog)//if it is in dB
		{
			currMag = CLAM_pow(10.0,(peakMagBuffer[i]/20.0));
		}
		else	currMag = peakMagBuffer[i];
		currFreq = peakFreqBuffer[i];
  		phase = peakPhaseBuffer[i];
		Cos = CLAM_cos(phase);		// we assume the phase is constant throughout the frame
		Sin = CLAM_sin(phase);
		// we use the frequency specify position of the peak, not the binpos !
		currBinPos = currFreq*binPosFactor;
    
		double Loc = currBinPos;
		TIndex firstBin = TIndex( int( Loc ) - firstBinFactor );
		double BinRemainder = Loc - floor (Loc);
		fIndex = (1.0 - BinRemainder);
		Index=(int)(0.5 + fIndex*MAINLOBE_TABLE_SIZE/(mainLobeBins));
		
		/* SpecPeakEnvelope holds the 7*(2^ZeroPadding) Bin approximation of a Peak which is  		*/
		/* convoluted with the transform of a BlackmanHarris 92 window Main Lobe  */
		int n;
		int currBin=Index;
		double mainLobe = sBlackHarris92TransMainLobe[currBin];
		for (n=0; currBin<=lastBin;n++)
		{
			approxMag = currMag * mainLobe;

			SpecPeakEnvelope[n].SetReal(TData(approxMag*Cos));
			SpecPeakEnvelope[n].SetImag(TData(approxMag*Sin));
			currBin+=incr;
			mainLobe = sBlackHarris92TransMainLobe[currBin];
		}

		if( (Index+4096-incr) >= MAINLOBE_TABLE_SIZE)
		{
			SpecPeakEnvelope[mainLobeBins-1].SetReal(0);
			SpecPeakEnvelope[mainLobeBins-1].SetImag(0);
		}
		else
		{
			approxMag = currMag * sBlackHarris92TransMainLobe[Index+4096-incr]; 
			SpecPeakEnvelope[mainLobeBins-1].SetReal(TData(approxMag*Cos));
			SpecPeakEnvelope[mainLobeBins-1].SetImag(TData(approxMag*Sin));
		}
    
		/* Add the current Peak to the spektrum and check for aliasing at the limits */
		for (k = 0, l = firstBin; k < mainLobeBins ; k++,l++)
		{
			if (l > 0 && l < mConfig.GetSpectrumSize()-1) // we are inside the spectrum limits
			{
		        	mSynthSineSpectrum[l] += SpecPeakEnvelope[k]; 
			}    
        
			else if (l < 0) // part of peaklobe is below zero frequency, mirror it back 
			{
				b = -l;
				mSynthSineSpectrum[b].SetReal(mSynthSineSpectrum[b].Real()+SpecPeakEnvelope[k].Real());
				mSynthSineSpectrum[b].SetImag(mSynthSineSpectrum[b].Imag()-SpecPeakEnvelope[k].Imag());
			}
        
			else if (l == 0) //  only real part, no phase
			{
				mSynthSineSpectrum[0].SetReal(mSynthSineSpectrum[0].Real()+2*(SpecPeakEnvelope[k].Real()));
			}
        
			else if (l > mConfig.GetSpectrumSize()-1&&(l<mConfig.GetSpectrumSize()*2-1)) //   part of peaklobe is over nyquist frequency, mirror it back 
			{
				b = 2 * (spectrumSize-1) - l;
				mSynthSineSpectrum[b].SetReal(mSynthSineSpectrum[b].Real()+SpecPeakEnvelope[k].Real());
				mSynthSineSpectrum[b].SetImag(mSynthSineSpectrum[b].Imag()-SpecPeakEnvelope[k].Imag());
			}
        
			else if (l == spectrumSize-1) //  only real part, no phase
			{	
				mSynthSineSpectrum[l].SetReal(mSynthSineSpectrum[l].Real()+2*(SpecPeakEnvelope[k].Real()));
			}
		}    
	}
}
//-------------------------------------------------------------------//
void SynthSineSpectrum::InitSynthSpec(TSize size)
{
	TSize i;
	mSynthSineSpectrum.Resize(size);	
	mSynthSineSpectrum.SetSize(size); 
	for(i=0;i<size;i++)
	{
		mSynthSineSpectrum[i] =  Complex(0,0);
	}
}

