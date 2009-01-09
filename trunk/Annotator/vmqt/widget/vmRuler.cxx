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
#include "vmRuler.hxx"
#include <CLAM/RulerTicks.hxx>
#include <QtGui/QPainter>
#include <iostream>

namespace CLAM
{
	namespace VM
	{
		Ruler::Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale)
			: QWidget(parent)
			, mShowFloats(true)
			, mPosition(pos)
			, mScale(scale)
		{
			mLabelHeight = fontMetrics().height();

			SetRange(0.0,1.0);
			setFocusPolicy(Qt::NoFocus);
		}

		Ruler::~Ruler()
		{
		}

		void Ruler::SetRange(double min, double max)
		{
			if(min >= max) return;
			mTotalRange.min = min;
			mTotalRange.max = max;
			mCurrentRange.min = mTotalRange.min-1.0;
			mCurrentRange.max = mTotalRange.max+1.0;
			updateRange(mTotalRange.min,mTotalRange.max);
		}

		void Ruler::SetScale(ERulerScale scale)
		{
			mScale = scale;
			update();
		}

		ERulerScale Ruler::GetScale() const
		{
			return mScale;
		}

		void Ruler::SetBackgroundColor(const QColor& c)
		{
			mBgColor = c;
			update();
		}
		void Ruler::SetForegroundColor(const QColor& c)
		{
			mFgColor = c;
			update();
		}

		void Ruler::ShowFloats()
		{
			mShowFloats = true;
			Rebuild();
			update();
		}

		void Ruler::ShowIntegers()
		{
			mShowFloats = false;
			Rebuild();
			update();
		}

		void Ruler::updateRange(double min, double max)
		{
			if((mCurrentRange.min==min) && (mCurrentRange.max==max)) return;
			CLAM_ASSERT(min < max, "VM::Ruler: inverted range");
			mCurrentRange.min = min;
			mCurrentRange.max = max;
			Rebuild();
			update();
		}
	
		void Ruler::paintEvent(QPaintEvent * event)
		{
			Rebuild();
			Draw(); 
		}

		void Ruler::Rebuild()
		{
			mShowFloats = mCurrentRange.Span()<20;
			bool vertical = (mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight);
			int maxWidth = GetMaxLabelWidth();
			if (vertical)
				setFixedWidth(maxWidth+20);
			else
				setFixedHeight(40);

			int size = vertical? height() : width();
			int labelSpan = 20 + (vertical? mLabelHeight*2: maxWidth); 
			_majorTicks.setWidth(size);
			_majorTicks.setMinGap(labelSpan);
			_majorTicks.setRange(mCurrentRange.min,mCurrentRange.max);
			_minorTicks.setWidth(size);
			_minorTicks.setMinGap(std::max(10.,_majorTicks.markGap()*.1*size/mCurrentRange.Span()));
			_minorTicks.setRange(mCurrentRange.min,mCurrentRange.max);
		}

		void Ruler::Draw()
		{
			QPainter painter(this);
			painter.setPen(QPen(mFgColor, 1));
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
					drawAtLeft(painter);
					break;
				case CLAM::VM::eRight:
					drawAtRight(painter);
					break;
				case CLAM::VM::eBottom:
					drawAtBottom(painter);
					break;
				case CLAM::VM::eTop:
					drawAtTop(painter);
					break;
			}
		}

		void Ruler::drawAtLeft(QPainter & painter)
		{
			const double axisPos = width()-1;
			double majorTickSize = 8.;
			double minorTickSize = 3.;
			QVector<QLineF> lines;
			for (unsigned i=0; i<_majorTicks.nTicks(); i++)
			{
				double tickValue = _majorTicks.tickValue(i);
				double tickPos = height()-_majorTicks.toPixel(tickValue);
				lines << QLineF(axisPos-majorTickSize,tickPos,axisPos,tickPos);
				int label_width = fontMetrics().width(QString::number(tickValue,'f',mShowFloats?2:0));
				double x = std::max(0,width() - label_width-8);
				double y = tickPos + mLabelHeight/4;
				painter.drawText(
					QRectF(x,y-mLabelHeight, label_width, mLabelHeight),
					GetLabel(tickValue));
			}
			for (unsigned i=0; i<_minorTicks.nTicks(); i++)
			{
				double tickValue = _minorTicks.tickValue(i);
				double tickPos = height()-_minorTicks.toPixel(tickValue);
				lines << QLineF(axisPos-minorTickSize,tickPos,axisPos,tickPos);
			}
			// draw axis
			lines << QLineF(axisPos,0,axisPos,height());
			painter.drawLines(lines);
		}

		void Ruler::drawAtRight(QPainter & painter)
		{
			const double axisPos = 1;
			double majorTickSize = 8.;
			double minorTickSize = 3.;
			QVector<QLineF> lines;
			for (unsigned i=0; i<_majorTicks.nTicks(); i++)
			{
				double tickValue = _majorTicks.tickValue(i);
				double tickPos = height()-_majorTicks.toPixel(tickValue);
				lines << QLineF(axisPos,tickPos,axisPos+majorTickSize,tickPos);
				int label_width = fontMetrics().width(QString::number(tickValue,'f',mShowFloats?2:0));
				painter.drawText(
					QRectF(axisPos+majorTickSize, tickPos -3.*mLabelHeight/4, label_width, mLabelHeight),
					GetLabel(tickValue));
			}
			for (unsigned i=0; i<_minorTicks.nTicks(); i++)
			{
				double tickValue = _minorTicks.tickValue(i);
				double tickPos = height()-_minorTicks.toPixel(tickValue);
				lines << QLineF(axisPos,tickPos,axisPos+minorTickSize,tickPos);
			}
			lines << QLineF(axisPos,0,axisPos,height());
			painter.drawLines(lines);
		}
	
		void Ruler::drawAtBottom(QPainter & painter)
		{
			const double axisPos = 1;
			const double minorTickSize = 9;
			const int margin=3;
			double labelWidth = _majorTicks.markGap()*width()/mCurrentRange.Span() -2*margin;
			QVector<QLineF> lines;
			for (unsigned i=0; i<_majorTicks.nTicks(); i++)
			{
				double tickValue = _majorTicks.tickValue(i);
				double tickPos = _majorTicks.toPixel(tickValue);
				lines << QLineF(tickPos,height()/2,tickPos,axisPos);
				painter.drawText(
					QRectF(
						tickPos + margin, axisPos+(height()-mLabelHeight)/2,
						labelWidth, mLabelHeight), 
					GetLabel(tickValue));
			}
			for (unsigned i=0; i<_minorTicks.nTicks(); i++)
			{
				double tickValue = _minorTicks.tickValue(i);
				double tickPos = _minorTicks.toPixel(tickValue);
				lines << QLineF(tickPos,axisPos,tickPos,axisPos+minorTickSize);
			}
			// draw axis
			lines << QLineF(0,axisPos,width(),axisPos);
			painter.drawLines(lines);
		}

		void Ruler::drawAtTop(QPainter & painter)
		{
			const double axisPos = height()-1;
			const double minorTickSize = 9;
			const int margin=3;
			double labelWidth = _majorTicks.markGap()*width()/mCurrentRange.Span() -2*margin;
			QVector<QLineF> lines;
			for (unsigned i=0; i<_majorTicks.nTicks(); i++)
			{
				double tickValue = _majorTicks.tickValue(i);
				double tickPos = _majorTicks.toPixel(tickValue);
				lines << QLineF(tickPos,height()/2,tickPos,axisPos);
				painter.drawText(
					QRectF(
						tickPos + margin, (height()-mLabelHeight)/2,
						labelWidth, mLabelHeight), 
					GetLabel(tickValue));
			}
			for (unsigned i=0; i<_minorTicks.nTicks(); i++)
			{
				double tickValue = _minorTicks.tickValue(i);
				double tickPos = _minorTicks.toPixel(tickValue);
				lines << QLineF(tickPos,axisPos-minorTickSize,tickPos,axisPos);
			}
			// draw axis
			lines << QLineF(0,axisPos,width(),axisPos);
			painter.drawLines(lines);
		}

		int Ruler::GetMaxLabelWidth()
		{
			if(mScale==CLAM::VM::eLogScale) return fontMetrics().width("-00.0e+00");
			// TODO: Should take the whole range (not just displayed) 
			// so that the ruler does not change size when scrolling.
			return std::max(
				fontMetrics().width(GetLabel(mCurrentRange.min)),
				fontMetrics().width(GetLabel(mCurrentRange.max)));
		}

		QString Ruler::GetLabel(double value)
		{
			if(mScale==CLAM::VM::eLinearScale)
				return QString::number(value,'f',mShowFloats?2:0);
			return QString::number(value,'e',1);
		}

	}
}

// END

