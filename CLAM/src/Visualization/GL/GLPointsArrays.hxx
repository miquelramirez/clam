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
