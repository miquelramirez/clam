#ifndef __DPSPECTRUM__
#define __DPSPECTRUM__

#include <string>
#include "Spectrum.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrum(	const Spectrum& spec,
							const std::string& label="",
							int x=100,
							int y=100,
							int w=500,
							int h=225 );
	}
}

#endif

