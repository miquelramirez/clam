#ifndef __FL_GL_SINGLE_DISPLAY__
#define __FL_GL_SINGLE_DISPLAY__

#include "Fl_Gl_2DSurface.hxx"
#include "GLRenderingManager.hxx"

namespace CLAMVM
{
	class Fl_Gl_Single_Display : public Fl_Gl_2DSurface
	{
	public:
			Fl_Gl_Single_Display( int X, int Y, int W, int H, const char* label = 0);

			~Fl_Gl_Single_Display();

			void SetRenderer( GLRenderingManager& renderer )
			{
					mRenderer = &renderer;
					mRenderer->SetSurface( this );
			}

	protected:

			virtual void DrawContents();
	private:
			GLRenderingManager* mRenderer;
	};
}

#endif // Fl_Gl_Single_Display.hxx
