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



typedef Search < List < Frame >, Frame> FrameSearch;

/**	Processing Data class to encapsulate a CLAM segment. A Segment is basically an ordered
 *	list of Frames (@see Frame). It also has a BeginTime and EndTime time tags, an associated
 *	Audio (usually a large audio chunk from which smaller Audio Frames are obtained9 and
 *	a global SamplingRate.
 *	Appart from these internal Processing Data, a Segment also holds a list of child segments
 *	named Children. These are usually smaller segments that result from applying some sort of
 *	segmentation algorithm to the original Segment. These children do not hold data (namely
 *	Frames and Audio. The boolean value HoldsData gives account whether the segment has data
 *	(i.e. is the parent) or not. Finally the child segments also have a pointer to their
 *	parent to be able to access the data.
 *	@see Audio, Frame, ProcessingData, List*/
class Segment : public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Segment, 7, ProcessingData);
	DYN_ATTRIBUTE (0, public, TTime, BeginTime);
	DYN_ATTRIBUTE (1, public, TTime, EndTime);
	DYN_ATTRIBUTE (2, private, bool, prHoldsData);
	DYN_ATTRIBUTE (3, public, List<Frame>, FramesArray);
	DYN_ATTRIBUTE (4, public, Audio, Audio);
	DYN_ATTRIBUTE (5, public, List<Segment>, Children);
	DYN_ATTRIBUTE (6, public, TData, SamplingRate);
	
private:
	/**	Auxiliary class for performing searches in ordered list of frames*/
	FrameSearch mFramesSearch;
	/** Pointer to parent segment*/
	Segment* pParent;

public:

	/** Index used when processing for keeeping trace of current location in Frame list*/
	TIndex mCurrentFrameIndex;
	
	/** Getter for accessing parent segment*/
	Segment* GetpParent(){return pParent;}
	/** Setter for parent segment*/
	void SetpParent(Segment*  newParent){pParent=newParent;}
	void DefaultInit();

public:
	void CopyInit(const Segment& prototype);

	/** Returns the number of frames in segment*/
	int GetnFrames() const;

//Interface for accessing and modifying frames

	/** Returns a reference to the frame found in a given position
	 *	@see const Frame& GetFrame(TIndex pos) const
	 *	@see GetFrame(TTime time)
	 */
	Frame& GetFrame(TIndex pos);
	/** Returns a constant reference to the frame found in a given position*/
	const Frame& GetFrame(TIndex pos) const;
	/** Adds a new frame at the end of the segment*/
	void AddFrame(Frame& newFrame);
	/** Deletes frame in a given position */
	void DeleteFrame(TIndex pos);
	/** Finds frame with center time closest to the one given 
	 *	@return position of frame in segment 
	 */
	TIndex FindFrame(TTime time) const;
	/** Returns a reference to the frame with center time closest to the one passed
	 *	@see const Frame& GetFrame(TTime time) const
	 *	@see GetFrame(TIndex pos)
	 */
	Frame& GetFrame(TTime time);
	/** Returns a reference to the frame with center time closest to the one passed
	 *	@see Frame& GetFrame(TTime time) 
	 */
	const Frame& GetFrame(TTime time) const;
	/**	Deletes frame with center time closest to the one passed
	 *	@see DeletFrame(TIndex pos)
	 */
	void DeleteFrame(TTime time);
	
	/** Interface for setting the prHoldData member and 
	 *  configuring the Segment accordingly
	 */
	void SetHoldsData(bool HoldsData);

	/** Interface for accessing the prHoldData private
	 *  member
	 */
	bool GetHoldsData() const {return GetprHoldsData();}

private:
	
	/** Internal convenience method */
	void SetFramesSearch (const FrameSearch & frame) {
		mFramesSearch=frame;
	}
	/** Internal convenience method */
	const FrameSearch & GetFramesSearch () const {
		return mFramesSearch;
	}
	/** Internal convenience method */
	FrameSearch & GetFramesSearch () {
		return mFramesSearch;
	}
	/** Internal convenience method */
	void CopyDataFromParent();
	void DefaultValues();
};

};//namespace

#endif
