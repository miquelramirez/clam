#ifndef __SDIFFrame__
#define __SDIFFrame__

#include "SDIFHeader.hxx"

class SDIFMatrix;
/** A special kind of frame at the beginning of each SDIF file
*/

class SDIFOpeningsFrame {
public:
	SDIFFrameHeader mHeader;
	TInt32 mSpecVersion;
	TInt32 mStandardTypesVersion;
public:	
	/** Create the frame */
	SDIFOpeningsFrame():mHeader("SDIF")
	{
		mHeader.mSize = 8;
		mSpecVersion = 0; mStandardTypesVersion = 0;
	}
};

/** SDIF data is stored in frames, that contain number of matrices.
*       An SDIFFrame can be aware of it's successors and precessors in time 
* (both in the whole list of frames and in the streams)
*/
class SDIFFrame {
public:
	SDIFDataFrameHeader mHeader;
	SDIFMatrix* mpFirst;
	SDIFMatrix* mpLast;
	SDIFFrame *mpPrev;
	SDIFFrame *mpNext;
	SDIFFrame *mpNextInStream;
	SDIFFrame *mpPrevInStream;
public:
	/** Create a new SDIFFrame
	* @param type Identifier specifying the frame type
	* @param time Timestamp of the frame
	* @param id Stream ID
	*/
	SDIFFrame(const SDIFType& type = SDIFType::sDefault,
		TFloat64 time =0.,TInt32 streamId = 0);
	~SDIFFrame();

	/** Add a SDIFMatrix to the frame
	* @param pMatrix pointer to the SDIFMatrix to add to the frame
	*/
	void Add(SDIFMatrix* pMatrix);
};

#endif
