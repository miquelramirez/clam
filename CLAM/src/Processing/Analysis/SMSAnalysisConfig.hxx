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

#ifndef _SMSAnalysisConfig_
#define _SMSAnalysisConfig_

#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"
#include "SpectralAnalysisConfig.hxx"
#include "SpectralPeakDetectConfig.hxx"
#include "FundFreqDetectConfig.hxx"
#include "SinTrackingConfig.hxx"
#include "SynthSineSpectrumConfig.hxx"

namespace CLAM {

class EWindowType;

class SMSAnalysisConfig:public ProcessingConfig
{

	friend class SMSAnalysis;

	DYNAMIC_TYPE_USING_INTERFACE (SMSAnalysisConfig,8,ProcessingConfig);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(0,public,SpectralAnalysisConfig, SinSpectralAnalysis);
	DYN_ATTRIBUTE(1,public,SpectralAnalysisConfig, ResSpectralAnalysis);

	DYN_ATTRIBUTE(2,public,SpectralPeakDetectConfig, PeakDetect);
	DYN_ATTRIBUTE(3,public,FundFreqDetectConfig, FundFreqDetect);
	DYN_ATTRIBUTE(4,public,SinTrackingConfig, SinTracking);
/** Flags that are used in order to decide what debug display is shown*/
	
	DYN_ATTRIBUTE(5,protected,int, prSamplingRate);
	DYN_ATTRIBUTE(6,protected,int, prFFTSize);
	DYN_ATTRIBUTE(7,public, SynthSineSpectrumConfig,SynthSineSpectrum);

	
//Config shortcuts
public:
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
	TSize GetInitialOffset() const;

	TSize GetHopsInBiggerWindow() const;

private:

	
	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

}

#endif

