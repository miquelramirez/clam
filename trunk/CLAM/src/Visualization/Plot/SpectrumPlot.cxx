#include "SpectrumPlot.hxx"
#include "Spectrum.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_SMS_Spectrum.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	SpectrumPlot::SpectrumPlot( )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new LogMagSpectrumAdapter;
		mpWidget = new Fl_SMS_Spectrum( 0, 0, 100, 100 );

		mpAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );

		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumPlot::SpectrumPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new LogMagSpectrumAdapter;
		mpWidget = new Fl_SMS_Spectrum( 0, 0, 100, 100 );

		mpAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );

		SetName( name  );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumPlot::~SpectrumPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		delete mpAdapter;
	}

	void SpectrumPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void SpectrumPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void SpectrumPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void SpectrumPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetYRange( ymin, ymax );
	}

	void SpectrumPlot::SetTooltipFormat( const char* tooltipFmt )
	{
		mpWidget->SetTooltipFormat( tooltipFmt );
	}

	void SpectrumPlot::SetData( const CLAM::Spectrum& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
}
