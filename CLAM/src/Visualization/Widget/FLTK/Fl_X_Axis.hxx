#ifndef __FL_X_AXIS__
#define __FL_X_AXIS__

#include "Fl_Axis.hxx"

namespace CLAMVM
{

class Fl_X_Axis: public Fl_Axis
{

protected:
    int min_pos() const;
    int max_pos() const;
    void draw();
    void draw_grid();
public:

    Fl_X_Axis(int x, int y, int w, int h, const char * label=0);
	~Fl_X_Axis();

};

}

#endif // Fl_X_Axis.hxx
