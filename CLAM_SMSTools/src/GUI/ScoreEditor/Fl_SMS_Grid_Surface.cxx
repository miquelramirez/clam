#include "Fl_SMS_Grid_Surface.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
	Fl_SMS_Grid_Surface::Fl_SMS_Grid_Surface( int X, int Y, int W, int H, const char* label )
		: Fl_Gl_2DSurface( X, Y, W, H ), mXAxisGridWidth( 0.1 ), mYAxisGridWidth( 0.1 )
	{
	}

	Fl_SMS_Grid_Surface::~Fl_SMS_Grid_Surface()
	{
	}

	void Fl_SMS_Grid_Surface::SetGridWidth( double xAxis, double yAxis )
	{
		mXAxisGridWidth = xAxis;
		mYAxisGridWidth = yAxis;
	}

	void Fl_SMS_Grid_Surface::DrawContents()
	{
		if ( MustReproject() )
			WCSProject();

		glClearColor( 1.0, 1.0, 1.0, 0.0 );
		glClear( GL_COLOR_BUFFER_BIT );

		DrawGridLines();
	}


	void Fl_SMS_Grid_Surface::DrawGridLines()
	{
		DataBoundBox dataBBox;
		QueryDataBoundBox( dataBBox );

		double xOffset = dataBBox.mLeft;
		double yOffset = dataBBox.mBottom;

		glBegin( GL_LINES );
		glColor3f( 0.1f, 0.1f, 0.6f );
		glLineWidth( 0.5 );

		while ( yOffset <= dataBBox.mTop )
		{
			glVertex2d( dataBBox.mLeft, yOffset );
			glVertex2d( dataBBox.mRight, yOffset );
			yOffset+=mYAxisGridWidth;
		}
		
		while ( xOffset <= dataBBox.mRight )
		{
			glVertex2d( xOffset, dataBBox.mBottom );
			glVertex2d( xOffset, dataBBox.mTop );
			xOffset+=mXAxisGridWidth;
		}

		glLineWidth( 1.0 );
		glEnd();
	}
}
