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

#ifndef __ONSET_DETECTOR_CONFIG__
#define __ONSET_DETECTOR_CONFIG__

#include "ProcessingConfig.hxx"

namespace CLAM
{
	/** OnsetDetector configuration class **/
	class OnsetDetectorConfig : public ProcessingConfig 
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (OnsetDetectorConfig, 11,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TSize, nBands);
		DYN_ATTRIBUTE (2, public, int, LowFreq);
		DYN_ATTRIBUTE (3, public, int, HighFreq);
		DYN_ATTRIBUTE (4, public, TSize, SmoothFiltSize);
		DYN_ATTRIBUTE (5, public, TData, BandThreshold);
		DYN_ATTRIBUTE (6, public, TSize, MinPeakDist);
		DYN_ATTRIBUTE (7, public, TData, GlobalThreshold);
		DYN_ATTRIBUTE (8, public, TData, NoiseThreshold);
		DYN_ATTRIBUTE (9, public, bool, ComputeOffsets);
		DYN_ATTRIBUTE (10, public, TData, OffsetThreshold);
	
	protected:
		void DefaultInit();
	};



}


#endif // OnsetDetectorConfig.hxx

