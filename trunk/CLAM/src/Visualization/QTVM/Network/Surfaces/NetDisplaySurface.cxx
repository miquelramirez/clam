#include "NetDisplaySurface.hxx"

namespace CLAM
{
	namespace VM
	{
		NetDisplaySurface::NetDisplaySurface(QWidget* parent) 
			: QGLWidget(parent), _thread(this)
		{
			setAutoBufferSwap(false);
			SetBackgroundColor(0.0,0.0,0.0);
			InitView();
			_doResize = false;

			_controller = NULL;
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
			_thread.Start();
			_timer.start(40);
		}

		void NetDisplaySurface::stopRendering()
		{
			_thread.Stop();
			_timer.stop();
		}
		void NetDisplaySurface::refresh()
		{
			updateGL();
			_timer.start(40);
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

