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

#ifndef _MDPModeler_
#define _MDPModeler_

#include "MDP.hxx"
#include "Candidates.hxx"
#include "Processing.hxx"
#include "MatrixTmpl.hxx"

namespace CLAM
{

	class MDPModelerConfig: public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (MDPModelerConfig, 6, ProcessingConfig);
		/** The processing name **/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** The semitone range **/
		DYN_ATTRIBUTE (1, public, TData, SemitoneRange);
		/** The amplitude range **/
		DYN_ATTRIBUTE (2, public, TData, AmplitudeRange);
		/** The number of candidates to track **/
		DYN_ATTRIBUTE (3, public, TSize, NToTrack);
		/** The total number of formant candidates **/
		DYN_ATTRIBUTE (4, public, TSize, NCandidates);
		/** The number of frames **/
		DYN_ATTRIBUTE (5, public, TSize, NFrames);
	protected:

		void DefaultInit( void );
	};

	class MDPModeler: public Processing
	{
	public:

		MDPModeler();

		MDPModeler( MDPModelerConfig &cfg );

		virtual ~MDPModeler() {}

		const char* GetClassName( void ) const { return "MPDModeler"; }

		bool Do( void ) { return true; }

		bool Do( const Candidates &candidates, MDP &mdp ) ;

		const ProcessingConfig &GetConfig( void ) const { return mConfig; }

		inline void SetNFrames( const TSize &s ) { mNFrames = s; }

	protected:

		bool ConcreteConfigure( const ProcessingConfig& ) throw( std::bad_cast );

		bool ConcreteStart();

		bool ConcreteStop();

		void CalculateNStates( void );

		inline void CheckTypes( const Candidates &candidates, const MDP &mdp ) const;

		void GenerateStates( const Candidates& candidates, MDP& mdp );

		TData GetProbTrans( const unsigned int s1, const unsigned int s2,
							const unsigned int k, const Candidates &c,
							const MDP &mdp );

		bool HandleLastFrame( const Candidates &candidates, MDP &mdp );

		bool HandleFirstFrame( const Candidates &candidates, MDP &mdp );

		bool HandleMidFrames( const Candidates &candidates, MDP &mdp );

		void Permute( const unsigned &ntrack, const unsigned &ncandidates, MDP& mdp, 
					  MatrixTmpl<bool> &binState, unsigned &nstates, Array<bool> s );

	private:

		MDPModelerConfig			mConfig;

		TSize						mNCandidates;

		TSize						mNToTrack;

		TSize						mNFrames;

		TSize						mNStates;

		TData						mSemitoneRange;

		TData						mAmplitudeRange;

	};

}	// namespace

#endif // MDPModeler.hxx

