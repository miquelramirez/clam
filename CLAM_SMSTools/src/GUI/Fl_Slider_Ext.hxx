#ifndef __FL_SLIDER_EXT__
#define __FL_SLIDER_EXT__

#include "Signalv1.hxx"
#include <FL/Fl_Slider.H>

namespace CLAMVM
{
	using SigSlot::Signalv1;

	class Fl_Slider_Ext : public Fl_Slider
	{
	public:
		Fl_Slider_Ext( int x, int y, int w, int h, const char* label = 0 );

		virtual ~Fl_Slider_Ext();

		Signalv1<double>   ValueChanged;

	protected:

		static void ValueChanged_cb( Fl_Widget* w, void* data );
	};
}

#endif // Fl_Slider_Ext.hxx
