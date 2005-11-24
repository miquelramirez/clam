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

#include <qtimer.h>
#include <qtooltip.h>
#include "PlotController.hxx"
#include "DisplaySurface.hxx"

namespace CLAM
{
    namespace VM
    {
		DisplaySurface::DisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
			, mController(0)
			, mRed(1.0)
			, mGreen(1.0)
			, mBlue(1.0)
			, mWidth(0)
			, mHeight(0)
			, mDoResize(false)
			, mTimer(0)
		{
			setMouseTracking(true);
			setAutoBufferSwap(false);
			setFocusPolicy(StrongFocus);
		}

		DisplaySurface::~DisplaySurface()
		{
			if(mController) delete mController;
		}

		void DisplaySurface::SetBackgroundColor(const double& r, const double& g, const double& b)
		{
			mRed = r;
			mGreen= g;
			mBlue= b;
			updateGL();
		}

		void DisplaySurface::SetController(PlotController* controller)
		{
			mController = controller;
			connect(mController,SIGNAL(viewChanged(GLView)),this,SLOT(updateView(GLView)));
			connect(mController,SIGNAL(requestRefresh()),this,SLOT(updateGL()));
			connect(mController,SIGNAL(toolTip(QString)),this,SLOT(updateToolTip(QString)));
			connect(mController,SIGNAL(cursorChanged(QCursor)),this,SLOT(changeCursor(QCursor)));
			if(mController->IsPlayable())
			{
				connect(mController,SIGNAL(startPlaying()),this,SLOT(startTimer()));
				connect(mController,SIGNAL(stopPlaying()),this,SLOT(stopTimer()));
			}
		}

		void DisplaySurface::paintGL()
		{
			if(mDoResize)
			{
				glViewport(0, 0, mWidth, mHeight);
				mDoResize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(mView.left,mView.right,mView.bottom,mView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(mRed, mGreen, mBlue, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			mController->Draw();
			swapBuffers();
			if(mTimer) if(!mTimer->isActive()) mTimer->start(TIMER_INTERVAL,true);
		}

		void DisplaySurface::mousePressEvent(QMouseEvent * e) 
		{
			mController->MousePressEvent(e);
		}

		void DisplaySurface::mouseReleaseEvent(QMouseEvent* e)
		{
			mController->MouseReleaseEvent(e);
		}

		void DisplaySurface::mouseMoveEvent(QMouseEvent* e)
		{
			mController->MouseMoveEvent(e);
		}

		void DisplaySurface::focusInEvent(QFocusEvent* e)
		{
			QWidget::focusInEvent(e);
			emit focusIn();
		}

		void DisplaySurface::focusOutEvent(QFocusEvent* e)
		{
			QWidget::focusOutEvent(e);
			emit focusOut();
		}

		void DisplaySurface::updateView(GLView v)
		{
			mView = v;
		}

		void DisplaySurface::resizeEvent(QResizeEvent *e)
		{
			mWidth = e->size().width();
			mHeight = e->size().height();
			mDoResize = true;
			if(mController) mController->DisplayDimensions(mWidth,mHeight);
		}

		void DisplaySurface::updateToolTip(QString s)
		{
			QToolTip::remove(this);
			QToolTip::add(this,s);
		}

		void DisplaySurface::leaveEvent(QEvent* e)
		{
			mController->LeaveMouse();
		    QWidget::leaveEvent(e);
		}

		void DisplaySurface::enterEvent(QEvent* e)
		{
			mController->EnterMouse();
		    QWidget::enterEvent(e);
		}

		void DisplaySurface::changeCursor(QCursor cursor)
		{
			setCursor(cursor);
		}

		void DisplaySurface::startTimer()
		{
			if(!mTimer) 
			{
				mTimer = new QTimer(this);
				connect(mTimer,SIGNAL(timeout()),this,SLOT(update()));
				update();
			}
		}

		void DisplaySurface::stopTimer()
		{
			if(mTimer)
			{
				mTimer->stop();
				delete mTimer;
				mTimer = 0;
				update();
			}
		}

		void DisplaySurface::mouseDoubleClickEvent(QMouseEvent* e)
		{
			if(e->button()==LeftButton)
			{
				mController->OnDoubleClick();
			}
		}
    }
}

// END

