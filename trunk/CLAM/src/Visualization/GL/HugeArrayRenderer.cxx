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
