#include "PlotSpectrum.hxx"
#include "Spectrum.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_Spectrum.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{

	void plot(const CLAM::Spectrum &model, const char *label)
	{
		// Create model adaptor:
		LogMagSpectrumAdapter ma;

		// Bind model to the model adapter:
		ma.BindTo(model);

		// Create widget to serve as the presentation of the model:
		Fl_Spectrum presentation(100, 100, 640, 480, label);

		// Attach model adapter to presentation:
		presentation.AttachTo(ma);

		// Let the model adaptor publish it's data:
		ma.Publish();

		// Force presentation (widget) to refresh:	
		presentation.Show();

		// Run widget toolkit wrapper:
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();
	}

	void deferredPlot( const CLAM::Spectrum& model, const char* label)
	{
		LogMagSpectrumAdapter* adapter = new LogMagSpectrumAdapter;
		adapter->BindTo( model );
		
		Fl_Spectrum* widget = new Fl_Spectrum(100, 100, 640, 480, label);
	
		widget->AttachTo( *adapter );

		adapter->Publish();

		Plotter::Hold( adapter, widget );
	}

}
