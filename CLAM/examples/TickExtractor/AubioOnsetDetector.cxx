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

#include "AubioOnsetDetector.hxx"
#include <algorithm>

namespace CLAM
{
	namespace RhythmDescription
	{
		EODAlgorithms::EODAlgorithms()
			: Enum( sEnumValues, sDefault )
		{
		}

		EODAlgorithms::EODAlgorithms( tValue v )
			: Enum( sEnumValues, v )
		{
		}

		EODAlgorithms::EODAlgorithms( std::string s )
			: Enum( sEnumValues, s )
		{
		}

		Enum::tEnumValue EODAlgorithms::sEnumValues[] = {
			{ eEnergy, "Energy" },
			{ eSpectralDifference, "Spectral difference" },
			{ eHFC, "HFC" },
			{ eComplexDomain, "Complex domain"},
			{ ePhase, "Phase" },
			{ 0,      NULL }
		};
		
		Enum::tValue EODAlgorithms::sDefault = EODAlgorithms::eEnergy;

		Component* EODAlgorithms::Species() const
		{
			return new EODAlgorithms( "Energy method");
		}

		void AubioOnsetDetectorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();

			SetOnsetThreshold( 0.1 );
			SetSilenceThreshold( -1000. );
			SetWindowSize( 1024 );
			SetHopSize( 512 );
			SetMethod( EODAlgorithms::eEnergy );
		}

		AubioOnsetDetector::AubioOnsetDetector()
			: mPhaseVocoder( NULL ),
			  mOnsetDetector( NULL ),
			  mPeakPickingParms( NULL ),
			  mCurrentFrame( NULL ),
			  mSpectrum( NULL ),
			  mOnsetBuffer( NULL ),
			  mInputAudio( NULL ), 
			  mOnsetType( energy )
		{
		}

		AubioOnsetDetector::~AubioOnsetDetector()
		{
			if ( mCurrentFrame != NULL )
				del_fvec( mCurrentFrame );
			if ( mSpectrum != NULL )
				del_cvec( mSpectrum );
			if ( mPhaseVocoder!= NULL )
				del_aubio_pvoc( mPhaseVocoder );
			if ( mOnsetDetector != NULL )
				aubio_onsetdetection_free( mOnsetDetector );
			if ( mOnsetBuffer != NULL )
				del_fvec( mOnsetBuffer );
			if ( mInputAudio != NULL )
				del_fvec( mInputAudio );

		}

		const char* AubioOnsetDetector::GetClassName() const
		{
			return "AubioOnsetDetector";
		}

		const ProcessingConfig& AubioOnsetDetector::GetConfig() const
		{
			return mConfig;
		}

		bool AubioOnsetDetector::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );
			
			if ( mCurrentFrame != NULL )
				del_fvec( mCurrentFrame );
			mCurrentFrame = new_fvec( mConfig.GetHopSize(), 1 );
			
			if ( mSpectrum != NULL )
				del_cvec( mSpectrum );
			mSpectrum = new_cvec( mConfig.GetWindowSize(), 1 );

			if ( mPhaseVocoder!= NULL )
				del_aubio_pvoc( mPhaseVocoder );
			mPhaseVocoder = new_aubio_pvoc( mConfig.GetWindowSize(), mConfig.GetHopSize(), 1 );

			mPeakPickingParms = new_aubio_peakpicker( mConfig.GetOnsetThreshold() );

			if ( mOnsetDetector != NULL )
				aubio_onsetdetection_free( mOnsetDetector );
				
			if ( mConfig.GetMethod() == 0 )
				mOnsetType = energy;
			else if ( mConfig.GetMethod() == 1 )
				mOnsetType = specdiff;
			else if ( mConfig.GetMethod() == 2 )
				mOnsetType = hfc;
			else if ( mConfig.GetMethod() == 3 )
				mOnsetType = complexdomain;
			else if ( mConfig.GetMethod() == 4 )
				mOnsetType = phase;

			mOnsetDetector = new_aubio_onsetdetection( mOnsetType , mConfig.GetWindowSize(), 1 );
			
			if ( mOnsetBuffer != NULL )
				del_fvec( mOnsetBuffer );
			mOnsetBuffer = new_fvec( mConfig.GetHopSize(), 1 );

			for ( int frames = 0; frames < mConfig.GetHopSize(); frames++ )
				mOnsetBuffer->data[0][frames] = 0.;

			if ( mInputAudio != NULL )
				del_fvec( mInputAudio );
			mInputAudio = new_fvec( mConfig.GetHopSize(), 1 );

			
			return true;
		}

		bool AubioOnsetDetector::Do()
		{
			return false;
		}

		bool AubioOnsetDetector::Do( Audio& input, Array<TimeIndex>& onsets )
		{
			TIndex offset = 0;
			TData* inSamples = input.GetBuffer().GetPtr();
			int isOnset = 0;
			int frames = 0;
			
			// MRJ: it seems that there exists a bug that obliges to
			// execute twice the PhaseVocoder before it can be
			// actually used
			for ( frames = 0; frames < 2; frames++ )
				aubio_pvoc_do( mPhaseVocoder, mInputAudio, mSpectrum );

			frames++;
			
			std::copy( inSamples, inSamples + mConfig.GetHopSize(), fvec_get_channel( mInputAudio, 0 ) );		       
			offset += mConfig.GetHopSize();			
			
			aubio_pvoc_do( mPhaseVocoder, mInputAudio, mSpectrum );
			aubio_onsetdetection( mOnsetDetector, mSpectrum, mOnsetBuffer );


			//frames = 3;
			while( offset < input.GetSize() - mConfig.GetHopSize() )
			{
				std::copy( inSamples+offset, inSamples+offset+mConfig.GetHopSize(),
					   fvec_get_channel( mInputAudio, 0 ) );

				offset += mConfig.GetHopSize();

				isOnset = 0;

				unsigned int i, j ;
				int pos = 0;
				smpl_t peakWeight = 0.0;

				for ( j = 0; j < mConfig.GetHopSize(); j++ )
				{
					fvec_write_sample( mCurrentFrame, mInputAudio->data[0][j], 0, pos );
					
					if ( pos == mConfig.GetHopSize() - 1 )
					{
						if ( aubio_silence_detection( mCurrentFrame, mConfig.GetSilenceThreshold()) == 0 )
						{

							aubio_pvoc_do( mPhaseVocoder, mCurrentFrame, mSpectrum );
							aubio_onsetdetection( mOnsetDetector, mSpectrum, mOnsetBuffer );
							
							isOnset = aubio_peakpick_pimrt_wt( mOnsetBuffer, mPeakPickingParms, &peakWeight );
							
							/*
							  if ( isOnset )
							  if ( aubio_silence_detection( mCurrentFrame, mConfig.GetSilenceThreshold() ) == 1 )
							  isOnset = 0;
							*/
						}
						pos = -1;

					}
					
					pos++;
				}

				if ( isOnset )
				{
					TimeIndex newOnset;
					newOnset.SetPosition( (frames-3)*mConfig.GetHopSize() );
					newOnset.SetWeight( peakWeight );
					
					onsets.AddElem( newOnset );
				}
				
				frames++;
			}

			// Detected onsets' weight normalization

			if ( onsets.Size() > 0 )
			{

				TData max = 0.0;
				
				for ( int i = 0; i < onsets.Size(); i++ )
				{
					max = ( onsets[i].GetWeight() > max ) ? onsets[i].GetWeight() : max;
				}

				const TData invMax = 1.0 / max;

				for ( int i = 0; i < onsets.Size(); i++ )
				{
					onsets[i].SetWeight( onsets[i].GetWeight()*invMax );
				}
			}

			return true;
		}


	}
}

