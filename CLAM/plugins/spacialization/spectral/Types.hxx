#ifndef spectral_Types_hxx
#define spectral_Types_hxx

#include <vector>
#include <complex>

#ifdef USE_DOUBLE
typedef double Float;
typedef _Complex double C_Complex;
#else
typedef float Float;
typedef _Complex float C_Complex;
#endif
typedef std::complex<Float> Complex;
typedef std::vector<Float> SpectralPower;

#endif // spectral_types_hxx
