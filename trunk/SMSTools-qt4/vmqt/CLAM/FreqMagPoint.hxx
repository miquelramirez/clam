#ifndef __FREQMAGPOINT__
#define __FREQMAGPOINT__

#include <CLAM/DataTypes.hxx>

namespace CLAM
{
	namespace VM
	{
		struct FreqMagPoint
		{
			TData freq;
			TData mag;

			FreqMagPoint()
				: freq(TData(0.0))
				, mag(TData(0.0))
				{}

			FreqMagPoint(const TData& f, const TData& m)
				: freq(f)
				, mag(m)
				{}

			FreqMagPoint(const FreqMagPoint& fmp)
				: freq(fmp.freq)
				, mag(fmp.mag)
				{}

			FreqMagPoint& operator=(const FreqMagPoint& fmp)
				{
					freq = fmp.freq;
					mag = fmp.mag;
					return *this;
				}
		};
	}
}

#endif

