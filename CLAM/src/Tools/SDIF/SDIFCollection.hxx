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

#ifndef __SDIFCollection__
#define __SDIFCollection__

#include "DataTypes.hxx"
#include "SDIFStorage.hxx"

namespace SDIF
{
	class Frame;
	class Stream;

	/** A special Storage to store Frames in
	*	a linked list. 
	*
	* The Collection has the possibility to create the 
	*	Streams to which the frames belong, either on the fly, or on command
	*
	*	Typically, Collection is passed to File::Read, as the place
	*	the store the read Frames. @see File::Read
	*/
	class Collection:public Storage
	{
	public:
		typedef std::list<Stream*>::const_iterator StreamIterator;

	private:
		//* The list of Frames
		std::list<Frame*> mFrameList;

		//* The list of Streams
		std::list<Stream*> mStreamList;

		//* Whether or not to create the Streams on the fly
		bool mAutoStreamAdding;
	public:
		//* Create a new Collection.
		Collection(bool autoStreamAdding = true);
		virtual ~Collection() {}

		void Add(Frame* pFrame);
		void Add(Stream* pStream);

		FrameIterator Begin(void) const { return mFrameList.begin(); }
		FrameIterator End(void) const { return mFrameList.end(); }

		StreamIterator BeginStreams(void) const { return mStreamList.begin(); }
		StreamIterator EndStreams(void) const { return mStreamList.end(); }

		Stream* FindStream(CLAM::TInt32 streamId);
		Stream* FindStream(TypeId streamTypeId);

		void ParseStreams(void);
	};

}

#endif

