#include "AudioPlot.hxx"
#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	AudioPlot::AudioPlot()
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new AudioAdapter;
		mpWidget = new Fl_SMS_Browsable_Playable_Audio( 0, 0, 100, 100 );
		mpAdapter->ObjectPublished.Connect( mpWidget->SetAudio );
		
		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	AudioPlot::AudioPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new AudioAdapter;
		mpWidget = new Fl_SMS_Browsable_Playable_Audio( 0, 0, 100, 100 );
		mpAdapter->ObjectPublished.Connect( mpWidget->SetAudio );		

		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	AudioPlot::~AudioPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		delete mpAdapter;
	}

	void AudioPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void AudioPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void AudioPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void AudioPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetYRange( ymin, ymax );
	}

	void AudioPlot::SetTooltipFormat( const char* tooltipFmt )
	{
		mpWidget->SetTooltipFormat( tooltipFmt );
	}

	void AudioPlot::SetData( const CLAM::Audio& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
}
