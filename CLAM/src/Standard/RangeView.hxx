#ifndef RangeView_hxx
#define RangeView_hxx

#include <cmath>
#include <algorithm>
#include "Assert.hxx"
namespace CLAM
{

/*
	Implements the logic to navigate along a Range.
	@todo out of place centering
*/
class RangeView
{
	public:
		static double zoomExcentricity(double low, double high, double stiked)
		{
			if (stiked<low) return 0.;
			if (stiked>high) return 1.;
			return (stiked-low)/(high-low);
		}
		static void zoom(double &low, double &high, double factor, double centering)
		{
			CLAM_ASSERT(centering>=0. && centering<=1.,
				"RangeView: zooming using a centering factor not in the [0,1] interval.");
			double span = high-low;
			double midPoint = low + (high-low)*centering;
			low = midPoint - span*factor*centering;
			high = midPoint + span*factor*(1-centering);
		}
		static void keepWithinInterval(double & low, double & high, double lowest, double highest)
		{
			double span = high-low;
			if (high>highest)
			{
				high=highest;
				low=high-span;
			}
			if (low<lowest)
			{
				low=lowest;
				high=std::min(highest,low+span);
			}
		}
};

} // namespace CLAM

#endif//RangeView_hxx

