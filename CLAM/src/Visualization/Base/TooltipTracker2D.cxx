#include "TooltipTracker2D.hxx"
#include "DrawingSurface2D.hxx"
#include "BoundingBoxes.hxx"
#include "Assert.hxx"
#include "CLAM_Math.hxx"
#include <cstdio>

#if _MSC_VER <= 1310
#define STD_SNPRINTF snprintf
#else
#define STD_SNPRINTF std::snprintf
#endif


namespace CLAMVM
{
	TooltipTracker2D::TooltipTracker2D()
		: mTrackedSurface( NULL )
	{
		RenderTooltipText.Wrap( this, &TooltipTracker2D::DefaultTooltipTextRender );
	}

	TooltipTracker2D::~TooltipTracker2D()
	{
	}

	void TooltipTracker2D::Track( DrawingSurface2D* surf )
	{
		CLAM_ASSERT( surf!=NULL, "The surface was NULL" );
		mTrackedSurface = surf;
	}

	bool TooltipTracker2D::HandleMotion( int ex, int ey )
	{
		if ( !mTrackedSurface )
			return false;
		// First check if the motion is inside or not of the widget
		ScreenBoundBox sbbox;
		mTrackedSurface->QueryScreenBoundBox( sbbox );

		if  ( !sbbox.IsPointInside( ex, ey ) )
			return false;

		RenderTooltipText( ex - sbbox.pixel_left, ey -sbbox.pixel_top, 
				   mTextBuffer, 256 );
		mTrackedSurface->SetTooltipText( mTextBuffer );
		mTrackedSurface->RedrawOverlay();
			
		return true;
	}
	
	void TooltipTracker2D::ForceText( const char* text )
	{
		if ( !mTrackedSurface )
			return;
		mTrackedSurface->SetTooltipText( text );
		mTrackedSurface->RedrawOverlay();
	}

	void TooltipTracker2D::DefaultTooltipTextRender( int x, int y, char* textBuffer, int maxLength )
	{
		// world coordinates
		DataBoundBox    worldBox;
		ScreenBoundBox  screenBox;
		mTrackedSurface->QueryDataBoundBox( worldBox );
		mTrackedSurface->QueryScreenBoundBox( screenBox );

		double wX = ((double)x / (double)screenBox.pixel_width)* ( fabs( worldBox.mRight - worldBox.mLeft ) ) + worldBox.mLeft;
		double wY = ((double)y / (double)screenBox.pixel_height) * ( fabs( worldBox.mTop - worldBox.mBottom ) );

		STD_SNPRINTF( textBuffer, maxLength, "X=%.3g, Y=%.3g", wX, wY);
		
	}

}
