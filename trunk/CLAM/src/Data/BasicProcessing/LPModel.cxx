#include "LPModel.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "SpectrumConversions.hxx"

namespace CLAM
{
	void LPModel::DefaultInit()
	{
		AddAll();
		UpdateData();
		// MRJ: 11 seems to be a 'wise' number
		UpdateModelOrder( 11 );
	}

	void LPModel::UpdateModelOrder( TSize order )
	{
		SetOrder( order );
		GetFilterCoefficients().Resize( order );
		GetFilterCoefficients().SetSize( order );
		GetReflectionCoefficients().Resize( order );
		GetReflectionCoefficients().SetSize( order );
	}

	void LPModel::ToSpectrum( Spectrum& spec )
	{
		SpecTypeFlags specFlags;
		spec.GetType( specFlags );
		spec.SetScale( EScale::eLinear );
		
		spec.SetSpectralRange( GetSpectralRange() );

		// we build the array of polarCoeffs from the filter poles
		Array< Complex > spectrumCoeffs;
		Array< Complex > cmplxCoeffs;
		spectrumCoeffs.Resize( spec.GetSize() );
		spectrumCoeffs.SetSize( spec.GetSize() );
		cmplxCoeffs.Resize( GetFilterCoefficients().Size() );
		cmplxCoeffs.SetSize( GetFilterCoefficients().Size() );

		DataArray& ak_vec = GetFilterCoefficients();

		const TData dw = PI/TData(spec.GetSize()-1);
		TData w =0.0;
		Complex unitComplex;
		unitComplex.SetReal( 1.0 );
		unitComplex.SetImag( 0.0 );

		for ( int j = 0; j < spec.GetSize(); j++ )
		{
			spectrumCoeffs[j].SetReal( 1.0 );
			spectrumCoeffs[j].SetImag( 0.0 );

			for ( int i = 0; i < GetFilterCoefficients().Size(); i++ )
			{
				cmplxCoeffs[i].SetReal( ak_vec[i]*cos( -1.0*(float)(i+1)*w ) );
				cmplxCoeffs[i].SetImag( ak_vec[i]*sin( 1.0*(float)(i+1)*w ) );

				spectrumCoeffs[j] += cmplxCoeffs[i];
			}
			
			spectrumCoeffs[j] = unitComplex / spectrumCoeffs[j];

			w += dw;
		}
		
		if ( specFlags.bComplex )
		{
			spec.SetComplexArray( spectrumCoeffs );
		}
		if ( specFlags.bPolar )
		{
		//	Complex2Polar( spectrumCoeffs, spec.GetPolarArray() );
		}
		if ( specFlags.bMagPhase )
		{
			Complex2MagPhase( spectrumCoeffs, spec.GetMagBuffer(), spec.GetPhaseBuffer() );
		}
	}
}
