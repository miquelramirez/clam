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

#ifndef __SINTRACKVCLIPPER__
#define __SINTRACKVCLIPPER__

#include <CLAM/SinTracksDef.hxx>

namespace CLAM
{
	namespace VM
	{
		class SinTrackVerClipper
		{
			typedef SineTrackSpanEnds::iterator span_iterator;
			typedef SinusoidalTrack::iterator   peak_iterator;
				
			typedef enum { Inside = 0x0, Outside = 0x1 } outcode; // 0x0 means Inside

		public:
			SinTrackVerClipper( TData f = 30.0 );
			~SinTrackVerClipper();

			void Cull( TData f_lower, TData f_upper, SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e );

			void SetMinimumFreqRange( TData f )
				{
					mMinFreqRange = f;
				}

		protected:	
			void ClipSpans( TData f_lower, TData f_upper, peak_iterator& si, peak_iterator& ei, 
								SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e  );
		private:
			inline outcode in_out_test( TData lower, TData upper, TData f )
				{

					if ( f > upper)
						return Outside;
					if ( f < lower ) 
						return Outside;

					return Inside;
				}

			inline peak_iterator find_first_in( TData f_lo, TData f_hi, peak_iterator pi, peak_iterator ei )
				{				
					outcode outcodei = in_out_test( f_lo, f_hi, pi->mFreq );

					while ( (pi != ei) && (outcodei & Outside) )
					{
						outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
						pi++;
					}

					return pi;
				}

			inline peak_iterator find_last_in( TData f_lo, TData f_hi, peak_iterator pi, peak_iterator ei )
				{
					peak_iterator last_in = pi;
					pi++;

					outcode outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
						
					while ( (pi!=ei) && ( !outcodei &0x01 ) )
					{
						last_in = pi;
						pi++;
						outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
					}

					return last_in;
				}

			inline peak_iterator find_first_out( TData f_lo, TData f_hi, peak_iterator pi, peak_iterator ei )
				{
					// Precondition: pi is inside i.e. in_out_test( f_lo, f_hi, pi->mFreq ) == Inside
					pi++;
					outcode outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
						
					while( (pi!=ei) )
					{
						if ( outcodei & Outside ) 
							return pi;
						pi++;
						outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
					}

					return pi;
				}
				
			inline peak_iterator find_last_out( TData f_lo, TData f_hi, peak_iterator pi, peak_iterator ei )
				{
					peak_iterator last_out = pi;
					pi++;
					outcode outcodei = in_out_test( f_lo, f_hi, pi->mFreq );

					while ( pi!=ei )
					{
						if ( outcodei & Outside )
						{
							return last_out;
						}

						pi++;
						outcodei = in_out_test( f_lo, f_hi, pi->mFreq );
					}

					return last_out;
				}

			TData mLowFreq;
			TData mHiFreq;
			TData mMinFreqRange;
		};
	}
}

#endif 

