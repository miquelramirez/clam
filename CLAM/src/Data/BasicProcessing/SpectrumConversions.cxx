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
						linBuffer[i] = pow( TData(10), logBuffer[i]*inv_20 ) ;
				}
		}

		void Log2LinearMagnitude( DataArray& dataBuffer )
		{
				const TSize nBins = dataBuffer.Size();
				static const TData inv_20 = 1.0f/20.0f;

				for ( int i = 0; i < nBins; i++ )
				{
						dataBuffer[i] = pow( TData(10), dataBuffer[i]*inv_20 ) ;
				}
		}

		void Linear2LogMagnitude( const DataArray& linearBuffer, DataArray& logData )
		{
				const TSize nBins = linearBuffer.Size();

				AssertProperSize( nBins, logData );
				
				for ( int i = 0; i < nBins; i++ )
				{
					logData[i]=20*log10(linearBuffer[i]);
				}

		}

		void Linear2LogMagnitude( DataArray& dataBuffer )
		{
				const TSize nBins = dataBuffer.Size();

				for ( int i = 0; i < nBins; i++ )
				{
					dataBuffer[i]=20*log10( dataBuffer[i] );
				}
		}

}
