#ifndef __SDIFStream__
#define __SDIFStream__

#include <list>
#include "SDIFType.hxx"

namespace SDIF
{

	class Frame;

	/** SDIF has a notion of streams: each frame contains a stream id.
	*       However, in the files the streams are not explicitely present,
	*       as the frames are ordered by time, not by stream. The 
	*       Stream is a list of Frames with the same
	*       id.
	*       
	*       @see Collection.
	*/
	class Stream
	{
	public:
		typedef std::list<Frame*>::const_iterator FrameIterator;
	private:
		std::list<Frame*> mFrameList;
		TypeId mType;
		CLAM::TInt32 mStreamId;
	public:
		/** Create a Stream
		* @param streamId The StreamID for the frames in this stream
		*/
		Stream(TypeId typeId, CLAM::TInt32 streamId);
		/** Add a Frame to the stream
		* @param pFrame The Frame that this stream starts with.
		*/
		void Add(Frame* pFrame);

		FrameIterator Begin(void) const { return mFrameList.begin(); }
		FrameIterator End(void) const { return mFrameList.end(); }

		Frame* Front(void) const { return mFrameList.front(); }
		Frame* Back(void) const { return mFrameList.back(); }

		CLAM::TInt32 StreamId(void) { return mStreamId; }		
		TypeId StreamType(void) { return mType; }
	};

}
		
#endif
