#ifndef __GLVIEW__
#define __GLVIEW__

namespace CLAM
{
    namespace VM
    {
		struct GLView
		{
			double left, right, bottom, top;

			GLView()
				: left(0.0)
				, right(1.0)
				, bottom(0.0)
				, top(1.0)
				{
				}

			GLView(const double& left, const double& right, const double& bottom, const double& top)
				: left(left)
				, right(right)
				, bottom(bottom)
				, top(top)
				{
				}

			GLView(const GLView& view)
				: left(view.left)
				, right(view.right)
				, bottom(view.bottom)
				, top(view.top) 
				{
				}

			~GLView(){}
	    
			GLView& operator=(const GLView& view)
				{
					left = view.left;
					right = view.right;
					bottom = view.bottom;
					top = view.top;
					return *this;
				}

		};
    }
}

#endif


