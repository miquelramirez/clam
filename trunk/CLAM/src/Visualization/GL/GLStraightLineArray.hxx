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

#ifndef __GLSTRAIGHTLINEARRAY__
#define __GLSTRAIGHTLINEARRAY__

#include "GLRenderer.hxx"
#include "DataTypes.hxx"

namespace CLAMVM
{
	using CLAM::TData;

	class GLStraightLineArray
		: public GLRenderer
	{
	public:

		GLStraightLineArray();

		virtual ~GLStraightLineArray();

		void SetLineColor( unsigned char r, unsigned char g, unsigned char b )
		{
			mLineColor[0] = r; 
			mLineColor[1] = g; 
			mLineColor[2] = b;
		}

		void SetStartAndHopSize( TData xstart, TData xhop )
		{
			mXStart = xstart;
			mXHopSize = xhop;
		}

		void SetDataPtr( const TData* pValues, unsigned nelems )
		{
			mValues = pValues;
			mElemsToDraw = nelems;
		}

		virtual void ExecuteGLCommands();

	protected:

		unsigned mElemsToDraw;
		const TData* mValues;
		TData mXStart;
		TData mXHopSize;
		unsigned char mLineColor[3];
	};

}

#endif // GLStraightLineArray.hxx
