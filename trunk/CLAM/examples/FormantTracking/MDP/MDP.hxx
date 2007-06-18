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

#ifndef _MDP_
#define _MDP_

#include "ProcessingData.hxx"
#include "MatrixTmpl.hxx"

namespace CLAM
{

	class MDP: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MDP, 4, ProcessingData);
		/** The maximum number of formant candidates */
		DYN_ATTRIBUTE (0, public, TSize , NCandidates);
		/** The number of formant candidates to track */
		DYN_ATTRIBUTE (1, public, TSize, NToTrack );
		/** The total number of states per frame **/
		DYN_ATTRIBUTE (2, public, TSize, NStates );
		/** The number of frames **/
		DYN_ATTRIBUTE (3, public, TSize, NFrames );
	protected:

		void DefaultInit( void ); // To be used from the dynamic-type-generated constructor.

		inline void CheckTypes( void ) const;

	public:

		bool SetAccStates( const unsigned int f, const unsigned int s );

		void AllocateMem( void );

		inline const TSize GetStateTable( const unsigned int s, 
										  const unsigned int cand  ) const
		{ return mStateTable( s, cand ); }

		inline void SetStateTable( const unsigned int s, const unsigned int cand,
								   const unsigned int val )
		{ mStateTable( s, cand ) = val; }

		inline const TSize GetNStatesPerFrame( const unsigned int f ) const
		{ return mNStatesPerFrame[f]; }

		inline void SetProbTransition( const unsigned int s1, const unsigned int s2,
									   const unsigned int k, const TData p )
		{ mProbMatrix[s1][s2][k] = p; }

		inline const TData &GetProbTransition( const unsigned int s1, const unsigned int s2, 
											   const unsigned int k ) const
		{ return mProbMatrix[s1][s2][k]; }

		inline void SetBeta( const unsigned int s, const unsigned int k, const TData b )
		{ mBeta( s, k ) = b; }

		inline const TData &GetBeta( const unsigned int s, const unsigned int k ) const
		{ return mBeta( s, k ); }

	private:

		/** The 3D matrix of probabilities and dimensions
			NStates x NStates x NFrames **/
		Array< Array< DataArray > >		mProbMatrix;
		/** The table of states, where rows are states 
			and columns the formant candidates to track**/
		MatrixTmpl<int>					mStateTable;
		/** The matrix of backward probabilities **/
		MatrixTmpl< TData >				mBeta;

		MatrixTmpl<int>					mAccStates;

		Array<int>						mNStatesPerFrame;
	
	};

} //namespace

#endif // MDP.hxx

