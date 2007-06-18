#include "TDFilterGen.hxx"

#include "XMLStorage.hxx"

int main()
{
	try
	{
		int order = 3;
		float spectralRange = 5000.;
		CLAM::DataArray A;

		A.Resize( order );
		A.SetSize( order );

		A[ 0 ] = 1.0;
		A[ 1 ] = 0.38462;
		A[ 2 ] = 0.76923;

		CLAM::TDFilterKernel myKernel;

		CLAM::TDFilterGenConfig GenCfg;

		GenCfg.SetName( "my_TDFilterGenerator" );
		GenCfg.SetType( CLAM::ETDFilterType::eAutoregressive );
		GenCfg.SetSpectralRange( spectralRange );
		GenCfg.SetACoefficients( A );

		CLAM::TDFilterGen	myGenerator( GenCfg );

		myGenerator.Start();

		myGenerator.Do( myKernel );

		CLAM::XMLStorage::Dump( myKernel.Poles(), "Roots", "Roots.xml" );
		CLAM::XMLStorage::Dump( myKernel.GetAmpPoles(), "Amplitudes", "Amps.xml" );
		CLAM::XMLStorage::Dump( myKernel.GetFreqPoles(), "Frequencies", "Freqs.xml" );

	}
	catch( CLAM::Err err )
	{
		err.Print();
	}

	return 0;
}
