#ifndef __SINTRACKNODE__
#define __SINTRACKNODE__

#include "DataTypes.hxx"
#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {
		struct SinTrackNode
		{
			TData freq;
			TIndex timeIndex;
			Color color;

			SinTrackNode()
				: freq(TData(0.0))
				, timeIndex(0)
				{}

			SinTrackNode(const TData& f, const TIndex& ti, const Color& c)
				: freq(f)
				, timeIndex(ti)
				, color(c)
				{}

			SinTrackNode(const SinTrackNode& node)
				: freq(node.freq)
				, timeIndex(node.timeIndex)
				, color(node.color)
				{}

			SinTrackNode& operator=(const SinTrackNode& node)
				{
					freq = node.freq;
					timeIndex = node.timeIndex;
					color = node.color;
					return *this;
				}
		};
    }

}

#endif

