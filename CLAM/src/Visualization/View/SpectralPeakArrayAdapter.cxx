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

				if ( !peakArray.GetIndexArray().Size() )
					return false;

				const Array<TIndex>& indexBuff = peakArray.GetIndexArray();

				mPartialBuffer.Resize( nPeaks );
				mPartialBuffer.SetSize( nPeaks );

				for ( int i = 0; i < nPeaks; i++ )
				{
						mPartialBuffer[i].mMag = magBuff[i];
						mPartialBuffer[i].mFreq = freqBuff[i];
						mPartialBuffer[i].mPhase = phaseBuff[i];
						mPartialBuffer[i].mTrackId = indexBuff[i];
				}
				
				if ( peakArray.GetScale() == EScale::eLinear )
				{
						for ( int i=0; i < nPeaks; i++ )
								mPartialBuffer[i].mMag = 20.0f*log10( mPartialBuffer[i].mMag );
				}

				return true;
		}
}
