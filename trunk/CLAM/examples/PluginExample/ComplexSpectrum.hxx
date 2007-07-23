#ifndef ComplexSpectrum_hxx
#define ComplexSpectrum_hxx

#include <vector>
#include <complex>
#include <CLAM/DataTypes.hxx>

namespace CLAM
{

/**
 * Experimental light-weight spectrum
 * Linear scale is implicit.
 */
struct ComplexSpectrum 
{
	std::vector<std::complex<TData> > bins;
	TData spectralRange;
};

} // namespace CLAM

#endif // ComplexSpectrum_hxx
