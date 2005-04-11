/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <qevent.h>
#include <qtooltip.h>
#include "PlotController.hxx"
#include "DisplaySurface.hxx"

namespace CLAM
{
	namespace VM
	{
		DisplaySurface::DisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
		{
			SetBackgroundColor(1.0,1.0,1.0);
			InitView();

			_controller = NULL;
			setMouseTracking(true);
		}

		DisplaySurface::~DisplaySurface()
		{
			if(_controller) delete _controller;
		}

		void DisplaySurface::SetBackgroundColor(double r, double g, double b)
		{
			_bkColor.r = r;
			_bkColor.g = g;
			_bkColor.b = b;
		}

		void DisplaySurface::SetController(PlotController* controller)
		{
			_controller = controller;
			connect(_controller,SIGNAL(sendView(View)),this,SLOT(receivedView(View)));
			connect(_controller,SIGNAL(requestRefresh()),this,SLOT(updateGL()));
			connect(_controller,SIGNAL(toolTip(QString)),this,SLOT(updateToolTip(QString)));
			connect(this,SIGNAL(leavingMouse()),_controller,SIGNAL(leavingMouse()));
			connect(_controller,SIGNAL(changeCursor(QCursor)),this,SLOT(changeCursor(QCursor)));
		}

		void DisplaySurface::initializeGL()
		{
			glClearColor(_bkColor.r,_bkColor.g,_bkColor.b,1.0);
			glShadeModel(GL_FLAT);
		}

		void DisplaySurface::resizeGL(int w,int h)
		{
			glViewport(0,0,w,h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(_view.left,_view.right,_view.bottom,_view.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
		}

		void DisplaySurface::paintGL()
		{
			if(_controller)
			{
				glClearColor(_bkColor.r,_bkColor.g,_bkColor.b,1.0);
				glClear(GL_COLOR_BUFFER_BIT);
				_controller->Draw();
			}
		}

		void DisplaySurface::mousePressEvent(QMouseEvent * e) 
		{
			if(_controller)
			{
				if(e->button() == LeftButton)
				{
				        _controller->SetLeftButtonPressed(true);
					float left = float(_controller->GetLeftBound());
					float right = float(_controller->GetRightBound());
					float xcoord = e->x();
					xcoord *= float(_view.right);
					xcoord /= float(width());
					xcoord += left;
					if(xcoord >= left && xcoord <= right)
					{
						_controller->SetSelPos(TData(xcoord));
						updateGL();
					}
				}
			}
		}

	        void DisplaySurface::mouseReleaseEvent(QMouseEvent* e)
		{
		    if(_controller)
		    {
			if(e->button() == LeftButton)
			{
			    _controller->SetLeftButtonPressed(false);
			}
		    }
		}

		void DisplaySurface::mouseMoveEvent(QMouseEvent* e)
		{
			if(_controller)
			{
				float left = float(_controller->GetLeftBound());
				float right = float(_controller->GetRightBound());
				float xcoord = float(e->x());
				xcoord *= float(_view.right);
				xcoord /= float(width());
				xcoord += left;
				float ycoord = float(-e->y())+float(height());
				if(xcoord >= left && xcoord <= right)
				{
					_controller->SetMousePos(TData(xcoord),TData(ycoord));
				}
			}
		}

		void DisplaySurface::receivedView(View v)
		{
			_view.left = v.left;
			_view.right = v.right;
			_view.bottom = v.bottom;
			_view.top = v.top;
			resizeGL(width(),height());
		}

		void DisplaySurface::InitView()
		{
			_view.left = 0.0f;
			_view.right = 1.0f;
			_view.bottom = -1.0f;
			_view.top = 1.0f;
		}

		void DisplaySurface::resizeEvent(QResizeEvent *e)
		{
			QGLWidget::resizeEvent(e);
			_controller->SurfaceDimensions(e->size().width(),e->size().height());
		}

		void DisplaySurface::updateToolTip(QString s)
		{
			QToolTip::remove(this);
			QToolTip::add(this,s);
		}

		void DisplaySurface::leaveEvent(QEvent* e)
		{
		    if(_controller)
		    {
			_controller->LeaveMouse();
		    }
		    QWidget::leaveEvent(e);
		    emit leavingMouse();
		}

	        void DisplaySurface::enterEvent(QEvent* e)
		{
		    if(_controller)
		    {
			_controller->EnterMouse();
			updateGL();
			resizeGL(width(),height());
			updateGL();
		    }
		}

	        void DisplaySurface::changeCursor(QCursor cursor)
		{
		    setCursor(cursor);
		}
	}
}

// END

