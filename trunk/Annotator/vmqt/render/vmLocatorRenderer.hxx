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

#ifndef __VMQT_LOCATOR_RENDERER_H__
#define __VMQT_LOCATOR_RENDERER_H__

#include "vmRenderer2D.hxx"
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		class Locator : public Renderer2D
		{
			Q_OBJECT
		public:
			Locator();
			~Locator();

			void SetLocatorColor(const QColor& c);
			void SetRegionColor(const QColor& c);

		signals:
			void selectedRegion(double,double);
			void regionChanged(double, double, bool);
			
		public slots:
		    void updateLocator(double);
			void updateLocator(double, bool);
			void updateRegion(double, double, bool);

		protected:
			void Render();
			void SetXRange(double xmin, double xmax);
			void MousePressEvent(double x, double y);
			void MouseReleaseEvent(double x, double y);
			void MouseMoveEvent(double x, double y);
			void KeyPressEvent(int key);
			void KeyReleaseEvent(int key);
			void LeaveEvent();
			
		private:
			bool   mUpdatingLocator;
			double mLocatorPos;
			Range  mRegion;
			QColor  mLocatorColor;
			QColor  mRegionColor;

			enum { TOLERANCE=3 };
			enum EditionMode 
			{
				Idle,
				DraggingLocator,
				DraggingBeginRegion,
				DraggingEndRegion
			};

			EditionMode mEditionMode;

			void DrawLocator();
			void DrawRegion(int mode);
			int DrawRegionMode();
			bool PickBeginRegion(double x, double tolerance);
			bool PickEndRegion(double x, double tolerance);
		};
	}
}

#endif

