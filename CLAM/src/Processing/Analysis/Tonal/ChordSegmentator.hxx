/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

/* 
David Garcia Garzon
Universitat Pompeu Fabra

Katy Noland
Queen Mary, University of London

Adapted from MATLAB code by Chris Harte at Queen Mary
*/

#ifndef ChordSegmentator_hxx
#define ChordSegmentator_hxx

#include <iostream>
#include <fstream>
#include <cmath>
#include "Array.hxx"
#include "DiscontinuousSegmentation.hxx"
#include <CLAM/Assert.hxx>

namespace Simac
{

class ChordSegmentator
{
	CLAM::DiscontinuousSegmentation _segmentation;
	std::vector<unsigned> _chordIndexes;  ///< Holds the chord index for each segment

	unsigned _currentSegment;
	unsigned _lastChord;

	unsigned _method;
public:
	ChordSegmentator(unsigned method=0)
		: _segmentation(1000)
		, _currentSegment(0)
		, _lastChord(0)
		, _method(method)
	{};
	~ChordSegmentator() {};

	void doIt(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		switch(_method)
		{
			case 0:
				doItSimple(currentTime, correlation, firstCandidate, secondCandidate); 
				break;
		}
	}
	
	void doItSimple(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		CLAM::TData firstCandidateWeight = correlation[firstCandidate];
		CLAM::TData secondCandidateWeight = correlation[secondCandidate];
		CLAM::TData noCandidateWeigth = correlation[0];
		
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : firstCandidate;
		
		if (currentChord!=_lastChord)
		{	
			// Closes a segment opened in one of the previous iterations
			if (_lastChord != 0) {
				_segmentation.dragOffset(_currentSegment, currentTime);
			}
			// Opens a new chord segment
			if (currentChord != 0)
			{
				_chordIndexes.push_back(currentChord);
				_currentSegment = _segmentation.insert(currentTime);
			}
			_lastChord = currentChord;
		}
	}
	
	void closeLastSegment(CLAM::TData & currentTime )
	{
		if (_lastChord != 0)
		{
			_segmentation.dragOffset(_currentSegment, currentTime);
		}
	}

	const CLAM::DiscontinuousSegmentation & segmentation() const { return _segmentation; };
	const std::vector<unsigned> & chordIndexes() const { return _chordIndexes; };
};
}
#endif//ChordSegmentator
