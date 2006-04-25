#ifndef _KEYSPACE_
#define _KEISPACE_

#include "PcpTorus.hxx"
namespace CLAM {
namespace VM {

class KeySpace : public PcpTorus
{
protected:
	int x, y, w, h;
	float x_res, y_res;

public:
	KeySpace(QWidget * parent);
	virtual void initializeGL()
	{
		glShadeModel(GL_FLAT);
		glClearColor(1,0,0,0); // rgba
//		glEnable(GL_BLEND);
//		glEnable (GL_LINE_SMOOTH);
//		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
//		glEnable(GL_CULL_FACE);
	}
	virtual void resizeGL(int width, int height)
	{
		x = 0;
		y = 0;
		w = width;
		h = height;
		glViewport(x,y,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,x_res,y_res,0,-2,2);
//		glOrtho(0,1,0,2,0,1); // From PcpTorus
		glMatrixMode(GL_MODELVIEW);
	}
	virtual void paintGL();

	double wdist(double x1,double x2)
	{
		if (x2 > x1+.5)
			return 1. - (x2-x1);
		else
		if (x2 < x1-.5)
			return 1. - (x1-x2);
		else
		if (x2 >= x1)
			return x2-x1;
		else
			return x1-x2;
	}

	float centroidx_,centroidy_;
	float pKeySpaceValue_[24];
	float pRColor[201],pGColor[201],pBColor[201];
	int ColorsIndex[6];
};

}
}

#endif
