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


#include "Descriptor.hxx"

#include "AudioDescriptors.hxx"
#include "FrameDescriptors.hxx"
#include "List.hxx"

namespace CLAM {

	class Segment;

/*
 * This class holds Descriptors computed from Segment data
 *
 */
class SegmentDescriptors : public Descriptor {
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SegmentDescriptors, 4, Descriptor);
		/*All these attributes refer to mean values of spectrum descriptors across
		frames in the segment*/
		DYN_ATTRIBUTE (0, public, FrameDescriptors, MeanD);
	
		/** Mean value for fundamental across all frames */
		DYN_ATTRIBUTE (1,public, TData, Fundamental);

		DYN_ATTRIBUTE (2,public, AudioDescriptors, AudioD);
		DYN_ATTRIBUTE (3,public, List<FrameDescriptors>, FramesD);

	public:

		SegmentDescriptors(Segment* pSegment);

		void SetpSegment(Segment* pSegment);
		const Segment* GetpSegment() const;
		FrameDescriptors& GetFrameD(TIndex pos) {return GetFramesD()[pos];}
		void Compute();
		void ConcreteCompute();
		void SetFramePrototype(const FrameDescriptors& proto,int nFrames);



	private:
		void DefaultInit();
		void CopyInit(const SegmentDescriptors & copied);

	private:
		Segment* mpSegment;

	};



// Implementation

inline void SegmentDescriptors::DefaultInit() {
	mpSegment=0;
	mpStats=0;
	AddFramesD();
	//AddAudioD();
	UpdateData();
}

inline void SegmentDescriptors::CopyInit(const SegmentDescriptors & copied) {
	mpSegment=copied.mpSegment;
	mpStats=0;
}

inline const Segment* SegmentDescriptors::GetpSegment() const {
	return mpSegment;
}


}



#endif /* __SegmentDescriptors_H__ */
