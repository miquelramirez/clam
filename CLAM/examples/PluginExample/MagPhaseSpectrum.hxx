#ifndef MagPhaseSpectrum_hxx
#define MagPhaseSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Spectrum.hxx>
#include <CLAM/SpecTypeFlags.hxx>
#include <vector>
#include <algorithm>

namespace CLAM
{

/**
 * Experimental light-weight spectrum
 * Linear scale is implicit.
 */
struct MagPhaseSpectrum 
{
	std::vector<TData> magnitudes;
	std::vector<TData> phases;
	TData spectralRange;
};

} // namespace CLAM

#endif // MagPhaseSpectrum_hxx
