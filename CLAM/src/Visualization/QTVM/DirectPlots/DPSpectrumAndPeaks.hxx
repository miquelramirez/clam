#ifndef __DPSPECTRUMANDPEAKS__
#define __DPSPECTRUMANDPEAKS__

#include <string>
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrumAndPeaks(	const Spectrum& spec,
									const SpectralPeakArray& peaks,
									const std::string& label="",
									int x=100,
									int y=100,
									int w=500,
									int h=225 );
	}
}

#endif

