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

#ifndef _TrackFinder_
#define _TrackFinder_

#include "MDP.hxx"
#include "Candidates.hxx"
#include "SpectralPeakArray.hxx"
#include "Processing.hxx"

namespace CLAM
{
	class TrackFinderConfig: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (TrackFinderConfig, 4, ProcessingConfig);
		/** The processing name **/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** The number of candidates to track **/
		DYN_ATTRIBUTE (1, public, TSize, NToTrack);
		/** The total number of formant candidates **/
		DYN_ATTRIBUTE (2, public, TSize, NCandidates);
		/** The number of frames **/
		DYN_ATTRIBUTE (3, public, TSize, NFrames);
	protected:

		void DefaultInit( void );
	
	};

	class TrackFinder: public Processing
	{
	public:

		TrackFinder();

		TrackFinder( TrackFinderConfig &cfg );

		virtual ~TrackFinder(){}

		const char* GetClassName( void ) const { return "TrackFinder"; }

		bool Do( void ) { return true; }

		bool Do( const Candidates& candidates, const MDP& mdp, Array<SpectralPeakArray> &formantsTracks );

		const ProcessingConfig &GetConfig( void ) const { return mConfig; }

		inline void SetNFrames( const TSize &s ) { mNFrames = s; }

	protected:

		bool ConcreteConfigure( const ProcessingConfig& ) throw( std::bad_cast );

		bool ConcreteStart();

		bool ConcreteStop();

		inline void CheckTypes( const Matrix &candidates, const MDP &mdp, const Matrix &freqFormant ) const;

	private:

		TrackFinderConfig	mConfig;

		Array<int>				mStateTrace;

		TSize					mNCandidates;

		TSize					mNToTrack;

		TSize					mNFrames;

	};
}

#endif // TrackFinder.hxx

