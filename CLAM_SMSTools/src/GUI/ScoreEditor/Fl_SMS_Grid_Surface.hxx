#ifndef __FL_SMS_GRID_SURFACE__
#define __FL_SMS_GRID_SURFACE__

#include "Fl_Gl_2DSurface.hxx"

namespace CLAMVM
{
	class Fl_SMS_Grid_Surface : public Fl_Gl_2DSurface
	{
	public:

		Fl_SMS_Grid_Surface( int X, int Y, int W, int H, const char* label = 0);
		~Fl_SMS_Grid_Surface();		

		void SetGridWidth( double xAxis, double yAxis );

	protected:

		virtual void DrawContents();
		void DrawGridLines();

	protected:
		
		double  mXAxisGridWidth;
		double  mYAxisGridWidth;
	};

}

#endif // Fl_SMS_Grid_Surface.hxx
