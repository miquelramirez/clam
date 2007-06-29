#ifndef RulerTicks_hxx
#define RulerTicks_hxx

#include <cmath>
namespace CLAM
{

class RulerTicks
{
		double _min;
		double _max;
		double _markGap;
		double _minGap;
		double _width;
	public:
		/// Sets the domain value range
		void setRange(double min, double max)
		{
			_min = min;
			_max = max;
		}
		/// Sets the width in pixels to be mapped the range into
		void setWidth(double width)
		{
			_width = width;
		}
		/// Sets the minimum phisical space in pixels between between marks
		void setMinGap(double minGap)
		{
			_minGap = minGap;
		}
		double markOffset() const
		{
			double gap = markGap();
			double ceil = std::ceil(_min/gap);
			if (std::fabs(ceil)<1e-5) return 0.;
			return ceil*gap;
		}
		double markGap() const
		{
			double mappedMinGap = std::fabs(_minGap*(_max-_min)/_width);
			double markGap=1;
			if (mappedMinGap>=1)
			{
				while (true)
				{
					if (markGap>=mappedMinGap) return markGap;
					if (markGap*2>=mappedMinGap) return markGap*2;
					if (markGap*5>=mappedMinGap) return markGap*5;
					markGap*=10;
				}
			}
			else
			{
				while (true)
				{
					if (markGap<1e-8) return 1e-8;
					if (markGap<mappedMinGap) return markGap;
					if (markGap/2<mappedMinGap) return markGap/2;
					if (markGap/5<mappedMinGap) return markGap/5;
					markGap/=10;
				}
			}
			return markGap;
		}
};

} // namespace CLAM

#endif//RulerTicks_hxx

