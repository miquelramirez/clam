#ifndef __LPMODEL__
#define __LPMODEL__

#include "ProcessingData.hxx"
#include "Array.hxx"

namespace CLAM
{
	class Spectrum;

	class LPModel /*of the spectral characteristics of a given signal*/
		: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( LPModel, 5, ProcessingData );

		DYN_ATTRIBUTE( 0, public, DataArray,   FilterCoefficients );
		DYN_ATTRIBUTE( 1, public, DataArray,   ReflectionCoefficients );
		DYN_ATTRIBUTE( 2, public, TData,       AvgSqrFilterError );
		DYN_ATTRIBUTE( 3, public, TData,       SpectralRange );
		DYN_ATTRIBUTE( 4, protected, unsigned, Order );

	public:
		void ToSpectrum( Spectrum& spec );
		void UpdateModelOrder( TSize order );

	protected:
		void DefaultInit();
	};
}


#endif // LPModel.hxx
