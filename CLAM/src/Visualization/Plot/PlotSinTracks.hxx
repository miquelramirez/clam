#ifndef __PLOTSINTRACKS__
#define __PLOTSINTRACKS__

// forward declaration
namespace CLAM
{
	template < typename T > class Array;
	class SpectralPeakArray;
	class Segment;
}

namespace CLAMVM
{
	/**
	 *  Overload of plot() for CLAM::Segment
	 */
	void plot( const CLAM::Segment& model, const char* label = 0 );
	void plot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label = 0 );
	void deferredPlot( const CLAM::Segment& model, const char* label = 0 );
	void deferredPlot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label = 0 );
}


#endif // PlotSinTracks.hxx
