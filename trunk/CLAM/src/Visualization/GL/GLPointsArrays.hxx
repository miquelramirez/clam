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

#ifndef __GLPOINTSARRAYS__
#define __GLPOINTSARRAYS__

#include "GLRenderer.hxx"
#include "DataTypes.hxx"

namespace CLAMVM
{
	using CLAM::TData;
	using CLAM::TSize;

	class GLPointsArrays : public GLRenderer
	{
	public:
		GLPointsArrays();
		virtual ~GLPointsArrays();
				
		void SetLineColor( unsigned char r, unsigned char g, unsigned char b )
		{
			mLineColor[0] = r; mLineColor[1]= g; mLineColor[2]=b;
		}

		void SetXArray( const TData* xdata, TSize nelems )
		{
			mXDataArray = xdata;
			mXDataLen = nelems;
		}

		void SetYArray( const TData* ydata, TSize nelems )
		{
			mYDataArray = ydata;
			mYDataLen = nelems;
		}

		void SetYMinimum( TData minY )
		{
			mMinY = minY;
		}

		virtual void ExecuteGLCommands();

	private:

		const TData*  mXDataArray;
		const TData*  mYDataArray;
		TSize         mXDataLen;
		TSize         mYDataLen;
		TData         mMinY;
		unsigned char mLineColor[3];
	};

}


#endif // GLPointsArrays.hxx
