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

#include "DelayStreamRegion.hxx"

namespace CLAM {

	DelayStreamRegion::DelayStreamRegion(unsigned int hop,
	                                     unsigned int length,
	                                     SourceStreamRegion *src)
		: ReadStreamRegion(hop,length,src),
		  SourceStreamRegion(hop,length)
	{}


	void DelayStreamRegion::Activate()
	{
		ReadStreamRegion::Activate();
		SourceStreamRegion::Activate();
	}

	void DelayStreamRegion::LeaveAndAdvance()
	{
		ReadStreamRegion::LeaveAndAdvance();
		SourceStreamRegion::LeaveAndAdvance();
	}



	bool DelayStreamRegion::FulfilsInvariant() const
	{
		bool ok = true;
		ok &= SourceStreamRegion::FulfilsInvariant();
		ok &= ReadStreamRegion::FulfilsInvariant();
		ok &= (SourceStreamRegion::Pos() == ReadStreamRegion::Pos());
		ok &= (SourceStreamRegion::Hop() == ReadStreamRegion::Hop());
		ok &= (SourceStreamRegion::Len() == ReadStreamRegion::Len());
		ok &= (SourceStreamRegion::End() == ReadStreamRegion::End());
		return ok;
	}

}
