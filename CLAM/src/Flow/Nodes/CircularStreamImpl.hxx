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

#ifndef _CIRCULAR_STREAM_IMPL_H
#define _CIRCULAR_STREAM_IMPL_H

#include "StreamRegionContainer.hxx"
#include "PhantomBuffer.hxx"

namespace CLAM {


	template<class T>
	class CircularStreamImpl;

	template<class T>
	class CircularRegionCheck : public RegionCheck
	{
		const CircularStreamImpl<T> &mStream;
		bool  CircularOverlap(const StreamRegion&, 
		                      const StreamRegion&) const;
	public:
		CircularRegionCheck(const CircularStreamImpl<T> &stream)
			: mStream(stream) 
		{}
		bool Visit(const WriteStreamRegion&);
	};

	/** Circular buffer based implementation class for StreamBuffer.
	 * @param T: Data type.
	 */
	template<class T>
	class CircularStreamImpl {

		friend class CircularRegionCheck<T>;

		const StreamRegionContainer &mRegions;
		PhantomBuffer<T> mBuffer;
		unsigned int mLogicalSize;

		CircularRegionCheck<T> mChecker;

		int ComputeLogicalLength() const;
		int ComputePhantomLength() const;
	public:

		CircularStreamImpl(unsigned int max_window_size,
		                   const StreamRegionContainer &regions);

		unsigned int MaxSize() const {return mLogicalSize;}

		/** Configuration method.
		 * This method will be called from StreamBuffer::Configure().
		 * It performs the actual buffer allocation.
		 */
		inline void Configure(unsigned int max_buffer_length=0);

		/** @name Buffer access methods.
		 * These methods return a pointer to the first data element in
		 * the stream referenced by a region. They will be called from
		 * the StreamBuffer::GetAndActivate() methods.
		 */
		//@{
		inline T* GetData(const StreamRegion*);
		inline T* GetData(const DelayStreamRegion*);
		inline T* GetData(const InplaceStreamRegion*);
		inline void GetData(const StreamRegion*, Array<T> &a);
		inline void GetData(const DelayStreamRegion*, Array<T> &a);
		inline void GetData(const InplaceStreamRegion*, Array<T> &a);
		//@}

		/** @name Methods for notifications of buffer modifications.
		 *  These methods are called from the
		 *  StreamBuffer::LeaveAndAdvance() methods. Here low level
		 *  operations such as phantom zone updates are made.
		 */
		//@{
		inline void Leave(WriteStreamRegion*);
		inline void Leave(InplaceStreamRegion*);
		inline void Leave(ReadStreamRegion*) {}
		inline void Leave(DelayStreamRegion*) {}
		//@}

		/**
		 * CircularStreamImpl consistency check.
		 * This method asumes that the high level consistency invariant
		 * is satisfied. It only makes extra consistency checks related
		 * to the circular buffer implementation.
		 */
		bool FulfilsInvariant();
	};

}

#include "CircularStreamImpl.cxx"

#endif
