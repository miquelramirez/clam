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

#ifndef _Candidates_
#define _Candidates_

#include "ProcessingData.hxx"
#include "Matrix.hxx"

namespace CLAM
{
	class Candidates:public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Candidates, 5, ProcessingData);
		/** The matrix of frequencies values per frame */
		DYN_ATTRIBUTE (0, public, Matrix, FreqMatrix);
		/** The matrix of magnitudes values per frame */
		DYN_ATTRIBUTE (1, public, Matrix, MagMatrix);
		/** The total number of candidates per frame **/
		DYN_ATTRIBUTE (2, public, Array<TSize>, NCandPerFrame);
		/** The maximum number of candidates per frame **/
		DYN_ATTRIBUTE (3, public, TSize, NCandidates);
		/** The number of frames **/
		DYN_ATTRIBUTE (4, public, TSize, NFrames);
	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
		}

	public:
		
		void Allocate( const unsigned nCandidates, const unsigned nFrames )
		{
			SetNCandidates(nCandidates);
			SetNFrames(nFrames);

			GetNCandPerFrame().Resize(GetNFrames());
			GetNCandPerFrame().SetSize(GetNFrames());

			SetFreqMatrix( Matrix(GetNCandidates(), GetNFrames() ) );
			SetMagMatrix( Matrix(GetNCandidates(), GetNFrames() ) );
		}
	};
}	//namespace

#endif // Candidates.hxx

