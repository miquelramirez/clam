#ifndef __SPECTRUMCONVERSIONS__
#define __SPECTRUMCONVERSIONS__

#include "Array.hxx"
#include "Complex.hxx"
#include "Polar.hxx"
#include "BPF.hxx"

namespace CLAM
{
		void Complex2Polar( const Array<Complex>&, Array<Polar>& );
		
		void Polar2Complex( const Array<Polar>&, Array<Complex>& );

		void Complex2MagPhase( const Array<Complex>&, DataArray&, DataArray& );
		
		void Polar2MagPhase( const Array<Polar>&, DataArray&, DataArray& );

		void BPF2MagPhase( const BPF&, const BPF&, DataArray&, DataArray&, TSize, TData );

		void MagPhase2Polar( const DataArray&, const DataArray&, Array<Polar>& );

		void MagPhase2Complex( const DataArray&, const DataArray&, Array<Complex>& );

		void Log2LinearMagnitude( const DataArray&, DataArray& );
		
		void Linear2LogMagnitude( const DataArray&, DataArray& );

		void Complex2LogMagPhase(const Array<Complex> &src, DataArray &destMag, DataArray &destPhase);

		// Inplace conversion function overloads

		void Log2LinearMagnitude( DataArray& );
		
		void Linear2LogMagnitude( DataArray& );
}


#endif // SpectrumConversions.hxx
