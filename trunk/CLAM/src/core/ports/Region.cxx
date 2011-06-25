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

#include "Region.hxx"
#include "Assert.hxx"

namespace CLAM
{

Region::Region() 
	: mPos(0), mSize(1), mHop(1), mBeginDistance(0)
{
}

Region::~Region()
{
}

const long & Region::Pos() const 
{
	return mPos;
}

const int & Region::Size() const
{
	return mSize;
}

const int & Region::Hop() const
{
	return mHop;
}

const int & Region::BeginDistance()
{
	return mBeginDistance;
}

void Region::BeginDistance( const int &dist )
{
	mBeginDistance = dist;		
}

void Region::Pos( const long & pos )
{
	mPos = pos;
}

void Region::Size( const int & size )
{
	mSize = size;
	SizeChanged(size);
}

void Region::Hop( const int & hop )
{
	CLAM_ASSERT( hop <= mSize, "Region::hop() - hop can't be greater than size" );
	mHop = hop;
}

} // namespace CLAM

