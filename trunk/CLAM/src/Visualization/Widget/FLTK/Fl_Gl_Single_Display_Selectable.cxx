#include "Fl_Gl_Single_Display_Selectable.hxx"

namespace CLAMVM
{

	int Fl_Gl_Single_Display_Selectable::handle( int evtCode )
	{
		return 0;
	}

	void Fl_Gl_Single_Display_Selectable::draw_overlay()
	{
	}

	void Fl_Gl_Single_Display_Selectable::DrawContents()
	{
		Fl_Gl_Single_Display::DrawContents();

		draw_overlay();
	}

}
