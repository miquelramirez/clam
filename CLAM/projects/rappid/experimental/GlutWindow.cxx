
#include "GlutWindow.h"
#include <pthread.h>

GlutWindow::GlutWindow(const char *name, unsigned int mode)
	: mId(0)
{
	GlutManager *manager = GlutManager::Instance();

	glutInitDisplayMode( mode );

	mId = manager->Open(this,name);

	glEnableClientState(GL_VERTEX_ARRAY);
}

GlutWindow::~GlutWindow()
{
	GlutManager *manager = GlutManager::Instance();
	manager->Close(mId);
}

void GlutWindow::SetCurrent()
{
	glutSetWindow(mId);
}

void GlutWindow::DisplayCb()
{
	glutSwapBuffers();
	glFlush();
}

void GlutWindow::ReshapeCb(int w, int h)
{
}

void GlutWindow::TimerCb(int)
{
}

void GlutWindow::IdleCb(void)
{
}

void GlutWindow::MouseCb(int button, int state, int x, int y)
{
}

void GlutWindow::MotionCb(int x, int y)
{
}

void GlutWindow::KeyboardCb(unsigned char key, int x, int y)
{
}

void GlutWindow::RunInBackground()
{
	GlutManager::Start();
}




Glut2DWindow::Glut2DWindow(const char *name,
						   bool norm)
	: GlutWindow(name, GLUT_RGBA | GLUT_DOUBLE),
	  normalize(norm),
	  x_range(false),
	  y_range(false)
{
	
}

void Glut2DWindow::DisplayCb()
{
//	std::cout << "Display " << mId << std::endl;
	glutSwapBuffers();
	glFlush();
}

void Glut2DWindow::ReshapeCb(int width, int height)
{
	GLdouble
		x = 0.0,
		y = 0.0,
		w = GLdouble(width),
		h = GLdouble(height);

	mWidth = width;
	mHeight = height;

	glViewport(0,0,width,height);

	if (normalize) {
		x = -1.0;
		y = -1.0;
		w =  2.0;
		h =  2.0;
	}
	if (x_range) {
		x = mXmin;
		w = mXwidth;
	}
	if (y_range) {
		y = mYmin;
		h = mYwidth;
	}
		
// 	std::cout << "Reshape " << mId
// 			  << '(' << width << ',' << height << ')'
// 			  << std::endl
// 			  << "Ortho " 
// 			  << '(' << x << ',' << x+w << ')'
// 			  << '(' << y << ',' << y+h << ')';
			  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x, x+w, y, y+h);
}

void Glut2DWindow::SetXRange(GLdouble min, GLdouble width)
{
	x_range = true;
	mXmin = min;
	mXwidth = width;
	SetCurrent();
	ReshapeCb(mWidth,mHeight);
	glutPostRedisplay();
}

void Glut2DWindow::SetYRange(GLdouble min, GLdouble width)
{
	y_range = true;
	mYmin = min;
	mYwidth = width;
	SetCurrent();
	ReshapeCb(mWidth,mHeight);
	glutPostRedisplay();
}


void Glut2DWindow::TimerCb(int v)
{
}

void Glut2DWindow::IdleCb(void)
{
}

void Glut2DWindow::MouseCb(int button, int state, int x, int y)
{
}

void Glut2DWindow::MotionCb(int x, int y)
{
}

void Glut2DWindow::KeyboardCb(unsigned char key, int x, int y)
{
}

