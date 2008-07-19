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

#ifndef __VMQT_RENDERER2D_H__
#define __VMQT_RENDERER2D_H__

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QCursor>
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class Grid;

		class Renderer2D : public QObject
		{
			friend class Plot2D;

			Q_OBJECT
		public:
			Renderer2D();
			virtual ~Renderer2D();

			void SetGrid(Grid* grid);

		signals:
			void requestRefresh();
			void requestUpdate();
			void hZoomRef(double);
			void toolTip(QString);
			void cursorChanged(QCursor);
			void working(QString,bool);

		protected:
			virtual void SetEnabled(bool enabled);
			virtual void Render()=0;
			virtual void SetXRange(double xmin, double xmax);
			virtual void SetYRange(double ymin, double ymax);
			virtual void SetHBounds(double left, double right);
			virtual void SetVBounds(double bottom, double top);
			virtual void MousePressEvent(double x, double y);
			virtual void MouseReleaseEvent(double x, double y);
			virtual void MouseMoveEvent(double x, double y);
			virtual void MouseDoubleclickEvent(double x, double y);
			virtual void KeyPressEvent(int key);
			virtual void KeyReleaseEvent(int key);
			virtual void EnterEvent();
			virtual void LeaveEvent();
			virtual void SetViewport(const GLViewport& v);
			
			void SetKey(const QString& key);
			void CatchEvents(bool ce);

			int GetglList();

			bool mEnabled;

			bool    mCatchEvents;
			QString mKey;

			GLView     mView;
			GLViewport mViewport;
			Range      mXRange;
			Range      mYRange;
			Keyboard   mKeyboard;
			Grid*      mGrid;
		};
	}
}

#endif

