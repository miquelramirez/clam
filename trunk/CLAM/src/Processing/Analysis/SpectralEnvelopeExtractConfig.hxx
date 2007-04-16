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

#ifndef _SpectralEnvelopeExtractConfig_
#define _SpectralEnvelopeExtractConfig_

#include "ProcessingConfig.hxx"
#include "GlobalEnums.hxx"

namespace CLAM {

	/**
	 * Config class for the SpectralEnvelopeExtract Processing
	 */
	class SpectralEnvelopeExtractConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeExtractConfig, 2,ProcessingConfig);
		/** Type of interpolation for resultant Spectrum BPF*/
		DYN_ATTRIBUTE (0, public, EInterpolation, InterpolationType);
		DYN_ATTRIBUTE (1, public, int, MaxPeaks);
		
	protected:
		void DefaultInit();
	public:
	  /** Initialize configuration object with default values (name 'SpectralEnvelopeExtract', 513 bands, Magnitude treshold set to -80 and MaxSpectralPeaks setted to 100
	   */
		void DefaultValues();
		~SpectralEnvelopeExtractConfig(){};
		
 	}; 

} // namespace CLAM

#endif

