#ifndef __FL_SMS_HARMONIZER__
#define __FL_SMS_HARMONIZER__

#include <FL/Fl_Group.H>
#include "BoundingBoxes.hxx"

class Fl_Box;
class Fl_Light_Button;

namespace CLAMVM
{
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_SMS_Grid_Surface;

	class Fl_SMS_Harmonizer : public Fl_Group
	{
	public:
		Fl_SMS_Harmonizer( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SMS_Harmonizer();

		int handle( int event );
		
		virtual void Show();
		virtual void Hide();

	protected:

		Fl_X_Axis*           mpXAxis;
		Fl_Y_Axis*           mpYAxis;
		Fl_SMS_Grid_Surface* mpDisplay;
		Fl_Box*              mpImposter;
		DataBoundBox         mWSC;
		Fl_Light_Button*     mAddButton;
		Fl_Light_Button*     mRemoveButton;
		Fl_Light_Button*     mMoveButton;
	};
}


#endif // Fl_SMS_Harmonizer.hxx
