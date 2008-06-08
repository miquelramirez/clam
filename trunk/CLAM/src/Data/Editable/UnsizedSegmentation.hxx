#ifndef UnsizedSegmentation_hxx
#define UnsizedSegmentation_hxx

#include "Segmentation.hxx"

namespace CLAM
{
	class UnsizedSegmentation : public Segmentation
	{
	public:
		class InsertedOutOfBounds : public std::exception
		{
			public:
			const char * what() const throw () { return "Segmentation point inserted out of limits";}
		};
		typedef std::vector<double> TimePositions;
	public:
		UnsizedSegmentation()
			: Segmentation(0)
		{
		}
		UnsizedSegmentation(double maxPosition)
			: Segmentation(maxPosition)
		{
		}
		template <typename Iterator>
		UnsizedSegmentation(double maxPosition, Iterator begin, Iterator end)
			: Segmentation(maxPosition)
		{
			for (Iterator it=begin; it!=end; it++)
				insert(*it);
		}
		~UnsizedSegmentation()
		{
		}
		/**
		 * Inserts a new border at timePosition.
		 */
		unsigned insert(double timePosition)
		{
			if (timePosition<0.0) throw InsertedOutOfBounds();
			if (timePosition>=maxPosition()) throw InsertedOutOfBounds();
			TimePositions::iterator insertPoint = 
				std::lower_bound(_offsets.begin(), _offsets.end(), timePosition);
			if (insertPoint==_offsets.end())
			{
				_offsets.push_back(timePosition);
				_onsets.push_back(timePosition);
				_selection.push_back(false);
				return _offsets.size()-1;
			}
			unsigned position = insertPoint - _offsets.begin();
			_offsets.insert(insertPoint, timePosition);
			_onsets.insert(_onsets.begin()+position, timePosition);
			_selection.insert(_selection.begin()+position, false);
			if (position<=_current) _current++;
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
			TimePositions::const_iterator upperBound =
				std::lower_bound(_offsets.begin(), _offsets.end(), timePosition);
			if (upperBound == _offsets.begin()) return 0;
			if (timePosition - *(upperBound-1) > *upperBound - timePosition) return upperBound - _offsets.begin();
			return upperBound - _offsets.begin()-1;
		}
		/**
		 * Performs a dragging movement for the Onset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		void dragOnset(unsigned segment, double newTimePosition)
		{
			// The onset is attached to the offset
			dragOffset(segment, newTimePosition);
		}
		/**
		 * Performs a dragging movement for the Offset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		void dragOffset(unsigned segment, double newTimePosition)
		{
			if (segment==_offsets.size()) return; // Invalid segment

			// Limit movement on the left to the onset
			double leftLimit = segment!=0 ?
				_onsets[segment-1] : 0.0;
			double rightLimit = segment+1<_offsets.size()? 
					_onsets[segment+1] : maxPosition();
			if (newTimePosition<leftLimit)
				newTimePosition = leftLimit;
			// Limit movement on the right to the next offset
			if (newTimePosition>rightLimit)
				newTimePosition = rightLimit;

			// The offset and the onset change together
			_offsets[segment]=newTimePosition;
			_onsets[segment]=newTimePosition;
		}
		/**
		* Performs an empty implementation
		*/
		void fillArray(DataArray& segmentation)
		{
			unsigned nSegments = _onsets.size();
			segmentation.Resize(nSegments);
			segmentation.SetSize(nSegments);
			for (unsigned i=0; i<nSegments; i++)
				segmentation[i]=_onsets[i];
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



#endif//UnsizedSegmentation_hxx

