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

#include <CLAM/SineTracksAdapter.hxx>

namespace CLAM
{
	namespace VM
	{
		SineTracksAdapter::SineTracksAdapter()
			: mSegment(0) 
			, mMatrix(0)
		{
			mTrackBuilder.SetTrackList( mExtractedList );
		}

		SineTracksAdapter::~SineTracksAdapter()
		{
		}

		SineTrackList& SineTracksAdapter::GetTrackList(const Segment& seg)
		{
			mMatrix = 0;
			mSegment = &seg;

			CLAM_ASSERT( mSegment->HasSamplingRate(), "Not a valid segment: it doesn't know its sampling rate " );
			CLAM_ASSERT( mSegment->HasBeginTime(), "Not a valid segment: it doesn't know its begin time " );
			CLAM_ASSERT( mSegment->HasEndTime(), "Not a valid segment: it doesn't know its end time " );

			// new object, new list
			mTrackBuilder.Flush();
			mExtractedList.clear();
			TransmitPeakArrays();
			
			return mExtractedList;
		}

		SineTrackList& SineTracksAdapter::GetTrackList(const Array< SpectralPeakArray >& peakMtx)
		{
			mSegment = 0;
			mMatrix = &peakMtx;

			mTrackBuilder.Flush();
			mExtractedList.clear();

			TransmitPeakArrays();

			return mExtractedList;
		}

		bool SineTracksAdapter::TranscribePeakArray( const SpectralPeakArray& peakArray )
		{
			TSize nPeaks = peakArray.GetMagBuffer().Size();
				
			CLAM_DEBUG_ASSERT(	nPeaks==peakArray.GetFreqBuffer().Size() &&
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

			const Array<TIndex>* pIndexBuff = 0;

			if ( thereAreIndexes )
				pIndexBuff = &peakArray.GetIndexArray();

			mPartialBuffer.Resize( nPeaks );
			mPartialBuffer.SetSize( nPeaks );

			bool isLinear = (peakArray.GetScale() == EScale::eLinear);

			for ( int i = 0; i < nPeaks; i++ )
			{
				mPartialBuffer[i].mMag = (isLinear) ? 20.0f*log10(magBuff[i]) : magBuff[i];
				mPartialBuffer[i].mFreq = freqBuff[i];
				mPartialBuffer[i].mPhase = phaseBuff[i];
				mPartialBuffer[i].mTrackId = (thereAreIndexes) ? (*pIndexBuff)[i] : i;
			}
			return true;
		}

		void SineTracksAdapter::TransmitPeakArrays()
		{
			CLAM_ASSERT( mSegment!=0 || mMatrix!=0, "Precondition violation!" );

			TSize numFrames = 0;

			if ( mSegment )
			{
				numFrames = mSegment->GetnFrames();
				TSize idx = 0;
				
				while ( idx < numFrames )
				{
					const Frame& f = mSegment->GetFrame( idx );
					
					if ( TranscribePeakArray( f.GetSpectralPeakArray() ) )
						mTrackBuilder.AddFrame( mPartialBuffer, idx );
					
					idx++;
				}
					
			}
			else if ( mMatrix )
			{
				numFrames = mMatrix->Size();
				TSize idx = 0;

				while( idx < numFrames )
				{
					if ( TranscribePeakArray( (*mMatrix)[idx] ) )
						mTrackBuilder.AddFrame( mPartialBuffer, idx );
					idx++;
				}
			}
		}
	}
}

// END

