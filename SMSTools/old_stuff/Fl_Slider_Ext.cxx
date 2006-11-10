#include "Fl_Slider_Ext.hxx"

namespace CLAMVM
{
	Fl_Slider_Ext::Fl_Slider_Ext( int x, int y, int w, int h, const char* label )
		: Fl_Slider( x, y, w, h, label )
	{
		when( FL_WHEN_CHANGED );
		callback( (Fl_Callback* ) ValueChanged_cb, this );
	}

	Fl_Slider_Ext::~Fl_Slider_Ext()
	{
	}

	void Fl_Slider_Ext::ValueChanged( Fl_Widget* w, void* data )
	{
		Fl_Slider_Ext* pThis = (Fl_Slider_Ext*)data;
		pThis->ValueChanged.Emit( pThis->value() );
	}

}
