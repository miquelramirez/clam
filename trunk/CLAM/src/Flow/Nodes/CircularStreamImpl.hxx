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
		inline void Leave(AddStreamRegion*);
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
		/**
		 * Circular overlap checking method
		 */
		inline bool CanActivateSourceWithNoCircularOverlap( 
			const SourceStreamRegion & sourceToBeActivated ) const;
	private:
		// helper methods:
		inline bool ExistCircularOverlapWhenActivateSource( 
			const ReadStreamRegion& read, 
			const SourceStreamRegion& ) const;

	};

///////////////////////////////////////////////////////////////////////////////
// Here goes the implementation
///////////////////////////////////////////////////////////////////////////////

	template<class T>
	CircularStreamImpl<T>::CircularStreamImpl(unsigned int max_window_size,
	                                          const StreamRegionContainer &regions)
		: mRegions(regions),
		  mLogicalSize(max_window_size),
		  mChecker(*this)
	{}

	template<class T>
	T* CircularStreamImpl<T>::GetData(const StreamRegion *r)
	{
		CLAM_ASSERT(mLogicalSize != 0,
		            "CircularStreamImpl.GetData(): "
		            "Unconfigured stream. Call Configure() before using it." );
		CLAM_ASSERT(r->Accept(mChecker),"");
		return mBuffer.Read(r->Pos() % mLogicalSize,
		                    r->Len());
	}


	template<class T>
	void CircularStreamImpl<T>::GetData(const StreamRegion *r, Array<T> &a)
	{
		CLAM_ASSERT(mLogicalSize != 0,
		            "CircularStreamImpl.GetData(): "
		            "Unconfigured stream. Call Configure() before using it." );
		CLAM_ASSERT(r->Accept(mChecker),"");
		mBuffer.Read(a,r->Pos() % mLogicalSize,
		                    r->Len());
	}

	// BIG TODO: Here we hit one of the limitations of the current
	// stream/regions design: for regions with multiple inheritance
	// from the base, we would need to provide different GetData
	// methods (GetReadData, GetSourceData), to let the user specify
	// if he wants to get the "read" portion of the stream, or the
	// "write" portion of the stream.
	//
	// The solution to this problem would require some interface
	// changes in the StreamBuffer and the Node classes, and probably
	// some redesing in the Port class.  I have not yet started to
	// think about how inplace or delay ports would be
	// implemented. Right now we just have Input and Output ports...
	// -- Enrique. 21/6/2002
	template<class T>
	void CircularStreamImpl<T>::GetData(const DelayStreamRegion *r, Array<T> &a)
	{
		CLAM_ASSERT(0,"Not implemented method called.");
	}
	template<class T>
	void CircularStreamImpl<T>::GetData(const InplaceStreamRegion *r, Array<T> &a)
	{
		CLAM_ASSERT(0,"Not implemented method called.");
	}


	template<class T>
	void CircularStreamImpl<T>::Leave(WriteStreamRegion*w)
	{
		mBuffer.Touch(w->Pos() % mLogicalSize,
		              w->Len());
	}

	template<class T>
	void CircularStreamImpl<T>::Leave(AddStreamRegion*w)
	{
		mBuffer.Touch(w->Pos() % mLogicalSize,
		              w->Len());
	}

	template<class T>
	void CircularStreamImpl<T>::Leave(InplaceStreamRegion*w)
	{
		Leave(static_cast<WriteStreamRegion*>(w));
	}

	template<class T>
	int CircularStreamImpl<T>::ComputeLogicalLength() const
	{
		if (mLogicalSize)
			return mLogicalSize;
		CLAM_ASSERT(false,"Not Implemented");
		return 0;
	}

	template<class T>
	int CircularStreamImpl<T>::ComputePhantomLength() const
	{
		unsigned int max_length = mRegions.Writer()->Len();
		SourceStreamRegion::reader_const_iterator rit;
		StreamRegionContainer::source_const_iterator sit;
		for (sit=mRegions.sources_begin();
		     sit!=mRegions.sources_end();
		     sit++)
			for (rit=(*sit)->readers_begin();
			     rit != (*sit)->readers_end();
			     rit++)
				if ((*rit)->Len() > max_length)
					max_length = (*rit)->Len();

		return max_length;
	}

	template<class T>
	void CircularStreamImpl<T>::Configure(unsigned int max_buffer_length)
	{
		CLAM_ASSERT(max_buffer_length != 0,
		            "CircularStreamImpl.Configure(): "
		            "Zero buffer size not allowed." );

		CLAM_ASSERT(mRegions.Writer() != 0,
		            "CircularStreamImpl.Configure(): "
		            "No writer region registered yet.");

		if (max_buffer_length <=  mLogicalSize) return;
		mLogicalSize = max_buffer_length;
		mBuffer.Resize(ComputeLogicalLength(),
		               ComputePhantomLength(),
		               0);
	}

	template<class T>
	bool CircularStreamImpl<T>::FulfilsInvariant()
	{
		return mRegions.Writer()->Accept(mChecker);
	}

	template<class T>
	bool CircularRegionCheck<T>::CircularOverlap(const StreamRegion& first,
	                                             const StreamRegion& second) const
	{
		unsigned int first_start        = first.Pos() % mStream.mLogicalSize;
		unsigned int second_start        = second.Pos() % mStream.mLogicalSize;
		unsigned int first_end_phantom  = first_start + first.ActiveLen();
		unsigned int second_end_phantom  = second_start + second.ActiveLen();
		return (first_end_phantom > second_start &&
		        first_start < second_end_phantom );
	}

	template<class T>
	bool CircularRegionCheck<T>::Visit(const WriteStreamRegion& w)
	{
		RegionCheck::Visit(w);

		CLAM_ASSERT(&w == mStream.mRegions.Writer(),
		            "CircularStreamImpl::FulfilsInvariant(): "
		            "Incorrect write region argument.");

		const SourceStreamRegion* last_source = mStream.mRegions.LastSource();
		SourceStreamRegion::reader_const_iterator it;
		for (it  = last_source->readers_begin();
		     it != last_source->readers_end();
		     it++)
			CLAM_ASSERT(!CircularOverlap(w,**it),
						"CircularRegionCheck::Visit(WriteRegion): "
						"Invariant not satisfied.");
		return true;
	}

	template<class T>
	bool CircularStreamImpl<T>::CanActivateSourceWithNoCircularOverlap( const SourceStreamRegion & source) const
	{
		if( !source.HasReaders() ) return true; 

		return !ExistCircularOverlapWhenActivateSource( source.GetLastReading(), source );
	}
	
	template<class T>
	bool CircularStreamImpl<T>::ExistCircularOverlapWhenActivateSource( 
		const ReadStreamRegion& read, 
		const SourceStreamRegion& source) const
	{
		unsigned int sourceEndWhenActive = source.Pos()+source.Len();
		return sourceEndWhenActive - read.Pos() > MaxSize();
	}


} // namespace

#endif
