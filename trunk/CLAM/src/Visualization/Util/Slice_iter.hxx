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

#ifndef __SLICE_ITER__
#define __SLICE_ITER__

#include <valarray>


namespace CLAMGUI{
/**
 *   Stroustrup's Slice iterator, with some minor tweaks for
 *   achieving the efficiency of traditional C hand-coded implementation
 *   of the concept
 */
template < typename T, typename SliceType = std::slice >
class Slice_iter
{
public:

	Slice_iter( std::valarray<T>* vv, SliceType ss )
		: mValarray ( vv ), mSlice( ss ), mCurrIdx( 0 )
	{
	}

	Slice_iter end()
	{
		Slice_iter t = *this;
		t.mCurrIdx = s.size();
		return t;
	}

	Slice_iter& operator++() 
	{
		mCurrIdx++;
		return *this;
	}

	Slice_iter operator++(int)
	{
		Slice_iter t = *this;
		mCurrIdx++;
		return t;
	}

	// C-Style subscripting

	T& operator[]( unsigned int idx )
	{
		return ref( mCurrIdx = idx );
	}

	T& operator()( unsigned int idx )
	{
		return ref( mCurrIdx = idx );
	}

	T& operator*()
	{
		return ref( mCurrIdx );
	}


private:

	std::valarray<T>*    mValarray;
	SliceType            mSlice;
	unsigned int          mCurrIdx; // index of current element

	inline T& ref( unsigned int i ) const
	{
		return (*mValarray)[ mSlice.start() + i*mSlice.stride() ];
	}


};

// Some operations needed to make this useful

template < typename T, typename SliceType >
bool operator==( const Slice_iter< T, SliceType >& p, const Slice_iter< T, SliceType >& q )
{
	return p.mCurrIdx==q.mCurrIdx && p.s.stride() == q.s.stride() && p.s.start() == q.s.start();
}

template < typename T, typename SliceType >
bool operator!=( const Slice_iter< T, SliceType >& p, const Slice_iter< T, SliceType >& q )
{
	return !( p == q );
}

template < typename T, typename SliceType >
bool operator<( const Slice_iter< T, SliceType >&p, const Slice_iter< T, SliceType >& q )
{
	return p.curr < q.curr && p.s.stride() == q.s.stride() && p.s.start() == q.s.start();
}


};//CLAMGUI

#endif // Slice_iter.hxx
