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

#ifndef _Segment_
#define _Segment_

#include "DataTypes.hxx"
#include "DynamicType.hxx"
#include "Array.hxx"
#include "List.hxx"
#include "Search.hxx"
#include "SearchArray.hxx"
#include "IndexArray.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"
#include "Frame.hxx"
#include "Audio.hxx"

namespace CLAM{


//TODO: Now FramesArray is just an array, should be a DynamicBranch
//typedef Search < BranchCompactMem < Frame >, Frame> FrameSearch;
typedef Search < List < Frame >, Frame> FrameSearch;
////////////////////////////////////////////////////////////////////////////////////////////////
// Class Segment :
//

class Segment : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Segment, 7, ProcessingData);
	DYN_ATTRIBUTE (0, public, TTime, BeginTime);
	DYN_ATTRIBUTE (1, public, TTime, EndTime);
	DYN_ATTRIBUTE (2, private, bool, prHoldsData);
	//TODO: Now FramesArray is just an array, should be a DynamicBranch
	DYN_ATTRIBUTE (3, public, List<Frame>, FramesArray);
	DYN_ATTRIBUTE (4, public, Audio, Audio);
	DYN_ATTRIBUTE (5, public, List<Segment>, Children);
	DYN_ATTRIBUTE (6, public, TData, SamplingRate);
	
private:
	FrameSearch mFramesSearch;
	Segment* pParent;

public:

	/*Index used when processing for keeeping trace of current location	**/
	TIndex mCurrentFrameIndex;
	
	Segment* GetpParent(){return pParent;}
	void SetpParent(Segment*  newParent){pParent=newParent;}
	void DefaultInit();
protected:
	void DefaultValues();
public:
	void CopyInit(const Segment& prototype);

	int GetnFrames() const;

//Interface for accessing and modifying frames
//By Index
public:
	Frame& GetFrame(TIndex pos);
	const Frame& GetFrame(TIndex pos) const;
	void AddFrame(Frame& newFrame);
	void DeleteFrame(TIndex pos);
//ByTime
	TIndex FindFrame(TTime time) const;
	Frame& GetFrame(TTime time);
	const Frame& GetFrame(TTime time) const;
	void DeleteFrame(TTime time);
	
	/** Interface for setting the prHoldData member and 
	 *  configuring the Segment accordingly*/
	void SetHoldsData(bool HoldsData);

	/** Interface for accessing the prHoldData private
	 *  member*/
	bool GetHoldsData() const {return GetprHoldsData();}
	
	void SetFramesSearch (const FrameSearch & frame) {
		mFramesSearch=frame;
	}
	const FrameSearch & GetFramesSearch () const {
		return mFramesSearch;
	}
	FrameSearch & GetFramesSearch () {
		return mFramesSearch;
	}

private:
	void CopyDataFromParent();

};

};//namespace

#endif
