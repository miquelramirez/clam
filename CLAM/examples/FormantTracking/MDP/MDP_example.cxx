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

#include "MDPModeler.hxx"


int main()
{
	unsigned nToTrack, nCandidates, nFrames;
	double semitoneRange, amplitudeRange;

	nToTrack = 2;
	nCandidates = 4;
	nFrames = 5;
	semitoneRange = .2599; // 4 semitones
	amplitudeRange = 2;

	CLAM::Candidates cand;

	cand.Allocate( nCandidates, nFrames );

	cand.GetNCandPerFrame()[0]=nCandidates-2;
	cand.GetNCandPerFrame()[1]=nCandidates-1;
	for(unsigned n=2; n<nFrames; n++)
		cand.GetNCandPerFrame()[n]=nCandidates;

	// First Frame
	cand.GetFreqMatrix()( 0, 0 )  = 200;
	cand.GetFreqMatrix()( 1, 0 )  = 400;
	//cand.GetFreqMatrix()( 2, 0 )  = 600;
	//cand.GetFreqMatrix()( 3, 0 )  = 800;

	cand.GetMagMatrix()( 0, 0 )  = 0.8;
	cand.GetMagMatrix()( 1, 0 )  = 0.6;
	cand.GetMagMatrix()( 2, 0 )  = 0.5;
	//cand.GetMagMatrix()( 3, 0 )  = 0.4;


	// Second Frame
	cand.GetFreqMatrix()( 0, 1 )  = 250;
	cand.GetFreqMatrix()( 1, 1 )  = 400;
	cand.GetFreqMatrix()( 2, 1 )  = 600;
	cand.GetFreqMatrix()( 3, 1 )  = 800;

	cand.GetMagMatrix()( 0, 1 )  = 0.8;
	cand.GetMagMatrix()( 1, 1 )  = 0.7;
	cand.GetMagMatrix()( 2, 1 )  = 0.5;
	cand.GetMagMatrix()( 3, 1 )  = 0.4;


	// Third Frame
	cand.GetFreqMatrix()( 0, 2 )  = 250;
	cand.GetFreqMatrix()( 1, 2 )  = 450;
	cand.GetFreqMatrix()( 2, 2 )  = 600;
	cand.GetFreqMatrix()( 3, 2 )  = 800;

	cand.GetMagMatrix()( 0, 2 )  = 0.9;
	cand.GetMagMatrix()( 1, 2 )  = 0.6;
	cand.GetMagMatrix()( 2, 2 )  = 0.5;
	cand.GetMagMatrix()( 3, 2 )  = 0.4;


	// Fourth Frame
	cand.GetFreqMatrix()( 0, 3 )  = 250;
	cand.GetFreqMatrix()( 1, 3 )  = 425;
	cand.GetFreqMatrix()( 2, 3 )  = 650;
	cand.GetFreqMatrix()( 3, 3 )  = 800;

	cand.GetMagMatrix()( 0, 3 )  = 0.9;
	cand.GetMagMatrix()( 1, 3 )  = 0.8;
	cand.GetMagMatrix()( 2, 3 )  = 0.7;
	cand.GetMagMatrix()( 3, 3 )  = 0.5;


	// Fifth Frame
	cand.GetFreqMatrix()( 0, 4 )  = 250;
	cand.GetFreqMatrix()( 1, 4 )  = 450;
	cand.GetFreqMatrix()( 2, 4 )  = 650;
	cand.GetFreqMatrix()( 3, 4 )  = 850;

	cand.GetMagMatrix()( 0, 4 )  = 0.9;
	cand.GetMagMatrix()( 1, 4 )  = 0.8;
	cand.GetMagMatrix()( 2, 4 )  = 0.6;
	cand.GetMagMatrix()( 3, 4 )  = 0.5;


	CLAM::MDPModelerConfig Modelercfg;

	Modelercfg.SetName("MDPModeler");
	Modelercfg.SetSemitoneRange( semitoneRange );
	Modelercfg.SetAmplitudeRange( amplitudeRange );
	Modelercfg.SetNFrames( nFrames );
	Modelercfg.SetNToTrack( nToTrack );
	Modelercfg.SetNCandidates( nCandidates );

	CLAM::MDPModeler myModeler( Modelercfg );
	CLAM::MDP mdp;

	myModeler.Start();
	myModeler.Do( cand, mdp );

	return 0;
}
