#ifndef RulerTicks_hxx
#define RulerTicks_hxx

#include <cmath>
#include "Assert.hxx"
namespace CLAM
{

/*
	Implements the logic to place graphically several markers
	in a ruler so that the markers are placed with a given
	minimal separation and corresponds to round numbers
	on the mapped domain interval.
	So, given a domain value range, a pixel size and a minimal pixel gap,
	it gives you the domain offset for the first tick and the intertick gap
	also in domain units.
	Chosen round numbers are series so that the last non zero
	decimal steps in 1, 2 or 5 jumps and include 0. For instance:
	- 3.1, 3.2, 3.3...
	- 22, 24, 26...
	- 0.35, 0.40, 0.45...
*/
class RulerTicks
{
		mutable bool _needsUpdate;

		double _min;
		double _max;
		double _minGap;
		double _width;

		mutable double _markOffset;
		mutable double _markGap;
	public:
		RulerTicks()
			: _needsUpdate(true)
			, _min(0)
			, _max(1)
			, _minGap(5)
			, _width(100)
			, _markOffset(666)
			, _markGap(69)
		{
		}
		/// Sets the domain value range
		void setRange(double min, double max)
		{
			CLAM_ASSERT(min<max, "RulerTicks: Empty or inverse order range");
			_min = min;
			_max = max;
			_needsUpdate = true;
		}
		/// Sets the width in pixels to be mapped the range into
		void setWidth(double width)
		{
			CLAM_ASSERT(width>0, "RulerTicks: Pixel width should be greater than zero");
			_width = width;
			_needsUpdate = true;
		}
		/// Sets the minimum phisical space in pixels between between marks
		void setMinGap(double minGap)
		{
			_minGap = minGap;
			_needsUpdate = true;
		}
		double markOffset() const
		{
			if (_needsUpdate) update();
			return _markOffset;
		}
		double markGap() const
		{
			if (_needsUpdate) update();
			return _markGap;
		}
		void update() const
		{
			double mappedMinGap = std::fabs(_minGap*(_max-_min)/_width);
			_markGap=1;
			if (mappedMinGap>=1)
			{
				while (true)
				{
					if (_markGap>=mappedMinGap) break;
					if (_markGap*2>=mappedMinGap) {_markGap*=2; break;}
					if (_markGap*5>=mappedMinGap) {_markGap*=5; break;}
					_markGap*=10;
				}
			}
			else
			{
				while (true)
				{
					if (_markGap<1e-8) {_markGap=1e-8; break;}
					if (_markGap<mappedMinGap) {break;}
					if (_markGap/2<mappedMinGap) {_markGap/=2; break;}
					if (_markGap/5<mappedMinGap) {_markGap/=5; break;}
					_markGap/=10;
				}
			}
			double ceil = std::ceil(_min/_markGap);
			_markOffset = (std::fabs(ceil)<1e-5)? 0. : ceil*_markGap;

			_needsUpdate = false;
		}
		/// Returns the domain value corresponding to tick i
		double tickValue(unsigned i) const
		{
			if (_needsUpdate) update();
			return _markOffset+_markGap*i;
		}
		/// Returns the pixel position of tick i
		double pixelTickPos(unsigned i) const
		{
			return toPixel(tickValue(i));
		}
		// Returns the pixel position of a given domain value
		double toPixel(double value) const
		{
			return (value-_min)*_width/(_max-_min);
		}
		// Returns the number of ticks that will appear
		unsigned nTicks() const
		{
			if (_needsUpdate) update();
			return std::ceil((_max-_markOffset)/_markGap);
		}
};

} // namespace CLAM

#endif//RulerTicks_hxx

