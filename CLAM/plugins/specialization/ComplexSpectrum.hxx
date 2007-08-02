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
class ComplexSpectrum
{
public:
	std::vector<std::complex<TData> > bins;
	TData spectralRange;
	void dump(std::ostream & os)
	{
		os << "ComplexSpectrum (spectralRange " << spectralRange << ") {";
		for (unsigned i=0; i<bins.size(); i++)
			os << bins[i] << " ";
		os << "}" << std::endl;
	}
};

} // namespace CLAM

#endif // ComplexSpectrum_hxx
