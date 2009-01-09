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
			, mFont(font())
			, mPosition(pos)
			, mScale(scale)
		{
//			mFont.setFamily("fixed");
//			mFont.setPointSize(8);
//			mFont.setBold(false);
//			mFont.setStyleHint(QFont::Courier);

			QFontMetrics font_metrics(mFont);
			mLabelHeight = font_metrics.height();

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

		const QFont& Ruler::GetFont() const
		{
			return mFont;
		}

		void Ruler::updateRange(double min, double max)
		{
			if((mCurrentRange.min==min) && (mCurrentRange.max==max)) return;
			if(min >= max) return;
			mCurrentRange.min = min;
			mCurrentRange.max = max;
			Rebuild();
			update();
			emit valueChanged(mCurrentRange.min,mCurrentRange.max);
		}
	
		void Ruler::paintEvent(QPaintEvent * event)
		{
			Rebuild();
			Draw(); 
		}

		void Ruler::Rebuild()
		{
			mShowFloats = mCurrentRange.Span()<20;
			mValuesToDraw.clear();
			bool vertical = (mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight);
			int size = vertical? height() : width();
			int labelSpan = 20 + (vertical? mLabelHeight*2: GetMaxLabelWidth()); 
			_majorTicks.setWidth(size);
			_majorTicks.setMinGap(labelSpan);
			_majorTicks.setRange(mCurrentRange.min,mCurrentRange.max);
			_minorTicks.setWidth(size);
			_minorTicks.setMinGap(std::max(10.,_majorTicks.markGap()*.1*size/mCurrentRange.Span()));
			_minorTicks.setRange(mCurrentRange.min,mCurrentRange.max);
			double markGap = _majorTicks.markGap();
			for (double tick = _majorTicks.markOffset();
				tick<mCurrentRange.max;
				tick+=markGap)
			{
				mValuesToDraw.push_back(tick);
			}
		}

		void Ruler::Draw()
		{
			QPainter painter(this);
			painter.setPen(QPen(mFgColor, 1));
//			painter.setFont(mFont);
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
					DrawLeft(painter);
					break;
				case CLAM::VM::eRight:
					DrawRight(painter);
					break;
				case CLAM::VM::eBottom:
					DrawBottom(painter);
					break;
				case CLAM::VM::eTop:
					DrawTop(painter);
					break;
			}
		}

		void Ruler::DrawLeft(QPainter & painter)
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
				QFontMetrics font_metrics(mFont);
				int label_width = font_metrics.width(QString::number(tickValue,'f',mShowFloats?2:0));
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

		void Ruler::DrawRight(QPainter & painter)
		{
			double x0 = 1.0;
			double x1 = 5.0;
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				glVertex2d(x0,mValuesToDraw[i]);
				glVertex2d(x1,mValuesToDraw[i]);
			}
			// drqw axis
			glVertex2d(x0,mCurrentRange.min);
			glVertex2d(x0,mCurrentRange.max);
			glEnd();		 
			DrawLabels(painter);
		}
	
		void Ruler::DrawBottom(QPainter & painter)
		{
			double y0 = double(height())-6.0;
			double y1 = double(height())-1.0;
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				glVertex2d(mValuesToDraw[i],y0);
				glVertex2d(mValuesToDraw[i],y1);
			}
			// drqw axis
			glVertex2d(mCurrentRange.min,y1);
			glVertex2d(mCurrentRange.max,y1);
			glEnd();			
			DrawLabels(painter);
		}

		void Ruler::DrawTop(QPainter & painter)
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

		void Ruler::DrawLabels(QPainter & painter)
		{
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				painter.drawText(
					GetLabelCoords(mValuesToDraw[i]),
					GetLabel(mValuesToDraw[i]));
			}
		}

		int Ruler::GetMaxLabelWidth()
		{
			QFontMetrics fm(mFont);
			if(mScale==CLAM::VM::eLogScale) return fm.width("-00.0e+00");
			return 8*fm.width('0');
		}

		bool Ruler::IsVisible(double value)
		{
			bool visible = false;
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
				case CLAM::VM::eRight:
				{
					double margin = double(mLabelHeight)*mCurrentRange.Span()/double(height());
					if(value+margin < mCurrentRange.max) visible = true;
				}
				break;
				case CLAM::VM::eBottom:
				case CLAM::VM::eTop:
				{
					QFontMetrics fm(mFont);
					int label_width = (mScale==CLAM::VM::eLogScale)
						? fm.width("-0.0e+00")
						: fm.width(QString::number(value,'f',mShowFloats?2:0));
					double margin = double(label_width/2)*mCurrentRange.Span()/double(width());
					if(value+margin < mCurrentRange.max) visible = true;
				}
				break;
				default:
					break;
			}
			return visible;
		}

		QString Ruler::GetLabel(double value)
		{
			if(mScale==CLAM::VM::eLinearScale)
				return QString::number(value,'f',mShowFloats?2:0);
			return QString::number(value,'e',1);
		}

		QPointF Ruler::GetLabelCoords(double value)
		{
			QFontMetrics font_metrics(mFont);
			int label_width = font_metrics.width(QString::number(value,'f',mShowFloats?2:0));
			double x=0.0, y=0.0;
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
				{
					
					double tickPos = height() - (value-mCurrentRange.min)*height()/mCurrentRange.Span();
					x = std::max(0,width() - label_width-8);
					y = tickPos + mLabelHeight/4;
				}
				break;
				case CLAM::VM::eRight:
				{
					double tickPos = height() - (value-mCurrentRange.min)*height()/mCurrentRange.Span();
					x = 8;
					y = tickPos + mLabelHeight/4;
				}
				break;
				case CLAM::VM::eBottom:
				{
					double tickPos = (value-mCurrentRange.min)*width()/mCurrentRange.Span();
					x = tickPos + 3;
					y = height() - 3;
				}
				break;
				case CLAM::VM::eTop:
				{
					double tickPos = (value-mCurrentRange.min)*width()/mCurrentRange.Span();
					x = tickPos + 3;
					y = mLabelHeight + 3;
				}
				break;
				default:
					break;
			}
			return QPointF(x,y);
		}
	}
}

// END

