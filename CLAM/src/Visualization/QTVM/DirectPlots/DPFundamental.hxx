#ifndef __DPFUNDAMENTAL__
#define __DPFUNDAMENTAL__

#include <string>
#include "Segment.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotFundamental(	const Segment& segment,
								const std::string& label="",
								int x=100,
								int y=100,
								int w=500,
								int h=225 );
	}
}

#endif

