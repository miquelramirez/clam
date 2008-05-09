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

#ifndef _SpectralPeakDetectConfig_
#define _SpectralPeakDetectConfig_
#include <typeinfo>

#include "GlobalEnums.hxx"
#include "DataTypes.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	/** Configuration class for SpectralPeakDetect objects
	 *  @see ProcessingConfig, SpectralPeakDetect
	 */
	class SpectralPeakDetectConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralPeakDetectConfig, 3, ProcessingConfig);
		/** Maximum namber of SpectralPeaks*/
		DYN_ATTRIBUTE (0, public, TSize, MaxPeaks);
		/** Threshold of magnitude; SpectralPeaks will be located above this value*/
		DYN_ATTRIBUTE (1, public, TData, MagThreshold);
		/** Maximum frequency, spectral peaks will only be detected under this threshold*/
		DYN_ATTRIBUTE (2, public, TData, MaxFreq);
		/** Type of Magnitude interpolation between bins*/
//		DYN_ATTRIBUTE (3, public, EInterpolation, InterpolMag);
		/** Type of Phase interpolation between bins */
//		DYN_ATTRIBUTE (4, public, EInterpolation, InterpolPhase);

//	protected:
	private:
		void DefaultInit();

	public:
	  /** Initialize configuration object with default values (name 'SpectralPeakDetect', 513 bands, Magnitude treshold set to -80 and MaxSpectralPeaks setted to 100
	   */
		void DefaultValues();
//		~SpectralPeakDetectConfig(){};
		
 	}; 

}

#endif

