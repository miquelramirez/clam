#include "SpectrumAndPeaksPlot.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "Fl_SMS_SpectrumAndPeaks.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "SpectralPeakArrayAdapter.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	SpectrumAndPeaksPlot::SpectrumAndPeaksPlot()
		: mpSpectrumAdapter( NULL ), mpPeakArrayAdapter( NULL ), mpWidget( NULL )
	{
		mpSpectrumAdapter = new LogMagSpectrumAdapter;
		mpPeakArrayAdapter = new SpectralPeakArrayAdapter;
		mpWidget = new Fl_SMS_SpectrumAndPeaks( 0, 0, 100, 100 );
		
		mpSpectrumAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );
		mpPeakArrayAdapter->ObjectPublished.Connect( mpWidget->SetPartials );
		
		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumAndPeaksPlot::SpectrumAndPeaksPlot( std::string name )
		: mpSpectrumAdapter( NULL ), mpPeakArrayAdapter( NULL ), mpWidget( NULL )
	{
		mpSpectrumAdapter = new LogMagSpectrumAdapter;
		mpPeakArrayAdapter = new SpectralPeakArrayAdapter;
		mpWidget = new Fl_SMS_SpectrumAndPeaks( 0, 0, 100, 100 );
		
		mpSpectrumAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );
		mpPeakArrayAdapter->ObjectPublished.Connect( mpWidget->SetPartials );
		
		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumAndPeaksPlot::~SpectrumAndPeaksPlot( )
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		if ( mpSpectrumAdapter )
			delete mpSpectrumAdapter;
		if ( mpPeakArrayAdapter )
			delete mpPeakArrayAdapter;
	}

	void SpectrumAndPeaksPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void SpectrumAndPeaksPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}
	
	void SpectrumAndPeaksPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void SpectrumAndPeaksPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetYRange( ymin, ymax );
	}

	void SpectrumAndPeaksPlot::SetTooltipFormat( const char* fmtStr )
	{
		mpWidget->SetTooltipFormat( fmtStr );
	}

	void SpectrumAndPeaksPlot::SetData( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& p )
	{
		mpSpectrumAdapter->BindTo( s );
		mpSpectrumAdapter->Publish();
		mpPeakArrayAdapter->BindTo( p );
		mpPeakArrayAdapter->Publish();
	}

	void SpectrumAndPeaksPlot::SetData( const CLAM::Spectrum& s )
	{
		mpSpectrumAdapter->BindTo( s );
		mpSpectrumAdapter->Publish();
	}

	void SpectrumAndPeaksPlot::SetData( const CLAM::SpectralPeakArray& p, double spectralRange  )
	{
		mpPeakArrayAdapter->BindTo( p );
		mpPeakArrayAdapter->Publish();
		mpWidget->SetSpectralRange( spectralRange );
	}
}
