#ifndef __FL_Y_AXIS__
#define __FL_Y_AXIS__

#include "Fl_Axis.hxx"

namespace CLAMVM
{

class Fl_Y_Axis
		: public Fl_Axis
{
		int min_pos() const;
		int max_pos() const;
		
		void draw();
		void draw_grid();
public:

		Fl_Y_Axis(int x, int y, int w, int h, const char * label=0);
		~Fl_Y_Axis();
	
		
};

}

#endif // Fl_Y_Axis.hxx
