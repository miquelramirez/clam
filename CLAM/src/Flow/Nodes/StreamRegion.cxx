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

#include "StreamRegion.hxx"
#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "DelayStreamRegion.hxx"
#include "InplaceStreamRegion.hxx"
#include "Assert.hxx"

namespace CLAM {

	StreamRegion::StreamRegion(unsigned int hop,
	                           unsigned int length)
		: mHop(hop),
		  mPos(0),
		  mLen(length),
		  mEnd(0)
	{
		CLAM_ASSERT( length>0, "StreamRegion() ctr.: bad length size value");
		CLAM_ASSERT( hop>0, "StreamRegion() ctr.: bad hopsize value");
	}



	void StreamRegion::Activate()
	{
		mEnd = mPos+mLen;
	}

	void StreamRegion::Leave()
	{
		mEnd=mPos;
	}


	void StreamRegion::LeaveAndAdvance()
	{
		mPos += mHop;
	}

	bool StreamRegion::PreceedsWithNoOverlap(const StreamRegion *forwardRegion) const
	{
		return End() <= forwardRegion->Pos();
	}

	bool StreamRegion::FulfilsInvariant() const
	{
		CLAM_ASSERT( false, "you just discovered that this method is used ;-). Please contatct pau or xavi rubio ");

		if (mPos > mEnd)
			return false;
		if (mEnd != mPos + mLen &&
			mEnd != mPos + mLen - mHop)
			return false;
		return true;
	}

	bool RegionCheck::Visit(const ReadStreamRegion& r)
	{
		CLAM_ASSERT(r.FulfilsInvariant(),
		            "RegionCheck::Visit(const ReadRegion): "
		            "Invariant not satisfied.");
		return true;
	}

	bool RegionCheck::Visit(const SourceStreamRegion& r)
	{
		CLAM_ASSERT(r.FulfilsInvariant(),
		            "RegionCheck::Visit(const SourceRegion): "
		            "Invariant not satisfied.");
		return true;
	}

	bool RegionCheck::Visit(const WriteStreamRegion& r)
	{
		CLAM_ASSERT(r.FulfilsInvariant(),
		            "RegionCheck::Visit(const WriteRegion): "
		            "Invariant not satisfied.");
		return true;
	}

	bool RegionCheck::Visit(const DelayStreamRegion& r)
	{
		CLAM_ASSERT(r.FulfilsInvariant(),
		            "RegionCheck::Visit(const DelayRegion): "
		            "Invariant not satisfied.");
		return true;
	}

	bool RegionCheck::Visit(const InplaceStreamRegion& r)
	{
		CLAM_ASSERT(r.FulfilsInvariant(),
		            "RegionCheck::Visit(const InplaceRegion): "
		            "Invariant not satisfied.");
		return true;
	}

}
