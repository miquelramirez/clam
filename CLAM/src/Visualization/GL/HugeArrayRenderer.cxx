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

#include "HugeArrayRenderer.hxx"
#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include <algorithm>

namespace CLAMVM
{
		
  void HugeArrayRenderer::CacheData( const DataArray& hugeArray )
  {
  }

  void HugeArrayRenderer::BuildMaxMinArrays( const DataArray& hugeArray )
  {
    int newSize = ((int)(( hugeArray.Size()/10)+1));
    mMaxs.Resize( newSize );
    mMaxs.SetSize(newSize);
    mMins.Resize( newSize );
    mMins.SetSize( newSize );
    newSize = ((int)(( mMaxs.Size()/10)+1));
    mMaxs10.Resize( newSize );
    mMaxs10.SetSize(newSize);
    mMins10.Resize( newSize );
    mMins10.SetSize( newSize );
    
    for(int i=0; i<mMaxs.Size(); i++)
      {
	int begin = (i-0.5)*10;
	int end = (i+0.5)*10;
	begin = std::max(begin , 0);
	begin = std::min(begin , hugeArray.Size()-2);
	end = std::min(end , hugeArray.Size());
	float top = hugeArray[begin];
	float bottom = hugeArray[begin];
	for(int k=begin+1; k<end; k++)
	  {
	    if (hugeArray[k] > top)
	      top = hugeArray[k];
	    else if (hugeArray[k] < bottom)
	      bottom = hugeArray[k];
	  }
	mMaxs[i] = top;
						mMins[i] = bottom;
      }
    for(int i=0; i<mMaxs10.Size(); i++)
      {
	int begin = (i-0.5)*10;
	int end = (i+0.5)*10;
	begin = std::max(begin , 0);
	begin = std::min(begin , mMaxs.Size()-2);
	end = std::min(end , mMaxs.Size());
	float top = mMaxs[begin];
	float bottom = mMins[begin];
	for(int k=begin+1; k<end; k++)
	  {
	    if (mMaxs[k] > top)
	      top = mMaxs[k];
	    else if (mMins[k] < bottom)
	      bottom = mMins[k];
	  }
	mMaxs10[i] = top;
						mMins10[i] = bottom;
      }
    
  }

  
}
