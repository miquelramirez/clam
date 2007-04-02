#ifndef __DPSPECTROGRAM__
#define __DPSPECTROGRAM__

#include <vector>
#include <string>
#include "Spectrum.hxx"

namespace CLAM
{
    namespace VM
    {
		/**
		 * Viewing sonogram directly.
		 */

		void PlotSpectrogram( const Array<Spectrum>& specMtx,
							  const TData& duration,
							  const std::string& label="",
							  int x=100,
							  int y=100,
							  int w=600,
							  int h=300 );

		void PlotSpectrogram( const Array<Spectrum>& specMtx,
							  const TData& duration,
							  std::vector<unsigned>& marks,
							  const std::string& label="",
							  int x=100,
							  int y=100,
							  int w=600,
							  int h=300 );
    }
}

#endif

