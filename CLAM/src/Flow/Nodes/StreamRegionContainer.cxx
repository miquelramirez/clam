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

#include "StreamRegionContainer.hxx"
#include "Assert.hxx"
#include <algorithm>

namespace CLAM {

	StreamRegionContainer::StreamRegionContainer()
		: mFinder(*this)
	{}

	StreamRegionContainer::StreamRegionContainer(const StreamRegionContainer&)
		: mFinder(*this)
	{}

	void StreamRegionContainer::SetWriter(WriteStreamRegion *writer)
	{
		CLAM_ASSERT(!Writer(),
		            "StreamRegionContainer::SetWriter():"
		            " Writer already set.");
		mSources.push_back(writer);
	}

	void StreamRegionContainer::SetAdder(AddStreamRegion *adder)
	{
		CLAM_ASSERT(!Adder(),
		            "StreamRegionContainer::SetAdder():"
		            " Adder already set.");
		mSources.push_back(adder);
	}

	void StreamRegionContainer::AddDelay(DelayStreamRegion *new_delay,
	                                     SourceStreamRegion *source)
	{
		CLAM_ASSERT(!Contains(static_cast<SourceStreamRegion*>(new_delay)),
		            "StreamRegionContainer::AddDelay(): "
		            "New source already in the container");

		source_iterator it;
		it = std::find(sources_begin(),sources_end(),source);

		CLAM_ASSERT(it != sources_end(),
		            "StreamRegionContainer::AddDelay(): "
		            "Origianl source is not in the container");

		(*it)->AddReader(new_delay);
		it++;
		mSources.insert(it,new_delay);

	}

	void StreamRegionContainer::AddInplace(InplaceStreamRegion *new_inplace,
	                                       SourceStreamRegion *source)
	{
		CLAM_ASSERT(!Contains(static_cast<SourceStreamRegion*>(new_inplace)),
		            "StreamRegionContainer::AddInplace(): "
		            "New source already in the container");

		source_iterator it;
		it = std::find(sources_begin(),sources_end(),source);

		CLAM_ASSERT(it != sources_end(),
		            "StreamRegionContainer::AddInplace(): "
		            "Origianl source is not in the container");

		(*it)->AddReader(new_inplace);
		it++;
		mSources.insert(it,new_inplace);

	}

	void StreamRegionContainer::AddReader(ReadStreamRegion *new_reader,
	                                      SourceStreamRegion *src)
	{
		CLAM_ASSERT(src?Contains(src):true,
		            "StreamRegionContainer::AddReader(): "
		            "Origianl source is not in the container");
		CLAM_ASSERT(src?true:mSources.size()!=0,
		            "StreamRegionContainer::AddReader(): "
		            "You must set the writer before adding readers.");
		CLAM_ASSERT(!Contains(new_reader),
		            "StreamRegionContainer::AddReader(): "
		            "New reader already in the container");
		SourceStreamRegion *source = src;
		if (!source)
			source=mSources.front();
		source->AddReader(new_reader);
	}
	void StreamRegionContainer::RemoveReader(ReadStreamRegion * reader)
	{
 		CLAM_ASSERT(Contains(reader), "StreamRegionContainer::RemoveReader(): "
			    " Reader to remove is not present in container" );
		
		// we remove the reader from writer region
		// todo: remove reader from all the sources
		mSources.front()->RemoveReader(reader); 
	}

	WriteStreamRegion *StreamRegionContainer::Writer()
	{
		if (mSources.size() == 0)
			return 0;
		WriteStreamRegion* writer;
		source_const_iterator sit;
		for (sit=sources_begin();sit!=sources_end();sit++)
		{
			if(writer=dynamic_cast<WriteStreamRegion*>(*sit))
				return writer;
		}
		return 0;
	}

	const WriteStreamRegion *StreamRegionContainer::Writer() const
	{
		if (mSources.size() == 0)
			return 0;
		const WriteStreamRegion* writer;
		source_const_iterator sit;
		for (sit=sources_begin();sit!=sources_end();sit++)
		{
			if(writer=dynamic_cast<WriteStreamRegion*>(*sit))
				return writer;
		}
		return 0;
	}

	AddStreamRegion *StreamRegionContainer::Adder()
	{
		if (mSources.size() == 0)
			return 0;
		AddStreamRegion* adder;
		source_const_iterator sit;
		for (sit=sources_begin();sit!=sources_end();sit++)
		{
			if(adder=dynamic_cast<AddStreamRegion*>(*sit))
				return adder;
		}
		return 0;
	}

	const AddStreamRegion *StreamRegionContainer::Adder() const
	{
		if (mSources.size() == 0)
			return 0;
		const AddStreamRegion* adder;
		source_const_iterator sit;
		for (sit=sources_begin();sit!=sources_end();sit++)
		{
			if(adder=dynamic_cast<AddStreamRegion*>(*sit))
				return adder;
		}
		return 0;
	}

	const SourceStreamRegion *StreamRegionContainer::LastSource() const
	{
		return mSources.back();
	}

	StreamRegionContainer::source_iterator
	StreamRegionContainer::sources_begin()
	{
		return mSources.begin();
	}

	StreamRegionContainer::source_iterator
	StreamRegionContainer::sources_end()
	{
		return mSources.end();
	}

	StreamRegionContainer::source_const_iterator
	StreamRegionContainer::sources_begin() const
	{
		return mSources.begin();
	}

	StreamRegionContainer::source_const_iterator
	StreamRegionContainer::sources_end() const
	{
		return mSources.end();
	}


	bool StreamRegionContainer::Contains(const StreamRegion *r)
	{
		return r->Accept(mFinder);
	}

	bool StreamRegionContainer::Contains(const DelayStreamRegion *r)
	{
		return r->Accept(mFinder);
	}

	bool StreamRegionContainer::Contains(const InplaceStreamRegion *r)
	{
		return r->Accept(mFinder);
	}

	bool StreamRegionContainer::FulfilsInvariant() const
	{
		if (mSources.size() == 0)
			return true;

		source_const_iterator sit;
		SourceStreamRegion::reader_const_iterator rit;

		for (sit=sources_begin();
		     sit!=sources_end();
		     sit++)
			for (rit=(*sit)->readers_begin();
			     rit != (*sit)->readers_end();
			     rit++)
				if ((*rit)->Source() != *sit)
					return false;

		return true;
	}


	//XA
	void StreamRegionContainer::Init()
	{
		source_iterator sit;
		for (sit=sources_begin(); sit!=sources_end(); sit++)
			(*sit)->Init();
	}

	unsigned int StreamRegionContainer::FindLargestReadRegionLength() const
	{
		if (mSources.size() == 0)
			return 0;

		unsigned int len=0;

		source_const_iterator sit;
		SourceStreamRegion::reader_const_iterator rit;

		for (sit=sources_begin(); sit!=sources_end();  sit++)
			for (rit=(*sit)->readers_begin();  rit != (*sit)->readers_end();  rit++)
				if ((*rit)->Len() >len) len=(*rit)->Len();
		
		return len;
	}

	bool RegionFinder::Visit(const WriteStreamRegion& src)
	{
		if (&src == static_cast<const WriteStreamRegion*>(mContainer.Writer()))
			return true;
		return false;
	}

	bool RegionFinder::Visit(const SourceStreamRegion& src)
	{
		StreamRegionContainer::source_const_iterator it;
		for (it=mContainer.sources_begin();
		     it!=mContainer.sources_end();
		     it++)
			if (&src == *it)
				return true;
		return false;
	}

	bool RegionFinder::Visit(const ReadStreamRegion& reader)
	{
		SourceStreamRegion::reader_const_iterator rit;
		StreamRegionContainer::source_const_iterator sit;
		for (sit=mContainer.sources_begin();
		     sit!=mContainer.sources_end();
		     sit++)
			for (rit=(*sit)->readers_begin();
			     rit != (*sit)->readers_end();
			     rit++)
				if (&reader == *rit)
					return true;
		return false;
	}

	bool RegionFinder::Visit(const DelayStreamRegion& delay)
	{
		return Visit(static_cast<const SourceStreamRegion&>(delay));
	}

	bool RegionFinder::Visit(const InplaceStreamRegion& inplace)
	{
		return Visit(static_cast<const SourceStreamRegion&>(inplace));
	}


	


}
