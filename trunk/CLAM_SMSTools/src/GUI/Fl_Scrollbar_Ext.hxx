#ifndef __FL_SCROLLBAR_EXT__
#define __FL_SCROLLBAR_EXT__

#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include <FL/Fl_Scrollbar.H>

namespace CLAMVM
{
	class Fl_Scrollbar_Ext : public Fl_Scrollbar
	{
	public:
		Fl_Scrollbar_Ext( int x, int y, int w, int h, const char* label = 0 );

		virtual ~Fl_Scrollbar_Ext();

		Signalv1<double>   ValueChanged;
		Slotv1<double>     ChangeStep;
		
	protected:
		
		static void ValueChanged_cb( Fl_Widget* w, void* data );
	};
}

#endif // Fl_Scrollbar_Ext.hxx
