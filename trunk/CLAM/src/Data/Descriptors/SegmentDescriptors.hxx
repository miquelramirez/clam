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

#ifndef __SegmentDescriptors_H__
#define __SegmentDescriptors_H__

#include "Descriptor.hxx"

#include "AudioDescriptors.hxx"
#include "FrameDescriptors.hxx"
#include "MorphologicalSegmentDescriptors.hxx"
#include "List.hxx"

namespace CLAM 
{

class Segment;

/*
 * This class holds Descriptors computed from Segment data
 *
 */
class SegmentDescriptors : public Descriptor 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SegmentDescriptors, 9, Descriptor);
	/** All these attributes refer to mean values of descriptors across
	frames in the segment*/
	DYN_ATTRIBUTE (0, public, FrameDescriptors, MeanD);
	/** All these attributes refer to maximum values of descriptors across
	frames in the segment*/
	DYN_ATTRIBUTE (1, public, FrameDescriptors, MaxD);
	/** All these attributes refer to minimum values of descriptors across
	frames in the segment*/
	DYN_ATTRIBUTE (2, public, FrameDescriptors, MinD);
	/** All these attributes refer to the variance of descriptors across
	frames in the segment*/
	DYN_ATTRIBUTE (3, public, FrameDescriptors, VarianceD);

	/** Mean value for fundamental across all frames */
	DYN_ATTRIBUTE (4,public, TData, Fundamental);

	DYN_ATTRIBUTE (5,public, AudioDescriptors, AudioD);
	
	/** @todo: this should better be a List but by now Stats do not operate on lists*/
	DYN_ATTRIBUTE (6,public, Array<FrameDescriptors>, FramesD);

	/** Mean value for fundamental across all frames. Morphological descriptors still not
	 *	computable in CLAM
	 */
	DYN_ATTRIBUTE (7,public, MorphologicalSegmentDescriptors, MorphologicalSegmentD);

	/** Children **/
	DYN_ATTRIBUTE (8,public, List<SegmentDescriptors>, ChildrenD);

public:

	SegmentDescriptors(Segment* pSegment);

	void SetpSegment(const Segment* pSegment);
	const Segment* GetpSegment() const;
	FrameDescriptors& GetFrameD(TIndex pos) {return GetFramesD()[pos];}
	void Compute();
	void ConcreteCompute();
	void SetFramePrototype(const FrameDescriptors& proto,int nFrames);



private:
	void DefaultInit();
	void CopyInit(const SegmentDescriptors & copied);

private:
	const Segment* mpSegment;
	StatsTmpl<false,FrameDescriptors,FrameDescriptors>* mSegmentStats;

};

} // namespace CLAM

#endif /* __SegmentDescriptors_H__ */

