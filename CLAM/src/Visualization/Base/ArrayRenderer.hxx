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

/** \file ArrayRenderer.hxx
 */

#ifndef __ARRAYRENDERER__
#define __ARRAYRENDERER__

#include "DataTypes.hxx"
#include "Viewport.hxx"
#include "Array.hxx"
using CLAM::TData;
using CLAM::DataArray;

/** \namespace CLAMGUI
 *  \brief     The old Visualization Module namespace.
 *
 *  This namespace is deprecated so its usage is STRONGLY discouraged. The official
 *  namespace now is CLAMVM 
 *  @see CLAMVM
 */
namespace CLAMGUI
{

/** \class ArrayRenderer
 *  \brief Deprecated Interface.
 *
 *  This interface is <em>deprecated</em>, so its usage is strongly discouraged. Since
 *  it will be removed soon.
 */
class ArrayRenderer
{
public:

	virtual void CacheData( const DataArray& ) = 0;

	virtual void DefineViewport( const DataArray&, Viewport& ) = 0;

	virtual void Draw() = 0;

protected:

	virtual void DataTransform( const DataArray& ) = 0;

	virtual void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer ) = 0;

	virtual void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer ) = 0;

};

}

#endif // ArrayRenderer.hxx

