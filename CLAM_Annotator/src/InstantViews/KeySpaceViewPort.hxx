#ifndef KeySpaceViewPort_hxx
#define KeySpaceViewPort_hxx

#include <QtOpenGL/QGLWidget>
#undef GetClassName

class KeySpaceViewPort : public QGLWidget
{
protected:
	int x, y, w, h;
	float x_res, y_res;

public:
	KeySpaceViewPort()
	{
		x = y = w = h = 0;
		x_res = y_res = 1;
	}

	virtual void resize(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	virtual void update() = 0;

	virtual void display()
	{
		// change viewport
		glViewport(x,y,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,x_res,y_res,0);
	}
};

#endif
