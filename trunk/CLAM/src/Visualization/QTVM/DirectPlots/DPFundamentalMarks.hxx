#ifndef __DPFUNDAMENTALMARKS__
#define __DPFUNDAMENTALMARKS__

#include <list>
#include <string>
#include "Segment.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotFundamentalMarks(	const Segment& segment,
									std::list<unsigned>& marks,
									const std::string& label="",
									int x=100,
									int y=100,
									int w=500,
									int h=225	);
	}
}

#endif

