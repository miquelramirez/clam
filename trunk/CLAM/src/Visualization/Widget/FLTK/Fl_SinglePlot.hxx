#ifndef __FL_SINGLEPLOT__
#define __FL_SINGLEPLOT__

#include "Fl_Plot.hxx"

namespace CLAMVM
{
	class GLRenderingManager;

	class Fl_SinglePlot : public Fl_Plot
	{		
	public:
		Fl_SinglePlot();
		virtual ~Fl_SinglePlot();
		
		void SetRenderingManager( GLRenderingManager* mgr );

	protected:

		virtual Fl_Gl_2DSurface* CreateDisplay( int X, int Y, int W, int H );
		virtual void AddRenderersToDisplay( Fl_Gl_2DSurface* display );

	protected:
		GLRenderingManager*    mRenderer;
	};
}


#endif // Fl_SinglePlot.hxx
