
#ifndef _FUNCTION_WINDOW_H
#define _FUNCTION_WINDOW_H

#include "GlutWindow.h"
#include "FunctionDisplay.hxx"
#include <string>

namespace CLAMGUI {

	class GlutFunctionWindow : public FunctionDisplay, public Glut2DWindow
	{
	public:

		GlutFunctionWindow();

	private:

		struct MousePos {
			int x;
			int y;
			MousePos() : x(0), y(0) {}
		};

		MousePos mPrevMouse;

		GLdouble mMinXmin,mMaxXwidth,mMinXwidth;
		GLdouble mMinYmin,mMaxYwidth,mMinYwidth;

		std::string mArgString;

		void IdleCb(void);

		void DisplayCb(void);

		void MotionCb(int x, int y);

		void MouseCb(int button, int state, int x, int y);

		void KeyboardCb(unsigned char key, int x, int y);

		void SetFunctionArea(CLAM::TData x_min, CLAM::TData x_width, CLAM::TData x_delta,
							 CLAM::TData y_min, CLAM::TData y_width, CLAM::TData y_delta);

		// User commands:

		MousePos mZoomCenter;
		bool mZooming;
		bool mPanning;

		void Continue(int repeat=1);
		void Quit();
		void Zoom(float,float);
		void Fit();
		void Whole();
		void Freeze();
		void Pan(float,float);

	};

}


#endif
