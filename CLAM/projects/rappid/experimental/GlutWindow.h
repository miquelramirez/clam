#ifndef _GLUT_WINDOW
#define _GLUT_WINDOW

#include <GL/glut.h>
#include "GlutManager.h"

class GlutWindow {

protected:

	int mId;

	int mWidth;

	int mHeight;

	GlutManager::Callbacks mCallbacks;

public:

	GlutWindow(const char* name = "GlutWindow", 
			   unsigned mode  =  GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE );
	virtual ~GlutWindow();

	int Id() const { return mId; }

	int Width() const { return mWidth; }
	int Height() const { return mHeight; }

	void SetCurrent();

	virtual void     IdleCb(void);
	virtual void  DisplayCb(void);
	virtual void    TimerCb(int);
	virtual void   MotionCb(int x, int y);
	virtual void  ReshapeCb(int w, int h);
	virtual void    MouseCb(int button, int state, int x, int y);
	virtual void KeyboardCb(unsigned char key, int x, int y);

	void RunInBackground();

};

class Glut2DWindow : public GlutWindow {

	bool normalize;
	bool x_range;
	bool y_range;

protected:

	GLdouble mXmin,mXwidth;
	GLdouble mYmin,mYwidth;

	virtual void    IdleCb();
	virtual void DisplayCb();
	virtual void   TimerCb(int v);
	virtual void  MotionCb(int x, int y);
	virtual void ReshapeCb(int w, int h);
	virtual void   MouseCb(int button, int state, int x, int y);
	virtual void KeyboardCb(unsigned char key, int x, int y);

public:

	Glut2DWindow(const char* name = "Glut2DWindow",
				 bool normalize=false);

	void SetXRange(GLdouble min, GLdouble width);

	void SetYRange(GLdouble min, GLdouble width);

	GLdouble XMin()   const { return mXmin; }
	GLdouble XWidth() const { return mXwidth; }
	GLdouble YMin()   const { return mYmin; }
	GLdouble YWidth() const { return mYwidth; }

};

#endif
