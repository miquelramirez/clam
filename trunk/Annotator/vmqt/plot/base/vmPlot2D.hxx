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

#ifndef __VMQT_PLOT2D_H__
#define __VMQT_PLOT2D_H__

#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <map>
#include <vector>
#include <QtGui/QCursor>
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{

		class Renderer2D;

		/**
		 * 	A Plot2D is an OpenGl surface for a 2D plot.
		 * 	The plot is conformed by two coordinates axis
		 * 	and the content is drawn by a stack of interactive
		 * 	layers, called Renderer2D's.
		 */
		class Plot2D : public QGLWidget
		{
			typedef std::map<QString, Renderer2D*> Renderers;
			
			Q_OBJECT
		public:
			Plot2D(QWidget* parent=0);
			~Plot2D();

			void SetBackgroundColor(const QColor& c);

			void SetXRange(double xmin, double xmax);
			void SetYRange(double ymin, double ymax);
	
			void SetZoomSteps(int xsteps, int ysteps);
  
			bool AddRenderer(const QString& key, Renderer2D* renderer);
			bool SetRendererEnabled(const QString& key, bool enabled);
	
			Renderer2D* GetRenderer(const QString& key);

			void BringToFront(const QString& key);
			void SendToBack(const QString& key);

		signals:
			void hZoomRatio(QString);
			void vZoomRatio(QString);
			void hScrollValue(int);
			void vScrollValue(int);
			void hScrollMaxValue(int);
			void vScrollMaxValue(int);
			void xRulerRange(double,double);
			void yRulerRange(double,double);
			void hBoundsChanged(double,double);
			
		public slots:
			void hZoomIn();
			void hZoomOut();	
			void vZoomIn();
			void vZoomOut();
			void updateHScrollValue(int);
			void updateVScrollValue(int);
			void setHBounds(double, double);
			void setVBounds(double, double);
		public:
			// Renderer interface
			/** Sets the tooltip text. If an empty string is given,
			 * the tooltip is disabled. */
			void setToolTip(QString);
			/** Sets the location to center the zoom on */
			void setHZoomPivot(double);
			/** Makes further events to be captured just by the 
			 * given renderer. Other renderers will ignore events. */
			void rendererWorking(QString key, bool isWorking);
			void updateRenderers();
			/** Marks the plot to be updated when the cpu is free.
			* Several calls to this method wihout update are safely
			* ignored. */
			void needUpdate();

		protected:
			void paintGL(); 
			void resizeEvent(QResizeEvent* e);
			void mouseMoveEvent(QMouseEvent* e);
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
			void mouseDoubleClickEvent(QMouseEvent* e);
			void keyPressEvent(QKeyEvent * e);
			void keyReleaseEvent(QKeyEvent * e);
			void enterEvent(QEvent* e);
			void leaveEvent(QEvent* e);


		protected:
			QColor mBgColor;

		private:
			Renderers mRenderers;
			QString   mToolTip;
			QFont     mToolTipFont;

			int    mXZoomSteps;
			int    mYZoomSteps;
			int    mCurrentXZoomStep;
			int    mCurrentYZoomStep;
			double mHZoomRef;
			double mCurrentXSpan;
			double mCurrentYSpan;
			bool   mDoResize;

			unsigned mUpdatePending;

			Range mXRange;
			Range mYRange;

			GLView     mView;
			GLViewport mViewport;

			std::pair<int,int>   mMousePos;
			std::vector<QString> mDrawOrder;

		private:
			void DrawRenderers();
			void RenderToolTip();
			void ClearRenderers();
			bool ExistRenderer(const QString& key);

			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);

			void UpdateHBounds(bool zin);
			void UpdateVBounds(bool zin);

			int GetXPixels();
			int GetYPixels();
			int GetHScrollValue();
			int GetVScrollValue();

			bool ReferenceIsVisible() const;
			double GetReference() const;

			void SetRenderersHBounds(double left, double right);
			void SetRenderersVBounds(double bottom, double top);
			std::pair<double,double> GetXY(int x, int y);
			QRect ToolTipRect();
		};
	}
}

#endif

