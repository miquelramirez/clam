#ifndef MagPhaseSpectrum_hxx
#define MagPhaseSpectrum_hxx

#include <CLAM/DataTypes.hxx>
#include <vector>

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
