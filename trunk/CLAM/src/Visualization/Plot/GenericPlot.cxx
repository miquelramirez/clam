#include "GenericPlot.hxx"
#include "Fl_Plot.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{

	GenericPlot::~GenericPlot()
	{
		DestroyWidget();
	}

	void GenericPlot::CreateWidget( )
	{
		mWidget = NULL;
	}

	void GenericPlot::DestroyWidget( )
	{
		if ( mWidget != NULL && mWidget->parent() == NULL )
		{
			delete mWidget;
			SetWidget( NULL );
		}

	}

	void GenericPlot::RegisterAnonymously( )
	{
		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), GetWidget() );
	}

	void GenericPlot::RegisterAs( std::string name )
	{
		if ( SystemPlots::IsNameInUse( name ) )
			SetName( SystemPlots::GenerateNewUniqueName() );
		
		SystemPlots::RegisterWidget( GetName(), GetWidget() );

	}

	void GenericPlot::SetPosition( int x, int y )
	{
		GetWidget()->position( x, y );		
	}

	void GenericPlot::SetSize( int w, int h )
	{
		GetWidget()->size( w, h );
	}

	void GenericPlot::SetLabel( const char* label )
	{
		GetWidget()->label( label );
	}

	void GenericPlot::SetYRange( double ymin, double ymax )
	{
		GetWidget()->SetYRange( ymin, ymax );
	}

	void GenericPlot::SetTooltipFormat( const char* tooltipFmt )
	{
		GetWidget()->SetTipFormatString( tooltipFmt );
	}

}
