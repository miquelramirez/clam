#include "Plot.hxx"
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

	CLAMVM::Plot myUglyPlot( "igloo" );

	myUglyPlot.SetPosition( 100, 100 );
	myUglyPlot.SetSize( 640, 480 );
	myUglyPlot.SetLabel( "My ugly plot" );
	myUglyPlot.SetYRange( 0.0, 1.0 );
	myUglyPlot.SetColor( CLAMVM::Color( 150, 150, 0 ) );

	CLAMVM::Plot myUglyPlot2( "igloo2" );

	myUglyPlot2.SetPosition( 100, 100 );
	myUglyPlot2.SetSize( 640, 480 );
	myUglyPlot2.SetLabel( "My ugly plot" );
	myUglyPlot2.SetYRange( 0.0, 1.0 );
	myUglyPlot2.SetTooltipFormat( "x=%3.2f, P(x)=%1.3f" );
	myUglyPlot2.SetColor( CLAMVM::Color( 0, 150, 150 ) );

	buildTestArray( population );

	myUglyPlot.SetData( population, 0, 99 );

	//CLAMVM::SystemPlots::Display( "igloo" );

	buildTestBPF( func );

	myUglyPlot2.SetData( func );

	//CLAMVM::SystemPlots::Display( "igloo2" );

	CLAMVM::SystemPlots::Display( "igloo" );
	return 0;
}

