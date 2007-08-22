/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <QtGui/QResizeEvent>
#include "vmRenderer2D.hxx"
#include "vmPlot2D.hxx"
#include <QtCore/QTimer>
#include <iostream>

namespace CLAM
{
	namespace VM
	{

		Plot2D::Plot2D(QWidget* parent)
			: Plot(parent)
			, mToolTip("")
			, mXZoomSteps(0)
			, mYZoomSteps(0)
			, mHZoomRef(0.0)
			, mCurrentXSpan(1.0)
			, mCurrentYSpan(1.0)
			, mUpdatePending(0)
			, mXRange(-1.0,1.0)
			, mYRange(-1.0,1.0)
			, mViewport(0,0,10,10)
		{
			setMouseTracking(true);
			setAutoBufferSwap(false);
			
			mToolTipFont.setFamily("fixed");
			mToolTipFont.setPointSize(8);
			mToolTipFont.setBold(true);
			mToolTipFont.setStyleHint(QFont::Courier);
		}

		Plot2D::~Plot2D()
		{
			ClearRenderers();
		}

		void Plot2D::SetXRange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			mXRange.min = xmin;
			mXRange.max = xmax;
			mCurrentXSpan = mXRange.Span();
			mView.left = mXRange.min;
			mView.right = mXRange.max;
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) it->second->SetXRange(mXRange.min,mXRange.max);
			SetRenderersHBounds(mXRange.min,mXRange.max);
			int sv = GetHScrollValue();
			emit hScrollMaxValue(GetXPixels());
			emit hScrollValue(sv);
			emit xRulerRange(mXRange.min,mXRange.max);
		}

		void Plot2D::SetYRange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			mYRange.min = ymin;
			mYRange.max = ymax;
			mCurrentYSpan = mYRange.Span();
			mView.bottom = mYRange.min;
			mView.top = mYRange.max;
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) it->second->SetYRange(mYRange.min,mYRange.max);
			SetRenderersVBounds(mYRange.min,mYRange.max);
			int sv = GetVScrollValue();
			emit vScrollMaxValue(GetYPixels());
			emit vScrollValue(sv);
			emit yRulerRange(mYRange.min,mYRange.max);
		}
	
		void Plot2D::SetZoomSteps(int xsteps, int ysteps)
		{
			if(xsteps < 0 || ysteps < 0) return;
			mXZoomSteps = xsteps;
			mYZoomSteps = ysteps;
			mCurrentYZoomStep = mYZoomSteps;
			SetXRange(mXRange.min,mXRange.max);
			SetYRange(mYRange.min,mYRange.max);
		}

		bool Plot2D::AddRenderer(const QString& key, Renderer2D* renderer)
		{
			if(ExistRenderer(key)) return false;
			mRenderers[key] = renderer;
			Renderer2D* rd = GetRenderer(key);
			if(!rd) return false;
			connect(rd,SIGNAL(requestUpdate()),this,SLOT(updateRenderers()));
			connect(rd,SIGNAL(requestRefresh()),this,SLOT(needUpdate()));
			connect(rd,SIGNAL(hZoomRef(double)),this,SLOT(updateHZoomRef(double)));
			connect(rd,SIGNAL(toolTip(QString)),this,SLOT(setToolTip(QString)));
			connect(rd,SIGNAL(cursorChanged(QCursor)),this,SLOT(changeCursor(QCursor)));
			connect(rd,SIGNAL(working(QString,bool)),this,SLOT(rendererWorking(QString,bool)));
			rd->SetXRange(mXRange.min,mXRange.max);
			rd->SetYRange(mYRange.min,mYRange.max);
			rd->SetKey(key);
			mDrawOrder.push_back(key);
			return true;
		}

		bool Plot2D::SetRendererEnabled(const QString& key, bool enabled)
		{
			if(!ExistRenderer(key)) return false;
			mRenderers[key]->SetEnabled(enabled);
			return true;
		}
	
		Renderer2D* Plot2D::GetRenderer(const QString& key)
		{
			if(!ExistRenderer(key)) return 0;
			return mRenderers[key];
		}

		void Plot2D::hZoomIn()
		{
			double newSpan = mCurrentXSpan*0.5;
			double midPoint = ReferenceIsVisible() ? GetReference(): ((mView.right+mView.left)/2);
			double left=std::max(mXRange.min, midPoint-newSpan/2);
			double right=std::min(mXRange.max, left+newSpan);
			mCurrentXSpan = right - left;
			SetHBounds(left,right);
			emit hScrollMaxValue(GetXPixels());
			emit hScrollValue(GetHScrollValue());
			emit hBoundsChanged(mView.left, mView.right);
		}

		void Plot2D::hZoomOut()
		{
			double newSpan = mCurrentXSpan*2;
			double midPoint = ReferenceIsVisible()? GetReference(): ((mView.right+mView.left)/2);
			double left=std::max(mXRange.min, midPoint-newSpan/2);
			double right=left+newSpan;
			if (right > mXRange.max)
			{
				right=mXRange.max;
				left=std::max(mXRange.min, right-newSpan);
			}
			mCurrentXSpan = right - left;
			SetHBounds(left,right);
			emit hScrollMaxValue(GetXPixels());
			emit hScrollValue(GetHScrollValue());
			emit hBoundsChanged(mView.left, mView.right);
		}

		void Plot2D::vZoomIn()
		{
			if(!mCurrentYZoomStep) return;
			mCurrentYSpan /= 2.0;
			UpdateVBounds(true);
			mCurrentYZoomStep--;
			int yratio=1 << mCurrentYZoomStep;
			emit vZoomRatio("1:"+QString::number(yratio));
			emit vScrollMaxValue(GetYPixels());
			emit vScrollValue(GetVScrollValue());
		}

		void Plot2D::vZoomOut()
		{
			if(mCurrentYZoomStep == mYZoomSteps) return;
			mCurrentYSpan *= 2.0;
			UpdateVBounds(false);
			mCurrentYZoomStep++;
			int yratio=1 << mCurrentYZoomStep;
			emit vZoomRatio("1:"+QString::number(yratio));
			emit vScrollMaxValue(GetYPixels()); 
			emit vScrollValue(GetVScrollValue());
		}
		
		void Plot2D::updateHScrollValue(int value)
		{
			double left = mXRange.Span()/double(GetXPixels())*double(value)+mXRange.min;
			double right = left+mCurrentXSpan;
			SetHBounds(left,right);
			emit hBoundsChanged(left,right);
		}

		void Plot2D::updateVScrollValue(int value)
		{
			double bottom = mYRange.Span()/double(GetYPixels())*double(value)+mYRange.min;
			double top = bottom+mCurrentYSpan;
			double tmp = bottom;
			bottom = mYRange.min+(mYRange.max-top);
			top = mYRange.max-(tmp-mYRange.min);
			SetVBounds(bottom,top);
		}

		void Plot2D::setHBounds(double left, double right)
		{
			SetHBounds(left,right);
			mCurrentXSpan = mView.right-mView.left;
			emit hScrollMaxValue(GetXPixels());
			emit hScrollValue(GetHScrollValue());
		}

		void Plot2D::setVBounds(double bottom, double top)
		{
			SetVBounds(bottom,top);
		}

		void Plot2D::paintGL()
		{
			if(mDoResize)
			{
				glViewport(0,0,mViewport.w,mViewport.h);
				mDoResize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(mView.left,mView.right,mView.bottom,mView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			qglClearColor(mBgColor);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glEnable (GL_LINE_SMOOTH);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
			Draw();
			RenderToolTip(); 
			swapBuffers();
			mUpdatePending=0;
		}
		
		void Plot2D::resizeEvent(QResizeEvent* e)
		{
			mViewport.w = e->size().width();
			mViewport.h = e->size().height();
			mDoResize = true;
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) it->second->SetViewport(mViewport);
			int sv = GetHScrollValue();
			emit hScrollMaxValue(GetXPixels());
			emit hScrollValue(sv);

			sv = GetVScrollValue();
			emit vScrollMaxValue(GetYPixels());
			emit vScrollValue(sv);
	
			emit xRulerRange(mView.left,mView.right);
			emit yRulerRange(mView.bottom,mView.top);
		}

		void Plot2D::mouseMoveEvent(QMouseEvent* e)
		{
			mMousePos = std::make_pair(e->x(),e->y());
			std::pair<double,double> coords = GetXY(mMousePos.first,mMousePos.second);
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->MouseMoveEvent(coords.first,coords.second);
		}

		void Plot2D::mousePressEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->MousePressEvent(coords.first,coords.second);
		}

		void Plot2D::mouseReleaseEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->MouseReleaseEvent(coords.first,coords.second);
		}

		void Plot2D::mouseDoubleClickEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->MouseDoubleclickEvent(coords.first,coords.second);
		}

		void Plot2D::keyPressEvent(QKeyEvent * e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->KeyPressEvent(e->key());
		}

		void Plot2D::keyReleaseEvent(QKeyEvent * e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->KeyReleaseEvent(e->key());
		}

		void Plot2D::enterEvent(QEvent* e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->EnterEvent();
			QGLWidget::enterEvent(e);
		}

		void Plot2D::leaveEvent(QEvent* e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++) it->second->LeaveEvent();
			QGLWidget::leaveEvent(e);
		}

		void Plot2D::updateRenderers()
		{
			SetRenderersHBounds(mView.left,mView.right);
			SetRenderersVBounds(mView.bottom,mView.top);
			needUpdate();
		}

		void Plot2D::updateHZoomRef(double ref)
		{
			if(ref < mXRange.min || ref > mXRange.max) return;
			mHZoomRef = ref;
		}

		void Plot2D::changeCursor(QCursor cursor)
		{
			setCursor(cursor);
		}

		void Plot2D::needUpdate()
		{
			if (mUpdatePending++) return;
			QTimer::singleShot(10, this, SLOT(updateGL()));
		}

		void Plot2D::setToolTip(QString str)
		{
			mToolTip = str;
			needUpdate();
		}

		void Plot2D::rendererWorking(QString key,bool working)
		{
			Renderers::iterator it = mRenderers.begin();
			if(working)
			{
				for(;it != mRenderers.end(); it++)
				{
					if(it->first == key) continue;
					it->second->CatchEvents(false);
				}
			}
			else
			{
				for(;it != mRenderers.end(); it++)
				{
					it->second->CatchEvents(true);
				}

			}
		}

		void Plot2D::Draw()
		{
			std::vector<QString>::iterator it = mDrawOrder.begin();
			for(; it != mDrawOrder.end(); it++) mRenderers[(*it)]->Render();
		}

		void Plot2D::RenderToolTip()
		{
			if(mToolTip.isEmpty()) return;

			QRect rect = ToolTipRect();
			QFontMetrics font_metrics(mToolTipFont);
			int font_height = font_metrics.height();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,mViewport.w,mViewport.h,0.0,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glColor3ub(255,255,0);
			glBegin(GL_QUADS);
			glVertex2d(rect.left(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.bottom());
			glVertex2d(rect.left(),rect.bottom());
			glEnd();
			glLineWidth(1);
			glColor3ub(0,0,0);
			glBegin(GL_LINE_STRIP);
			glVertex2d(rect.left(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.top());
			glVertex2d(rect.left()+rect.width(),rect.bottom());
			glVertex2d(rect.left(),rect.bottom());
			glVertex2d(rect.left(),rect.top());
			glEnd();
   
			renderText(rect.left()+5,rect.top()+font_height+2,mToolTip,mToolTipFont);

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void Plot2D::ClearRenderers()
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) delete it->second;
				
			mRenderers.clear();
		}

		bool Plot2D::ExistRenderer(const QString& key)
		{
			return (mRenderers.find(key) != mRenderers.end());
		}

		void Plot2D::SetHBounds(double left, double right)
		{
			if(left==mView.left && right==mView.right) return;
			mView.left = left;
			mView.right = right;
			SetRenderersHBounds(mView.left,mView.right);
			emit xRulerRange(mView.left, mView.right);
			needUpdate();
		}

		void Plot2D::SetVBounds(double bottom, double top)
		{
			if(bottom==mView.bottom && top==mView.top) return;
			mView.bottom = bottom;
			mView.top = top;
			SetRenderersVBounds(mView.bottom,mView.top);
			emit yRulerRange(mView.bottom, mView.top);
			needUpdate();
		}

		void Plot2D::UpdateVBounds(bool zin)
		{
			double bottom = mView.bottom;
			double top = mView.top;
			if(zin)
			{
				bottom += mCurrentYSpan/2.0;
				top -= mCurrentYSpan/2.0;
			}
			else
			{
				bottom -= mCurrentYSpan/4.0;
				top += mCurrentYSpan/4.0;
				if(bottom < mYRange.min)
				{
					bottom = mYRange.min;
					top = bottom+mCurrentYSpan;
				}
				if(top > mYRange.max)
				{
					top = mYRange.max;
					bottom = top-mCurrentYSpan;
				}
			}
			SetVBounds(bottom,top);
		}

		int Plot2D::GetXPixels() 
		{
			return int(mXRange.Span()*double(mViewport.w)/mCurrentXSpan);
		}
		
		int Plot2D::GetYPixels() 
		{
			return int(mYRange.Span()*double(mViewport.h)/mCurrentYSpan);
		}

		int Plot2D::GetHScrollValue() 
		{
			return int((mView.left-mXRange.min)*double(GetXPixels())/mXRange.Span());
		}

		int Plot2D::GetVScrollValue() 
		{
			return int((mYRange.max-mView.top)*double(GetYPixels())/mYRange.Span());
		}

		bool Plot2D::ReferenceIsVisible() const
		{
			return (mHZoomRef >= mView.left && mHZoomRef <= mView.right);
		}

		double Plot2D::GetReference() const
		{
			return mHZoomRef;
		}

		void Plot2D::SetRenderersHBounds(double left, double right)
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) it->second->SetHBounds(left,right); 
		}

		void Plot2D::SetRenderersVBounds(double bottom, double top)
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) it->second->SetVBounds(bottom,top);
		}

		std::pair<double,double> Plot2D::GetXY(int x, int y)
		{
			double xcoord = double(x);
			xcoord *= (mView.right-mView.left);
			xcoord /= double(mViewport.w);
			xcoord += mView.left;
			double ycoord = double(-y+mViewport.h);
			ycoord *= (mView.top-mView.bottom);
			ycoord /= double(mViewport.h);
			ycoord += mView.bottom;
			return std::make_pair(xcoord,ycoord);
		}

		QRect Plot2D::ToolTipRect()
		{
			QFontMetrics font_metrics(mToolTipFont);
			
			int x = mMousePos.first+2;
			int y = mMousePos.second+2;

			int w = font_metrics.width(mToolTip)+10;
			int h = font_metrics.height()+10;

			if(x+w > mViewport.w) x -= w;
			if(y+h > mViewport.h) y -= h;

			return QRect(x,y,w,h);
		}

		void Plot2D::BringToFront(const QString& key)
		{
			if(mDrawOrder.size() <= 1) return;
			if(mDrawOrder[mDrawOrder.size()-1] == key) return;
			unsigned i=0;
			for(; i < mDrawOrder.size(); i++) if(mDrawOrder[i]==key) break;
			if(i==mDrawOrder.size()) return;
			std::vector<QString> tmp;
			for(unsigned k=0; k < mDrawOrder.size(); k++)
			{
				if(k==i) continue;
				tmp.push_back(mDrawOrder[k]);
			}
			for(i=0; i < tmp.size(); i++) mDrawOrder[i]=tmp[i];
			mDrawOrder[mDrawOrder.size()-1] = key;
		}

		void Plot2D::SendToBack(const QString& key)
		{
			if(mDrawOrder.size() <= 1) return;
			if(mDrawOrder[0] == key) return;
			unsigned i=0;
			for(; i < mDrawOrder.size(); i++) if(mDrawOrder[i]==key) break;
			if(i==mDrawOrder.size()) return;
			std::vector<QString> tmp;
			for(unsigned k=0; k < mDrawOrder.size(); k++)
			{
				if(k==i) continue;
				tmp.push_back(mDrawOrder[k]);
			}
			for(i=1; i < mDrawOrder.size(); i++) mDrawOrder[i]=tmp[i-1];
			mDrawOrder[0] = key;
		}

	}
}

// END

