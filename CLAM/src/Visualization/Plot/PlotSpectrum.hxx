#ifndef __CLAMVM_PLOTSPECTRUM_HXX
#define __CLAMVM_PLOTSPECTRUM_HXX

// forward declaration
namespace CLAM
{
	class Spectrum;
}

namespace CLAMVM
{
	/**
	 * Overload of plot() for CLAM::Spectrum.
	 */
	void plot(const CLAM::Spectrum &model, const char *label = "");
	
	void deferredPlot( const CLAM::Spectrum& model, const char* label = 0 );


} // namespace CLAMVM

#endif // __CLAMVM_PLOTSPECTRUM_HXX
