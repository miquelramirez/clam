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
