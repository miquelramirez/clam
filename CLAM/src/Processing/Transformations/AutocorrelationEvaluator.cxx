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

#include "AutocorrelationEvaluator.hxx"

#include <string>

namespace CLAM
{
	void AutocorrelationEvaluatorConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetCorrelationSize( 0 );
	}

	AutocorrelationEvaluator::AutocorrelationEvaluator() :
		mSize( 0 )
	{
		AutocorrelationEvaluatorConfig cfg;

		Configure( cfg );
	}

	AutocorrelationEvaluator::AutocorrelationEvaluator( const AutocorrelationEvaluatorConfig& cfg ) :
		mSize( 0 )
	{
		Configure( cfg );
	}

	AutocorrelationEvaluator::~AutocorrelationEvaluator()
	{}

	bool AutocorrelationEvaluator::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast< const AutocorrelationEvaluatorConfig& > (cfg);

		if( mConfig.HasCorrelationSize() )
		{
			if( mConfig.GetCorrelationSize() < 0 )
				throw( ErrProcessingObj( "Wrong (negative) Size in Autocorrelation Evaluator Configuration", this ) );
			mSize = mConfig.GetCorrelationSize();
		}

		return true;
	}

	bool AutocorrelationEvaluator::ConcreteStart()
	{
		return true;
	}

	bool AutocorrelationEvaluator::ConcreteStop()
	{
		return true;
	}

	void AutocorrelationEvaluator::CheckTypes( const Audio& in, const Correlation& out) const
	{
		CLAM_BEGIN_CHECK

		if( out.GetSize() != mSize )
		{
			std::stringstream s;

			s << "Autocorrelation::Do: Wrong size in Autocorrelation output\n"
			  << "	Expected: " << mSize << " , used " << out.GetSize();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( !in.HasBuffer() )
			CLAM_ASSERT( 0, "Autocorrelation Do: Float attribute required for Audio object. " );

		CLAM_END_CHECK
	}

	bool AutocorrelationEvaluator::Do(const Audio& in, Correlation &out)
	{
/*** this is the "clean" version:
		TData *inBuffer, *outBuffer;
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
					outBuffer[ k ] = 0;
				else
					outBuffer[ k ] += inBuffer[ n ] * inBuffer[ n - k ] ;
			}

			outBuffer[ k ] /= in.GetSize() ;
		}
*/

/*
	The following does the same, but more efficient, by removing the condition
	from the for loop
*/
		int k = 0;
		TData *inBuffer, *inBuffer2, *outBuffer;
		TData *endInBuffer, *endOutBuffer;

		if( !AbleToExecute() ) return true;
		
		CheckTypes( in, out );

		inBuffer = in.GetBuffer().GetPtr();
		endInBuffer = inBuffer + in.GetSize();

		outBuffer = out.GetBuffer().GetPtr();
		endOutBuffer = outBuffer + out.GetSize();

		while( outBuffer != endOutBuffer )
		{
			inBuffer2 = inBuffer;
			inBuffer += k;

			while( inBuffer != endInBuffer )
				( *outBuffer ) += ( *inBuffer++ ) * ( *inBuffer2++ );

			( *outBuffer ) /= in.GetSize();
			inBuffer = in.GetBuffer().GetPtr();
			outBuffer++;
			k++;
		}

		return true;
	}
}