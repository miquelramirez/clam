#ifndef __DPSINTRACKSMARKS__
#define __DPSINTRACKSMARKS__

#include <list>
#include <string>
#include "Segment.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSinTracksMarks(	const Segment& segment,
									std::list<unsigned>& marks,
									const std::string& label="",
									int x=100,
									int y=100,
									int w=500,
									int h=225	);

		void PlotSinTracksMarks(	const Array< SpectralPeakArray >& peakMtx, 
									const TData& sr, 
									const TData& dur,
									std::list<unsigned>& marks,
									const std::string& label="",
									int x=100,
									int y=100,
									int w=500,
									int h=225	);
	}
}

#endif

