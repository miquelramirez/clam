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

#include "TDFilter.hxx"

#include <string>
#include <sstream>

namespace CLAM
{
	TDFilter::TDFilter() :
		mSize( 0 )
	{
		TDFilterConfig cfg;

		Configure( cfg );
	}

	TDFilter::TDFilter( TDFilterConfig &cfg ) :
		mSize( 0 )
	{
		Configure( cfg );
	}

	TDFilter::~TDFilter() {}

	bool TDFilter::ConcreteConfigure( const ProcessingConfig &cfg )
	{
		CopyAsConcreteConfig(mConfig,cfg);

		if( mConfig.HasAudioSize() )
		{
			CLAM_ASSERT( mConfig.GetAudioSize() >= 0,
				"Wrong (negative) size in TDFilter configuration." );
			mSize = mConfig.GetAudioSize();
		}

		return true;
	}

	void TDFilter::CheckTypes( const Audio &X, const Audio &Y ) const
	{
		CLAM_BEGIN_CHECK

		if( X.GetSize() != mSize )
		{
			std::stringstream ss;

			ss << "TDFilter::Do : Wrong size in audio input\n"
			   << "	Expected: " << mSize << ", used " << X.GetSize(); 
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( !X.HasBuffer() )
			CLAM_ASSERT(0,"TDFilter Do: Float attribute required for Audio object.");

		if( Y.GetSize() != mSize )
		{
			std::stringstream ss;
			ss << "TDFilter::Do: Wrong size in audio output.\n"
			   << "	Expected: " << mSize << ", used " << Y.GetSize();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		CLAM_END_CHECK
	}

	bool TDFilter::Do( const Audio &X, 
					   const TDFilterKernel &H, Audio &Y )
	{		
		int n, m;
		DataArray xv, yv;
		TData gain, sumA, sumB;
		TData *XBuffer, *YBuffer;
	
		if( !AbleToExecute() ) return true;
		
		CheckTypes( X, Y );

		XBuffer = X.GetBuffer().GetPtr();
		YBuffer = Y.GetBuffer().GetPtr();
		
		// Unused variable: TSize nIn = X.GetSize();
		
		/* a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
                          - a(2)*y(n-1) - ... - a(na+1)*y(n-na)         */

		switch( (int) H.GetType() )
		{
			case ETDFilterType::eFIR:
				{
/*					xv.Resize(H.GetZerosOrder());
					xv.SetSize(H.GetZerosOrder());
					yv.Resize(H.GetZerosOrder());
					yv.SetSize(H.GetZerosOrder());

					sumB = 0;
					for( m = 0; m < H.GetZerosOrder(); m++ )
						sumB += H.B()[m];

					gain = sumB;

					for ( n = 0; n < Y.GetSize(); n++ )
					{
						for( m = 0; m < H.GetZerosOrder()-1; m++  )
						{
							xv[m]=xv[m+1];
							yv[m]=yv[m+1];
						}
						xv[H.GetZerosOrder()-1] = XBuffer[n] /gain;
						yv[H.GetZerosOrder()-1] = xv[H.GetZerosOrder()-1]*H.B()[0];

						unsigned mm=H.GetZerosOrder()-2;
						for( m = 1; m < H.GetZerosOrder(); m++  )
						{
							yv[H.GetZerosOrder()-1] += xv[mm]*H.B()[m] - yv[mm]*H.A()[m];
							mm --;
						}
						YBuffer[n]=yv[H.GetZerosOrder()-1];
					}*/

					for( n = 0; n < Y.GetSize(); n++ )
					{
						YBuffer[ n ] = H.B()[ 0 ] * XBuffer[ n ];

						for( m = 1; ( n-m>=0 ) && ( m < H.GetZerosOrder() ); m++ )
							YBuffer[ n ] += H.B()[ m ] * XBuffer[ n - m ];
					}

					break;
				}
			case ETDFilterType::eAutoregressive:
				{
					for( n = 0; n < Y.GetSize(); n++ )
					{
						YBuffer[ n ] = XBuffer[ n ];

						for( m = 1;( n-m>=1 ) && ( m < H.GetPolesOrder() ); m++ )
							YBuffer[ n ] -= H.A()[ m ] * YBuffer[ n - m ];
					}

					break;
				}
			case ETDFilterType::eARMA:
				{
					xv.Resize(H.GetZerosOrder());
					xv.SetSize(H.GetZerosOrder());
					yv.Resize(H.GetZerosOrder());
					yv.SetSize(H.GetZerosOrder());


					sumA = sumB = 0;

					for( m = 0; m < H.GetZerosOrder(); m++ )
						sumB += H.B()[m];

					for( m = 0; m < H.GetPolesOrder(); m++ )
						sumA += H.A()[m];

					gain = sumB / sumA;

					for ( n = 0; n < Y.GetSize(); n++ )
					{
						for( m = 0; m < H.GetZerosOrder()-1; m++  )
						{
							xv[m]=xv[m+1];
							yv[m]=yv[m+1];
						}
						xv[H.GetZerosOrder()-1] = XBuffer[n] /gain;
						yv[H.GetZerosOrder()-1] = xv[H.GetZerosOrder()-1]*H.B()[0];

						unsigned mm=H.GetZerosOrder()-2;
						for( m = 1; m < H.GetZerosOrder(); m++  )
						{
							yv[H.GetZerosOrder()-1] += xv[mm]*H.B()[m] - yv[mm]*H.A()[m];
							mm --;
						}
						YBuffer[n]=yv[H.GetZerosOrder()-1];
					}

/*					unsigned N = H.GetPolesOrder();
					unsigned M = H.GetZerosOrder();
					unsigned MN = MAX(M,N);
					unsigned lw = MN-1;
					DataArray w;

					w.Resize(lw);
					w.SetSize(lw);
					
					sumA = sumB = 0;

					for( m = 0; m < M; m++ )
						sumB += H.B()[m];

					for( m = 0; m < N; m++ )
						sumA += H.A()[m];

					gain = sumB / sumA;



					for( n=0; n<X.GetSize(); n++ )
					{
						YBuffer[n]=w[0]+H.B()[0]*XBuffer[n]/gain;

						for( m=0; m<(lw-1); m++ )
							w[m] = w[m+1]-H.A()[m+1]*YBuffer[n]+H.B()[m+1]*XBuffer[n]/ gain;

						w[lw-1]=H.B()[lw]*XBuffer[n]-H.A()[lw]*YBuffer[n];
					}
*/

					
/*					for( n = 0; n < Y.GetSize(); n++ )
					{
						YBuffer[ n ] = H.B()[ 0 ];

						for( m = 1; m < H.GetZerosOrder(); m++ )
							if( n-m>=1 )
								YBuffer[ n ] += H.B()[ m ] * XBuffer[ n - m ]/2.047321703e+05;

						YBuffer[ n ] -= H.A()[ 0 ];

						for( m = 1; m < H.GetPolesOrder(); m++ )
							if( n-m>=1)
								YBuffer[ n ] -= H.A()[ m ] * YBuffer[ n - m ];
					}

*/
					break;
				}
		}

		return true;
	}
} //namespace
