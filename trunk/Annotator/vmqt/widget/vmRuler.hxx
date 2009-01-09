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

#ifndef __VMQT_RULER_H__
#define __VMQT_RULER_H__

#include <vector>
#include <QtGui/QColor>
#include <QtOpenGL/QGLWidget>
#undef GetClassName
#include <CLAM/RulerTicks.hxx>
#include <vector>
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler : public QWidget
		{
			Q_OBJECT
		public:
			Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale=CLAM::VM::eLinearScale);
			~Ruler();

			void SetRange(double min, double max);
			void SetScale(ERulerScale scale);
			ERulerScale GetScale() const;
	    
			void SetBackgroundColor(const QColor& c);
			void SetForegroundColor(const QColor& c);
	
			void ShowFloats();
			void ShowIntegers();

			const QFont& GetFont() const;

		signals:
			void valueChanged(double, double);
	    
		public slots:
			void updateRange(double, double);

		protected:
			void paintEvent(QPaintEvent * event);

		private:
			int    mLabelHeight;
			bool   mShowFloats;
			QFont  mFont;
	
			ERulerPos   mPosition;
			ERulerScale mScale;
			GLViewport  mViewport;
			Range       mTotalRange;
			Range       mCurrentRange;
			QColor       mBgColor;
			QColor       mFgColor;

			std::vector<double> mValuesToDraw;

			enum { MINTICKS=3 };

			void Rebuild();

			void Draw();
			void DrawLeft(QPainter & painter);
			void DrawRight(QPainter & painter);
			void DrawBottom(QPainter & painter);
			void DrawTop(QPainter & painter);
			void DrawLabels(QPainter & painter);

			int GetMaxLabelWidth();
			bool IsVisible(double value);
			QString GetLabel(double value);
			QPointF GetLabelCoords(double value);
			CLAM::RulerTicks _majorTicks;
			CLAM::RulerTicks _minorTicks;
		};
    }
}

#endif

