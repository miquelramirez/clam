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

#ifndef __MorphologicalFrameDescriptors__
#define __MorphologicalFrameDescriptors__
#include <typeinfo>

#include "Flags.hxx"
#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"

#include "AudioDescriptors.hxx"
#include "SpectralDescriptors.hxx"
#include "SpectralPeakDescriptors.hxx"
#include "Fundamental.hxx"
#include "PeriodogramPeak.hxx"

#include "Frame.hxx"



namespace CLAM{


/*
 * This class holds Descriptors computed from Frame data. Right now no specific
 * descriptors are computed for frame (only for spectrums, audios... that are held
 * inside the frame. TODO: add specific descriptors.
 *
 */
class MorphologicalFrameDescriptors : public CLAM::ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MorphologicalFrameDescriptors, 7, ProcessingData);
	DYN_ATTRIBUTE (0, public, bool, Silent);
	DYN_ATTRIBUTE (1, public, TData, PitchSalience);
	DYN_ATTRIBUTE (2, public, TData, Energy);
	DYN_ATTRIBUTE (3, public, TData, SpectralCentroid);
	DYN_ATTRIBUTE (4, public, TData, SpectralBandwidth);
	DYN_ATTRIBUTE (5, public, DataArray, Periodogram);	
	DYN_ATTRIBUTE (6, public, Array<PeriodogramPeak>, PeriodogramPeakArray);

private:
	void DefaultInit();
	};



};//SIMAC



#endif

