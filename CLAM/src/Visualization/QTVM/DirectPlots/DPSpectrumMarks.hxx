#ifndef __DPSPECTRUMMARKS__
#define __DPSPECTRUMMARKS__

#include <list>
#include <string>
#include "Spectrum.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrumMarks(	const Spectrum& spec,
								std::list<unsigned>& marks,
								const std::string& label="",
								int x=100,
								int y=100,
								int w=500,
								int h=225 );
	}
}

#endif

