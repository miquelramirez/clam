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
#error This .cxx file CAN NOT be included in the project.\
       It is already included at the end of the .hxx file
#endif

namespace CLAM {


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
		return mBuffer.Read(a,r->Pos() % mLogicalSize,
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
		unsigned int max_length = mRegions.Writer()->MaxLength();
		SourceStreamRegion::reader_const_iterator rit;
		StreamRegionContainer::source_const_iterator sit;
		for (sit=mRegions.sources_begin();
		     sit!=mRegions.sources_end();
		     sit++)
			for (rit=(*sit)->readers_begin();
			     rit != (*sit)->readers_end();
			     rit++)
				if ((*rit)->MaxLength() > max_length)
					max_length = (*rit)->MaxLength();

		return max_length;
	}

	template<class T>
	void CircularStreamImpl<T>::Configure(unsigned int max_buffer_length)
	{
		if (max_buffer_length)
			mLogicalSize = max_buffer_length;
		CLAM_ASSERT(mLogicalSize != 0,
		            "CircularStreamImpl.Configure(): "
		            "Zero buffer size not allowed." );
		CLAM_ASSERT(mRegions.Writer() != 0,
		            "CircularStreamImpl.Configure(): "
		            "No writer region registered yet.");
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
	bool CircularRegionCheck<T>::CircularOverlap(const StreamRegion& r1,
	                                             const StreamRegion& r2) const
	{
		unsigned int r1_start        = r1.Pos() % mStream.mLogicalSize;
		unsigned int r2_start        = r2.Pos() % mStream.mLogicalSize;
		unsigned int r1_end_phantom  = r1_start + r1.Len();
		unsigned int r2_end_phantom  = r2_start + r2.Len();
		return ! (r1_end_phantom <= r2_start ||
		          r2_end_phantom <= r1_start);
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

}
