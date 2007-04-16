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

#include "SinTracksAdapter.hxx"
#include "Segment.hxx"
#include "Frame.hxx"
#include "Array.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAMVM
{
	using CLAM::Frame;
	using CLAM::SpectralPeakArray;
	using CLAM::TSize;
		
	SinTracksAdapter::SinTracksAdapter()
		: mObserved( NULL ), mMatrixObserved( NULL ), mMatrixSampleRate ( 0 )
	{
		mTrackBuilder.SetTrackList( mExtractedList );

	}

	SinTracksAdapter::~SinTracksAdapter()
	{
	}

	bool SinTracksAdapter::BindTo( const ProcessingData& obj )
	{
		mMatrixObserved = NULL;
		mObserved = dynamic_cast< const Segment* >( &obj );

		CLAM_ASSERT( mObserved->HasSamplingRate(), "Not a valid segment: it doesn't know its sampling rate " );
		CLAM_ASSERT( mObserved->HasBeginTime(), "Not a valid segment: it doesn't know its begin time " );
		CLAM_ASSERT( mObserved->HasEndTime(), "Not a valid segment: it doesn't know its end time " );

		if ( !mObserved ) return false;

		// new object, new list
		mTrackBuilder.Flush();
		mExtractedList.clear();
		return true;
	}

	bool SinTracksAdapter::BindTo( const CLAM::Array< CLAM::SpectralPeakArray >& peakMtx, CLAM::TData sampleRate )
	{
		mObserved = NULL;
		mMatrixObserved = &peakMtx;
		mMatrixSampleRate = sampleRate;

		mTrackBuilder.Flush();
		mExtractedList.clear();
		return true;
	}

	bool SinTracksAdapter::Publish()
	{
		if ( !mObserved && !mMatrixObserved)
			return false;

		TransmitPeakArrays();
				
		if ( mObserved )
		{
			SpectralRangePublished.Emit( mObserved->GetSamplingRate()/2 );
			DurationPublished.Emit( mObserved->GetBeginTime(), mObserved->GetEndTime() );
		}
		else if ( mMatrixObserved )
		{
			SpectralRangePublished.Emit( mMatrixSampleRate/2 );
			DurationPublished.Emit( 0, mMatrixObserved->Size() );			
		}

		return true;
	}

	void SinTracksAdapter::TransmitPeakArrays()
	{
		CLAM_ASSERT( mObserved!=NULL || mMatrixObserved!=NULL, "Precondition violation!" );

		TSize numFrames = 0;

		if ( mObserved )
		{
			numFrames = mObserved->GetnFrames();
			TSize idx = 0;
			
			while ( idx < numFrames )
			{
				const Frame& f = mObserved->GetFrame( idx );
				
				if ( TranscribePeakArray( f.GetSpectralPeakArray() ) )
					mTrackBuilder.AddFrame( mPartialBuffer, idx );
				
				idx++;
			}
				
		}
		else if ( mMatrixObserved )
		{
			numFrames = mMatrixObserved->Size();
			TSize idx = 0;

			while( idx < numFrames )
			{
				if ( TranscribePeakArray( (*mMatrixObserved)[idx] ) )
					mTrackBuilder.AddFrame( mPartialBuffer, idx );
				idx++;
			}
		}

		TrackListPublished.Emit( mExtractedList, numFrames );
				
	}
}

