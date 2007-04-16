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

#ifndef _SpectralAnalysis_
#define _SpectralAnalysis_

#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"
#include "WindowGeneratorConfig.hxx"
#include "CircularShiftConfig.hxx"
#include "FFTConfig.hxx"

namespace CLAM {

/** Configuration class for Spectral Analysis class
 *  It includes specific parameters: HopSize, 
 *  ZeroPadding, Sampling Rate and FFTSize. Further 
 *  setter/getters are also offered for WindowSize and WindowType.
 *  It also includes complete configurations for child processings: 
 *  WindowGenerator, Circular Shift and FFT. These configurations
 *  should only be modified by advanced users
 */

class SpectralAnalysisConfig:public ProcessingConfig
{
	friend class SpectralAnalysis;

	DYNAMIC_TYPE_USING_INTERFACE (SpectralAnalysisConfig,9,ProcessingConfig);
/** Configuration for children Processing Objects*/
	DYN_ATTRIBUTE(0,public,WindowGeneratorConfig, WindowGenerator);
	DYN_ATTRIBUTE(1,public,CircularShiftConfig,CircularShift);
	DYN_ATTRIBUTE(2,public,FFTConfig, FFT);
	DYN_ATTRIBUTE(3,public,int, prHopSize);
	DYN_ATTRIBUTE(4,public,int, prZeroPadding);
	DYN_ATTRIBUTE(5,public,int, prSamplingRate);
	DYN_ATTRIBUTE(6,public,int, prFFTSize);
  //xamat: added these so they can be accessible from a configurator
        DYN_ATTRIBUTE(7,public,int, prWindowSize);
        DYN_ATTRIBUTE(8,public,EWindowType, prWindowType);

public:
//Config shortcuts

/** Sets Analysis Window size in num. of samples. 
 *  This setter actually changes window size in the window generator,
 *  size of the FFT, and amount of circular shift. If new window size
 *  is smaller than twice the hop size plus one, hop size is also 
 *  modified to that value.
 *  Note1: for best results, window size should be set to odd number.
 *  Note2: this setter should be called after the hop size has already
 *  been set. 
 */	
	void SetWindowSize(TSize w);

/** Getter for window size. Actually returns size of the window generator. */
	TSize GetWindowSize() const;

/** Sets Analysis Window type, modifying configuration of child Window Generator
 *  configuration.
 *  @see EWindowType
 */
	void SetWindowType(const EWindowType& t);

/** Getter for window type, returns window type in Window Generator */
	const EWindowType& GetWindowType() const;

/** Sets Zero padding factor. It changes FFTSize and Circular shift accordingly.
 *  If factor is 0, FFTSize is set to WindowSize-1 next power of two (assumes window
 *  size is odd!)
 *  Assuming a WindowSize of 513, see following table to understand how the factor
 *  works:
 *  ZeroPaddingFactor		FFTSize
 *		 -1					  256
 *        0					  512
 *		  1					 1024
 *		  2					 2048
 *  @see PowerOfTwo
 */
	void SetZeroPadding(int z);

/** returns ZeroPaddingFactor being used.*/
	int GetZeroPadding() const;

/** Sets Analysis Hop size in number of samples. This setter only changes attribute but
 *  does not change configurations accordingly! VIP: It assumes that a SetWindowSize is
 *  called afterwards.
 */	
	void SetHopSize(TSize h);

/** returns HopSize in number of samples.*/
	TSize GetHopSize() const;

/** Sets Sampling rate of the input audio. It calls SetWindowSize for synchronizing existing
	configurations.*/
	void SetSamplingRate(TData sr);

/** returns SamplingRate */
	TData GetSamplingRate() const;

/** synchronize configuration values just in case they had been modified directly **/
void Sync();


private:

/** Default Init adds all Dynamic Attributes */	
	void DefaultInit();

/** Sets default values: SamplingRate=44100, ZeroPaddingFactor=0, WindowSize=513,
 *  WindowType=Hamming, HopSize=256, and CircularShift's amount= 256*/
	void DefaultValues();

/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
};

}

#endif

