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
#error  This .cxx file CAN NOT be included in the project.\
        It is already included at the end of the .hxx file
#endif

#include "Array.hxx"

namespace CLAM {

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
		            "StreamBuffer::NewWirter(): Writer already registered");
		WriteStreamRegion *writer = new WriteStreamRegion(hop,length);
		mRegions.SetWriter(writer);
		return writer;
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
 		InplaceStreamRegion *inplace = new InplaceStreamRegion(hop,length,source);
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

		CLAM_ASSERT(mBuffer.FulfilsInvariant(),
		            "StreamBuffer::Configure(): "
		            "Inconsistent buffer after configuration.");
	}

}
