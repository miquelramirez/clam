/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "DrawingSurface2D.hxx"
#include <cmath>


namespace CLAMVM
{
	DrawingSurface2D::DrawingSurface2D()
		: mxmax( 1.0f ), mxmin( -1.0f ), mymax( 1.0f ), mymin( -1.0f ),
		  mcxmax( 1.0f ), mcxmin( -1.0f ), mcymax( 1.0f ), mcymin( -1.0f ),
		  mxdist( 0 ), mydist(0), mTooltipText( NULL )
	{
		mxdist = fabs( mxmax - mxmin );
		mydist = fabs( mymax - mymin );

		SetWorldSpace.Wrap( this, &DrawingSurface2D::_SetWorldSpace );
		AdjustXAxis.Wrap( this, &DrawingSurface2D::_AdjustXAxis );
		AdjustYAxis.Wrap( this, &DrawingSurface2D::_AdjustYAxis );
		SetLeft.Wrap( this, &DrawingSurface2D::_SetLeft );
		SetRight.Wrap( this, &DrawingSurface2D::_SetRight );
		SetBottom.Wrap( this, &DrawingSurface2D::_SetBottom );
		SetTop.Wrap( this, &DrawingSurface2D::_SetTop );
	}

	DrawingSurface2D::~DrawingSurface2D()
	{
	}

	void DrawingSurface2D::_SetWorldSpace( double xmax, double xmin, double ymax, double ymin )
	{
		mxmax = mcxmax = xmax;
		mxmin = mcxmin = xmin;
		mymax = mcymax = ymax;
		mymin = mcymin = ymin;
				
		mxdist = fabs( mxmax - mxmin );
		mydist = fabs( mymax - mymin );
				
		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::_SetLeft( double xmin )
	{
		mxmin = mcxmin = xmin;
		mxdist = fabs( mxmax - mxmin );
		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::_SetRight( double xmax )
	{
		mxmax = mcxmax = xmax;
		mxdist = fabs( mxmax - mxmin );
		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::_SetBottom( double ymin )
	{
		mymin = mcymin = ymin;
		mydist = fabs( mymax - mymin );
		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::_SetTop( double ymax )
	{
		mymax = mcymax = ymax;
		mydist = fabs( mymax-mymin );
		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::QueryDataBoundBox( DataBoundBox& bbox )
	{
		bbox.mLeft = mcxmin;
		bbox.mRight = mcxmax;
		bbox.mTop = mcymax;
		bbox.mBottom = mcymin;
		bbox.mNear = 0;
		bbox.mFar = -1;
	}

	void DrawingSurface2D::_AdjustXAxis( double offset, double scale )
	{
		mcxmin = mxmin + mxdist*offset;
		mcxmax = mcxmin + mxdist*scale;

		DamageProjection();
		Refresh();
	}

	void DrawingSurface2D::_AdjustYAxis( double offset, double scale )
	{
		mcymin = mymin + mydist*offset;
		mcymax = mcymin + mydist* scale;

		DamageProjection();
		Refresh();
	}		
}
