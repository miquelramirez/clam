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
