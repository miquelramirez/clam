// DEPRECATED: See NetDisplaySurface !!!
#if 0
#include "NetPlotController.hxx"
#include "NetDisplaySurface.hxx"
#include "GLThread.hxx"

namespace CLAM
{
	namespace VM
	{
		GLThread::GLThread(NetDisplaySurface* surf)
		    : _surf(surf), _thread(true)
		{
			_controller = NULL;
			_doRendering = false;
			_doResize = false;
			_terminated = false;
		  
		       _thread.SetThreadCode(makeMemberFunctor0((*this), GLThread, thread_code));
		}

	        GLThread::~GLThread()
		{
		    if(_terminated) _thread.Stop();
		}
	    		
		void GLThread::resizeViewport(int w, int h)
		{
		    _w = w;
		    _h = h;
		    _doResize = true;
		}
				
		void GLThread::thread_code()
		{
		    _surf->makeCurrent();    
		    while(_doRendering)
		    {
			if(_controller)
			{
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
			    _surf->swapBuffers();
			    _thread.Sleep(40);
			}
		    }
		    _surf->doneCurrent();
		}

		void GLThread::Start()
		{
		    if(!_doRendering)
		    {  
			_thread.Start();
			_doRendering = true;
		    }
		}

		void GLThread::Stop()
		{
		    if(_doRendering)
		    {
			_doRendering = false;
			_terminated = true;
		    }
		}

		void GLThread::setController(NetPlotController* controller)
		{
			_controller = controller;
		}

		void GLThread::setView(double left, double right, double bottom, double top)
		{
			_left = left;
			_right = right;
			_bottom = bottom;
			_top = top;
		}

		void GLThread::setBackgColor(double r, double g, double b)
		{
			_r = r;
			_g = g;
			_b = b;
		}
				
	}
}

// END
#endif
