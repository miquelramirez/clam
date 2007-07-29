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

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Assert.hxx>

namespace CLAM
{

class ChordSegmentator : public Processing
{

public:
// Processing object compliance methods

	const char* GetClassName() const { return "ChordSegmentator"; }

	ChordSegmentator(const Config& config = Config()) 
		: _inChordCorrelation("ChordCorrelation", this)
		, _outChordCorrelation("ChordCorrelation", this) 
		, _lastChord(0)
	{
		Configure( config );
	}
	~ChordSegmentator() {};
	
	// This for now only copies inChordCorrelation data to outChordCorrelation
	bool Do() 
	{
		std::vector<TData> & inChordCorrelation = _inChordCorrelation.GetData();
		std::vector<TData> & outChordCorrelation = _outChordCorrelation.GetData();
		
		outChordCorrelation.resize(inChordCorrelation.size());
		for(unsigned i = 0; i < inChordCorrelation.size(); ++i)
			outChordCorrelation[i] = inChordCorrelation[i]; 
		
		_outChordCorrelation.Produce();
		_inChordCorrelation.Consume();
		return true;
	}
	/*bool Do(CLAM::TData & currentTime, const std::vector<double> & correlation, const unsigned firstCandidate, const unsigned secondCandidate) 
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
				_timePositions.push_back(currentTime);
			}
			// Opens a new chord segment
			if (currentChord != 0)
			{
				_timePositions.push_back(currentTime);
				_chordIndexes.push_back(currentChord);
			}
			_lastChord = currentChord;
		}
		return true;
	}*/

private:
	InPort< std::vector<CLAM::TData> > _inChordCorrelation;
	OutPort< std::vector<CLAM::TData> > _outChordCorrelation;
	
	std::vector<double> _timePositions; ///< Holds the beggining and end time of each segment
	std::vector<unsigned> _chordIndexes;  ///< Holds the chord index for each segment

	unsigned _lastChord;
	
public:
	/** Closes the last segment when audio data ends */
	void closeLastSegment(CLAM::TData & currentTime )
	{
		if (_lastChord != 0 & _timePositions.size() != 0)
		{
			CLAM_ASSERT(_timePositions.size()%2==1, "Attempting to close last segment even though all segments have been closed");
			_timePositions.push_back(currentTime);
		}
	}

	const std::vector<double> timePositions() const { return _timePositions; };
	const std::vector<unsigned> chordIndexes() const { return _chordIndexes; };
};

}

#endif
