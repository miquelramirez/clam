#include "Fl_SinglePlot.hxx"
#include "GLRenderingManager.hxx"
#include "Fl_Gl_Single_Display.hxx"

namespace CLAMVM
{

	Fl_SinglePlot::Fl_SinglePlot( )
		: mRenderer( NULL )
	{
	}

	Fl_SinglePlot::~Fl_SinglePlot( )
	{
	}

	void Fl_SinglePlot::SetRenderingManager( GLRenderingManager* mgr )
	{
		if ( mRenderer == NULL )
			mRenderer = mgr;
		else
		{
			delete mRenderer;
			mRenderer = mgr;
		}
		AddRenderersToDisplay( GetDisplay() );
	}

	Fl_Gl_2DSurface* Fl_SinglePlot::CreateDisplay( int X, int Y, int W, int H )
	{
		return new Fl_Gl_Single_Display(  X, Y, W, H );

	}

	void Fl_SinglePlot::AddRenderersToDisplay( Fl_Gl_2DSurface* display )
	{
		if ( display == NULL )
			return;
		else
		{
			Fl_Gl_Single_Display* disp = static_cast<Fl_Gl_Single_Display*>(display);

			disp->SetRenderer( *mRenderer );
		}
	}
}
