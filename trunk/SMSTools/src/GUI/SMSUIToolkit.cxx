#include "SMSUIToolkit.hxx"
#include <FL/Fl.H>

namespace SMS
{

namespace ToolsUI
{
	void Toolkit::AdjustLookAndFeel()
	{
		Fl::get_system_colors();
		
		Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
		Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
		Fl::set_boxtype(FL_UP_FRAME,FL_THIN_UP_FRAME);
		Fl::set_boxtype(FL_DOWN_FRAME,FL_THIN_DOWN_FRAME);

	}

	void Toolkit::QueryDesktopMetrics( int& desktopW, int& desktopH )
	{
		desktopW = Fl::w();
		desktopH = Fl::h();
	}
}

}
