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

#ifndef __SegmentDescriptors_H__
#define __SegmentDescriptors_H__


#include "DynamicType.hxx"
#include "ProcessingData.hxx"
#include "DataTypes.hxx"

#include "Melody.hxx"

#include "AudioDescriptors.hxx"
#include "FrameDescriptors.hxx"

namespace CLAM {

	class Segment;

/*
 * This class holds Descriptors computed from Segment data
 *
 */
class SegmentDescriptors : public ProcessingData {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SegmentDescriptors, 18, ProcessingData);
		/*All these attributes refer to mean values of spectrum descriptors across
		frames in the segment*/
		DYN_ATTRIBUTE (0, public, TData, SpectralMean);
		DYN_ATTRIBUTE (1, public, TData, SpectralGeometricMean);
		DYN_ATTRIBUTE (2, public, TData, SpectralEnergy);
		DYN_ATTRIBUTE (3, public, TData, SpectralCentroid);
		DYN_ATTRIBUTE (4, public, TData, SpectralMoment2);
		DYN_ATTRIBUTE (5, public, TData, SpectralMoment3);
		DYN_ATTRIBUTE (6, public, TData, SpectralMoment4);
		DYN_ATTRIBUTE (7, public, TData, SpectralMoment5);
		DYN_ATTRIBUTE (8, public, TData, SpectralMoment6);
		DYN_ATTRIBUTE (9, public, TData, SpectralIrregularity);
		DYN_ATTRIBUTE (10, public, TData, SpectralTilt);
		DYN_ATTRIBUTE (11, public, TData, SpectralFlatness);
		DYN_ATTRIBUTE (12, public, TData, SpectralKurtosis);
		DYN_ATTRIBUTE (13, public, TData, SpectralStrongPeak);
		/** Mean value for fundamental across all frames */
		DYN_ATTRIBUTE (14,public, TData, Fundamental);
		DYN_ATTRIBUTE (15,public, Melody, Melody);

		DYN_ATTRIBUTE (16,public, AudioDescriptors, AudioD);
		DYN_ATTRIBUTE (17,public, List<FrameDescriptors>, FramesD);

	public:

		SegmentDescriptors(Segment* pSegment);

		void DefaultInit();
				
		const Segment* GetpSegment() const{return mpSegment;}
		void SetpSegment(Segment* pSegment) {mpSegment=pSegment;}
		FrameDescriptors& GetFrameD(TIndex pos) {return GetFramesD()[pos];}

		
	private:
		Segment* mpSegment;

	};


};


#endif /* __SegmentDescriptors_H__ */
