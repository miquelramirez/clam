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

