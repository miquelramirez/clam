#include "MultiPlot.hxx"
#include "SystemPlots.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include <iostream>

void buildTestArray( CLAM::Array<CLAM::TData>& array )
{
	array.Resize( 100 );
	array.SetSize( 100 );

	for ( int i = 0; i < 100; i++ )
	{
		array[i] = float(i)*0.005;
	}
}

void buildTestBPF( CLAM::BPF& bpf )
{
	bpf.Insert( -10.0, 0.0 );
	bpf.Insert( -5.0, 0.45 );
	bpf.Insert( 0.0, 1.0 );
	bpf.Insert( 5.0, 0.75 );
	bpf.Insert( 10.0, 0.0 );
}

int main( int argc, char** argv )
{
	
	CLAM::Array<CLAM::TData> population;
	CLAM::BPF func;

	buildTestArray( population );
	buildTestBPF( func );

	CLAMVM::MultiPlot myUglyPlot( "igloo" );

	myUglyPlot.SetPosition( 100, 100 );
	myUglyPlot.SetSize( 640, 480 );
	myUglyPlot.SetLabel( "My ugly plot" );
	myUglyPlot.SetYRange( 0.0, 1.0 );
	myUglyPlot.SetTooltipFormat( "x=%3.2f, P(x)=%1.3f" );

	myUglyPlot.SetColor( "Empirical PDF", CLAMVM::Color( 150, 150, 0 ) );
	myUglyPlot.AddData( "Empirical PDF", population, 0, 99 );
	myUglyPlot.SetColor( "Theoretical PDF", CLAMVM::Color( 0, 150, 150 ) );
	myUglyPlot.AddData( "Theoretical PDF", func );

	CLAMVM::SystemPlots::Display( "igloo" );


	return 0;
}

