#ifndef __GLVIEW__
#define __GLVIEW__

#include "CLAMGL.hxx"

namespace CLAM
{
    namespace VM
    {
	struct GLView
	{
	    GLdouble mLeft, mRight, mBottom, mTop;

	    GLView()
		: mLeft(0.0), mRight(1.0), mBottom(0.0), mTop(1.0)
		{
		}

	    GLView(const GLdouble& left, const GLdouble& right, const GLdouble& bottom, const GLdouble& top)
		: mLeft(left), mRight(right), mBottom(bottom), mTop(top)
		{
		}

	    GLView(const GLView& view)
		: mLeft(view.mLeft), mRight(view.mRight), mBottom(view.mBottom), mTop(view.mTop) 
		{
		}

	    ~GLView(){}
	    
	    GLView& operator=(const GLView& view)
		{
		    mLeft = view.mLeft;
		    mRight = view.mRight;
		    mBottom = view.mBottom;
		    mTop = view.mTop;
		    return *this;
		}

	};
    }
}

#endif


