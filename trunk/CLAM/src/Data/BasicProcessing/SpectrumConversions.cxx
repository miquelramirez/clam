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

#include "SpectrumConversions.hxx"
#include "DataTypes.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{
	inline void AssertProperSize( TSize size, DataArray& buffer )
	{
		if ( buffer.Size() != size )
		{
			buffer.Resize( size );
			buffer.SetSize( size );
		}
	}


	void Polar2MagPhase ( const Array<Polar>& polarBuffer, DataArray& magBuffer, 
			      DataArray& phaseBuffer )
	{
		TSize nBins = polarBuffer.Size();

		// target buffers resizing if necessary
				
		AssertProperSize( nBins, magBuffer );
		AssertProperSize( nBins, phaseBuffer );
				
		for ( TSize i = 0; i < nBins; i++ )
		{
			magBuffer[i] = polarBuffer[i].Mag();
			phaseBuffer[i] = polarBuffer[i].Ang();
		}
	} 

	void Complex2MagPhase( const Array<Complex>& complBuffer, DataArray& magBuffer, DataArray& phaseBuffer )
	{
		TSize nBins = complBuffer.Size();

		AssertProperSize( nBins, magBuffer );
		AssertProperSize( nBins, phaseBuffer );

		for ( TSize i = 0; i < nBins; i++ )
		{
			/** @see ComplexTmpl::Mag for details */
			magBuffer[i] = complBuffer[i].Mag();
			/** @see ComplexTmpl::Ang for details */
			phaseBuffer[i] = complBuffer[i].Ang();
		}
	}

	void BPF2MagPhase( const BPF& magBpf, 
			   const BPF& phaseBpf,
			   DataArray& magBuffer, 
			   DataArray& phaseBuffer, 
			   TSize nBins, 
			   TData specRange )
	{
		AssertProperSize( nBins, magBuffer );
		AssertProperSize( nBins, phaseBuffer );

		const TData delta = specRange/TData(nBins-1);
		TData freq = 0;

		for ( TSize i = 0; i < nBins; i++ )
		{
			magBuffer[i] = magBpf.GetValue( freq );
			phaseBuffer[i] = phaseBpf.GetValue( freq );

			freq += delta;
		}
	}

	void Log2LinearMagnitude( const DataArray& logBuffer, DataArray& linBuffer )
	{
		const TSize nBins = logBuffer.Size();
		static const TData inv_20 = 1.0f/20.0f;

		AssertProperSize( nBins, linBuffer );

		for ( int i = 0; i < nBins; i++ )
		{
			linBuffer[i] = CLAM_pow( TData(10), logBuffer[i]*inv_20 ) ;
		}
	}

	void Log2LinearMagnitude( DataArray& dataBuffer )
	{
		const TSize nBins = dataBuffer.Size();
		static const TData inv_20 = 1.0f/20.0f;

		for ( int i = 0; i < nBins; i++ )
		{
			dataBuffer[i] = CLAM_pow( TData(10), dataBuffer[i]*inv_20 ) ;
		}
	}

	void Complex2LogMagPhase(const Array<Complex> &src, DataArray &destMag, DataArray &destPhase)
	{
		CLAM_ASSERT(src.Size() == destMag.Size() && src.Size() == destPhase.Size(), "Complex2LogMagPhase() - input/output sizes inconsistent.");

		const int n = src.Size();
		for (int i = 0; i < n; ++i)
		{
			static const float minLinSquared = 1.0e-20f;
			static const float minLog = -200.f; // -200 dB

			const float re = src[i].Real();
			const float im = src[i].Imag();

			const float magSquared = re*re + im*im;

			if (magSquared <= minLinSquared)
			{
				destMag[i] = minLog;
			}
			else
			{
				destMag[i] = 10.f*CLAM_log10(magSquared); // = 20*log10(CLAM_sqrt(re^2 + im^2))
			}

			destPhase[i] = src[i].Ang(); // = atan2(im. re)
		}
	}

	void Linear2LogMagnitude( const DataArray& linearBuffer, DataArray& logData )
	{
		const TSize nBins = linearBuffer.Size();

		AssertProperSize( nBins, logData );
				
		for ( int i = 0; i < nBins; i++ )
		{
			logData[i]=CLAM_20log10(linearBuffer[i]);
		}

	}

	void Linear2LogMagnitude( DataArray& dataBuffer )
	{
		const TSize nBins = dataBuffer.Size();

		for ( int i = 0; i < nBins; i++ )
		{
			dataBuffer[i]=CLAM_20log10( dataBuffer[i] );
		}
	}

}

