#include "Fl_Gl_Single_Display.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
	Fl_Gl_Single_Display::Fl_Gl_Single_Display( int X, int Y, int W, int H, const char* label)
		: Fl_Gl_2DSurface( X, Y, W, H, label ), mRenderer( NULL )
	{
	}

	Fl_Gl_Single_Display::~Fl_Gl_Single_Display()
	{
	}

	void Fl_Gl_Single_Display::DrawContents()
	{
		CLAM_DEBUG_ASSERT( mRenderer!=NULL, "No renderer was assigned to this display!" );

		if ( MustReproject() )
		{
			WCSProject();
			DataBoundBox dataBBox;
			QueryDataBoundBox( dataBBox );
			ScreenBoundBox scrBBox = { x(), y(), w(), h() };
						
			mRenderer->SetWorldBounds( dataBBox );
			mRenderer->SetPixelBounds( scrBBox );
		}

		glClear( GL_COLOR_BUFFER_BIT );

		mRenderer->RenderData();

	}
}
