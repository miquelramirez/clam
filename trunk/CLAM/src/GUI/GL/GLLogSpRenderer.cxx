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

#include "GLLogSpRenderer.hxx"
#include "DataTypes.hxx"
using CLAM::TData;
using CLAM::DB;
using namespace CLAMGUI;

void GLLogSpRenderer::DataTransform( const DataArray& array )
{
	for ( int k = 0; k < array.Size(); k++ )
	{
		mIntertwined[k].y = GLfloat( DB( array[k], mDBScaling ) ); 
	} 
}

void GLLogSpRenderer::YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
{
	transtop = DB( top, mDBScaling );
	transtop = transtop + transtop * 0.1f;
	transbottom = bottom;
	integer = true;
}
