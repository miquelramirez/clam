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

#ifndef _SMSAnalysis_
#define _SMSAnalysis_


#include "SpectralPeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SinTracking.hxx"
#include "SMSSynthesis.hxx"//For Sinusoidal Synthesis
#include "SpectrumSubstracter2.hxx"
#include "SpectralAnalysis.hxx"

#include "Flags.hxx"

namespace CLAM {



class SMSAnalysisConfig:public ProcessingConfig
{

	friend class SMSAnalysis;

	DYNAMIC_TYPE_USING_INTERFACE (SMSAnalysisConfig,9,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(1,public,SpectralAnalysisConfig, SinSpectralAnalysis);
	DYN_ATTRIBUTE(2,public,SpectralAnalysisConfig, ResSpectralAnalysis);

	DYN_ATTRIBUTE(3,public,SpectralPeakDetectConfig, PeakDetect);
	DYN_ATTRIBUTE(4,public,FundFreqDetectConfig, FundFreqDetect);
	DYN_ATTRIBUTE(5,public,SinTrackingConfig, SinTracking);
/** Flags that are used in order to decide what debug display is shown*/
	
	DYN_ATTRIBUTE(6,protected,int, prSamplingRate);
	DYN_ATTRIBUTE(7,protected,int, prFFTSize);
	DYN_ATTRIBUTE(8,public, SynthSineSpectrumConfig,SynthSineSpectrum);
	
//Config shortcuts
public:
	~SMSAnalysisConfig(){};
/** Sinusoidal Analysis Window size in num. of samples */	
	void SetSinWindowSize(TSize w);
	TSize GetSinWindowSize() const;
/** Sinusoidal Analysis Window type*/
	void SetSinWindowType(const EWindowType& t);
	const EWindowType& GetSinWindowType() const;
/** Sinusoidal Zero padding factor*/
	void SetSinZeroPadding(int z);
	int GetSinZeroPadding() const;
/** Residual Analysis Window size in num. of samples */	
	void SetResWindowSize(TSize w);
	TSize GetResWindowSize() const;
/** Residual Analysis Window type*/
	void SetResWindowType(const EWindowType& t);
	const EWindowType& GetResWindowType() const;
/** Residual Zero padding factor*/
	void SetResZeroPadding(int z);
	int GetResZeroPadding() const;
/** Global hop size used both for sinusoidal and residual components */
	void SetHopSize(TSize h);
	TSize GetHopSize() const;

/** Sampling rate of the input audio*/
	void SetSamplingRate(TData sr);
	TData GetSamplingRate() const;

/** Initial offset that is applied to analysis frame computed from windowSize and hopSize*/
	TSize GetInitialOffset();


private:

	
	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};


/** Class that encapsulates all the building blocks of the SegmentAnalysis example */
class SMSAnalysis:public ProcessingComposite
{

public:
	
// Processing Object compliance methods.

	const char *GetClassName() const {return "SMSAnalysis";}


	SMSAnalysis(SMSAnalysisConfig& config);
	SMSAnalysis();
	~SMSAnalysis();
	
			
	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void){return false;}

	/** Unsupervised mode execution */
	bool Do(Segment& in);
	bool Do(Frame& in);
	bool Do(Spectrum& outSp, SpectralPeakArray& pkArray,Fundamental& outFn);
	bool Do(const Audio& in/*,const Audio& resIn*/, Spectrum& outSp,SpectralPeakArray& outPk,Fundamental& outFn,Spectrum& outResSpec,Spectrum& outSinSpec);

//private:

	// Configuration data
	SMSAnalysisConfig mConfig;

	// The internal Processing Objects
	SpectralAnalysis mPO_SinSpectralAnalysis;
	SpectralAnalysis mPO_ResSpectralAnalysis;
	FFT_rfftw		mPO_FFT;
	SpectralPeakDetect		mPO_PeakDetect;
	FundFreqDetect  mPO_FundDetect;
	SinTracking		mPO_SinTracking;
	/**For Sinusoidal Synthesis*/
	SynthSineSpectrum		mPO_SynthSineSpectrum;
	SpectrumSubstracter2		mPO_SpecSubstract;

	//Internal DataObjects
/** object only used for initializing frames */	
	Spectrum mSpec;
/** object only used for initializing frames */	
	Fundamental mFund;


#ifdef WITH_GUI
#endif

	// Internal convenience methods.

	void AttachChildren();
	bool ConfigureChildren();
	void ConfigureData();

	
	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

};

};//namespace

#endif
