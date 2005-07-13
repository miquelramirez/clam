#include "AnnotatorBrowserGL.hxx"
#include <GL/glu.h>
#include <cmath>

#include <iostream>


void AnnotatorBrowserGL::initializeGL()
{  	
	//glClearColor( 1.0, 1.0,1.0, 0.0 );
	qglClearColor( white );
	glShadeModel( GL_FLAT );
	frame=0;
	play=false;
}

void AnnotatorBrowserGL::paintDiracs()
{
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex2f(-0.2,0);
	glVertex2f(-0.2,0.5);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.2,0);
	glVertex2f(0.2,0.2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.3,0);
	glVertex2f(0.3,0.9);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.9,0);
	glVertex2f(0.9,0.7);
	glEnd();
}

void AnnotatorBrowserGL::paintWave()
{
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
	for (int i=-100;i<100;i++)
	{
		glVertex2f(i/100.0,(std::sin(static_cast<double>(i))+std::sin(static_cast<double>(i/25))+std::sin(static_cast<double>(i*2)))/4.0);
	}
	glEnd();
}

void AnnotatorBrowserGL::paintBoundaries()
{
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(-0.25,-1);
	glVertex2f(-0.25,1);
	glVertex2f(0.25,-1);
	glVertex2f(0.25,1);
	glVertex2f(-0.75,1);
	glVertex2f(-0.75,-1);
	glVertex2f(0.75,1);
	glVertex2f(0.75,-1);
	glEnd();
}

void AnnotatorBrowserGL::paintPlay()
{
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(static_cast<double>(frame%50)/25.0-1,1);

	glVertex2f(static_cast<double>(frame%50)/25.0-1,-1);
	frame++;
	glEnd();
}

void AnnotatorBrowserGL::paintMark()
{
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex2f(-0.8,1);

	glVertex2f(-0.8,-1);
	frame++;
	glEnd();
}

void AnnotatorBrowserGL::paintBase()
{
	glColor3f(1.0,0.6,1.0);
	glBegin(GL_LINE_STRIP);
	for (int i=-100;i<100;i++)
	{
		glVertex2f(i/100.0,std::sin(static_cast<double>(i))/2.0);
	}
	glEnd();
}

void AnnotatorBrowserGL::paintGL(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	paintWindow(-0.5,0.5);    
	paintWindow(0.5,0.6);
	paintWindow(0.7,0.8);
	paintWave();
	paintBase();
	paintBoundaries();
	paintPlay();
	paintMark();
	paintDiracs();


	glFlush();
}

void AnnotatorBrowserGL::playStop()
{
	play=!play;
}

void AnnotatorBrowserGL::paintWindow(double start, double end)
{
	glColor3f(0.5,0.5,0.8);
	glBegin(GL_POLYGON);
	glVertex2f(start,-1);
	glVertex2f(start,1);
	glVertex2f(end,1);
	glVertex2f(end,-1);
	glEnd();
	glColor3f(0.0,0.0,0.5);
	glBegin(GL_LINE_LOOP);
	glVertex2f(start,-1);
	glVertex2f(start,1);
	glVertex2f(end,1);
	glVertex2f(end,-1);
	glEnd();
}

void AnnotatorBrowserGL::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-10,-10,10,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void AnnotatorBrowserGL::update()
{
	if (play) updateGL();
}

