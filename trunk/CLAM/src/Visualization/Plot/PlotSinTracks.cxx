#include "PlotSinTracks.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Array.hxx"
#include "SinTracksAdapter.hxx"
#include "Fl_SinTracks.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{

	void plot( const CLAM::Segment& model, const char* label )
	{
		CLAMVM::SinTracksAdapter ma;

		ma.BindTo( model );

		// create the widget
		CLAMVM::Fl_SinTracks presWidget( 100, 100, 640, 480, label );
		
		presWidget.AttachTo( ma );

		ma.Publish();

		presWidget.Show();

		// Run widget toolkit wrapper:
		CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();
		
	}

	void plot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label )
	{
		CLAMVM::SinTracksAdapter ma;

		ma.BindTo( model, sampleRate );

		// create the widget
		CLAMVM::Fl_SinTracks presWidget( 100, 100, 640, 480, label );
		
		presWidget.AttachTo( ma );

		ma.Publish();

		presWidget.Show();

		// Run widget toolkit wrapper:
		CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();

	}

	void deferredPlot( const CLAM::Segment& model, const char* label )
	{
		CLAMVM::SinTracksAdapter* ma = new SinTracksAdapter;

		ma->BindTo( model );

		// create the widget
		CLAMVM::Fl_SinTracks*  presWidget = new CLAMVM::Fl_SinTracks( 100, 100, 640, 480, label );
		
		presWidget->AttachTo( *ma );

		ma->Publish();

		Plotter::Hold( ma, presWidget );
	}

	void deferredPlot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label )
	{
		CLAMVM::SinTracksAdapter* ma = new SinTracksAdapter;

		ma->BindTo( model, sampleRate );

		// create the widget
		CLAMVM::Fl_SinTracks*  presWidget = new CLAMVM::Fl_SinTracks( 100, 100, 640, 480, label );
		
		presWidget->AttachTo( *ma );

		ma->Publish();

		Plotter::Hold( ma, presWidget );		
	}

}
