#ifndef __DPSINTRACKS__
#define __DPSINTRACKS__

#include <string>
#include "Segment.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSinTracks(	const Segment& segment,
							const std::string& label="",
							int x=100,
							int y=100,
							int w=500,
							int h=225 );

		void PlotSinTracks(	const Array< SpectralPeakArray >& peakMtx, 
							const TData& sr, 
							const TData& dur,
							const std::string& label="",
							int x=100,
							int y=100,
							int w=500,
							int h=225 );
	}
}

#endif

