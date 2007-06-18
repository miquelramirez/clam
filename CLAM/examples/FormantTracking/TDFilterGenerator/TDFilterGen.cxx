#include "TDFilterGen.hxx"
#include "ComplexToPolarCnv.hxx"

namespace CLAM
{

	void TDFilterGenConfig::DefaultInit(void)
	{
		AddAll();
		UpdateData();
	}

	TDFilterGen::TDFilterGen() :
		mSpectralRange( 0. ),
		mType( ETDFilterType::eAutoregressive )
	{ 
		TDFilterGenConfig cfg;

		Configure( cfg );
	}

	TDFilterGen::TDFilterGen( TDFilterGenConfig &cfg ) :
		mSpectralRange( 0. ),
		mType( ETDFilterType::eAutoregressive )
	{
		Configure( cfg );
	}

	bool TDFilterGen::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig,c);

		if( mConfig.HasType() )
			mType = mConfig.GetType();
		if( mConfig.HasSpectralRange() )
			mSpectralRange = mConfig.GetSpectralRange();
		if( mConfig.HasSamplingRate() )
			mSamplingRate = mConfig.GetSamplingRate();

		return true;
	}

	bool TDFilterGen::Do( TDFilterKernel &out )
	{
		out.SetType( mType );

		switch( (int) mType )
		{
		case ETDFilterType::eFIR:
			{
	/*			TSize order = mConfig.GetBCoefficients().Size();

				out.SetZerosSize( order - 1 );

				out.B( mConfig.GetBCoefficients() );

				out.CheckTypes();

				CalculateRoots( out.B(), out.Zeros() );

				CalculateFreqs( out.Zeros() , out.GetFreqZeros() );
				CalculateMagnitudeFR( out.Zeros(), out.GetAmpZeros() );
*/
				break;
			}
		case ETDFilterType::eAutoregressive:
			{
				TSize order = mConfig.GetACoefficients().Size();
				Array< Complex > poles;
				bool hasFreqs;
				unsigned nfreqs;
		
				out.SetPolesSize( order - 1 );

				TSize newSize = out.Poles().Size() / 2; 
				poles.Resize(newSize);
				poles.SetSize(newSize);

				out.A( mConfig.GetACoefficients() );

				CalculateRoots( out.A() , out.Poles());
				CalculateFreqs( out.Poles() , out.GetFreqPoles(), poles, hasFreqs, nfreqs);
				out.SetNPoles(nfreqs);

				if( hasFreqs )
					CalculateMagnitudeFR( poles, out.GetAmpPoles() );

				break;
			}
		case ETDFilterType::eARMA:
			{
				TSize orderPoles = mConfig.GetACoefficients().Size();
				TSize orderZeros = mConfig.GetBCoefficients().Size();

				
				out.SetPolesSize( orderPoles - 1 );
				out.SetZerosSize( orderZeros - 1 );

				out.A( mConfig.GetACoefficients() );
				out.B( mConfig.GetBCoefficients() );


				CalculateRoots( out.A(), out.Poles() );
				CalculateRoots( out.B(), out.Zeros() );

//				CalculateFreqs( out.Poles() , out.GetFreqPoles() );
//				CalculateFreqs( out.Zeros() , out.GetFreqZeros() );

//				CalculateMagnitudeFR( out.Poles(), out.GetAmpPoles() );
				//CalculateMagnitudeFR( out.Zeros(), out.GetAmpPoles() );

				break;
			}			
		}


		return true;
	}

	bool TDFilterGen::ConcreteStart(void)
	{
		return true;
	}

	bool TDFilterGen::ConcreteStop(void)
	{
		return true;
	}

	void TDFilterGen::CalculateRoots( const DataArray &coeff , Array<Complex> &roots)
	{
		DataArray	u, v, conv;
		int degree,i;
		long maxiter, fig;

		degree = coeff.Size()-1;
		maxiter = 160;
		fig = 4;

		u.Resize( coeff.Size() );
		u.SetSize( coeff.Size() );

		v.Resize( coeff.Size() );
		v.SetSize( coeff.Size() );

		conv.Resize( coeff.Size() );
		conv.SetSize( coeff.Size() );

		FindPolynomialRoots( coeff.GetPtr(),
							 u.GetPtr(), v.GetPtr(), conv.GetPtr(),
							 degree, maxiter, fig );

		for( i = 0; i < roots.Size(); i++)
			roots[ i ] = Complex( u[i], v[i] );
	}

	void TDFilterGen::CalculateFreqs( const Array< Complex > &p, DataArray &freqs, Array< Complex > &pp,
									  bool &hasFreqs, unsigned &nfreqs)
	{
		
		int i;
		TData SR = mSamplingRate / ( 2.*PI );
		nfreqs = 0;

		for( i = 0; i < p.Size()-1; i++ )
		{
			// Unused variable: TData k=p[ i ].Ang();
			if( /*(j<freqs.Size())*/(p[ i ].Imag() != 0)&&(p[i].Ang()>0) )
			{
				freqs[ nfreqs ] = p[ i ].Ang() * SR;
				pp[ nfreqs ] = p[ i ];
				nfreqs++;
			}
		}

		if( nfreqs == 0 ) hasFreqs = false; 
		else		hasFreqs = true; 
	}

	void TDFilterGen::CalculateMagnitudeFR( const Array< Complex > &r, DataArray &mag )
	{
		int m;
		ComplexToPolarCnv_ conv;
		Array< Polar > polar;
		
		polar.Resize( r.Size() );
		polar.SetSize( r.Size() );

		conv.ToPolar( r, polar );

		for( m = 0; m < r.Size(); m++ )
		{
			mag[ m ] = 1./fabs( 1.- polar[m].Mag() );
/*			w = - polar[ m ].Ang() ;

//			if( w != 0 )
//			{
				denom = 1.;

				for( p = 0; p < r.Size() - 1 ; p++ )
					denom *= 1 - polar[p].Mag() * cos( w + polar[p].Ang() );

				mag[ m ] = 1. / denom;
//			}
//			else
//				mag[ m ] = 0;
*/
		}
	}

} // namespace
