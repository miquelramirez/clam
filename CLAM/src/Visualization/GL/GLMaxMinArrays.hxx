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

#ifndef __GLMAXMINSARRAYS__
#define __GLMAXMINSARRAYS__

#include "GLRenderer.hxx"
#include "DataTypes.hxx"

namespace CLAMVM
{

	using CLAM::TSize;
	using CLAM::TData;

class GLMaxMinArrays
		: public GLRenderer
{
public:
		GLMaxMinArrays();
		~GLMaxMinArrays();
		
		void SetArrays( const TData* maxsArray, const TData* minsArray, TSize nElems   )
		{
				mMaxsPtr = maxsArray;
				mNElems = nElems;
				mMinsPtr = minsArray;

		}
		
		void YChanged( float newMaxY, float newMinY )
		{
			mMaxY = newMaxY;
			mMinY = newMinY;
		}

		void ExecuteGLCommands();

protected:
		


private:
		const TData* mMaxsPtr;
		const TData* mMinsPtr;
		TSize  mNElems;
		float mMaxY;
		float mMinY;
};

}

#endif // GLMaxMinsArrays.hxx
