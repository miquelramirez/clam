#ifndef __SDIFStream__
#define __SDIFStream__

class SDIFFrame;

/** SDIF has a notion of streams: each frame contains a stream id.
*       However, in the files the streams are not explicitely present,
*       as the frames are ordered by time, not by stream. The 
*       SDIFStream is a list of SDIFFrames with the same
*       id.
*       
*       @see SDIFCollection.
*/
class SDIFStream
{
public:
	SDIFFrame* mpFirst;
	SDIFFrame* mpLast;
	SDIFFrame* mpCurrent;
	SDIFStream* mpPrev;
	SDIFStream* mpNext;
public:
	/** Create a SDIFStream
	* @param pFrame The SDIFFrame that this stream starts with.
	*/
	SDIFStream(SDIFFrame* pFrame);
	/** Add a SDIFFrame to the stream
	* @param pFrame The SDIFFrame that this stream starts with.
	*/
	void Add(SDIFFrame* pFrame);
	/** Parse all frames after the starting frame that have the same
	* stream ID into the streams list 
	*/
	void Parse(void);
};
		
#endif
