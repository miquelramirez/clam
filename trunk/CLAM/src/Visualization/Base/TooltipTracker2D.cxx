/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "TooltipTracker2D.hxx"
#include "DrawingSurface2D.hxx"
#include "BoundingBoxes.hxx"
#include "Assert.hxx"
#include "CLAM_Math.hxx"
#include <cstdio>

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

		std::snprintf( textBuffer, maxLength, "X=%.3g, Y=%.3g", wX, wY);
		
	}

}
