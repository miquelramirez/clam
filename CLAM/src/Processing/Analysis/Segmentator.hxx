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

#ifndef _Segmentator_
#define _Segmentator_

#include "Processing.hxx"
#include "Segment.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "SearchArray.hxx"
#include "Matrix.hxx"
#include "SegmentDescriptors.hxx"
#include "Point.hxx"
#include <iosfwd>

namespace CLAM{

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
	SpectralIrregularityId = SpectralDescBase+9,
	SpectralTiltId = SpectralDescBase+10,
	SpectralFlatnessId = SpectralDescBase+11,
	SpectralKurtosisId = SpectralDescBase+12,
	SpectralStrongPeakId = SpectralDescBase+13,

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


typedef struct //may need further additions
{
	TIndex id;
	TData threshold;
	TData percentil;
	TData nPreviousFrames;
}TDescriptorsParams;

inline bool operator<(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id<param2.id) return true;
	else return false;
}

inline bool operator>=(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id>=param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,const TDescriptorsParams& param2)
{
	if (param1.id==param2.id) return true;
	else return false;
}

inline bool operator==(const TDescriptorsParams& param1,TIndex id)
{
	if (param1.id==id) return true;
	else return false;
}

std::ostream& operator << (std::ostream& myStream, const TDescriptorsParams& a);

std::istream& operator >> (std::istream& myStream, const TDescriptorsParams& a);


class SegmentatorConfig : public ProcessingConfig
{
friend class Segmentator;
public:
	DYNAMIC_TYPE_USING_INTERFACE(SegmentatorConfig,3,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, MinSegmentLength);
	DYN_ATTRIBUTE (2, public, Array<TDescriptorsParams>, DescriptorsParams);
protected:
	void DefaultInit();
/*Public Interface*/
public:
	void AddDescParams(const TDescriptorsParams& descParams);
	bool FindDescParams(TDescriptorsParams& descParams);
	void ClearDescParams();

	void SetDescriptorsSearch (const SearchArray<TDescriptorsParams> & frame) {
		mDescriptorsSearch=frame;
	}
	const SearchArray<TDescriptorsParams> & GetDescriptorsSearch () const {
		return mDescriptorsSearch;
	}
	SearchArray<TDescriptorsParams> & GetDescriptorsSearch () {
		return mDescriptorsSearch;
	}
	~SegmentatorConfig(){};
private:
	SearchArray<TDescriptorsParams> mDescriptorsSearch;
};
	
class Segmentator:public Processing
{
public:
	Segmentator();
	Segmentator(const SegmentatorConfig& c);
	~Segmentator();
	const char * GetClassName() const {return "Segmentator";}
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
	bool Do(Segment& originalSegment,SegmentDescriptors& descriptors); //for the time being only inplace processing is enabled

	void StoreOn(Storage &s) {};

		
protected:
	SegmentatorConfig  mConfig;

private:
	void DataFusion(Segment& s,const Array<Array<PointTmpl<int,TData> > >& segmentBoundaries);
	bool ConcreteConfigure(const ProcessingConfig& c);
	/* All Algorithms should follow this prototype, taking as an input a segment and
	// a Matrix where the descriptors values are stored
	*/
	virtual void Algorithm(Segment& s,const Matrix& values);
protected:
	virtual void UnwrapDescriptors(const Segment& originalSegment,SegmentDescriptors& descriptors ,Matrix& descriptorsValues);
};

};//namespace CLAM

#endif
