#include "Fl_Gl_Multi_Display.hxx"
#include "GLRenderingManager.hxx"
#include "Assert.hxx"

namespace CLAMVM
{

		Fl_Gl_Multi_Display::Fl_Gl_Multi_Display( int X, int Y, int W, int H, const char* label )
				: Fl_Gl_2DSurface( X, Y, W, H, label )
		{
		}

		Fl_Gl_Multi_Display::~Fl_Gl_Multi_Display()
		{
		}

		void Fl_Gl_Multi_Display::AddRenderer( GLRenderingManager& mgr )
		{
				mRenderers.push_back( &mgr );
		}				

		void Fl_Gl_Multi_Display::DrawContents()
		{
				CLAM_DEBUG_ASSERT( mRenderers.size() > 0, "No renderers associated!" );

				tContainer::iterator i, end = mRenderers.end();

				if ( MustReproject() )
				{
						WCSProject();
						DataBoundBox dataBBox;
						QueryDataBoundBox( dataBBox );
						ScreenBoundBox scrBBox = { 0,0, w(), h() };

						for ( i = mRenderers.begin(); i != end; i++ )
						{
								(*i)->SetWorldBounds( dataBBox );
								(*i)->SetPixelBounds( scrBBox );
						}
				}

				for ( i = mRenderers.begin(); i != end; i++ )
				{
						(*i)->RenderData();
				}
		}
}
