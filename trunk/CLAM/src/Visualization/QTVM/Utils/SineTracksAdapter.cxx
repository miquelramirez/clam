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

#include "SineTracksAdapter.hxx"

namespace CLAM
{
	namespace VM
	{
		SineTracksAdapter::SineTracksAdapter()
			: _segment(NULL), _matrix(NULL)
		{
			_trackBuilder.SetTrackList( _extractedList );
		}

		SineTracksAdapter::~SineTracksAdapter()
		{
		}

		SineTrackList& SineTracksAdapter::GetTrackList(const Segment& seg)
		{
			_matrix=NULL;
			_segment = &seg;

			CLAM_ASSERT( _segment->HasSamplingRate(), "Not a valid segment: it doesn't know its sampling rate " );
			CLAM_ASSERT( _segment->HasBeginTime(), "Not a valid segment: it doesn't know its begin time " );
			CLAM_ASSERT( _segment->HasEndTime(), "Not a valid segment: it doesn't know its end time " );

			// new object, new list
			_trackBuilder.Flush();
			_extractedList.clear();
			TransmitPeakArrays();
			
			return _extractedList;
		}

		SineTrackList& SineTracksAdapter::GetTrackList(const Array< SpectralPeakArray >& peakMtx)
		{
			_segment=NULL;
			_matrix = &peakMtx;

			_trackBuilder.Flush();
			_extractedList.clear();

			TransmitPeakArrays();

			return _extractedList;
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

			const Array<TIndex>* pIndexBuff = NULL;

			if ( thereAreIndexes )
				pIndexBuff = &peakArray.GetIndexArray();

			_partialBuffer.Resize( nPeaks );
			_partialBuffer.SetSize( nPeaks );

			for ( int i = 0; i < nPeaks; i++ )
			{
				_partialBuffer[i].mMag = magBuff[i];
				_partialBuffer[i].mFreq = freqBuff[i];
				_partialBuffer[i].mPhase = phaseBuff[i];
				_partialBuffer[i].mTrackId = (thereAreIndexes) ? (*pIndexBuff)[i] : i;
			}
				
			if ( peakArray.GetScale() == EScale::eLinear )
			{
				for ( int i=0; i < nPeaks; i++ )
					_partialBuffer[i].mMag = 20.0f*log10( _partialBuffer[i].mMag );
			}

			return true;
		}

		void SineTracksAdapter::TransmitPeakArrays()
		{
			CLAM_ASSERT( _segment!=NULL || _matrix!=NULL, "Precondition violation!" );

			TSize numFrames = 0;

			if ( _segment )
			{
				numFrames = _segment->GetnFrames();
				TSize idx = 0;
				
				while ( idx < numFrames )
				{
					const Frame& f = _segment->GetFrame( idx );
					
					if ( TranscribePeakArray( f.GetSpectralPeakArray() ) )
						_trackBuilder.AddFrame( _partialBuffer, idx );
					
					idx++;
				}
					
			}
			else if ( _matrix )
			{
				numFrames = _matrix->Size();
				TSize idx = 0;

				while( idx < numFrames )
				{
					if ( TranscribePeakArray( (*_matrix)[idx] ) )
						_trackBuilder.AddFrame( _partialBuffer, idx );
					idx++;
				}
			}
		}
	}
}

// END

