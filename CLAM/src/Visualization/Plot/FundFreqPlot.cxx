#include "FundFreqPlot.hxx"
#include "Segment.hxx"
#include "FundamentalAdapter.hxx"
#include "Fl_SMS_FundFreq_Browser.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	FundFreqPlot::FundFreqPlot()
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new FundamentalAdapter;
		mpWidget = new Fl_SMS_FundFreq_Browser( 0, 0, 100, 100 );
		
		mpAdapter->TrajectoryExtracted.Connect( mpWidget->NewTrajectory );
		mpAdapter->TimeSpanChanged.Connect( mpWidget->NewTimeSpan );

		mpWidget->NewFreqRangeHint( 50, 8000 );

		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );

	}

	FundFreqPlot::FundFreqPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new FundamentalAdapter;
		mpWidget = new Fl_SMS_FundFreq_Browser( 0, 0, 100, 100 );
		
		mpAdapter->TrajectoryExtracted.Connect( mpWidget->NewTrajectory );
		mpAdapter->TimeSpanChanged.Connect( mpWidget->NewTimeSpan );

		mpWidget->NewFreqRangeHint( 50, 8000 );

		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );

	}

	FundFreqPlot::~FundFreqPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		if ( mpAdapter )
			delete mpAdapter;		
	}

	void FundFreqPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void FundFreqPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void FundFreqPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void FundFreqPlot::SetYRange( double ymin, double ymax )
	{
	}

	void FundFreqPlot::SetTooltipFormat( const char* fmtStr )
	{
		mpWidget->SetTooltipFormat( fmtStr );
	}

	void FundFreqPlot::SetData( const CLAM::Segment& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
	
}
