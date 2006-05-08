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

namespace CLAM
{
	namespace VM
	{
		Ruler::Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale)
			: QGLWidget(parent)
			, mDoResize(false)
			, mShowFloats(true)
			, mStep(0.0)
			, mPosition(pos)
			, mScale(scale)
		{
			setAutoBufferSwap(false);

			mFont.setFamily("fixed");
			mFont.setPointSize(9);
			mFont.setBold(true);
			
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

		void Ruler::SetStep(double step)
		{
			if(step < 0) return;
			mStep = step;
			Rebuild();
			updateGL();
		}

		void Ruler::SetScale(ERulerScale scale)
		{
			mScale = scale;
			updateGL();
		}

		ERulerScale Ruler::GetScale() const
		{
			return mScale;
		}

		void Ruler::SetBackgroundColor(const Color& c)
		{
			mBgColor = c;
			updateGL();
		}

		void Ruler::SetForegroundColor(const Color& c)
		{
			mFgColor = c;
			updateGL();
		}

		void Ruler::ShowFloats()
		{
			mShowFloats = true;
			Rebuild();
			updateGL();
		}

		void Ruler::ShowIntegers()
		{
			mShowFloats = false;
			Rebuild();
			updateGL();
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
			updateGL();
			emit valueChanged(mCurrentRange.min,mCurrentRange.max);
		}
	
		void Ruler::paintGL()
		{
			if(mDoResize)
			{
				glViewport(0,0,mViewport.w,mViewport.h);
				mDoResize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			(mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight)
				? glOrtho(double(mViewport.x),double(mViewport.w),mCurrentRange.min,mCurrentRange.max,-1.0,1.0)
				: glOrtho(mCurrentRange.min,mCurrentRange.max,double(mViewport.y),double(mViewport.h),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(double(mBgColor.r)/255.0, 
						 double(mBgColor.g)/255.0, 
						 double(mBgColor.b)/255.0, 
						 double(mBgColor.a)/255.0);
			glClear(GL_COLOR_BUFFER_BIT);
			Draw(); 
			swapBuffers();
		}

		void Ruler::resizeEvent(QResizeEvent* e)
		{
			mViewport.x = 0;
			mViewport.y = 0;
			mViewport.w = e->size().width();
			mViewport.h = e->size().height();
			Rebuild();
			mDoResize = true;
		}

		void Ruler::Rebuild()
		{
			mValuesToDraw.clear();
			int nTicks = GetTicks();
			if(nTicks <= 0) return;
			if(mStep)
			{
				double div = Round(mCurrentRange.Span()/mStep);
				if(!div) return;
				int tick;
				double value;
				// first tick
				if(mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight)
				{
					tick = mViewport.h/(nTicks-1);
					value =  double(tick)*mCurrentRange.Span()/double(mViewport.h)+mCurrentRange.min;
				}
				else
				{
					tick = mViewport.w/(nTicks-1);
					value = double(tick)*mCurrentRange.Span()/double(mViewport.w)+mCurrentRange.min;;
				}
				mValuesToDraw.push_back(Round((value-mTotalRange.min)/mStep)*mStep+mTotalRange.min);
				if(nTicks == 1) return;
				double step = Ceil(div/double(nTicks-1))*mStep;
				for(int i=1; i < nTicks-1; i++)
				{
					value = mValuesToDraw[0]+double(i)*step;
					if(!IsVisible(value)) break;
					mValuesToDraw.push_back(value);
				}
			}
			else
			{
				for(int i=1; i < nTicks-1; i++)
				{
					int tick;
					double value;
					if(mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight)
					{
						tick = i*(mViewport.h/(nTicks-1));
						value = double(tick)*mCurrentRange.Span()/double(mViewport.h)+mCurrentRange.min;
					}
					else
					{
						tick = i*(mViewport.w/(nTicks-1));
						value = double(tick)*mCurrentRange.Span()/double(mViewport.w)+mCurrentRange.min;;
					}
					mValuesToDraw.push_back(value);
				}
			}
		}

		void Ruler::Draw()
		{
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
					DrawLeft();
					break;
				case CLAM::VM::eRight:
					DrawRight();
					break;
				case CLAM::VM::eBottom:
					DrawBottom();
					break;
				case CLAM::VM::eTop:
					DrawTop();
					break;
				default:
					break;
			}
		}

		void Ruler::DrawLeft()
		{
			double x0 = double(mViewport.w)-5.0;
			double x1 = double(mViewport.w)-1.0;
			glLineWidth(1);
			glColor3ub(mFgColor.r,mFgColor.g,mFgColor.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				glVertex2d(x0,mValuesToDraw[i]);
				glVertex2d(x1,mValuesToDraw[i]);
			}
			// drqw axis
			glVertex2d(x1,mCurrentRange.min);
			glVertex2d(x1,mCurrentRange.max);
			glEnd();
			DrawLabels();
		}

		void Ruler::DrawRight()
		{
			double x0 = 0.0;
			double x1 = 5.0;
			glLineWidth(1);
			glColor3ub(mFgColor.r,mFgColor.g,mFgColor.b);
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
			DrawLabels();
		}
	
		void Ruler::DrawBottom()
		{
			double y0 = double(mViewport.h)-6.0;
			double y1 = double(mViewport.h)-1.0;
			glLineWidth(1);
			glColor3ub(mFgColor.r,mFgColor.g,mFgColor.b);
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
			DrawLabels();
		}

		void Ruler::DrawTop()
		{
			double y0 = 0.0;
			double y1 = 5.0;
			glLineWidth(1);
			glColor3ub(mFgColor.r,mFgColor.g,mFgColor.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				glVertex2d(mValuesToDraw[i],y0);
				glVertex2d(mValuesToDraw[i],y1);
			}
			// drqw axis
			glVertex2d(mCurrentRange.min,y0);
			glVertex2d(mCurrentRange.max,y0);
			glEnd();		   
			DrawLabels();
		}

		void Ruler::DrawLabels()
		{
			glColor3ub(mFgColor.r,mFgColor.g,mFgColor.b);
			for(unsigned i=0; i < mValuesToDraw.size(); i++)
			{
				std::pair<double,double> coords = GetLabelCoords(mValuesToDraw[i]);
				renderText(coords.first,coords.second,-1.0,GetLabel(mValuesToDraw[i]),mFont);
			}
		}

		int Ruler::GetTicks()
		{
			int nTicks = TestTicksForSpan();
			if(nTicks) return nTicks;
			int label_span = (mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight) 
				? mLabelHeight+6
				: GetMaxLabelWidth()+6;
			nTicks = (mPosition==CLAM::VM::eLeft || mPosition==CLAM::VM::eRight) 
				? mViewport.h/label_span
				: mViewport.w/label_span;
			return nTicks;
		}

		int Ruler::TestTicksForSpan()
		{
			int nTicks = 0;
			if(mCurrentRange.Span() < 0.05) nTicks =  MINTICKS; 
			return nTicks;
		}

		int Ruler::GetMaxLabelWidth()
		{
			QFontMetrics fm(mFont);
			if(mScale==CLAM::VM::eLogScale) return fm.width("-0.0e+00");
			if(mShowFloats)
			{
				int length_min = QString::number(mCurrentRange.min,'f',2).length();
				int length_max = QString::number(mCurrentRange.max,'f',2).length();
				int length = (length_min > length_max)
					? fm.width(QString::number(mCurrentRange.min,'f',2))
					: fm.width(QString::number(mCurrentRange.max,'f',2));
				return length;
			}
			else
			{
				int length_min = QString::number(mCurrentRange.min,'f',0).length();
				int length_max = QString::number(mCurrentRange.max,'f',0).length();
				int length = (length_min > length_max)
					? fm.width(QString::number(mCurrentRange.min,'f',0))
					: fm.width(QString::number(mCurrentRange.max,'f',0));
				return length;
			}
		}

		bool Ruler::IsVisible(double value)
		{
			bool visible = false;
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
				case CLAM::VM::eRight:
				{
					double margin = double(mLabelHeight)*mCurrentRange.Span()/double(mViewport.h);
					if(value+margin < mCurrentRange.max) visible = true;
				}
				break;
				case CLAM::VM::eBottom:
				case CLAM::VM::eTop:
				{
					QFontMetrics fm(mFont);
					int label_width = (mScale==CLAM::VM::eLogScale)
						? fm.width("-0.0e+00")
						: (mShowFloats) 
						? fm.width(QString::number(value,'f',2)) 
						: fm.width(QString::number(value,'f',0));
					double margin = double(label_width/2)*mCurrentRange.Span()/double(mViewport.w);
					if(value+margin < mCurrentRange.max) visible = true;
				}
				break;
				default:
					break;
			}
			return visible;
		}

		double Ruler::Round(double x) 
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac >= 0.5) ? i+1.0 : i;
		}
	
		double Ruler::Ceil(double x)
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac) ? i+1.0 : i;
		}

		QString Ruler::GetLabel(double value)
		{
			if(mScale==CLAM::VM::eLinearScale)
			{
				if(mShowFloats) 
				{
					return QString::number(value,'f',2);
				}
				else
				{
					return QString::number(value,'f',0);
				}
			}
			return QString::number(value,'e',1);
		}

		std::pair<double,double> Ruler::GetLabelCoords(double value)
		{
			QFontMetrics font_metrics(mFont);
			int label_width = (mShowFloats) 
				? font_metrics.width(QString::number(value,'f',2))
				: font_metrics.width(QString::number(value,'f',0));
			double x=0.0, y=0.0;
			switch(mPosition)
			{
				case CLAM::VM::eLeft:
				{
					double adjust = double(mLabelHeight/4)*mCurrentRange.Span()/double(mViewport.h);
					x = mViewport.w - label_width-8;
					y = value - adjust;
				}
				break;
				case CLAM::VM::eRight:
				{
					double adjust = double(mLabelHeight/4)*mCurrentRange.Span()/double(mViewport.h);
					x = 8;
					y = value - adjust;
				}
				break;
				case CLAM::VM::eBottom:
				{
					double adjust = double(label_width/2)*mCurrentRange.Span()/double(mViewport.w);
					x = value - adjust;
					y = mViewport.h-(mLabelHeight/2+8);
				}
				break;
				case CLAM::VM::eTop:
				{
					double adjust = double(label_width/2)*mCurrentRange.Span()/double(mViewport.w);
					x = value - adjust;
					y = 8;
				}
				break;
				default:
					break;
			}
			return std::make_pair(x,y);
		}
	}
}

// END

