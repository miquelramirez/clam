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

#ifndef _SMSSynthesisConfig_
#define _SMSSynthesisConfig_

#include "ProcessingConfig.hxx"
#include "PhaseManagementConfig.hxx"
#include "SpectralSynthesisConfig.hxx"
#include "OverlapAddConfig.hxx"
#include "SynthSineSpectrumConfig.hxx"

namespace CLAM {

class SMSSynthesisConfig:public ProcessingConfig
{

friend class SMSSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SMSSynthesisConfig,7,ProcessingConfig);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(0,public, SynthSineSpectrumConfig,SynthSineSpectrum);
	DYN_ATTRIBUTE(1,public, PhaseManagementConfig,PhaseMan);
	DYN_ATTRIBUTE(2,public, SpectralSynthesisConfig,SpectralSynth);
	DYN_ATTRIBUTE(3,public,OverlapAddConfig,OverlapAddSin);
	DYN_ATTRIBUTE(4,public,OverlapAddConfig,OverlapAddRes);
	DYN_ATTRIBUTE(5,public,OverlapAddConfig,OverlapAddGlobal);
	DYN_ATTRIBUTE(6,protected,int,prSamplingRate);


//Config shortcuts

public:
/** Analysis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
	void SetAnalWindowSize(TSize w);
	TSize GetAnalWindowSize() const;
/** Analysis Window type*/
	void SetAnalWindowType(const EWindowType& t);
	const EWindowType& GetAnalWindowType() const;
/** Synthesis window size*/
	void SetSynthWindowSize(TSize w);
	TSize GetSynthWindowSize()const;

/** Synthesis Hop size in miliseconds. Must be < (WindowSize-(1/SR))/2*/	
	void SetHopSize(TSize h);
	TSize GetHopSize() const;
/** Sampling rate of the output audio*/
	void SetSamplingRate(TData sr);
	TData GetSamplingRate() const;
/** Spectrum Size **/
	void SetSpectrumSize(TSize specSize);
	TSize GetSpectrumSize() const;
/** Frame Size **/
	void SetFrameSize(TSize f);
	TSize GetFrameSize();

private:

	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

}

#endif

