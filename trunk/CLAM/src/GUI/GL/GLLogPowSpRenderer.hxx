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

#ifndef __GLLOGPOWSPRENDERER__
#define __GLLOGPOWSPRENDERER__

#include "GLLinearSpRenderer.hxx"

namespace CLAMGUI
{
	class GLLogPowSpRenderer
	: public GLLinearSpRenderer
	{
	public:

		GLLogPowSpRenderer( unsigned char r = 0, unsigned char g = 255, unsigned char b = 0 )
		: GLLinearSpRenderer( r, g, b ), mDBScaling( 30 )
		{
		}

		inline void SetDBScaling( int scale )
		{
			mDBScaling = scale;
		}

	protected:

	virtual void DataTransform( const DataArray& array);
	virtual void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer );


	private:

	int mDBScaling;
};

}

#endif // GLLogPowSpRenderer.hxx
