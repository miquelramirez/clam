#ifndef __PLOTSPECTRALPEAKS__
#define __PLOTSPECTRALPEAKS__

namespace CLAM
{
	class Spectrum;
	class SpectralPeakArray;
}

namespace CLAMVM
{
	void plot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks, const char* label = 0 );
	void deferredPlot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks, const char* label = 0 );
}

#endif // PlotSpectralPeaks.hxx
