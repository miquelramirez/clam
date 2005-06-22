#include "NetDisplaySurface.hxx"
#include <qwidget.h>
#include <iostream>
#include <qtooltip.h>

namespace CLAM
{
	namespace VM
	{
		NetDisplaySurface::NetDisplaySurface(QWidget* parent) 
			: QGLWidget(parent)
			, mController(0)
			, mTimer( * new QTimer(this) )
			, mDoResize(false)
			, mRefreshInterval(200)
		{
			setMouseTracking(true);
			setAutoBufferSwap(false);
			SetBackgroundColor(0.0,0.0,0.0);
			InitView();
			connect(&mTimer, SIGNAL(timeout()), SLOT(updateGL()));
		}

		NetDisplaySurface::~NetDisplaySurface()
		{
			if(mController) 
			{
				delete mController;
			}
		}

		void NetDisplaySurface::SetBackgroundColor(double r, double g, double b)
		{
			mRed = r;
			mGreen = g;
			mBlue = b;
		}

		void NetDisplaySurface::SetController(NetPlotController* controller)
		{
			mController = controller;
			connect(mController,SIGNAL(sendView(SView)),this,SLOT(receivedView(SView)));
			connect(mController,SIGNAL(toolTip(QString)),this,SLOT(updateToolTip(QString)));
			setMouseTracking(true);
		}

		void NetDisplaySurface::startRendering()
		{
			if (!mTimer.isActive())
				mTimer.start(mRefreshInterval,true);
		}

		void NetDisplaySurface::stopRendering()
		{
			if (mTimer.isActive())
				mTimer.stop();
		}

		void NetDisplaySurface::paintGL()
		{
			if (!mController) return;
			if(mDoResize)
			{
				glViewport(0, 0, mWidth, mHeight);
				mDoResize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(mLeft, mRight, mBottom, mTop, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(mRed, mGreen, mBlue, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			mController->Draw();
			swapBuffers();
			if (!mTimer.isActive())
				mTimer.start(mRefreshInterval,true);
		}

		void NetDisplaySurface::resizeEvent(QResizeEvent* re)
		{
			mWidth = re->size().width();
			mHeight = re->size().height();
		    mDoResize = true;
		}

		void NetDisplaySurface::receivedView(SView v)
		{
			mLeft = v.left;
			mRight = v.right;
			mBottom = v.bottom;
			mTop = v.top;
		}

		void NetDisplaySurface::InitView()
		{
			mLeft = 0.0;
			mRight = 1.0;
			mBottom = -1.0;
			mTop = 1.0;
		}
	    
		void NetDisplaySurface::mousePressEvent(QMouseEvent* e)
		{
		    if(mController)
		    {
				if(e->button() == LeftButton)
				{
					mController->SetLeftButtonPressed(true);
					double xcoord = double(e->x());
					xcoord *= (mRight-mLeft);
					xcoord /= double(width());
					xcoord += mLeft;
					double ycoord = double(-e->y())+double(height());
					ycoord *= (mTop-mBottom);
					ycoord /= double(height());
					ycoord += mBottom;
					mController->SetPoint(TData(xcoord),TData(ycoord));
			    
				}
				if(e->button() == RightButton)
				{
					mController->SetRightButtonPressed(true);
				}
		    }
		}

		void NetDisplaySurface::mouseReleaseEvent(QMouseEvent* e)
		{
		    if(mController)
		    {
				if(e->button() == LeftButton)
				{
					mController->SetLeftButtonPressed(false);
					double xcoord = double(e->x());
					xcoord *= (mRight-mLeft);
					xcoord /= double(width());
					xcoord += mLeft;
					double ycoord = double(-e->y())+double(height());
					ycoord *= (mTop-mBottom);
					ycoord /= double(height());
					ycoord += mBottom;
					mController->SetPoint(TData(xcoord),TData(ycoord));	    
				}
				if(e->button() == RightButton)
				{
					mController->SetRightButtonPressed(false);
				}
		    }  
		}

		void NetDisplaySurface::mouseMoveEvent(QMouseEvent* e)
		{
		    if(mController)
		    {
				double xcoord = double(e->x());
				xcoord *= (mRight-mLeft);
				xcoord /= double(width());
				xcoord += mLeft;
				double ycoord = double(-e->y())+double(height());
				ycoord *= (mTop-mBottom);
				ycoord /= double(height());
				ycoord += mBottom;
				mController->UpdatePoint(TData(xcoord),TData(ycoord));
		    }
		}  
	
		void NetDisplaySurface::updateToolTip(QString s)
		{
		    if(!s.isEmpty())
		    {
				QToolTip::remove(this);
				QToolTip::add(this,s);
		    }
		}

	}
}

// END

