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
#include "vmPlotCanvas.hxx"
#include <QtCore/QTimer>
#include <iostream>
#include "vmRuler.hxx"
#include "vmScrollGroup.hxx"
#include <CLAM/RangeView.hxx>

namespace CLAM
{
	namespace VM
	{

		PlotCanvas::PlotCanvas(QWidget* parent)
			: QGLWidget(parent)
			, mBgColor(255,255,255,255)
			, mToolTip("")
			, mHZoomRef(0.0)
			, mUpdatePending(0)
			, mXRange(-1.0,1.0)
			, mYRange(-1.0,1.0)
			, mXRuler(0)
			, mYRuler(0)
			, mXRangeController(0)
			, mYRangeController(0)
		{
			setFocusPolicy(Qt::StrongFocus);
			setMouseTracking(true);
			setAutoBufferSwap(false);
			
			mToolTipFont.setFamily("fixed");
			mToolTipFont.setPointSize(8);
			mToolTipFont.setBold(true);
			mToolTipFont.setStyleHint(QFont::Courier);
		}

		PlotCanvas::~PlotCanvas()
		{
			ClearRenderers();
		}

		bool PlotCanvas::AddRenderer(const QString& key, Renderer2D* renderer)
		{
			if(ExistRenderer(key)) return false;
			mRenderers[key] = renderer;
			Renderer2D* rd = GetRenderer(key);
			if(!rd) return false;
			rd->setContainer(this);
			rd->SetXRange(mXRange.min,mXRange.max);
			rd->SetYRange(mYRange.min,mYRange.max);
			rd->SetKey(key);
			mDrawOrder.push_back(key);
			return true;
		}

		bool PlotCanvas::SetRendererEnabled(const QString& key, bool enabled)
		{
			if(!ExistRenderer(key)) return false;
			mRenderers[key]->SetEnabled(enabled);
			return true;
		}
	
		Renderer2D* PlotCanvas::GetRenderer(const QString& key)
		{
			if(!ExistRenderer(key)) return 0;
			return mRenderers[key];
		}

		void PlotCanvas::BringToFront(const QString& key)
		{
			std::list<QString>::iterator it =
				std::find(mDrawOrder.begin(), mDrawOrder.end(), key);
			if (it == mDrawOrder.end()) return; // Not found!
			if (mDrawOrder.back() == key) return; // Already there
			mDrawOrder.erase(it);
			mDrawOrder.push_back(key);
		}

		void PlotCanvas::SendToBack(const QString& key)
		{
			std::list<QString>::iterator it =
				std::find(mDrawOrder.begin(), mDrawOrder.end(), key);
			if (it == mDrawOrder.end()) return; // Not found!
			if (mDrawOrder.front() == key) return; // Already there
			mDrawOrder.erase(it);
			mDrawOrder.push_front(key);
		}

		void PlotCanvas::SetXRange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			mXRange.min = xmin;
			mXRange.max = xmax;
			for (Renderers::iterator it = mRenderers.begin();
				it != mRenderers.end(); it++)
				it->second->SetXRange(mXRange.min,mXRange.max);
			SetHBounds(mXRange.min, mXRange.max);
			updateXRangeController();
		}

		void PlotCanvas::SetHBounds(double left, double right)
		{
			if(left==mView.left && right==mView.right) return;
			CLAM::RangeView::keepWithinInterval(left, right, mXRange.min, mXRange.max);
			mView.left = left;
			mView.right = right;
			SetRenderersHBounds(mView.left,mView.right);
			if (mXRuler) mXRuler->updateRange(mView.left, mView.right);
			needUpdate();
			emit xRangeChanged(mView.left, mView.right);
		}

		void PlotCanvas::SetYRange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			mYRange.min = ymin;
			mYRange.max = ymax;
			for (Renderers::iterator it = mRenderers.begin();
				it != mRenderers.end(); it++)
				it->second->SetYRange(mYRange.min,mYRange.max);
			SetVBounds(mYRange.min,mYRange.max);
			updateYRangeController();
		}

		void PlotCanvas::SetVBounds(double bottom, double top)
		{
			if(bottom==mView.bottom && top==mView.top) return;
			CLAM::RangeView::keepWithinInterval(bottom, top, mYRange.min, mYRange.max);
			mView.bottom = bottom;
			mView.top = top;
			SetRenderersVBounds(mView.bottom, mView.top);
			if (mYRuler) mYRuler->updateRange(mView.bottom, mView.top);
			updateYRangeController();
			emit yRangeChanged(mView.bottom, mView.top);
			needUpdate();
		}

		void PlotCanvas::hZoomIn()
		{
			double low = mView.left;
			double high = mView.right;
			double excentricity = CLAM::RangeView::zoomExcentricity(
				low, high, GetReference());
			CLAM::RangeView::zoom(low, high, .5, excentricity);
			SetHBounds(low,high);
			updateXRangeController();
			emit hBoundsChanged(low, high);
		}
		void PlotCanvas::hZoomOut()
		{
			double low = mView.left;
			double high = mView.right;
			double excentricity = CLAM::RangeView::zoomExcentricity(
				low, high, GetReference());
			CLAM::RangeView::zoom(low, high, 2., excentricity);
			SetHBounds(low,high);
			updateXRangeController();
			emit hBoundsChanged(low, high);
		}
		void PlotCanvas::vZoomIn()
		{
			double low = mView.bottom;
			double high = mView.top;
			CLAM::RangeView::zoom(low, high, .5, .5);
			SetVBounds(low,high);
			updateYRangeController();
		}
		void PlotCanvas::vZoomOut()
		{
			double low = mView.bottom;
			double high = mView.top;
			CLAM::RangeView::zoom(low, high, 2., .5);
			SetVBounds(low,high);
			updateYRangeController();
		}

	
		void PlotCanvas::SetZoomSteps(unsigned xsteps, unsigned ysteps)
		{
			SetXRange(mXRange.min,mXRange.max);
			SetYRange(mYRange.min,mYRange.max);
		}

		void PlotCanvas::updateXRangeController()
		{
			if (!mXRangeController) return;
			double low=mView.left;
			double high=mView.right;
			int page = width();
			double span=high-low;
			int fullPixelSize = std::floor(mXRange.Span()*page/(high-low));
			int max = std::max(0,fullPixelSize-page);
			int val = (low-mXRange.min)*page/span;
			mXRangeController->setMaxScrollValue(max, page);
			mXRangeController->updateScrollValue(val);
		}

		void PlotCanvas::updateYRangeController()
		{
			if (!mYRangeController) return;
			double low=mView.bottom;
			double high=mView.top;
			int page = height();
			double span=high-low;
			int fullPixelSize = std::floor(mYRange.Span()*page/span);
			int max = std::max(0,fullPixelSize-page);
			int val = (mYRange.max-high)*page/span;
			mYRangeController->setMaxScrollValue(max, page);
			mYRangeController->updateScrollValue(val);
		}
		
		void PlotCanvas::updateHScrollValue(int action)
		{
			CLAM_ASSERT(mXRangeController,
				"PlotCanvas::updateHScrollValue: Received a scroll signal "
				"without bein connected.");
			// TODO: Use the RangeView for that
//			double offset = CLAM::RangeView::scrollOffset(low, high, lowest,mYRange.max);
			double pixelOffset = mXRangeController->GetScrollValue();
			double low = mView.left;
			double high = mView.right;
			int page = width();
			double span = high-low;
			low = mXRange.min+pixelOffset*span/page;
			high = low+span;
			SetHBounds(low,high);
			emit hBoundsChanged(low,high);
		}

		void PlotCanvas::updateVScrollValue(int action)
		{
			CLAM_ASSERT(mYRangeController,
				"PlotCanvas::updateVScrollValue: Received a scroll signal "
				"without bein connected.");
			double pixelOffset = mYRangeController->GetScrollValue();
			double low = mView.bottom;
			double high = mView.top;
			int page = height();
			double span = high-low;
			high = mYRange.max-pixelOffset*span/page;
			low = high-span;
			SetVBounds(low,high);
		}

		void PlotCanvas::setHBounds(double left, double right)
		{
			SetHBounds(left,right);
		}

		void PlotCanvas::setVBounds(double bottom, double top)
		{
			SetVBounds(bottom,top);
		}

		void PlotCanvas::paintGL()
		{
			if(mDoResize)
			{
				glViewport(0,0,width(),height());
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
			DrawRenderers();
			RenderToolTip(); 
			swapBuffers();
			mUpdatePending=0;
		}
		
		void PlotCanvas::resizeEvent(QResizeEvent* e)
		{
			GLViewport viewPort(0,0,
				e->size().width(),
				e->size().height());
			mDoResize = true;
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++)
				it->second->SetViewport(viewPort);
			updateXRangeController();
			updateYRangeController();
	
			if (mXRuler) mXRuler->updateRange(mView.left, mView.right);
			emit xRangeChanged(mView.left, mView.right);
			if (mYRuler) mYRuler->updateRange(mView.bottom, mView.top);
			emit yRangeChanged(mView.bottom, mView.top);
			needUpdate();
		}

		void PlotCanvas::mouseMoveEvent(QMouseEvent* e)
		{
			mMousePos = std::make_pair(e->x(),e->y());
			std::pair<double,double> coords = GetXY(mMousePos.first,mMousePos.second);
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->MouseMoveEvent(coords.first,coords.second);
		}

		void PlotCanvas::mousePressEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->MousePressEvent(coords.first,coords.second);
		}

		void PlotCanvas::mouseReleaseEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->MouseReleaseEvent(coords.first,coords.second);
		}

		void PlotCanvas::mouseDoubleClickEvent(QMouseEvent* e)
		{
			std::pair<double,double> coords = GetXY(e->x(),e->y());
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->MouseDoubleclickEvent(coords.first,coords.second);
		}

		void PlotCanvas::keyPressEvent(QKeyEvent * e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->KeyPressEvent(e->key());
		}

		void PlotCanvas::keyReleaseEvent(QKeyEvent * e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->KeyReleaseEvent(e->key());
		}

		void PlotCanvas::enterEvent(QEvent* e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->EnterEvent();
			QGLWidget::enterEvent(e);
		}

		void PlotCanvas::leaveEvent(QEvent* e)
		{
			Renderers::iterator it = mRenderers.begin();
			for(;it != mRenderers.end(); it++)
				it->second->LeaveEvent();
			QGLWidget::leaveEvent(e);
		}

		void PlotCanvas::updateRenderers()
		{
			SetRenderersHBounds(mView.left,mView.right);
			SetRenderersVBounds(mView.bottom,mView.top);
			needUpdate();
		}

		void PlotCanvas::setHZoomPivot(double ref)
		{
			if (ref < mXRange.min) return;
			if (ref > mXRange.max) return;
			mHZoomRef = ref;
		}

		void PlotCanvas::needUpdate()
		{
			if (mUpdatePending++) return;
			QTimer::singleShot(10, this, SLOT(updateGL()));
		}

		void PlotCanvas::setToolTip(QString str)
		{
			mToolTip = str;
			needUpdate();
		}

		void PlotCanvas::rendererWorking(QString key,bool working)
		{
			// TODO: Maybe using the event handling system is enough
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

		void PlotCanvas::DrawRenderers()
		{
			std::list<QString>::iterator it = mDrawOrder.begin();
			for(; it != mDrawOrder.end(); it++) mRenderers[(*it)]->Render();
		}

		void PlotCanvas::RenderToolTip()
		{
			if(mToolTip.isEmpty()) return;

			QRect rect = ToolTipRect();
			QFontMetrics font_metrics(mToolTipFont);
			int font_height = font_metrics.height();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0,width(),height(),0.0,-1.0,1.0);
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

		void PlotCanvas::ClearRenderers()
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++) delete it->second;
				
			mRenderers.clear();
		}

		bool PlotCanvas::ExistRenderer(const QString& key)
		{
			return (mRenderers.find(key) != mRenderers.end());
		}

		bool PlotCanvas::ReferenceIsVisible() const
		{
			return (mHZoomRef >= mView.left && mHZoomRef <= mView.right);
		}

		double PlotCanvas::GetReference() const
		{
			return mHZoomRef;
		}

		void PlotCanvas::SetRenderersHBounds(double left, double right)
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++)
				it->second->SetHBounds(left,right); 
		}

		void PlotCanvas::SetRenderersVBounds(double bottom, double top)
		{
			Renderers::iterator it = mRenderers.begin();
			for(; it != mRenderers.end(); it++)
				it->second->SetVBounds(bottom,top);
		}

		std::pair<double,double> PlotCanvas::GetXY(int x, int y)
		{
			double xcoord = double(x);
			xcoord *= (mView.right-mView.left);
			xcoord /= double(width());
			xcoord += mView.left;
			double ycoord = double(-y+height());
			ycoord *= (mView.top-mView.bottom);
			ycoord /= double(height());
			ycoord += mView.bottom;
			return std::make_pair(xcoord,ycoord);
		}

		QRect PlotCanvas::ToolTipRect()
		{
			QFontMetrics font_metrics(mToolTipFont);
			
			int x = mMousePos.first+2;
			int y = mMousePos.second+2;

			int w = font_metrics.width(mToolTip)+10;
			int h = font_metrics.height()+10;

			if(x+w > width()) x -= w;
			if(y+h > height()) y -= h;

			return QRect(x,y,w,h);
		}

		void PlotCanvas::SetBackgroundColor(const QColor& c)
		{
			mBgColor = c;
			updateGL();
		}
	}
}

// END

