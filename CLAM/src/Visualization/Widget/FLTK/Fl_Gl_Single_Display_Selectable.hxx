#ifndef __FLGLSINGLEDISPLAYSELECTABLE__
#define __FLGLSINGLEDISPLAYSELECTABLE__

namespace CLAMVM
{

	class Fl_Gl_Display_Selectable : public Fl_Gl_Single_Display
	{
	public:
		virtual DrawContents();

		int handle( int evtCode );

	protected:

		void draw_overlay();
	};

}


#endif // Fl_Gl_Single_Display_Selectable.hxx
