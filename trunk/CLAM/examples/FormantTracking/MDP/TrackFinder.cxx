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

#include "TrackFinder.hxx"
#include <fstream>
#include <iostream>

namespace CLAM
{
	void TrackFinderConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();

		SetNFrames(0);
	}

	TrackFinder::TrackFinder() :
		mNFrames( 0 )
	{
		TrackFinderConfig cfg;

		Configure( cfg );
	}

	TrackFinder::TrackFinder( TrackFinderConfig &cfg ) :
		mNFrames( 0 )
	{
		Configure( cfg );
	}

	bool TrackFinder::ConcreteStart(void)
	{
		return true;
	}

	bool TrackFinder::ConcreteStop(void)
	{
		return true;
	}

	bool TrackFinder::ConcreteConfigure( const ProcessingConfig& cfg ) throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const TrackFinderConfig& > (cfg);

		if( mConfig.HasNToTrack() )
			mNToTrack = mConfig.GetNToTrack();
		if( mConfig.HasNCandidates() )
			mNCandidates = mConfig.GetNCandidates();
		if( mConfig.HasNFrames() )
			mNFrames = mConfig.GetNFrames();

		mStateTrace.Resize( mNFrames );
		mStateTrace.SetSize( mNFrames );

		return true;
	}

	inline void TrackFinder::CheckTypes( const Matrix &freqMatrix, const MDP &mdp, const Matrix &freqFormant ) const
	{
		CLAM_BEGIN_CHECK
		
		if( freqMatrix.GetNumColumns() != mNFrames )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong columns size in matrix of frequencies\n"
			   << "	Expected: " << mNFrames << "	, used " << freqMatrix.GetNumColumns();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( freqMatrix.GetNumRows() != mNCandidates )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong rows size in matrix of frequencies\n"
			   << "	Expected: " << mNCandidates << "	, used " << freqMatrix.GetNumRows();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( freqFormant.GetNumRows() != mNToTrack )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong rows size in matrix of formant frequencies\n"
			   << "	Expected: " << mNToTrack << "	, used " << freqFormant.GetNumRows();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( freqFormant.GetNumColumns() != mNFrames )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong columns size in matrix of formant frequencies\n"
			   << "	Expected: " << mNFrames << "	, used " << freqFormant.GetNumColumns();
			CLAM_ASSERT(0,ss.str().c_str());
		}


		if( mdp.GetNCandidates() != mNCandidates )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong number of candidates in MDP class\n"
			   << "	Expected: " << mNCandidates << "	, used " << mdp.GetNCandidates();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( mdp.GetNFrames() != mNFrames )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong number of frames in MDP class\n"
			   << "	Expected: " << mNFrames << "	, used " << mdp.GetNFrames();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		if( mdp.GetNToTrack() != mNToTrack )
		{
			std::stringstream ss;

			ss << "TrackFinder::Do : Wrong number of candidates to track in MDP class\n"
			   << "	Expected: " << mNToTrack << "	, used " << mdp.GetNToTrack();
			CLAM_ASSERT(0,ss.str().c_str());
		}

		CLAM_END_CHECK
	}

	bool TrackFinder::Do( const Candidates &candidates, const MDP &mdp, Array<SpectralPeakArray> &formantTracks )
	{
		unsigned f, s, n, cand;
		TData A = 0;
		TData maxArg = 0;
		SpectralPeak spectralPeak;
		spectralPeak.AddPhase();
		spectralPeak.UpdateData();
		Matrix formantFreq( mNFrames, mNToTrack );

		if( !AbleToExecute() ) return true;

		//CheckTypes( freqMatrix, mdp, freqFormant );


		// First frame
		for( s = 0; s < mdp.GetNStatesPerFrame(0); s++ )
		{
			if( maxArg < mdp.GetBeta(s,0) )
			{
				maxArg = mdp.GetBeta(s,0); 
				mStateTrace[ 0 ] = s;
			}
		}

		maxArg = 0;
		formantTracks[ 0 ].RemoveBinWidthBuffer();
		formantTracks[ 0 ].RemoveBinPosBuffer();
		formantTracks[ 0 ].UpdateData();
		formantTracks[ 0 ].SetnPeaks( mNToTrack );
		formantTracks[ 0 ].SetScale( EScale( EScale::eLog ) );


		spectralPeak.SetScale( EScale( EScale::eLog ) );
		
		for( n = 0; n < mNToTrack; n++ )
		{
			cand = mdp.GetStateTable( mStateTrace[ 0 ], n ) - 1;
			spectralPeak.SetMag( candidates.GetMagMatrix()( cand, 0 ) );
			spectralPeak.SetFreq(  candidates.GetFreqMatrix()( cand, 0 ) );
			formantTracks[ 0 ].SetSpectralPeak( n, DB(spectralPeak), n);

//			to << candidates.GetFreqMatrix()( cand, 0 ) << " ";
		}

		for( f = 1; f < mNFrames; f++ )
		{
			formantTracks[ f ].RemoveBinWidthBuffer();
			formantTracks[ f ].RemoveBinPosBuffer();
			formantTracks[ f ].UpdateData();
			
			formantTracks[ f ].SetnPeaks( mNToTrack );
			formantTracks[ f ].SetScale( EScale( EScale::eLog ) );

			for( s = 0; s < mdp.GetNStatesPerFrame(f); s++ )
			{
				A = mdp.GetProbTransition(mStateTrace[f-1],s,f)*mdp.GetBeta(s,f);	

				if( maxArg < A )
				{
					maxArg = A; 
					mStateTrace[ f ] = s;
				}
			}

			maxArg = 0;

			for( n = 0; n < mNToTrack; n++ )
			{
				cand = mdp.GetStateTable( mStateTrace[ f ], n ) - 1;
				spectralPeak.SetMag( candidates.GetMagMatrix()( cand, f ) );
				spectralPeak.SetFreq(  candidates.GetFreqMatrix()( cand, f ) );
				formantTracks[ f ].SetSpectralPeak( n, DB(spectralPeak), n);				
				//formantTracks[ f ].SetSpectralPeak( n, spectralPeak, n);				

				//to << candidates.GetFreqMatrix()( cand, f ) << " ";
			}
			//std::cout << std::endl;
		}

		return true;
	}

} //namespace
