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
