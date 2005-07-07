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

#ifndef _Segmentator_hxx_
#define _Segmentator_hxx_

#include "Processing.hxx"
#include "SegmentatorConfig.hxx"

#include "Matrix.hxx"
/* TODO: this should be moved to the .cxx but a simple forward declaration
	 won't work, so the method Algorithm should be implemented in a different
	 way. since this will break the interface with possible subclasses, we 
	 leave it for later. MDB
*/

namespace CLAM
{

class Segment;
class SegmentDescriptors;
class SegmentBoundaries;

enum {
	SpectralDescBase=150, //for example
	SpectralMeanId = SpectralDescBase,
	SpectralGeometricMeanId = SpectralDescBase+1,
	SpectralEnergyId = SpectralDescBase+2,
	SpectralCentroidId = SpectralDescBase+3,
	SpectralMoment2Id = SpectralDescBase+4,
	SpectralMoment3Id = SpectralDescBase+5,
	SpectralMoment4Id = SpectralDescBase+6,
	SpectralMoment5Id = SpectralDescBase+7,
	SpectralMoment6Id = SpectralDescBase+8,
	SpectralFlatnessId = SpectralDescBase+9,
	SpectralKurtosisId = SpectralDescBase+10,

	FrameDescBase = 175, //for example
	FundamentalId = FrameDescBase,

	SegmentatorBase = 200, //for example
	MinSegmentLenghtId = SegmentatorBase,

	AudioDescriptorBase = 250,
	AudioMeanId = AudioDescriptorBase,
	AudioEnergyId = AudioDescriptorBase + 1,
	AudioVarianceId = AudioDescriptorBase + 2,
	AudioCentroidId = AudioDescriptorBase + 3,
	AudioZeroCrossingRateId = AudioDescriptorBase + 4
};


class Segmentator : public Processing
{
public:
	Segmentator();
	Segmentator(const SegmentatorConfig& c);
	virtual ~Segmentator();
	const char * GetClassName() const {return "Segmentator";}
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
	bool Do(Segment& originalSegment,SegmentDescriptors& descriptors); //for the time being only inplace processing is enabled

protected:
	SegmentatorConfig  mConfig;
private:
	void DataFusion(Segment& s,const SegmentBoundaries& segmentBoundaries);
	bool ConcreteConfigure(const ProcessingConfig& c);
	/* All Algorithms should follow this prototype, taking as an input a segment and
	// a Matrix where the descriptors values are stored
	*/
	virtual void Algorithm(Segment& s,const Matrix& values);
protected:
	virtual void UnwrapDescriptors(const Segment& originalSegment,SegmentDescriptors& descriptors ,Matrix& descriptorsValues);
};

} //namespace CLAM

#endif // _Segmentator_hxx_

