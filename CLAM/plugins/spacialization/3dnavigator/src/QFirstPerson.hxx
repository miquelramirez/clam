#ifndef QFirstPerson_hxx
#define QFirstPerson_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>
#include <iostream>
#include <cmath>

class QFirstPerson : public QGLWidget
{
	double _viewX;
	double _viewY;
	double _viewRotation;
	double _viewElevation;
	double _sourceX;
	double _sourceY;
	GLUquadric * _sphere;
	Q_OBJECT
public:
	QFirstPerson(QWidget * parent=0)
		: QGLWidget(parent)
		, _viewX(0)
		, _viewY(-1)
		, _viewRotation(0)
		, _viewElevation(0)
		, _sourceX(0)
		, _sourceY(0)
		, _sphere(0)
	{
	}
	void initializeGL()
	{
		std::cout << "init" << std::endl;
		glEnable(GL_TEXTURE_2D);       /* enable texture mapping */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glDepthFunc(GL_LESS);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		bool blend = true;
		if (blend) glEnable(GL_BLEND);
		bool ligth = true;
		if (ligth) glEnable(GL_LIGHTING);
	}
	void resizeGL(int width, int height)
	{
		std::cout << "resize" << std::endl;
		glViewport(0, 0, width, height);    /* Reset The Current Viewport And Perspective Transformation */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1250.0f);
		glMatrixMode(GL_MODELVIEW);
	}
	void paintGL()
	{
		std::cout << "Viewer " << _viewX << " " << _viewY << " " << _viewRotation << std::endl;
		std::cout << "Source " << _sourceX << " " << _sourceY << std::endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glPushMatrix();
		glRotatef( -_viewElevation, 1.0f, 0.0f, 0.0f);
		glRotatef(180-_viewRotation, 0.0f, 1.0f, 0.0f);
		glTranslatef(-_viewX, 0, -_viewY); // y is at z
		placeLights();
		paintScene();
		glPopMatrix();
		glDepthFunc(GL_NONE);
		paintDecoration();
	}
	void renderText(double, double, double, const QString &)
	{
	}
	float * vColor(const QColor & color)
	{
		static float vcolor[4];
		vcolor[0]=color.redF();
		vcolor[1]=color.greenF();
		vcolor[2]=color.blueF();
		vcolor[3]=color.alphaF();
		return vcolor;
	}
	void placeLight(unsigned i, float * pos, float * dir,
		int cutoff,
		float exponent,
		const QColor & diffuse=Qt::white,
		const QColor & specular=Qt::white,
		const QColor & ambient=Qt::black)
	{
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, vColor(ambient));
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, vColor(diffuse));
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, vColor(specular));
		glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, dir);
		glLightfv(GL_LIGHT0+i, GL_POSITION, pos);
		glLighti (GL_LIGHT0+i, GL_SPOT_CUTOFF, cutoff);
		glLightf (GL_LIGHT0+i, GL_SPOT_EXPONENT, exponent);
		glLightf (GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf (GL_LIGHT0+i, GL_LINEAR_ATTENUATION, 0.2f);
		glLightf (GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 0.0f);
		glEnable (GL_LIGHT0+i);
	}
	virtual void placeLights()
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vColor("#505060"));
		GLfloat position0[] = { 0.0f, 1.5f, 0.0f, 1.0f };
		GLfloat direction0[] = { 0.0f, -0.5f, 0.0f, 1.0f };
		placeLight(0, position0, direction0, 90, 2, "#555", "#555", "#555");
		GLfloat position1[] = { -5.0f, 1.0f, 5.0f, 1.0f };
		GLfloat direction1[] = { .5f, -.5f, -.5f, 1.0f };
		placeLight(1, position1, direction1, 90, 2, "#700", "#770", "#700");
		GLfloat position2[] = { 5.0f, 1.0f, -5.0f, 1.0f };
		GLfloat direction2[] = { -1.0f/5, -1.0f/5, 1.0f, 1.0f };
		placeLight(2, position2, direction2, 90, 2, "#077", "#007", "#077");
		drawLight(position0, direction0, "Light0", "#777");
		drawLight(position1, direction1, "Light1", "#a00");
		drawLight(position2, direction2, "Light2", "#0aa");
	}
	void roomWall(float bottom, float top)
	{
		glBegin(GL_QUAD_STRIP);
		for (unsigned i=0; i<20; i++)
		{
			glVertex3f(10.f-i,top,10.f);
			glVertex3f(10.f-i,bottom,10.f);
		}
		for (unsigned i=0; i<20; i++)
		{
			glVertex3f(-10.f,top,10.f-i);
			glVertex3f(-10.f,bottom,10.f-i);
		}
		for (unsigned i=0; i<20; i++)
		{
			glVertex3f(-10.f+i,top,-10.f);
			glVertex3f(-10.f+i,bottom,-10.f);
		}
		for (unsigned i=0; i<20; i++)
		{
			glVertex3f(10.f,top,-10.f+i);
			glVertex3f(10.f,bottom,-10.f+i);
		}
		glVertex3f(10.f,top,10.f);
		glVertex3f(10.f,bottom,10.f);
		glEnd();
	}
	virtual void paintScene()
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, vColor("white"));
		glMaterialfv(GL_FRONT, GL_AMBIENT, vColor("white"));
		glMaterialfv(GL_FRONT, GL_SPECULAR, vColor("#333"));
		glMaterialf(GL_FRONT, GL_SHININESS, 0);
		glColor4fv(vColor(Qt::white));
		roomWall(-1.75, -1);
		roomWall(-1, 0);
		roomWall(0, 1);
		roomWall(1, 1.75);
		qglColor(Qt::yellow);
		drawPlane(-1.75);
		glColor4fv(vColor("#aaa"));
		drawPlane(+1.75);
		qglColor(QColor("#ffa"));
		renderText(0., 1, 10.-1, "North (+Y)");
		renderText(0., 1, -10.+1, "South (-Y)");
		renderText(10.-1, 1, 0., "East (+X)");
		renderText(-10.+1, 1, 0., "West (-X)");
		glPushMatrix();
			glColor4fv(vColor("#b22"));
			glTranslatef(_sourceX,0,_sourceY);
			renderText(0., 1, 0., "Source");
			glBegin(GL_LINES);
				glVertex3f(0.,1.,0.);
				glVertex3f(0.,0.,0.);
			glEnd();
			gluSphere(sphere(), 0.5f, 40, 20);
		glPopMatrix();
	}
	void drawPlane(float height)
	{
		for (unsigned j=0; j<20; j++)
		{
			glBegin(GL_QUAD_STRIP);
			for (unsigned i=0; i<=20; i++)
			{
				glVertex3f(10.f-j,height,10.f-i);
				glVertex3f(10.f-(j+1),height,10.f-i);
			}
			glEnd();
		}
	}
	void drawLight(float * position, float * direction, const char * label, const QColor & color)
	{
		glPushMatrix();
		glColor4fv(vColor(color));
		glTranslatef(position[0],position[1],position[2]);
		renderText(0., 2, 0., label);
		glMaterialfv(GL_FRONT, GL_EMISSION, vColor(color));
		glBegin(GL_LINES);
			glVertex3f(0.,0.,0.);
			glVertex3fv(direction);
			glVertex3f(0.,2.,0.);
			glVertex3f(0.,0.,0.);
		glEnd();
		gluSphere(sphere(), 0.2f, 40, 20);
		glMaterialfv(GL_FRONT, GL_EMISSION, vColor("black"));
		glPopMatrix();
	}
	virtual void paintDecoration()
	{
		return;
		glDisable(GL_LIGHTING);
		glColor4fv(vColor("green"));
		renderText(-10.,1.,0.,
			tr("Emitter: %1 %2")
				.arg(QString::number(_sourceX))
				.arg(QString::number(_sourceY)));
		glBegin(GL_QUADS);
		glVertex3f(.1,.2,-1);
		glVertex3f(.1,.3,-1);
		glVertex3f(.2,.3,-1);
		glVertex3f(.2,.2,-1);
		glVertex3f(.3,.3,-1);
		glVertex3f(.3,.2,-1);
		glVertex3f(.4,.2,-1);
		glVertex3f(.4,.3,-1);
		glColor4fv(vColor("#f73"));
		glVertex3f(0,0,-1);
		glVertex3f(0,.9,-1);
		glVertex3f(.9,.9,-1);
		glVertex3f(.9,0,-1);
		glEnd();
//		gluSphere(sphere(), 0.015f, 40, 20);
		glEnable(GL_LIGHTING);
	}
	void keyPressEvent( QKeyEvent * event)
	{
		std::cout << "key" << std::endl;
		float cosOrientation = std::cos(_viewRotation*M_PI/180.f);
		float sinOrientation = std::sin(_viewRotation*M_PI/180.f);
		bool strafe = event->modifiers() & Qt::AltModifier;
		bool run = event->modifiers() & Qt::ShiftModifier;
		float step = run ? .4f : .1f;
		switch (event->key())
		{
			case Qt::Key_Left:
				if (strafe)
				{
					_viewX += cosOrientation * step;
					_viewY -= sinOrientation * step;
				}
				else _viewRotation+=10;
			break;
			case Qt::Key_Right:
				if (strafe)
				{
					_viewX -= cosOrientation * step;
					_viewY += sinOrientation * step;
				}
				else _viewRotation-=10;
			break;
			case Qt::Key_PageUp:
				_viewElevation+=10;
			break;
			case Qt::Key_PageDown:
				_viewElevation-=10;
			break;
			case Qt::Key_Up:
				_viewX += sinOrientation * step;
				_viewY += cosOrientation * step;
			break;
			case Qt::Key_Down:
				_viewX -= sinOrientation * step;
				_viewY -= cosOrientation * step;
			break;
			case Qt::Key_A:
				_sourceX+=step;
			break;
			case Qt::Key_D:
				_sourceX-=step;
			break;
			case Qt::Key_W:
				_sourceY+=step;
			break;
			case Qt::Key_S:
				_sourceY-=step;
			break;
			case Qt::Key_Escape:
				close();
			break;
			default:
				event->ignore();
				return;
			return;
		}
		while (_viewRotation>=360.f) _viewRotation-=360.f;
		while (_viewRotation<0.f) _viewRotation+=360.f;
		event->accept();
		updateGL();
	}
private:
	GLUquadric * sphere()
	{
		if (!_sphere) _sphere = gluNewQuadric();
		return _sphere;
	}
};


#endif// QFirstPerson_hxx
