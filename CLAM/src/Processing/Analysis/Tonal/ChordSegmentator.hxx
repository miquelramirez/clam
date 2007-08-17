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

/**
 * ChordSegmentator divides the analysed audio data into segments
 * characterized by different chords. It takes as input the chord
 * correlation for each frame and decides whether this new data
 * signifies a chord change in the music. If so it opens a new
 * segment for the new chord, at the same time closing the 
 * previous segment.
 */
class ChordSegmentator
{
	CLAM::DiscontinuousSegmentation _segmentation;
	std::vector<unsigned> _chordIndexes;

	unsigned _currentSegment;
	bool _segmentOpen;
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
			case 1:
				doItSimilarity(currentTime, correlation, firstCandidate, secondCandidate); 
				break;
			default:
				doItSimple(currentTime, correlation, firstCandidate, secondCandidate); 
		}
	}

	/** Simple chord segmentation method
	 */
	void doItSimple(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		CLAM::TData firstCandidateWeight = correlation[firstCandidate];
		CLAM::TData noCandidateWeigth = correlation[0];
		
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : firstCandidate;
		
		if(_segmentOpen)
		{
			if(!currentChord)
				closeSegmentSimilarity(currentTime);
			if(currentChord != _lastChord)
				closeSegmentSimilarity(currentTime);
		}
		if(!_segmentOpen)
		{	
			if(currentChord)
				openSegmentSimilarity(currentTime, currentChord);
		}
		
		_lastChord = currentChord;
		
		if(_segmentOpen)
			_segmentation.dragOffset(_currentSegment, currentTime);
	}
	void openSegmentSimple(CLAM::TData & currentTime, unsigned currentChord)
	{
		_chordIndexes.push_back(currentChord);
		_currentSegment = _segmentation.insert(currentTime);
		_segmentOpen = true;
	}
	void closeSegmentSimple(CLAM::TData & currentTime)
	{
		_segmentation.dragOffset(_currentSegment, currentTime);
		_segmentOpen = false;
	}

	/** 
	 * Chord similarity based segmentation method
	 */
	void doItSimilarity(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		CLAM::TData firstCandidateWeight = correlation[firstCandidate];
		CLAM::TData noCandidateWeigth = correlation[0];
		
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : firstCandidate;
		
		if(_segmentOpen)
		{
			if(!currentChord)
				closeSegmentSimilarity(currentTime);
			if(currentChord != _lastChord)
				closeSegmentSimilarity(currentTime);
		}
		if(!_segmentOpen)
		{	
			if(currentChord)
				openSegmentSimilarity(currentTime, currentChord);
		}
		
		_lastChord = currentChord;
		
		if(_segmentOpen)
			_segmentation.dragOffset(_currentSegment, currentTime);
	}
	void openSegmentSimilarity(CLAM::TData & currentTime, unsigned currentChord)
	{
		_chordIndexes.push_back(currentChord);
		_currentSegment = _segmentation.insert(currentTime);
		_segmentOpen = true;
	}
	void closeSegmentSimilarity(CLAM::TData & currentTime)
	{
		_segmentation.dragOffset(_currentSegment, currentTime);
		_segmentOpen = false;
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
