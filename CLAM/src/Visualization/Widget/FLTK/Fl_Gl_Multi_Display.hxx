#ifndef __FL_GL_MULTI_DISPLAY__
#define __FL_GL_MULTI_DISPLAY__

#include "Fl_Gl_2DSurface.hxx"
#include <vector>

namespace CLAMVM
{
	class GLRenderingManager;

	class Fl_Gl_Multi_Display : public Fl_Gl_2DSurface
	{
	public:
		Fl_Gl_Multi_Display( int X, int Y, int W, int H, const char* label = 0 );

		~Fl_Gl_Multi_Display();

		void AddRenderer( GLRenderingManager& mgr );
		void ClearRenderers();
	protected:
		void DrawContents();
	private:
		typedef std::vector< GLRenderingManager* > tContainer;

		tContainer    mRenderers;
	};

}

#endif // Fl_Gl_Multi_Display.hxx
