#include "NetDisplaySurface.hxx"

namespace CLAM
{
	namespace VM
	{
		NetDisplaySurface::NetDisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
		{
			SetBackgroundColor(0.0,0.0,0.0);
			InitView();

			_controller = NULL;
		}

		NetDisplaySurface::~NetDisplaySurface()
		{
			if(_controller) delete _controller;
		}

		void NetDisplaySurface::SetBackgroundColor(double r, double g, double b)
		{
			_bkColor.r = r;
			_bkColor.g = g;
			_bkColor.b = b;
		}

		void NetDisplaySurface::SetController(NetPlotController* controller)
		{
			_controller = controller;
			connect(_controller,SIGNAL(sendView(SView)),this,SLOT(receivedView(SView)));
			connect(_controller,SIGNAL(requestRefresh()),this,SLOT(update()));
		}

		void NetDisplaySurface::initializeGL()
		{
			glClearColor(_bkColor.r,_bkColor.g,_bkColor.b,1.0);
			glShadeModel(GL_FLAT);
		}

		void NetDisplaySurface::resizeGL(int w,int h)
		{
			glViewport(0,0,w,h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(_view.left,_view.right,_view.bottom,_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
		}

		void NetDisplaySurface::paintGL()
		{
			if(_controller)
			{
				glClearColor(_bkColor.r,_bkColor.g,_bkColor.b,1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				_controller->Draw();
			}
		}

		void NetDisplaySurface::receivedView(SView v)
		{
			_view.left = v.left;
			_view.right = v.right;
			_view.bottom = v.bottom;
			_view.top = v.top;
			resizeGL(width(),height());
		}

		void NetDisplaySurface::InitView()
		{
			_view.left = 0.0f;
			_view.right = 1.0f;
			_view.bottom = -1.0f;
			_view.top = 1.0f;
		}
	}
}

// END

