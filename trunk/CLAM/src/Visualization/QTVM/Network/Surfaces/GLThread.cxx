#include "NetPlotController.hxx"
#include "NetDisplaySurface.hxx"
#include "GLThread.hxx"

namespace CLAM
{
	namespace VM
	{
		GLThread::GLThread(NetDisplaySurface* surf)
			: _surf(surf)
		{
			_controller = NULL;
			_doRendering = false;
			_doResize = false;

		       _thread.SetThreadCode(makeMemberFunctor0((*this), GLThread, thread_code));
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
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(_left, _right, _bottom, _top, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(_r, _g, _b, 1.0);
			
			while(_doRendering)
			{
				if(_doResize)
				{
					glViewport(0, 0, _w, _h);
					_doResize = false;
				}
				if(_controller)
				{
					glClearColor(_r, _g, _b, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);

					_controller->Draw();
				}

				_surf->swapBuffers();
			}
		}

		void GLThread::Start()
		{
		    if(_doRendering) Stop();
		   
		    _thread.Start();
		    _doRendering = true;
		}

		void GLThread::Stop()
		{
		    if(_doRendering)
		    {
				_doRendering = false;
				_thread.Stop();
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

