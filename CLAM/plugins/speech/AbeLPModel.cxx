/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "AbeLPModel.hxx"
#include <CLAM/Spectrum.hxx>
#include <CLAM/SpectrumConfig.hxx>
#include <CLAM/SpectrumConversions.hxx>
#include <CLAM/ProcessingDataPlugin.hxx>

namespace CLAM
{
	namespace Hidden
	{
		static ProcessingDataPlugin::Registrator<AbeLPModel> dataRegistrator("orange");
	}

	void AbeLPModel::DefaultInit()
	{
		AddAll();
		UpdateData();
		// MRJ: 11 seems to be a 'wise' number
		//Abe: I think that 11 coefficients is good for 8kHz speech.  Maybe not optimal for hifi.
		UpdateModelOrder( 11 );
		
	}

	void AbeLPModel::UpdateModelOrder( TSize order )
	{
		SetOrder( order );
		GetFilterCoefficients().Resize( order );
		GetFilterCoefficients().SetSize( order );
		GetReflectionCoefficients().Resize( order );
		GetReflectionCoefficients().SetSize( order );
	}

	void AbeLPModel::ToSpectrum( Spectrum& spec ) const
	{
		SpecTypeFlags specFlags;
		spec.GetType( specFlags );
		spec.SetScale( EScale::eLinear );
		
		spec.SetSpectralRange( GetSpectralRange() );

		const DataArray& ak_vec = GetFilterCoefficients();
		int order = ak_vec.Size();

		// we build the array of polarCoeffs from the filter poles
		// Abe: I think this is not actually the poles, but rather the coefficients
		Array< Complex > spectrumCoeffs;
		Array< Complex > cmplxCoeffs;
		spectrumCoeffs.Resize( spec.GetSize() );
		spectrumCoeffs.SetSize( spec.GetSize() );
		cmplxCoeffs.Resize( order );
		cmplxCoeffs.SetSize( order );

		const TData dw = PI/TData(spec.GetSize()-1);
		TData w =0.0;
		Complex unitComplex;
		unitComplex.SetReal( 1.0 );
		unitComplex.SetImag( 0.0 );

		for ( int j = 0; j < spec.GetSize(); j++ )
		{
			spectrumCoeffs[j].SetReal( 1.0 );
			spectrumCoeffs[j].SetImag( 0.0 );

			for ( int i = 0; i < order; i++ )
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

	CLAM::Array<Complex> AbeLPModel::ToRoots(DataArray coefficients)   
	{
		return p.PolyRoots(coefficients);  //solve roots of LP coefficients
	}
  
}

