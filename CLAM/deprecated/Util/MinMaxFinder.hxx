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

#ifndef __MINMAXFINDER__
#define __MINMAXFINDER__

#include <utility>

namespace CLAMGUI
{
	/**  
	 *   This template function enables to search BOTH the minimum and the
	 *   maximum of a sequence defined by two iterators, one to the begin
	 *   of the sequence and the other to the end. Since this is done in
	 *   a single loop it's more efficient than using both the max_element
	 *   and min_element algorithms provided with STL.
	 *   @param  seq_begin Iterator to the beginning of the sequence
	 *   @param  seq_end   Iterator to the end of the sequence
	 *   @return min_max   std::pair where std::pair::first is the minimum 
	 *                     value found and std::pair::second the maximum value
     */

	template < typename Iterator, typename ValueType >
		void minmaxelements( Iterator seq_begin, Iterator seq_end, std::pair< ValueType, ValueType >& min_max )
	{
		min_max.first = *seq_begin;
		min_max.second = *seq_begin;

		seq_begin++;

		while ( seq_begin!=seq_end )
			{

				if ( *seq_begin > min_max.second )
					{
						min_max.second = *seq_begin;
					}
				else if( *seq_begin < min_max.first )
					{
						min_max.first = *seq_begin;
					}

				seq_begin++;
			}
	}



	/**
     *   Trivial helper function that helps us to tell wether a pair found with
	 *   CLAMGUI::minmaxelements is the same ( constant sequence )
	 *   @param  minmax       The pair to test
	 *   @return is_constant  True when constant sequence, false when not
     */

	template < typename ValueType >
		bool constant_sequence( const std::pair< ValueType, ValueType >& minmax )
	{
		return  ( minmax.first == minmax.second  );
	}

}


#endif // MinMaxFinder.hxx

