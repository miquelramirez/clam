#include "PlotSpectralPeaks.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakArrayAdapter.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_SpectrumPeaks.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{
	void plot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks,
		   const char* label )
	{
		LogMagSpectrumAdapter specAdapter;
		SpectralPeakArrayAdapter specPeaksAdapter;
		
		specAdapter.BindTo( s );
		specPeaksAdapter.BindTo( speaks );

		Fl_SpectrumPeaks presWidget( 100, 100, 640, 480, label);
		
		presWidget.AttachTo( specAdapter, specPeaksAdapter );
		
		specAdapter.Publish();
		specPeaksAdapter.Publish();

		presWidget.Show();
		
		// Run widget toolkit wrapper:
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();		
	}

	void deferredPlot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks, const char* label  )
	{
		LogMagSpectrumAdapter* specAdapter = new LogMagSpectrumAdapter;
		SpectralPeakArrayAdapter* specPeaksAdapter = new SpectralPeakArrayAdapter;
		
		specAdapter->BindTo( s );
		specPeaksAdapter->BindTo( speaks );

		Fl_SpectrumPeaks* presWidget = new Fl_SpectrumPeaks( 100, 100, 640, 480, label);
		
		presWidget->AttachTo( *specAdapter, *specPeaksAdapter );
		
		specAdapter->Publish();
		specPeaksAdapter->Publish();
	
		Plotter::Hold( specAdapter, presWidget );

		delete specPeaksAdapter;
	}
}
