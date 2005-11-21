#ifndef Segmentation_hxx
#define Segmentation_hxx

#include <vector>
#include <algorithm>
#include <iterator>

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
		typedef std::vector<double> Bounds;
	public:
		Segmentation(double maxLength)
			: _maxLength(maxLength)
		{
			_endBounds.push_back(maxLength);

		}
		/**
		 * Inserts a new bound at timePosition.
		 */
		unsigned insert(double timePosition)
		{
			if (timePosition<=0.0) throw InsertedOutOfBounds();
			Bounds::iterator insertPoint = 
				std::lower_bound(_endBounds.begin(), _endBounds.end(), timePosition);
			if (insertPoint == _endBounds.end()) throw InsertedOutOfBounds();
			// 'position' must be computed before the insertion to not invalidate iterators.
			int position = insertPoint - _endBounds.begin();
			_endBounds.insert(insertPoint, timePosition);
			return position;
		}
		/**
		 * Returns the index of the segment which is nearest to the
		 * given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (> nSegments)
		 */
		unsigned pickEndBound(double timePosition, double tolerance) const
		{
			Bounds::const_iterator insertPoint = 
				std::lower_bound(_endBounds.begin(), _endBounds.end(), timePosition-tolerance);
			unsigned segment = insertPoint - _endBounds.begin();
			return segment;
		}
		std::string boundsAsString() const
		{
			std::ostringstream os;
			double lastOffset = 0;
			for (unsigned i=0; i<_endBounds.size(); i++)
			{
				os << "(" << lastOffset << "," << _endBounds[i] << ") ";
				lastOffset = _endBounds[i];
			}
			return os.str();
		}
	private:
		Bounds _endBounds;
		double _maxLength;
	};

}

// Init single Segment
// Split segment
// Join partitions
// Drag Point
// Current




#endif//Segmentation_hxx
