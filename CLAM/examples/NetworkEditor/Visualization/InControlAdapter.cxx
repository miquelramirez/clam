/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "InControlAdapter.hxx"
#include "InControl.hxx"

namespace CLAMVM
{

InControlAdapter::InControlAdapter()
	: mObserved(0)
{
}

InControlAdapter::~InControlAdapter()
{
}

bool InControlAdapter::Publish()
{
	if (!mObserved)
		return false;

	AcquireName.Emit(mObserved->GetName());
}

bool InControlAdapter::BindTo( const CLAM::InControl& obj)
{
	mObserved = dynamic_cast< const CLAM::InControl* > (&obj);
	
	if ( !mObserved )
	{ 
		return false;
	}
	return true;
}

} // namespace CLAMVM
