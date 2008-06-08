#ifndef Segmentation_hxx
#define Segmentation_hxx

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <sstream>
#include <CLAM/Array.hxx>

namespace CLAM
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
		Segmentation()
			: _current(0)
			, _maxPosition(0)
		{
		}
		Segmentation(double maxPosition)
			: _current(0)
			, _maxPosition(maxPosition)
		{
		}
		virtual ~Segmentation();
		/**
		 * Inserts a new border at timePosition.
		 */
		virtual unsigned insert(double timePosition)=0;
		/**
		 * Removes the specified segment.
		 * The previous segment is expanded to cover the region.
		 * When removing the first segment, the next segment is the one expanded to start at 0.
		 * When just a single element, no efect at all.
		 */
		virtual void remove(unsigned segment)=0;
		/**
		 * Returns the index of the segment whose offset is nearest 
		 * to the given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (nSegments)
		 */
		virtual unsigned pickOffset(double timePosition, double tolerance) const=0;
		/**
		 * Returns the index of the segment whose onset is nearest
		 * to the given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (nSegments)
		 */
		virtual unsigned pickOnset(double timePosition, double tolerance) const=0;
		/**
		 * Returns the index of the segment which body is on timePosition.
		 */
		virtual unsigned pickSegmentBody(double timePosition) const=0;
		/**
		 * Performs a dragging movement for the Onset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		virtual void dragOnset(unsigned segment, double newTimePosition)=0;
		/**
		 * Performs a dragging movement for the Offset of the given
		 * segment in order to move it to the newTimePosition.
		 * Constraints for the segmentation mode are applied.
		 */
		virtual void dragOffset(unsigned segment, double newTimePosition)=0;

		/**
		* Performs a empty implementation
		*/
		virtual void fillArray(DataArray& segmentation)=0;
		
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

		/**
		 * Testing method for the unit tests.
		 */
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

		/**
		 * Returns a vector of time position of the segment onsets.
		 */
		const TimePositions & onsets() const
		{
			return _onsets;
		}
		/**
		 * Returns a vector of time position of the segment offsets.
		 */
		const TimePositions & offsets() const
		{
			return _offsets;
		}
		/**
		 * Returns a vector of segment labels
		 */
		const std::vector<std::string> & labels() const
		{
			return _labels;
		}
		/**
		 * Sets the label for a particular segment
		 */
		void setLabel(unsigned segment, std::string label)
		{
			if (segment>=_labels.size()) return; // Invalid segment
			_labels[segment] = label;
		}
		/**
		 * Returns a vector of time position of the segment selections.
		 */
		const std::vector<bool> & selections() const
		{
			return _selection;
		}
		/**
		 * Returns the current segmentation.
		 */
		unsigned current() const
		{
			return _current;
		}
		/**
		 * Changes teh current segmentation
		 */
		void current(unsigned index)
		{
			if (index>=_onsets.size()) return;
			_current = index;
		}
		double maxPosition() const
		{
			return _maxPosition;
		}
		void maxPosition(double maxPosition)
		{
			_maxPosition = maxPosition;
		}
		void xUnits(const std::string & units)
		{
			_xUnits=units;
		}
		const std::string & xUnits() const
		{
			return _xUnits;
		}
	protected:
		TimePositions _onsets;
		TimePositions _offsets;
		std::vector<std::string> _labels;
		std::vector<bool> _selection;
		unsigned _current;
		double _maxPosition;
		std::string _xUnits;
	};

}



#endif//Segmentation_hxx

