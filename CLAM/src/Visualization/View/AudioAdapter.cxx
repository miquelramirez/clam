#include "AudioAdapter.hxx"
#include "Audio.hxx"

namespace CLAMVM
{
		using CLAM::TSize;
		
		AudioAdapter::AudioAdapter()
				: mObserved( NULL )
		{
		}

		AudioAdapter::~AudioAdapter()
		{
		}

		bool AudioAdapter::BindTo( const ProcessingData& audioObj ) 
		{
				mObserved = dynamic_cast< const Audio* >( &audioObj );
			
				if ( !mObserved ) 
						return false;

				return true;
		}

		bool AudioAdapter::Publish()
		{
				if ( !mObserved )  // there is no object being observed
						return false;

				TData obsSR = 1.0f / mObserved->GetSampleRate();
				TSize nSamples = mObserved->GetBuffer().Size();

				SamplesPublished.Emit( mObserved->GetBuffer() );

				DurationPublished.Emit( nSamples * obsSR );

				StartTimePublished.Emit( mObserved->GetBeginTime() );

				SampleRatePublished.Emit( mObserved->GetSampleRate() );

				ObjectPublished.Emit( mObserved->GetBuffer(), 
									  mObserved->GetBeginTime(), 
									  nSamples * obsSR, 
									  mObserved->GetSampleRate()   ); 

				return true;
		}
}
