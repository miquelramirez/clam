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

#include "MDP.hxx"

namespace CLAM
{
	
	void MDP::DefaultInit( void )
	{
		AddAll();
		UpdateData();
	}

	bool MDP::SetAccStates( const unsigned int f, const unsigned int s )
	{
		unsigned int i;

		for( i = 0; i < mNStatesPerFrame[f]; i++)
			if( mAccStates(i,f) == s ) return true;

		mAccStates( mNStatesPerFrame[f], f ) = s;
		mNStatesPerFrame[f]++;

		return true;
	}


	inline void MDP::CheckTypes( void ) const
	{
		CLAM_BEGIN_CHECK

		CLAM_ASSERT(GetNCandidates()>0 ,"MPD::AllocateMem : Wrong number of total formant candidates.");

		CLAM_ASSERT(GetNToTrack()>0 ,"MPD::AllocateMem : Wrong number of formant candidates to track.");

		CLAM_ASSERT(GetNStates()>0 ,"MPD::AllocateMem : Wrong number of total states per frame.");

		CLAM_ASSERT(GetNFrames()>0 ,"MPD::AllocateMem : Wrong number of total frames.");

		CLAM_END_CHECK	
	}

	void MDP::AllocateMem( void )
	{
		unsigned int i,j;

		CheckTypes();

		mProbMatrix.Resize(GetNStates());
		mProbMatrix.SetSize(GetNStates());
		
		for( i=0; i<GetNStates(); i++ )
		{
			mProbMatrix[i].Resize(GetNStates());
			mProbMatrix[i].SetSize(GetNStates());

			for( j=0; j<GetNStates(); j++ )
			{
				mProbMatrix[i][j].Resize(GetNFrames());
				mProbMatrix[i][j].SetSize(GetNFrames());
			}
		}

		mStateTable = MatrixTmpl<int>(GetNStates(), GetNToTrack()); 

		mBeta = MatrixTmpl<TData>(GetNStates(), GetNFrames()); 

		mAccStates = MatrixTmpl<int>(GetNStates(), GetNFrames());

		mNStatesPerFrame.Resize( GetNFrames() );
		mNStatesPerFrame.SetSize( GetNFrames() );
	}
}	// namespace
