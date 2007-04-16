/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

