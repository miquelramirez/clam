#include "NetDisplaySurface.hxx"
#include <qwidget.h>

namespace CLAM
{
	namespace VM
	{
		NetDisplaySurface::NetDisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
		{
			setAutoBufferSwap(false);
			SetBackgroundColor(0.0,0.0,0.0);
			InitView();
			_doResize = false;

			_controller = NULL;
			connect(&_timer, SIGNAL(timeout()), SLOT(refresh()));
		}

		NetDisplaySurface::~NetDisplaySurface()
		{
			if(_controller) 
			{
			    delete _controller;
			}
		}

		void NetDisplaySurface::SetBackgroundColor(double r, double g, double b)
		{
			_r = r;
			_g = g;
			_b = b;
		}

		void NetDisplaySurface::SetController(NetPlotController* controller)
		{
			_controller = controller;
			connect(_controller,SIGNAL(sendView(SView)),this,SLOT(receivedView(SView)));
		}

		void NetDisplaySurface::startRendering()
		{
			_timer.start(10);
		}

		void NetDisplaySurface::stopRendering()
		{
			_timer.stop();
		}
		void NetDisplaySurface::refresh()
		{
			updateGL();
			_timer.start(10);
		}
		void NetDisplaySurface::paintGL()
		{
				if (!_controller) return;
		    makeCurrent();    
				if(_doResize)
				{
					glViewport(0, 0, _w, _h);
					_doResize = false;
				}
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(_left, _right, _bottom, _top, -1.0, 1.0);
				glMatrixMode(GL_MODELVIEW);
				glShadeModel(GL_FLAT);
				glClearColor(_r, _g, _b, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);

				_controller->Draw();
				swapBuffers();
		    doneCurrent();
				((QWidget*)parent())->update();

		}

		void NetDisplaySurface::resizeEvent(QResizeEvent* re)
		{ 
		    _w = re->size().width();
		    _h = re->size().height();
		    _doResize = true;
		}

		void NetDisplaySurface::receivedView(SView v)
		{
			_left = v.left;
			_right = v.right;
			_bottom = v.bottom;
			_top = v.top;
		}

		void NetDisplaySurface::InitView()
		{
			_left = 0.0;
			_right = 1.0;
			_bottom = -1.0;
			_top = 1.0;
		}
	}
}

// END

