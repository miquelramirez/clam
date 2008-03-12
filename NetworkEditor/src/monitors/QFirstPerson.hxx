#ifndef QFirstPerson_hxx
#define QFirstPerson_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>
#include <iostream>
#include <cmath>
static float * vColor(const QColor & color)
{
	static float vcolor[4];
	vcolor[0]=color.redF();
	vcolor[1]=color.greenF();
	vcolor[2]=color.blueF();
	vcolor[3]=color.alphaF();
	return vcolor;
}

class Light
{
	unsigned _index;
	QColor _ambient;
	QColor _specular;
	QColor _diffuse;
	float _position[4];
	float _direction[4];
	int _cutoff;
	float _exponent;
	bool _enabled;
public:
	Light(unsigned index,
		float * position, float * direction,
		int cutoff,
		float exponent,
		const QColor & diffuse=Qt::white,
		const QColor & specular=Qt::white,
		const QColor & ambient=Qt::black)
		: _index(index)
		, _ambient(ambient)
		, _specular(specular)
		, _diffuse(diffuse)
		, _cutoff(cutoff)
		, _exponent(exponent)
		, _enabled(true)
	{
		for (unsigned i=0; i<4; i++) _position[i]=position[i];
		for (unsigned i=0; i<4; i++) _direction[i]=direction[i];
	}
	void place()
	{
		glLightfv(GL_LIGHT0+_index, GL_AMBIENT, vColor(_ambient));
		glLightfv(GL_LIGHT0+_index, GL_DIFFUSE, vColor(_diffuse));
		glLightfv(GL_LIGHT0+_index, GL_SPECULAR, vColor(_specular));
		glLightfv(GL_LIGHT0+_index, GL_SPOT_DIRECTION, _direction);
		glLightfv(GL_LIGHT0+_index, GL_POSITION, _position);
		glLighti (GL_LIGHT0+_index, GL_SPOT_CUTOFF, _cutoff);
		glLightf (GL_LIGHT0+_index, GL_SPOT_EXPONENT, _exponent);
		glLightf (GL_LIGHT0+_index, GL_CONSTANT_ATTENUATION, 1.0f);
		glLightf (GL_LIGHT0+_index, GL_LINEAR_ATTENUATION, 0.2f);
		glLightf (GL_LIGHT0+_index, GL_QUADRATIC_ATTENUATION, 0.0f);
		(*(_enabled? &glEnable:&glDisable) )(GL_LIGHT0+_index);
	}
};

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
		glEnable(GL_TEXTURE_2D);
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
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1250.0f);
		glMatrixMode(GL_MODELVIEW);
	}
	void paintGL()
	{
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
	virtual void placeLights()
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vColor("#505060"));
		static GLfloat position0[] = { 0.0f, 1.5f, 0.0f, 1.0f };
		static GLfloat direction0[] = { 0.0f, -0.5f, 0.0f, 1.0f };
		Light light0(0, position0, direction0, 90, 2, "#555", "#555", "#555");
		light0.place();
		static GLfloat position1[] = { -5.0f, 1.0f, 5.0f, 1.0f };
		static GLfloat direction1[] = { .5f, -.5f, -.5f, 1.0f };
		Light light1(1, position1, direction1, 90, 2, "#700", "#770", "#700");
		light1.place();
		static GLfloat position2[] = { 5.0f, 1.0f, -5.0f, 1.0f };
		static GLfloat direction2[] = { -1.0f/5, -1.0f/5, 1.0f, 1.0f };
		Light light2(2, position2, direction2, 90, 2, "#077", "#007", "#077");
		light2.place();
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
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		QString emiterString = tr("Emitter: %1, %2")
				.arg(QString::number(_sourceX,'d',2))
				.arg(QString::number(_sourceY,'d',2))
				;
		QString receiverString = tr("Receiver: %1, %2 angle %3")
				.arg(QString::number(_viewX,'d',2))
				.arg(QString::number(_viewY,'d',2))
				.arg(QString::number(_viewRotation,'d',0))
				;
		glColor4fv(vColor("black"));
		renderText(10,20, receiverString);
		renderText(10,40, emiterString);
		glBegin(GL_LINES);
		glVertex3f(0,.06,-.1);
		glVertex3f(0,.02,-.1);
		glVertex3f(0,-.06,-.1);
		glVertex3f(0,-.02,-.1);
		glVertex3f(.06,0,-.1);
		glVertex3f(.02,0,-.1);
		glVertex3f(-.06,0,-.1);
		glVertex3f(-.02,0,-.1);
		glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
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
					emitPositionChange();
				}
				else
				{
					_viewRotation+=10;
					emit orientationChanged(_viewRotation);
				}
			break;
			case Qt::Key_Right:
				if (strafe)
				{
					_viewX -= cosOrientation * step;
					_viewY += sinOrientation * step;
					emitPositionChange();
					emit posChanged(QPointF(_viewX, _viewY));
				}
				else
				{
					_viewRotation-=10;
					emit orientationChanged(_viewRotation);
				}
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
				emitPositionChange();
			break;
			case Qt::Key_Down:
				_viewX -= sinOrientation * step;
				_viewY -= cosOrientation * step;
				emitPositionChange();
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
signals:
	double posChanged(QPointF point);
	double xPosChanged(double x);
	double yPosChanged(double y);
	double orientationChanged(double degrees);
private:
	GLUquadric * sphere()
	{
		if (!_sphere) _sphere = gluNewQuadric();
		return _sphere;
	}
	void emitPositionChange()
	{
		emit posChanged(QPointF(_viewX, _viewY));
		emit xPosChanged(_viewX);
		emit yPosChanged(_viewY);
	}
};


#endif// QFirstPerson_hxx
