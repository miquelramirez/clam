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

#ifndef __stl_optbinsearch__
#define __stl_optbinsearch__

#include <iterator>

namespace std
{

	namespace detail
		{
			template < typename RandomIterator, typename value_type >
				unsigned bisect( RandomIterator begin, RandomIterator end,
						 const value_type& val, 
						 unsigned lowerBound, unsigned upperBound )
				{
					lowerBound = ( lowerBound > 0) ? lowerBound-- : lowerBound;

					int dist = std::distance( begin, end );

					bool ascending = *(begin+dist-1) >= *(begin);

					while( upperBound - lowerBound > 1 )
					{
						unsigned mid = ( upperBound + lowerBound ) >> 1;
						
						if ( val >= *(begin+mid) == ascending )
							lowerBound = mid;
						else
							upperBound = mid;
					}

					return lowerBound;
				}

			template < typename RandomIterator, typename value_type, typename GreatEqCmp >
				unsigned bisect( RandomIterator begin, RandomIterator end,
						 const value_type& val, 
						 unsigned lowerBound, unsigned upperBound, GreatEqCmp cmpOp )
				{
					lowerBound = ( lowerBound > 0) ? lowerBound-- : lowerBound;

					int dist = std::distance( begin, end );

					bool ascending = cmpOp( *(begin+dist-1), *(begin) );

					while( upperBound - lowerBound > 1 )
					{
						unsigned mid = ( upperBound + lowerBound ) >> 1;
						
						if ( cmpOp( val, *(begin+mid) ) == ascending )
							lowerBound = mid;
						else
							upperBound = mid;
					}

					return lowerBound;
				}

		}


	template < typename RandomIterator, typename value_type, typename GreatEqCmp >
		RandomIterator hunt( RandomIterator begin, RandomIterator end,
				 const value_type& val, RandomIterator last, GreatEqCmp cmpOp  )
		{
			unsigned guess = std::distance( begin, last );
			unsigned dist = std::distance( begin, end );
			
			if ( last < begin ||  last >= end )
				return begin + detail::bisect( begin, end, val, 0, dist, cmpOp );

			bool ascending = cmpOp( *(begin+dist-1), *(begin) );
			
			unsigned inc = 1;
			unsigned upperBound;

			if ( cmpOp( val, *(begin+guess) ) == ascending )
			{
				if ( guess == dist - 1 )
					return end;

				upperBound = guess+1;

				while( cmpOp( val, *(begin+upperBound) ) == ascending )
				{
					guess = upperBound; 
					inc <<= 1;
					upperBound += inc;
					
					if ( upperBound >= dist )
					{
						RandomIterator result = begin + detail::bisect( begin, end, val, guess, dist, cmpOp );
						if ( result == end-1  )
							return end;
						else
							return result;
					}
				}
			}
			else
			{
				if ( guess == 0 ) return end;
				
				upperBound = guess--;
				
				while ( !(cmpOp( val, *(begin+guess))) == ascending )
				{
					upperBound = guess;
					inc <<= 1;
					
					if ( inc >= upperBound )
					{
						RandomIterator result = begin + detail::bisect( begin, end, val, 0, upperBound, cmpOp );
						
						if ( result == begin )
							return end;
						else
							return result;
					}
					
					guess = upperBound - inc;
				}
			}
			
			return begin + detail::bisect( begin, end, val, guess, upperBound, cmpOp );
		}		


	template < typename RandomIterator, typename value_type, typename GreatEqCmp >
		RandomIterator hunt( RandomIterator begin, RandomIterator end,
				     const value_type& val, GreatEqCmp cmpOp )
		{
			return hunt( begin, end, val, begin, cmpOp );
		}

	template < typename RandomIterator, typename value_type >
		RandomIterator hunt( RandomIterator begin, RandomIterator end,
				 const value_type& val, RandomIterator last  )
		{
			unsigned guess = std::distance( begin, last );
			unsigned dist = std::distance( begin, end );
			
			if ( last < begin ||  last >= end )
				return begin + detail::bisect( begin, end, val, 0, dist );

			bool ascending = *(begin+dist-1) >= *(begin);
			
			unsigned inc = 1;
			unsigned upperBound;

			if ( val >= *(begin+guess) == ascending )
			{
				if ( guess == dist - 1 )
					return end;

				upperBound = guess+1;

				while( val >= *(begin+upperBound) == ascending )
				{
					guess = upperBound; 
					inc <<= 1;
					upperBound += inc;
					
					if ( upperBound >= dist )
					{
						RandomIterator result = begin + detail::bisect( begin, end, val, guess, dist );
						if ( result == end-1  )
							return end;
						else
							return result;
					}
				}
			}
			else
			{
				if ( guess == 0 ) return end;
				
				upperBound = guess--;
				
				while ( !(val >= *(begin+guess)) == ascending )
				{
					upperBound = guess;
					inc <<= 1;
					
					if ( inc >= upperBound )
					{
						RandomIterator result = begin + detail::bisect( begin, end, val, 0, upperBound );
						
						if ( result == begin )
							return end;
						else
							return result;
					}
					
					guess = upperBound - inc;
				}
			}
			
			return begin + detail::bisect( begin, end, val, guess, upperBound );
		}		

  // TODO: Commented out as g++ 4.1 complaints, it seems not to be used
	template < typename RandomIterator, typename value_type >
		RandomIterator hunt( RandomIterator begin, RandomIterator end,
				     const value_type& val )
		{
			return hunt( begin, end, val, begin);
		}
	
}

#endif // stl_optbinsearch.h
