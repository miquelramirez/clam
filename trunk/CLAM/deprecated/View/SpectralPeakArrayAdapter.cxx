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

#include "SpectralPeakArrayAdapter.hxx"
#include "SpectralPeakArray.hxx"
#include "Assert.hxx"
#include "GlobalEnums.hxx"
#include <cmath>

namespace CLAMVM
{
		using CLAM::EScale;
		using CLAM::DataArray;
		using CLAM::TSize;

		SpectralPeakArrayAdapter::SpectralPeakArrayAdapter()
				: mObserved( NULL )
		{
		}

		SpectralPeakArrayAdapter::~SpectralPeakArrayAdapter()
		{
		}

		bool SpectralPeakArrayAdapter::BindTo( const ProcessingData& obj )
		{
				mObserved = dynamic_cast< const SpectralPeakArray* >( &obj );

				if ( !mObserved ) return false;

				return true;
		}

		bool SpectralPeakArrayAdapter::Publish()
		{
				if ( !mObserved )
						return false;
				
				if ( !TranscribePeakArray( *mObserved ) )
					return false;

				ObjectPublished.Emit( mPartialBuffer );

				return true;
		}

		bool SpectralPeakArrayAdapter::TranscribePeakArray( const SpectralPeakArray& peakArray )
		{
				TSize nPeaks = peakArray.GetMagBuffer().Size();
				
				CLAM_DEBUG_ASSERT( nPeaks==peakArray.GetFreqBuffer().Size() &&
								   nPeaks==peakArray.GetPhaseBuffer().Size(),
								   "Inconsistency detected at SpectralPeakArray: buffers differ in size" );

				if ( !peakArray.GetMagBuffer().Size() )
					return false;
				
				const DataArray& magBuff = peakArray.GetMagBuffer();
				
				if ( !peakArray.GetFreqBuffer().Size() )
					return false;

				const DataArray& freqBuff = peakArray.GetFreqBuffer();
				
				if ( !peakArray.GetPhaseBuffer().Size() )
					return false;

				const DataArray& phaseBuff = peakArray.GetPhaseBuffer();

				bool thereAreIndexes = peakArray.HasIndexArray();

				const Array<TIndex>* pIndexBuff = NULL;

				if ( thereAreIndexes )
					pIndexBuff = &peakArray.GetIndexArray();

				mPartialBuffer.Resize( nPeaks );
				mPartialBuffer.SetSize( nPeaks );

				for ( int i = 0; i < nPeaks; i++ )
				{
						mPartialBuffer[i].mMag = magBuff[i];
						mPartialBuffer[i].mFreq = freqBuff[i];
						mPartialBuffer[i].mPhase = phaseBuff[i];
						mPartialBuffer[i].mTrackId = (thereAreIndexes)? (*pIndexBuff)[i]:i;
				}
				
				if ( peakArray.GetScale() == EScale::eLinear )
				{
						for ( int i=0; i < nPeaks; i++ )
								mPartialBuffer[i].mMag = 20.0f*log10( mPartialBuffer[i].mMag );
				}

				return true;
		}
}

