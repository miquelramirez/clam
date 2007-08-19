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
#include "ChordCorrelator.hxx"
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
	
	// Chord similarity method variables
	std::vector< std::vector<double> > _chordSimilarity;
	std::vector<double> _segmentChordCorrelation;
public:
	ChordSegmentator(unsigned method=0)
		: _segmentation(1000)
		, _currentSegment(0)
		, _segmentOpen(false)
		, _lastChord(0)
		, _method(method)
	{
		if(method != 0 && method != 1)
			_method = 0;

		switch(_method)
		{
			case 2:
				ChordCorrelator chordCorrelator;
				_chordSimilarity = chordCorrelator.chordPatternsSimilarity();
				for(unsigned i=0; i<101; ++i)
					_segmentChordCorrelation.push_back(0);
				break;
		}
	};
	~ChordSegmentator() {};

	void doIt(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		switch(_method)
		{
			case 2:
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
				closeSegment(currentTime);
			if(currentChord != _lastChord)
				closeSegment(currentTime);
		}
		if(!_segmentOpen)
		{	
			if(currentChord)
				openSegment(currentTime, currentChord);
		}
		
		_lastChord = currentChord;
		
		if(_segmentOpen)
			_segmentation.dragOffset(_currentSegment, currentTime);
	}

	/** 
	 * Chord similarity based segmentation method
	 */
	void doItSimilarity(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
	{
		CLAM::TData firstCandidateWeight = correlation[firstCandidate];
		CLAM::TData noCandidateWeigth = correlation[0];
		for(unsigned i=0; i<correlation.size(); i++) 
		{
			_segmentChordCorrelation[i] += correlation[i]/correlation[0];
		}
		
		unsigned currentChord = firstCandidateWeight*0.6<=noCandidateWeigth || noCandidateWeigth<0.001 ?
				0 : firstCandidate;

		unsigned segmentChord=0;
		
		if(_segmentOpen)
		{ 
			estimateChord(_segmentChordCorrelation, segmentChord);
			_chordIndexes[_currentSegment] = segmentChord;
			_lastChord = segmentChord;

			if(!currentChord) 
			{
				closeSegment(currentTime);
				for(unsigned i=0; i<correlation.size(); i++) 
				_segmentChordCorrelation[i] = 0;
			}
			
			//
			// The most important condition for rooting out the small, pointless
			// segments that appear so often.
			//
			
			double similarityThreshold = 0.67;

			if (_chordSimilarity[currentChord][_lastChord] < similarityThreshold)
			{
				closeSegment(currentTime);
				for(unsigned i=0; i<correlation.size(); i++) 
				_segmentChordCorrelation[i] = 0;
			}
		}
		if(!_segmentOpen)
		{	
			if(currentChord)
				openSegment(currentTime, currentChord);
			_lastChord = currentChord;
		}
		
		
		if(_segmentOpen)
			_segmentation.dragOffset(_currentSegment, currentTime);
	}

	void openSegment(CLAM::TData & currentTime, unsigned currentChord)
	{
		_chordIndexes.push_back(currentChord);
		_currentSegment = _segmentation.insert(currentTime);
		_segmentOpen = true;
	}
	void closeSegment(CLAM::TData & currentTime)
	{
		_segmentation.dragOffset(_currentSegment, currentTime);
		_segmentOpen = false;
	}
	
	void closeLastSegment(CLAM::TData & currentTime )
	{
		if (_lastChord != 0)
		{
			_segmentation.dragOffset(_currentSegment, currentTime);
			_segmentOpen = false;
		}
		
		switch(_method)
		{
			case 1:
				removeSmallSegments();
				break;
		}
	}

	void estimateChord(const ChordCorrelator::ChordCorrelation & correlation, unsigned & estimatedChord)
	{
		double maxCorrelation = 0;
		double underMaxCorrelation = 0;
		unsigned maxIndex = 0;
		unsigned underMaxIndex = 0;
		for (unsigned i=0; i<correlation.size(); i++)
		{
			if (correlation[i]<underMaxCorrelation) continue;
			if (correlation[i]<maxCorrelation)
			{
				underMaxIndex=i;
				underMaxCorrelation=correlation[i];
				continue;
			}
			underMaxIndex=maxIndex;
			underMaxCorrelation=maxCorrelation;
			maxIndex=i;
			maxCorrelation=correlation[i];
		}
		estimatedChord = maxIndex;
	}
	
	//
	// Post Processing Functions
	//
	
	/**
	 * Finds segments shorter then minSegmentLength and
	 * assigns them the same chord as the chord in either
	 * the previous or the next segment.
	 */
	void removeSmallSegments()
	{
		double minSegmentLength = 0.4;
		
		std::vector<double> onsets = _segmentation.onsets();
		std::vector<double> offsets = _segmentation.offsets();
		unsigned lastSegment = onsets.size();

		for(unsigned segment=0; segment<lastSegment; segment++)
		{
			if(offsets[segment]-onsets[segment]<0.5)
			{
				if(segment<lastSegment)
					if(offsets[segment]==onsets[segment+1])
						_chordIndexes[segment] = _chordIndexes[segment+1];
				if(segment>0)
					if(onsets[segment]==offsets[segment-1])
						_chordIndexes[segment] = _chordIndexes[segment-1];
			}
			
		}
	}

	const CLAM::DiscontinuousSegmentation & segmentation() const { return _segmentation; };
	const std::vector<unsigned> & chordIndexes() const { return _chordIndexes; };
};
}
#endif//ChordSegmentator
