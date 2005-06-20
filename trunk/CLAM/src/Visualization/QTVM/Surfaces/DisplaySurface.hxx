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

#ifndef __DISPLAYSURFACE__
#define __DISPLAYSURFACE__

#include <qgl.h>
#include "DataTypes.hxx"
#include "PlotController.hxx"

class QTimer;

namespace CLAM
{
    namespace VM
    {
		class DisplaySurface : public QGLWidget
		{
			Q_OBJECT

		public:
			DisplaySurface(QWidget* parent = 0);
			~DisplaySurface();

			void SetBackgroundColor(double r, double g, double b);
			void SetController(PlotController* controller);

		signals:
			void leavingMouse();

		private slots:
			void receivedView(View);
			void updateToolTip(QString);
			void changeCursor(QCursor);
			void startTimer();
			void stopTimer();

		protected:
			void paintGL(); 
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
			void mouseMoveEvent(QMouseEvent* e);
			void resizeEvent(QResizeEvent *e);
			void leaveEvent(QEvent* e);
			void enterEvent(QEvent* e);
			void mouseDoubleClickEvent(QMouseEvent* e);

		private:
			View            mView;
			PlotController* mController;
			double          mRed;
			double          mGreen;
			double          mBlue;
			int             mWidth;
			int             mHeight;
			volatile bool   mDoResize;
			QTimer*         mTimer;

			enum { TIMER_INTERVAL=10 };
				
			void InitView();
		};
    }
}

#endif

