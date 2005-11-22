#ifndef Segmentation_hxx
#define Segmentation_hxx

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace CLAM_Annotator
{
	class Segmentation
	{
	public:
		class InsertedOutOfBounds : public std::exception
		{
			public:
			const char * what() const throw () { return "Segmentation point inserted out of limits";}
		};
		typedef std::vector<double> TimePositions;
	public:
		Segmentation(double maxLength)
		{
			_onsets.push_back(0);
			_offsets.push_back(maxLength);
			_selection.push_back(false);

		}
		/**
		 * Inserts a new bound at timePosition.
		 */
		unsigned insert(double timePosition)
		{
			if (timePosition<=0.0) throw InsertedOutOfBounds();
			TimePositions::iterator insertPoint = 
				std::lower_bound(_offsets.begin(), _offsets.end(), timePosition);
			if (insertPoint == _offsets.end()) throw InsertedOutOfBounds();
			// 'position' must be computed before the insertion to not invalidate iterators.
			int position = insertPoint - _offsets.begin() +1;
			_offsets.insert(insertPoint, timePosition);
			_onsets.insert(_onsets.begin()+position, _offsets[position-1]);
			_selection.insert(_selection.begin()+position, false);
			return position;
		}
		/**
		 * Removes the specified segment.
		 * The previous segment is expanded to cover the region.
		 * When removing the first segment, the next segment is the one expanded to start at 0.
		 * When just a single element, no efect at all.
		 */
		void remove(unsigned segment)
		{
			if (_offsets.size()==1) return;
			unsigned offsetToRemove = segment? segment-1 : 0;
			_offsets.erase(_offsets.begin()+offsetToRemove);
			_onsets.erase(_onsets.begin()+segment);
			_selection.erase(_selection.begin()+segment);
			if (segment==0) _onsets[0]=0;
		}
		/**
		 * Returns the index of the segment offset which is nearest to the
		 * given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (nSegments)
		 */
		unsigned pickOffset(double timePosition, double tolerance) const
		{
			return pickPosition(_offsets, timePosition, tolerance);
		}
		/**
		 * Returns the index of the segment onset which is nearest to the
		 * given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (nSegments)
		 */
		unsigned pickOnset(double timePosition, double tolerance) const
		{
			return pickPosition(_onsets, timePosition, tolerance);
		}
		/**
		 * Returns the index of the segment which body is on timePosition.
		 */
		unsigned pickSegmentBody(double timePosition) const
		{
			if (timePosition<0) return _offsets.size();
			TimePositions::const_iterator lowerBound =
				std::lower_bound(_offsets.begin(), _offsets.end(), timePosition);
			return lowerBound - _offsets.begin();
		}
		void dragOffset(unsigned segment, double newTimePosition)
		{
			if (segment==_offsets.size()) return;
			if (segment+1==_offsets.size()) return;

			double leftBound = _onsets[segment];
			if (newTimePosition<leftBound)
				newTimePosition = leftBound;
			if (newTimePosition>_offsets[segment+1])
				newTimePosition = _offsets[segment+1];

			_offsets[segment]=newTimePosition;
			_onsets[segment+1]=newTimePosition;
		}
		void dragOnset(unsigned segment, double newTimePosition)
		{
			if (segment==0) return;
			dragOffset(segment-1, newTimePosition);
		}
		void select(unsigned segment)
		{
			_selection[segment]=true;
		}
		void deselect(unsigned segment)
		{
			_selection[segment]=false;
		}
		void clearSelection()
		{
			for (unsigned i=0; i<_selection.size(); i++)
				_selection[i]=false;
		}
		std::string boundsAsString() const
		{
			std::ostringstream os;
			for (unsigned i=0; i<_offsets.size(); i++)
			{
				if (_selection[i]) os << "+";
				os << "(" << _onsets[i] << "," << _offsets[i] << ") ";
			}
			return os.str();
		}

		const TimePositions & onsets() const
		{
			return _onsets;
		}
		const TimePositions & offsets() const
		{
			return _offsets;
		}
		const std::vector<bool> & selections() const
		{
			return _selection;
		}
	private:
		TimePositions _onsets;
		TimePositions _offsets;
		std::vector<bool> _selection;
	private:
		/**
		 * Returns the index of the position which is nearest to the
		 * given time position and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * index is returned (nPositions)
		 */
		unsigned pickPosition(const TimePositions & positions, double timePosition, double tolerance) const
		{
			TimePositions::const_iterator lowerBound = 
				std::lower_bound(positions.begin(), positions.end(), timePosition-tolerance);
			TimePositions::const_iterator upperBound = 
				std::upper_bound(lowerBound, positions.end(), timePosition+tolerance);

			if (lowerBound==upperBound) return positions.size(); // None found
	
			// Pick the closest in range
			unsigned lowerSegment = lowerBound - positions.begin();
			unsigned upperSegment = upperBound - positions.begin();
			double lastDifference = std::fabs(timePosition-positions[lowerSegment]);
			for (unsigned i=lowerSegment; i<upperSegment; i++)
			{
				double newDifference = std::fabs(timePosition-positions[i]);
				if (newDifference>lastDifference) break;
				lastDifference = newDifference;
				lowerSegment = i;
			}
			return lowerSegment;
		}
	};

}

// Init single Segment
// Split segment
// Join partitions
// Drag Point
// Current




#endif//Segmentation_hxx
