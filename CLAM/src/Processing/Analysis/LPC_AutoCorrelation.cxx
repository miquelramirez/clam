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

#include "LPC_AutoCorrelation.hxx"
#include "Array.hxx"
#include "Audio.hxx"
#include "Assert.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{
	void LPCConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		// MRJ: Seems that eleven is a 'wise' number
		SetOrder( 11 );
	}

	LPC_AutoCorrelation::LPC_AutoCorrelation()
	{
	}

	LPC_AutoCorrelation::~LPC_AutoCorrelation()
	{
	}

	bool LPC_AutoCorrelation::Do()
	{
		return true;
	}

	bool LPC_AutoCorrelation::Do( const Audio& in, LPModel& out )
	{
		bool mustUpdateData = false;
		if ( !out.HasFilterCoefficients() )
		{
			out.AddFilterCoefficients();
			mustUpdateData =true;
		}
		if ( !out.HasReflectionCoefficients() )
		{
			out.AddReflectionCoefficients();
			mustUpdateData = true;
		}
		if ( !out.HasAvgSqrFilterError() )
		{
			out.AddAvgSqrFilterError();
			mustUpdateData = true;
		}
		if ( mustUpdateData )
			out.UpdateData();
		
		return Do( in, out.GetFilterCoefficients(),
			   out.GetReflectionCoefficients(),
			   out.GetAvgSqrFilterError() );
	}

	/*  this is the "clean" version: 
	bool LPC_AutoCorrelation::Do( const Audio& in, DataArray& A, DataArray& K, TData& E )
	{
		TData *inBuffer, *outBuffer;
		TData norm, N;
		int k, n;
		
  		if( !AbleToExecute() ) return true;
		
		CheckTypes( in, out );

  		inBuffer = in.GetBuffer().GetPtr();
		outBuffer = out.GetBuffer().GetPtr();

		for( k = 0; k < out.GetSize(); k++ )
		{
			for( n = 0; n < in.GetSize(); n++ )
			{
				if( n < k )	// k is out of the segment
					outBuffer[ k ] += 0;
				else
					outBuffer[ k ] += inBuffer[ n ] * inBuffer[ n - k ] ;
			}

			outBuffer[ k ] *= in.GetSize() ;
			if (k==0)
				norm = 1 / outBuffer[0];
			outBuffer[ k ] *= norm;
		}
	}
	*/

	// The following does the same, but more efficient, by removing the condition
	// from the for loop
	bool LPC_AutoCorrelation::Do( const Audio& in, DataArray& A, DataArray& K, TData& E )
	{
		if ( !AbleToExecute() )
			return false;

		DataArray R; // autocorrelation coefficients

		R.Resize( mCurrentConfig.GetOrder() );
		R.SetSize( mCurrentConfig.GetOrder() );

		ComputeAutocorrelation( in.GetBuffer(), R );

		if ( fabs( R[0] ) <= 1e-6 )
		{
			/** Special K!*/
			DataArray R2( R.GetPtr()+1, R.Size()-1 );
			SolveSystemByLevinsonDurbin( R2, A, K, E );
		}
		else
			SolveSystemByLevinsonDurbin( R, A, K, E );	

		return true;
	}

	bool LPC_AutoCorrelation::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mCurrentConfig, cfg );

		CLAM_ASSERT( mCurrentConfig.HasOrder(), 
			     "Invalid configuration object: it must have the 'Order' attribute available" );

		return true;
	}

	/** in1 and in2 must be pointers to vectors of same lenght */
	static inline  CLAM::TData dot_product( const CLAM::TData* in1,
						const CLAM::TData* in2,
						const CLAM::TData* endIn )
	{
		CLAM::TData accum = 0.0;
		
		while ( in1 != endIn )
		{
			accum+= (*in1++)*(*in2++);
		}

		return accum;
	}

	void LPC_AutoCorrelation::ComputeAutocorrelation(const Array<TData>& signal,
							 Array<TData>& acCoeffs)
	{
		//unsigned size = pow(2.,Round(log10(2.*signal.GetSize()-1.)/log10(2.)));
		int k = 0;
		TData norm, N = TData( signal.Size() );
		const TData *inBuffer = signal.GetPtr();
		const TData *endInBuffer = signal.GetPtr() + signal.Size();
		TData *outBuffer = acCoeffs.GetPtr();
		const TData *endOutBuffer = acCoeffs.GetPtr()+acCoeffs.Size();

		const TData *inBuffer2 = NULL;

		*outBuffer = dot_product( inBuffer, inBuffer, endInBuffer );
		*outBuffer *= N;
		norm = 1.0/ *outBuffer;
		*outBuffer *= norm;
		k++;
		outBuffer++;
		
		while( outBuffer != endOutBuffer )
		{
			inBuffer2 = inBuffer;
			inBuffer += k;

			*outBuffer = dot_product( inBuffer, inBuffer2, endInBuffer );
			*outBuffer*= N;
			*outBuffer *= norm;
			
			inBuffer = signal.GetPtr();
			outBuffer++;
			k++;
		}


	}

	void LPC_AutoCorrelation::SolveSystemByLevinsonDurbin( const Array<TData>& R,
							       Array<TData>& A,
							       Array<TData>& K,
							       TData&        E)
	{
		CLAM_ASSERT( A.Size() == mCurrentConfig.GetOrder(), 
					"A coefficient array size mismatch!" );
		CLAM_ASSERT( K.Size() == mCurrentConfig.GetOrder(), 
					"K coefficient array size mismatch!" );
		unsigned j;
		DataArray Ap;
		Ap.Resize( mCurrentConfig.GetOrder() );
		Ap.SetSize( mCurrentConfig.GetOrder() );

		E = R[0];
		A[ 0 ] = 1;

		for( unsigned i = 1 ; i < mCurrentConfig.GetOrder(); i++ )	
		{
			K[ i ] = R[ i ];
			for( j = 1; j < i; j++ )
				K[ i ] += A[ j ] * R[ i - j ];

			K[ i ] = - K[ i ] / E;

			for( j = 1; j < i; j++ )
				Ap[ j ] = A[ i - j ];

			for( j = 1; j < i; j++ )
				A[ j ] += K[ i ] * Ap[ j ];

			A[ i ] = K[ i ];

			E *= (1-K[ i ]*K[ i ]);
		}
		
	}
}
