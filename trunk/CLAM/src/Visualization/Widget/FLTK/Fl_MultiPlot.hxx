#ifndef __FL_MULTIPLOT__
#define __FL_MULTIPLOT__

#include "Fl_Plot.hxx"
#include <list>

namespace CLAMVM
{
	class GLRenderingManager;

	class Fl_MultiPlot : public Fl_Plot
	{
	public:
		Fl_MultiPlot();
		virtual ~Fl_MultiPlot();

		void AddRenderer( GLRenderingManager* mgr );
		void RemoveRenderer( GLRenderingManager* mgr );
		void ClearRenderers( );

	protected:

		virtual Fl_Gl_2DSurface* CreateDisplay( int X, int Y, int W, int H );
		virtual void AddRenderersToDisplay( Fl_Gl_2DSurface* display );

	protected:
		typedef std::list<GLRenderingManager* > tContainer;
		tContainer  mRenderers;
		
	};

}

#endif // Fl_MultiPlot.hxx
