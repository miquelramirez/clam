/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "Factory.hxx"
#include "SpecTypeFlags.hxx"

namespace CLAM
{
	namespace detail 
	{
		static Factory<Processing>::Registrator<LPC_AutoCorrelation> 
			registrator("LPC_AutoCorrelation");
	}
	void LPCConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		// MRJ: Seems that eleven is a 'wise' number
		SetOrder( 11 );
	}

	LPC_AutoCorrelation::LPC_AutoCorrelation()
		: mAudioIn("AudioIn",this)
		, mLPModelOut("LPModelOut",this)
		, mSpectrumOut("SpectrumOut",this)
	{
		Configure(LPCConfig());
	}

	LPC_AutoCorrelation::~LPC_AutoCorrelation()
	{
	}

	bool LPC_AutoCorrelation::Do()
	{
		const Audio & audio = mAudioIn.GetData();
		LPModel & lpc = mLPModelOut.GetData();
		Spectrum & spectrum = mSpectrumOut.GetData();

		lpc.UpdateModelOrder(mCurrentConfig.GetOrder());
		bool ok = Do(audio, lpc);
		CLAM::SpecTypeFlags flags;
		flags.bMagPhase=1;
		flags.bComplex = 0;
		spectrum.SetSize( audio.GetSize()/2+1 );
		spectrum.SetSpectralRange( audio.GetSampleRate()/2 );
		spectrum.SetType(flags);
		lpc.ToSpectrum(spectrum);

		mAudioIn.Consume();
		mLPModelOut.Produce();
		mSpectrumOut.Produce();
		return ok;
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
	
  		if( !AbleToExecute() ) return true;
		
  		TData * inBuffer = in.GetBuffer().GetPtr();
		TData * outBuffer = out.GetBuffer().GetPtr();
		TData norm = 1 / outBuffer[0];
		for (int k = 0; k < out.GetSize(); k++ )
		{
			for (int n = 0; n < in.GetSize(); n++ )
			{
				if( n < k )	// k is out of the segment
					outBuffer[ k ] += 0;
				else
					outBuffer[ k ] += inBuffer[ n ] * inBuffer[ n - k ] ;
			}

			outBuffer[ k ] *= in.GetSize() ;
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
			accum+= (*in1++)*(*in2++);
		return accum;
	}

	void LPC_AutoCorrelation::ComputeAutocorrelation(const Array<TData>& signal,
							 Array<TData>& acCoeffs)
	{
		//unsigned size = pow(2.,Round(log10(2.*signal.GetSize()-1.)/log10(2.)));
		int k = 0;
		TData N = TData( signal.Size() );
		const TData *inBuffer = signal.GetPtr();
		const TData *endInBuffer = signal.GetPtr() + signal.Size();
		TData *outBuffer = acCoeffs.GetPtr();
		const TData *endOutBuffer = acCoeffs.GetPtr()+acCoeffs.Size();

		const TData *inBuffer2 = NULL;

		*outBuffer = dot_product( inBuffer, inBuffer, endInBuffer );
		*outBuffer *= N;
		TData norm = 1.0/ *outBuffer;
		*outBuffer *= norm;
		outBuffer++;
		k++;
		
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
		unsigned order = mCurrentConfig.GetOrder();
		CLAM_ASSERT( A.Size() == order,
					"A coefficient array size mismatch!" );
		CLAM_ASSERT( K.Size() == order,
					"K coefficient array size mismatch!" );

		std::vector <TData> Ap(order);
		E = R[0];
		A[0] = 1;
		for( unsigned i = 1 ; i < order; i++ )	
		{
			K[i] = R[i];
			for(unsigned j=1; j<i; j++ )
				K[i] += A[j] * R[i-j];

			K[i] = - K[i] / E;

			for( unsigned j=1; j<i; j++ )
				Ap[j] = A[i-j];

			for( unsigned j=1; j<i; j++ )
				A[j] += K[i] * Ap[j];

			A[i] = K[i];
			E *= (1-K[i]*K[i]);
		}
	}
}

