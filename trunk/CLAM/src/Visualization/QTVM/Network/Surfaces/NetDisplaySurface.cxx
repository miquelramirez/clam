#include "NetDisplaySurface.hxx"
#include <qwidget.h>
#include <iostream>

namespace CLAM
{
	namespace VM
	{
		NetDisplaySurface::NetDisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
			, _timer( * new QTimer(this) )
		{
		        setMouseTracking(true);
			setAutoBufferSwap(false);
			SetBackgroundColor(0.0,0.0,0.0);
			InitView();
			_doResize = false;
			_refreshInterval = 200;

			_controller = NULL;
			connect(&_timer, SIGNAL(timeout()), SLOT(updateGL()));
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
			if (!_timer.isActive())
				_timer.start(_refreshInterval,true);
		}

		void NetDisplaySurface::stopRendering()
		{
			if (_timer.isActive())
				_timer.stop();
		}

		void NetDisplaySurface::paintGL()
		{
			if (!_controller) return;
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
			if (!_timer.isActive())
				_timer.start(_refreshInterval,true);
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
	    
	        void NetDisplaySurface::mousePressEvent(QMouseEvent* e)
		{
		    if(_controller)
		    {
			if(e->button() == LeftButton)
			{
			    _controller->SetLeftButtonPressed(true);
			    double xcoord = double(e->x());
			    xcoord *= (_right-_left);
			    xcoord /= double(width());
			    xcoord += _left;
			    double ycoord = double(-e->y())+double(height());
			    ycoord *= (_top-_bottom);
			    ycoord /= double(height());
			    ycoord += _bottom;
			    _controller->SetPoint(TData(xcoord),TData(ycoord));
			    
			}
			if(e->button() == RightButton)
			{
			    _controller->SetRightButtonPressed(true);
			}
		    }
		}

	        void NetDisplaySurface::mouseReleaseEvent(QMouseEvent* e)
		{
		    if(_controller)
		    {
			if(e->button() == LeftButton)
			{
			    _controller->SetLeftButtonPressed(false);
			    double xcoord = double(e->x());
			    xcoord *= (_right-_left);
			    xcoord /= double(width());
			    xcoord += _left;
			    double ycoord = double(-e->y())+double(height());
			    ycoord *= (_top-_bottom);
			    ycoord /= double(height());
			    ycoord += _bottom;
			    _controller->SetPoint(TData(xcoord),TData(ycoord));	    
			}
			if(e->button() == RightButton)
			{
			    _controller->SetRightButtonPressed(false);
			}
		    }  
		}

		void NetDisplaySurface::mouseMoveEvent(QMouseEvent* e)
		{
		    if(_controller)
		    {
			double xcoord = double(e->x());
			xcoord *= (_right-_left);
			xcoord /= double(width());
			xcoord += _left;
			double ycoord = double(-e->y())+double(height());
			ycoord *= (_top-_bottom);
			ycoord /= double(height());
			ycoord += _bottom;
			_controller->UpdatePoint(TData(xcoord),TData(ycoord));
		    }
		}  
	

	        void NetDisplaySurface::leaveEvent(QEvent* e)
		{
		    if(_controller)
		    {
			_controller->LeaveEvent();
		    }
		}

		void NetDisplaySurface::enterEvent(QEvent* e)
		{
		    if(_controller)
		    {
			_controller->EnterEvent();
		    }
		}

	}
}

// END

