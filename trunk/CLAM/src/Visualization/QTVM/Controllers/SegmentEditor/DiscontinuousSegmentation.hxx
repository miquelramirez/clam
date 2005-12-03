#ifndef DiscontinuousSegmentation_hxx
#define DiscontinuousSegmentation_hxx

#include "Segmentation.hxx"

namespace CLAM
{
	class DiscontinuousSegmentation : public Segmentation
	{
	public:
		class InsertedOutOfBounds : public std::exception
		{
			public:
			const char * what() const throw () { return "Segmentation point inserted out of limits";}
		};
		typedef std::vector<double> TimePositions;
	public:
		DiscontinuousSegmentation(double maxLength)
			: Segmentation(maxLength)
		{
		}
		/**
		 * Inserts a new border at timePosition.
		 */
		unsigned insert(double timePosition)
		{
			if (timePosition<0.0) throw InsertedOutOfBounds();
			if (timePosition>_maxLength) throw InsertedOutOfBounds();
			TimePositions::iterator nextOffset = 
				std::lower_bound(_offsets.begin(), _offsets.end(), timePosition);
			if (nextOffset == _offsets.end()) // Beyond any existing segment
			{
				_onsets.push_back(timePosition);
				_offsets.push_back(_maxLength);
				_selection.push_back(false);
				return _onsets.size()-1;
			}
			// 'nextOffsetPosition' must be computed before the insertion to not invalidate iterators.
			unsigned nextOffsetPosition = nextOffset - _offsets.begin();
			if (_onsets[nextOffsetPosition]<=timePosition) // Just in the middle of a segment
			{
				_offsets.insert(nextOffset, timePosition);
				_onsets.insert(_onsets.begin()+nextOffsetPosition+1, timePosition);
				_selection.insert(_selection.begin()+nextOffsetPosition+1, false);
				if (nextOffsetPosition<_current) _current++;
				return nextOffsetPosition+1;
			}
			else // In a gap before a segment
			{
				_offsets.insert(nextOffset, _onsets[nextOffsetPosition]);
				_onsets.insert(_onsets.begin()+nextOffsetPosition, timePosition);
				_selection.insert(_selection.begin()+nextOffsetPosition, false);
				if (_current>=nextOffsetPosition) _current++;
				return nextOffsetPosition;
			}
			
		}
		/**
		 * Removes the specified segment.
		 * The previous segment is expanded to cover the region.
		 * When removing the first segment, the next segment is the one expanded to start at 0.
		 * When just a single element, no efect at all.
		 */
		void remove(unsigned segment)
		{
			_offsets.erase(_offsets.begin()+segment);
			_onsets.erase(_onsets.begin()+segment);
			_selection.erase(_selection.begin()+segment);
			if (_current!=0 && segment<=_current) _current--;
		}
		/**
		 * Returns the index of the segment whose offset is nearest 
		 * to the given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (nSegments)
		 */
		unsigned pickOffset(double timePosition, double tolerance) const
		{
			return pickPosition(_offsets, timePosition, tolerance);
		}
		/**
		 * Returns the index of the segment whose onset is nearest
		 * to the given time position, and within the tolerance.
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
		/**
		 * Performs a dragging movement for the Onset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		void dragOnset(unsigned segment, double newTimePosition)
		{
			// The onset is attached to the previous offset
			if (segment>=_onsets.size()) return; // Invalid segment

			// Limit to the left to the previous onset or 0
			double leftBound = segment ? _offsets[segment-1] : 0;
			if (newTimePosition<leftBound)
				newTimePosition=leftBound;
			// Limit to the right to the own offset
			double rigthBound = _offsets[segment];
			if (newTimePosition>rigthBound)
				newTimePosition=rigthBound;

			// The offset and the next onset change together
			_onsets[segment]=newTimePosition;
		}
		/**
		 * Performs a dragging movement for the Offset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		void dragOffset(unsigned segment, double newTimePosition)
		{
			if (segment>=_offsets.size()) return; // Invalid segment

			// Limit to the right to the next offset or max
			double rigthBound = segment+1==_offsets.size()? _maxLength : _onsets[segment+1];
			if (newTimePosition>rigthBound)
				newTimePosition=rigthBound;
			// Limit to the left to the own onset
			double leftBound = _onsets[segment];
			if (newTimePosition<leftBound)
				newTimePosition=leftBound;

			// The offset and the next onset change together
			_offsets[segment]=newTimePosition;
		}

	private:
		/**
		 * Returns the index of the time position which is nearest
		 * to the given time position and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * index is returned (nPositions)
		 * @pre positions is a sorted array
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



#endif//DiscontinuousSegmentation_hxx
