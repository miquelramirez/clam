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

#ifndef _STREAM_BUFFER_H
#define _STREAM_BUFFER_H

#include "StreamRegionContainer.hxx"
#include "Assert.hxx"

namespace CLAM {

	template<class T>
	class Array;

	/** Buffer class for streams of data objects.
	 * 
	 * This class implements the CLAM data stream abstraction, which
	 * consist of a flow of data tokens with several "clients" which
	 * read or modify the data flow through StreamRegion objects.
	 * 
	 * The utility of this class is two-fold:
	 *   - It provides very eficient stream position book-keeping for
	 *     the stream clients. They only need to perform Activate() and
	 *     Leave() operations on the region.
	 *   - It provides (not so eficient) run time checking in debug
	 *     mode, which eases the manual implementation of client
	 *     execution sequences
	 * This is a generic stream class, which provides the higher level
	 * stream capabilities. Its second template argument must be a stream
	 * implementation class with lower level buffering capabilities.
	 * @param D Data type.
	 * @param B Stream Implementation. Existent implementations:
	 *              - CircularStreamImpl
	 */
	template<class D, class B>
	class StreamBuffer {

		StreamRegionContainer mRegions;
		/** Member buffer where reading/writing is performed*/
		B mBuffer;
		/** XA: Auxiliary array used when AddStreamRegions are activated. It is where the
		 *	actual addition takes place. This array should not be here but rather in the region */
		Array<D> mTmpBuffer;

	public:

		StreamBuffer(unsigned int max_window_size = 0);

		StreamBuffer(const StreamBuffer&);

		/** @name Region creation methods.
		 * These methods are the StreamBuffer interface to
		 * Register the stream writer, new readers, delays, etc.
		 */
		//@{
		WriteStreamRegion   *NewWriter (unsigned int hop,
		                                unsigned int length);

		void RemoveReader( ReadStreamRegion * reader);

		ReadStreamRegion    *NewReader (unsigned int hop,
		                                unsigned int length,
		                                SourceStreamRegion* source = 0);

		DelayStreamRegion   *NewDelay  (unsigned int hop,
		                                unsigned int length,
		                                SourceStreamRegion* source);

		InplaceStreamRegion *NewInplace(unsigned int hop, 
		                                unsigned int read_length,
		                                unsigned int write_length,
		                                SourceStreamRegion* source=0,
		                                unsigned int write_delay=0);
		AddStreamRegion *NewAdder(unsigned int hop, unsigned int length);
		//@}

		void SetPrototype(const D& proto) {};

		/** Configuration method.
		 * This method must be called after all the regions using the stream
		 * have been created. It will allocate all the necesary internal buffer
		 * resources (The lower level buffer class will actually do this).
		 */
		void Configure(unsigned int max_window_size=0);

		/** Region activation method.
		 * This method provides the interface to access the data
		 * inside a region. They must always be followed by a
		 * Region Advance method unless the activation has failed.
		 */
		template<class REGION>
		bool GetAndActivate(REGION* r, Array<D> &a)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Invalid region argument ");
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent before activation.");
			r->Activate();

			/* if r does not fulfil invariant after activation, it means that
			 *	it cannot be used consistently. The Leave() method should be called
			 *	afterwards, else results are not predictable when trying to use the region
			 *	as it does not point to a valid memory.*/
			if (!r->FulfilsInvariant()) return false;

			mBuffer.GetData(r,a);
			return true;
		}

		bool GetAndActivate(AddStreamRegion* r, Array<D> &a)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Invalid region argument ");
			CLAM_DEBUG_ASSERT(r->FulfilsInvariant(),
			                  "StreamBuffer::GetAndActivate(): "
			                  "Region inconsistent before activation.");
			r->Activate();
			
			/** if r does not fulfil invariant after activation, it means that
			 *	it cannot be used consistently. The Leave() method should be called
			 *	afterwards, else results are not predictable when trying to use the region
			 *	as it does not point to a valid memory.*/
			if (!r->FulfilsInvariant())	return false;

			mBuffer.GetData(r,a);
			mTmpBuffer=a;
			return true;

		}

		/** @name Region Advance methods.
		 * This methods notify the buffer that data processing in the
		 * region which was requested by a previous GetAndActivate
		 * call is done.
		 * These methods increment the region position, so that the
		 * next call to GetAndActivate on that region will take the
		 * next chunk of data.
		 */
		template<class REGION>
		void LeaveAndAdvance(REGION*r)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
							  "StreamBuffer::LeaveAndAdvance(): "
							  "Invalid region argument ");
			mBuffer.Leave(r);
			r->LeaveAndAdvance();
		}

		void LeaveAndAdvance(AddStreamRegion*r)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
							  "StreamBuffer::LeaveAndAdvance(): "
							  "Invalid region argument ");
			//I have to do the addition by hand!!
			Array<D> tmp;
			mBuffer.GetData(r,tmp);
			for(int i=0;i<tmp.Size();i++)
				tmp[i]+=mTmpBuffer[i];
			mBuffer.Leave(r);
			r->LeaveAndAdvance();
		}

		/** @name Region Advance methods.
		 * This methods notify the buffer that data processing in the
		 * region which was requested by a previous GetAndActivate
		 * call is done.
		 * Differently to LeaveAndAdvance, this method does not increment
		 * the region position.
		 */
		template<class REGION>
		void Leave(REGION*r)
		{
			CLAM_DEBUG_ASSERT(mRegions.Contains(r),
							  "StreamBuffer::Leave(): "
							  "Invalid region argument ");
			mBuffer.Leave(r);
			r->Leave();
		}

		bool CanActivateRegion( ReadStreamRegion& readRegion)
		{
			// notice that exist a overload for WriteStreamRegion.
			CLAM_DEBUG_ASSERT(mRegions.Contains(&readRegion), "StreamBuffer::CanActivate(): Invalid region argument ");
			return readRegion.CanActivate();
		}

		bool CanActivateRegion( SourceStreamRegion& writeRegion ) 
		{
			CLAM_DEBUG_ASSERT(&writeRegion == mRegions.Writer(), "CanActivate() argument should be the write region" );

			return writeRegion.CanActivate() && mBuffer.CanActivateSourceWithNoCircularOverlap( writeRegion );
		}

	};

// Template method implementations:

	template<class D, class B>
	StreamBuffer<D,B>::StreamBuffer(unsigned int max_window_size)
		: mRegions(),
		  mBuffer(max_window_size,mRegions)
	{}

	template<class D, class B>
	StreamBuffer<D,B>::StreamBuffer(const StreamBuffer& buf)
		: mRegions(buf.mRegions),
		  mBuffer(buf.mBuffer.MaxSize(),mRegions)
	{}


	template<class D, class B>
	WriteStreamRegion *StreamBuffer<D,B>::NewWriter (unsigned int hop,
	                                                 unsigned int length)
	{
		CLAM_ASSERT(mRegions.Writer() == 0,
		            "StreamBuffer::NewWriter(): Writer already registered");
		WriteStreamRegion *writer = new WriteStreamRegion(hop,length);
		mRegions.SetWriter(writer);
		return writer;
	}

	template<class D, class B>
	AddStreamRegion *StreamBuffer<D,B>::NewAdder(unsigned int hop, unsigned int length)
	{
		CLAM_ASSERT(mRegions.Adder() == 0,
		            "StreamBuffer::NewAdder(): Adder already registered");
		AddStreamRegion *adder= new AddStreamRegion(hop,length);
		mRegions.SetAdder(adder);
		return adder;
	}

	template<class D, class B>
	ReadStreamRegion *StreamBuffer<D,B>::NewReader (unsigned int hop,
	                                                unsigned int length,
	                                                SourceStreamRegion* source)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewReader(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		ReadStreamRegion *reader = new ReadStreamRegion(hop,length,source);
		mRegions.AddReader(reader,source);
		return reader;
	}

	template<class D, class B>
	void StreamBuffer<D,B>::RemoveReader( ReadStreamRegion * reader)
	{
		CLAM_ASSERT( mRegions.Contains( reader ) , "StreamBuffer::RemoveReader(): Invalid reader to remove." );
		mRegions.RemoveReader( reader );

	}

	template<class D, class B>
	DelayStreamRegion *StreamBuffer<D,B>::NewDelay  (unsigned int hop,
	                                                 unsigned int length,
	                                                 SourceStreamRegion* source = 0)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewDelay(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		DelayStreamRegion *delay = new DelayStreamRegion(hop,length,source);
		mRegions.AddDelay(delay,source);
		return delay;
	}

	template<class D, class B>
	InplaceStreamRegion *StreamBuffer<D,B>::NewInplace(unsigned int hop, 
	                                                   unsigned int read_length,
	                                                   unsigned int write_length,
	                                                   SourceStreamRegion* source,
	                                                   unsigned int write_delay)
	{
		CLAM_ASSERT(source?mRegions.Contains(source):true,
		            "StreamBuffer::NewInplace(): Invalid source argument.");
		if (!source)
			source = mRegions.Writer();
 		InplaceStreamRegion *inplace = new InplaceStreamRegion(hop,read_length,write_length,source);
		mRegions.AddInplace(inplace,source);
		return inplace;
	}

	template<class D, class B>
	void StreamBuffer<D,B>::Configure(unsigned int max_window_size)
	{
		CLAM_ASSERT(mRegions.FulfilsInvariant(),
		            "StreamBuffer::Configure(): "
		            "Inconsisntent regions before configuration")

		mBuffer.Configure(max_window_size);

/*  XA: Warning!!!: This initialization here suposes that the Configure is always performed 
	after having instantiated the regions. This would actually be good as this is the
	place where we assert that all regions are consistent, but we have no way of knowing
	whether the StreamBuffer has been added a new region after configuration. 
	Another thing that could be done is to initialize Regions every time a new one is
	instantiated. */
		mRegions.Init();

		CLAM_ASSERT(mBuffer.FulfilsInvariant(),
		            "StreamBuffer::Configure(): "
		            "Inconsistent buffer after configuration.");
	}

}


// Class speciallizations:

#include "AudioStreamBuffer.hxx"

#endif
