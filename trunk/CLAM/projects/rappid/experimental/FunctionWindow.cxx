
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "FunctionWindow.hxx"
extern "C" {
#include <GL/glut.h>
}

using namespace CLAMGUI;

GlutFunctionWindow::GlutFunctionWindow()
	: Glut2DWindow("GlutFunction",true),
	  mZooming(false),
	  mPanning(false)
{
	
}

void GlutFunctionWindow::SetFunctionArea(CLAM::TData x_min, CLAM::TData x_width, CLAM::TData x_delta,
										 CLAM::TData y_min, CLAM::TData y_width, CLAM::TData y_delta)
{
	mMinXmin   = x_min;
	mMaxXwidth = x_width;
	mMinXwidth = x_delta;
	
	mMinYmin   = y_min;
	mMaxYwidth = y_width;
	mMinYwidth = y_delta;

	SetXRange(x_min, x_width);
	SetYRange(y_min, y_width);
}

void GlutFunctionWindow::DisplayCb()
{
	glClear(GL_COLOR_BUFFER_BIT);
	std::list<MTPresentation*>::iterator it;
	for (it = mPresentations.begin();
		 it != mPresentations.end();
		 it++)
		(*it)->Draw();
	Glut2DWindow::DisplayCb();
}

void GlutFunctionWindow::MouseCb(int button, int state, int x, int y)
{
	mPrevMouse.x = x;
	mPrevMouse.y = y;
	if (state == GLUT_DOWN)
		std::cout << "click... " << std::endl;

	int mod = glutGetModifiers();

	if (mod & GLUT_ACTIVE_SHIFT) {
		mZooming=true;
		mPanning = false;
		return;
	}

	if (mod & GLUT_ACTIVE_ALT) {
		mZooming=true;
		mPanning = false;
		return;
	}

	mZooming = false;

	if (mod & GLUT_ACTIVE_CTRL) {
		mPanning = true;
		return;
	}

	mPanning = false;


	std::cout << "Button:    " << button << std::endl
			  << "State:     " << state  << std::endl
			  << "x,y:       " << x << "," << y << std::endl
			  << "Modifiers: " << mod << std::endl;
}

void GlutFunctionWindow::MotionCb(int x, int y)
{
	int delta_x,delta_y;
	float strength;

	std::cout << "Motion x,y: " << x << "," << y << std::endl;
	if (mPrevMouse.x == 0 &&
		mPrevMouse.y == 0) 
	{
		mPrevMouse.x = x;
		mPrevMouse.y = y;
		return;
	}
	delta_x = x - mPrevMouse.x;
	delta_y = y - mPrevMouse.y;

	mPrevMouse.x = x;
	mPrevMouse.y = y;

	strength = sqrt(delta_x*delta_x + delta_y*delta_y) / 50.0;


	if (mZooming) {
		float zx = 1.0 - (float(delta_x) / 100.0);
		float zy = 1.0 + (float(delta_y) / 100.0);
		Zoom(zx,zy);
	}

	if (mPanning)
		Pan(double(-10*delta_x) * mXwidth / Width() ,
			double( 10*delta_y) * mYwidth / Height());

}


void GlutFunctionWindow::IdleCb()
{
	if (mModified) {
		glutPostRedisplay();
		mModified = 0;
	}
}

void GlutFunctionWindow::KeyboardCb(unsigned char key, int x, int y)
{
//	std::cout << "key : " << key
//			  << "   x,y:    " << x << "," << y << std::endl;

	bool has_arg = false;
	float arg = 0.0;

	if (key >= '0' && key <= '9' || 
		key == '-' || 
		key == '.'    )
	{
		mArgString += key;
	}
	else if (mArgString != "")
	{
		char *end;
		arg = strtod(mArgString.c_str(),&end);
		if (end != mArgString.c_str())
			has_arg = true;
		mArgString = "";
		std::cout << " Argument:" << arg << std::endl;
	}

	switch (key) {
	case 'c':
		Continue(int(arg));
		break;
	case 'q':
		Quit();
		break;
	case 'f':
		Freeze();
		break;
	}

}

void GlutFunctionWindow::Continue(int n)
{
	if (n<=0)
		n=1;
	std::list<MTPresentation*>::iterator it;
	for (it = mPresentations.begin();
		 it != mPresentations.end();
		 it++)
	{
		for (int i=0; i<n; i++)
			(*it)->Step();
	}
}

void GlutFunctionWindow::Freeze()
{
	std::list<MTPresentation*>::iterator it;
	for (it = mPresentations.begin();
		 it != mPresentations.end();
		 it++)
		(*it)->TogleFreeze();
}

void GlutFunctionWindow::Quit()
{
	exit(0);
}

void GlutFunctionWindow::Pan(float px, float py)
{
	mXmin += px;
	mYmin += py;

	if (mXmin < mMinXmin)
		mXmin = mMinXmin;
	if (mYmin < mMinYmin)
		mYmin = mMinYmin;

	if (mXmin + mXwidth > mMinXmin + mMaxXwidth)
		mXmin = mMinXmin + mMaxXwidth - mXwidth;

	if (mYmin + mYwidth > mMinYmin + mMaxYwidth)
		mYmin = mMinYmin + mMaxYwidth - mYwidth;

	std::list<MTPresentation*>::iterator it;
	for (it = mPresentations.begin();
		 it != mPresentations.end();
		 it++)
	{
		(*it)->SetVisibleXRange(mXmin,mXwidth);
		(*it)->SetVisibleYRange(mYmin,mYwidth);
	}

	std::cout << " Panned: "
	          << mXmin << ',' << mYmin << std::endl;

	ReshapeCb(mWidth,mHeight);
	glutPostRedisplay();
}

void GlutFunctionWindow::Zoom(float fx, float fy)
{
	mXwidth *= fx;
	mYwidth *= fy;

	if (mXmin + mXwidth > mMinXmin + mMaxXwidth)
		mXwidth = mMinXmin + mMaxXwidth - mXmin;

	if (mYmin + mYwidth > mMinYmin + mMaxYwidth)
		mYwidth = mMinYmin + mMaxYwidth - mYmin;

	if (mXwidth < mMinXwidth)
		mXwidth = mMinXwidth;
	if (mYwidth < mMinYwidth)
		mYwidth = mMinYwidth;

	std::list<MTPresentation*>::iterator it;
	for (it = mPresentations.begin();
		 it != mPresentations.end();
		 it++)
	{
		(*it)->SetVisibleXRange(mXmin,mXwidth);
		(*it)->SetVisibleYRange(mYmin,mYwidth);
	}

	std::cout << " Zoomed: "
		      << mXwidth << ',' << mYwidth << std::endl;

	ReshapeCb(mWidth,mHeight);
	glutPostRedisplay();
}

void GlutFunctionWindow::Fit()
{
}
void GlutFunctionWindow::Whole()
{
}

