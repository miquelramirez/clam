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

#ifndef _SpectralSynthesisConfig_
#define _SpectralSynthesisConfig_

#include "ProcessingConfig.hxx"
#include "WindowGeneratorConfig.hxx"
#include "CircularShiftConfig.hxx"
#include "IFFTConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the SpectralSynthesis Processing
	 */
class SpectralSynthesisConfig:public ProcessingConfig
{

friend class SpectralSynthesis;

public:
	
	DYNAMIC_TYPE_USING_INTERFACE(SpectralSynthesisConfig,10,ProcessingConfig);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(0,public,WindowGeneratorConfig,AnalWindowGenerator);
	DYN_ATTRIBUTE(1,public,WindowGeneratorConfig,SynthWindowGenerator);
	DYN_ATTRIBUTE(2,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(3,public,IFFTConfig,IFFT);
	DYN_ATTRIBUTE(4,public,int, prZeroPadding);
	DYN_ATTRIBUTE(5,public,int,prSamplingRate);
	DYN_ATTRIBUTE(6,public,bool,Residual);
  //xamat: added this to be able to access from configurators
        DYN_ATTRIBUTE(7,public,int,prAnalWindowSize);
        DYN_ATTRIBUTE(8,public,EWindowType,prAnalWindowType);  
        DYN_ATTRIBUTE(9,public,int,prHopSize);

//Config shortcuts

public:
/** Setter for Analysis Window size in number of samples. It should match the size used in the
 *	SpectralAnalysis Processing. An assertion fault is raised if size is not odd. This
 *	method changes size of AnalysisWindowGenerator, IFFT and CircularShift accordingly.
 */	
	void SetAnalWindowSize(TSize w);
/** Getter for Analysis Window Size
 *	@see SetAnalWindowSize
 */
	TSize GetAnalWindowSize() const;

/** Setter for Analysis Window type
 *	@see EWindowType
 */
	void SetAnalWindowType(const EWindowType& t);
/** Getter for Analysis Window type
 *	@see SetAnalWindowType
 */
	const EWindowType& GetAnalWindowType() const;

/** Sets Zero padding factor.
 *	A factor of zero means that no zero padding has been applied in the analysis.
 *	@see SpectralAnalysis::SetZeroPadding
 */
	void SetZeroPadding(int z);
/** Getter for Zero padding factor 
 *	@see SetZeroPadding*/
	int GetZeroPadding() const;
/** Setter for Synthesis window size in number of samples. Just affects the Synthesis
 *	Window Generator Size
 */
	void SetSynthWindowSize(TSize w);
/**	Getter for Synthesis Window Size
 *	@see SetSynthWindowSize
 */
	TSize GetSynthWindowSize()const;

/** Setter for Synthesis Hop size in number of samples.
 *	Changes size of the Synthesis WindowGenerator, and the Hop Size and Buffer Size in the
 *	OverlapAdd Processing
 */	
	void SetHopSize(TSize h);
/** Getter for Synthesis Hop size
 *	@see  SetHopSize
 */
	TSize GetHopSize() const;
/** Setter for global Sampling rate. It synchronizes all existing sizes. */
	void SetSamplingRate(TData sr);
/** Getter for global Sampling rate */
	TData GetSamplingRate() const;
/** Setter for Spectrum Size **/
	void SetSpectrumSize(TSize specSize);
	TSize GetSpectrumSize() const;

private:

	void DefaultInit();
	void DefaultValues();
  /** Sync all configuration values*/
	void Sync();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

} // namespace CLAM

#endif

