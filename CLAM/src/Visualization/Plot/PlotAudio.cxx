#include "PlotAudio.hxx"
#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "Fl_Audio.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{

	void plot(const CLAM::Audio &model, const char *label)
	{
		// Create model adaptor:
		AudioAdapter ma;

		// Bind model to the model adapter:
		ma.BindTo( model);

		// Create widget to serve as the presentation of the model:
		Fl_Audio presentation(100, 100, 640, 480, label);

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

	void deferredPlot( const CLAM::Audio& model, const char* label )
	{
		
		// Create model adaptor:
		AudioAdapter* adapter = new AudioAdapter;

		// Bind model to the model adapter:
		adapter->BindTo( model);

		// Create widget to serve as the presentation of the model:
		Fl_Audio* widget = new Fl_Audio(100, 100, 640, 480, label);

		// Attach model adapter to presentation:
		widget->AttachTo( *adapter );

		// Let the model adaptor publish it's data:
		adapter->Publish();

		Plotter::Hold( adapter, widget );
	}
	
}
