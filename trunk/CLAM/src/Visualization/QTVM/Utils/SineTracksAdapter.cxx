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

