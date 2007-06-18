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

#include "FormantTracker.hxx"

namespace CLAM
{
	void FormantTrackerConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();

		SetNCandidates(1);
		SetSamplingRate(44100);
		SetSemitoneRange( TData(.2599) );  // 4 semitones
		SetAmplitudeRange( 2. );
	}

	FormantTracker::FormantTracker() :
		mNCandidates( 0 ), mNFrames( 0 )
	{
		FormantTrackerConfig cfg;

		AttachChildren();

	}

	FormantTracker::FormantTracker( FormantTrackerConfig &cfg ) :
		mNCandidates( 0 ), mNFrames( 0 )
	{
		AttachChildren();
	}

	bool FormantTracker::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		bool res = true;
		CopyAsConcreteConfig(mConfig,cfg);

		try{

			if( mConfig.HasNCandidates() )
			{
				if( mConfig.HasSemitoneRange() )
					mSemitoneRange = mConfig.GetSemitoneRange();
				if( mConfig.HasAmplitudeRange() )
					mAmplitudeRange = mConfig.GetAmplitudeRange();
				if( mConfig.HasNToTrack() )
					mNToTrack = mConfig.GetNToTrack();

				CLAM_ASSERT( mConfig.GetNCandidates() >= 0 ,
				 "Wrong (negative) number of required candidates in FormantTracker configuration");

				if( mConfig.HasNCandidates() )
					mNCandidates = mConfig.GetNCandidates();
				if( mConfig.HasHopSize() )
					mHopSize = mConfig.GetHopSize();
				if( mConfig.HasSamplingRate() )
					mSamplingRate = mConfig.GetSamplingRate();

				TData downFactor = floor( mSamplingRate / ( 2 * mConfig.GetFmax() ) );

				mWinSize = round(mConfig.GetMsWinSize()*1e-3*mSamplingRate/downFactor);

				mNFrames = floor( mConfig.GetAudioSize() / downFactor / mConfig.GetHopSize() );


				if( !ConfigureChildren() )
				{
					AddConfigErrorMessage("\nFailed to configure children:");
					res = false;			
				}
			}
		}
		catch( std::exception &e )
		{
			AddConfigErrorMessage("Failed to configure children:\n");
			AddConfigErrorMessage( e.what() );
			return false;			
		}

		return res;
	}

	bool FormantTracker::ConcreteStart(void)
	{
		ProcessingComposite::ConcreteStart();

		return true;
	}

	bool FormantTracker::ConcreteStop()
	{
		ProcessingComposite::ConcreteStop();
		
		return true;
	}

	void FormantTracker::AttachChildren(void)
	{
		mFormantExtractor.SetParent(this);
		mMDPModeler.SetParent(this);
		mTrackFinder.SetParent(this);
	}

	bool FormantTracker::ConfigureChildren(void)
	{
		FormantExtractorConfig feConfig;

		feConfig.SetName( "FormantExtractor" );
		feConfig.SetFmax( mConfig.GetFmax() );
		feConfig.SetFmin( mConfig.GetFmin() );
		feConfig.SetHopSize( mHopSize );
		feConfig.SetNCandidates( mNCandidates );
		feConfig.SetSamplingRate( mSamplingRate );
		feConfig.SetNFrames( mNFrames );
		feConfig.SetWinSize( mWinSize );
		feConfig.SetWindowType( mConfig.GetWindowType() );

		if( !mFormantExtractor.Configure( feConfig ) ) 
		{
			AddConfigErrorMessage("mFormantExtractor: ");
			AddConfigErrorMessage( mFormantExtractor.GetConfigErrorMessage() );
			return false;
		}

		MDPModelerConfig MDPModConfig;

		MDPModConfig.SetName( "MDPModeler" );
		MDPModConfig.SetSemitoneRange( mSemitoneRange );
		MDPModConfig.SetAmplitudeRange( mAmplitudeRange );
		MDPModConfig.SetNToTrack( mNToTrack );
		MDPModConfig.SetNCandidates( mNCandidates );
		MDPModConfig.SetNFrames(mNFrames);

		if( !mMDPModeler.Configure( MDPModConfig ) ) 
		{
			AddConfigErrorMessage("mMDPModeler: ");
			AddConfigErrorMessage( mMDPModeler.GetConfigErrorMessage() );
			return false;
		}

		TrackFinderConfig	tfCfg;

		tfCfg.SetName( "TrackFinder" );
		tfCfg.SetNToTrack( mNToTrack );
		tfCfg.SetNCandidates( mNCandidates );
		tfCfg.SetNFrames( mNFrames );

		if( !mTrackFinder.Configure( tfCfg ) ) 
		{
			AddConfigErrorMessage("mTrackFinder: ");
			AddConfigErrorMessage( mTrackFinder.GetConfigErrorMessage() );
			return false;
		}

		return true;
	}

	bool FormantTracker::Do( const Audio &in, Array< SpectralPeakArray > &out)
	{
		Candidates candidates;
		MDP mdp;

		candidates.SetNFrames( mNFrames );
		candidates.SetNCandidates( mNCandidates );

		if( !AbleToExecute() ) return true;

		mFormantExtractor.Do( in, candidates );
		mNFrames=mFormantExtractor.GetNFrames();
		mMDPModeler.SetNFrames(mNFrames);
		mTrackFinder.SetNFrames(mNFrames);
		mMDPModeler.Do( candidates, mdp );
		mTrackFinder.Do( candidates, mdp, out );

		return true;
	}

} //namespace 
