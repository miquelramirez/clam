#include <qpainter.h>
#include <CLAM/Ruler.hxx>
#include <CLAM/CLAM_Math.hxx>

namespace CLAM
{
    namespace VM
    {
	
		Ruler::Ruler(QWidget* parent, const Position& pos, const EScale& scale)
			: QWidget(parent) 
			, mPosition(pos)
			, mScale(scale)
			, mMin(0)
			, mMax(100)
		{
			mFont.setFamily("fixed");
			mFont.setPointSize(10);
			mFont.setBold(true);
			mFont.setStyleHint(QFont::Courier);

			QFontMetrics font_metrics(mFont);
			mMaxLabelWidth = font_metrics.width("9.99");
			mLabelHeight = font_metrics.height();

			updateRange(0.0,1.0);
			setFocusPolicy(QWidget::NoFocus);
		}

		Ruler::~Ruler()
		{
		}

		void Ruler::SetScale(const EScale& scale)
		{
			mScale = scale;
		}

		const EScale& Ruler::GetScale() const
		{
			return mScale;
		}

		void Ruler::SetBackgroundColor(const Color& c)
		{
			setPaletteBackgroundColor(QColor(int(c.r),int(c.g),int(c.b)));
		}

		void Ruler::SetForegroundColor(const Color& c)
		{
			mForegroundColor=c;
		}

		void Ruler::updateRange(double min, double max)
		{
			if(min==mMin && max==mMax) return;
			if(min >= max) return;
			mMin=min;
			mMax=max;
			mSpan=mMax-mMin;
			ComputeMaxLabelWidth();
			update();
			emit valueChanged(mMin,mMax);
		}
	
		void Ruler::paintEvent(QPaintEvent* pe)
		{
			switch(mPosition)
			{
				case CLAM::VM::Left:
					DrawLeft();
					break;
				case CLAM::VM::Right:
					DrawRight();
					break;
				case CLAM::VM::Bottom:
					DrawBottom();
					break;
				case CLAM::VM::Top:
					DrawTop();
					break;
				default:
					break;
			}
		}

		void Ruler::DrawLeft()
		{
			mTicks = GetnTicks();
			int r = int(mForegroundColor.r);
			int g = int(mForegroundColor.g);
			int b = int(mForegroundColor.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(mFont);

			int x0 = rect().right()-8;
			int x1 = rect().right()-3;
			int y0 = rect().bottom();
			int y1;

			for(int i=1; i < mTicks-1; i++)
			{
				y1 = y0 - i*(height()/(mTicks-1));
				painter.drawLine(x0,y1,x1,y1); // draw ticks
				int y = y1 - mLabelHeight/2;
				painter.drawText(rect().left()+2, y, rect().right()-10, mLabelHeight,
								 AlignRight | AlignVCenter, GetLabel(i));  // draw text
			}
			painter.drawLine(x1,y0,x1,rect().top()); // draw axis
		}

		void Ruler::DrawRight()
		{
			mTicks = GetnTicks();
			int r = int(mForegroundColor.r);
			int g = int(mForegroundColor.g);
			int b = int(mForegroundColor.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(mFont);

			int x0 = rect().left()+3;
			int x1 = rect().left()+8;
			int y0 = rect().bottom();
			int y1;

			for(int i=1; i < mTicks-1; i++)
			{
				y1 = y0 - i*(height()/(mTicks-1));
				painter.drawLine(x0,y1,x1,y1);
				int y = y1-mLabelHeight/2;
				painter.drawText(rect().left()+10, y, rect().right()-10, mLabelHeight,
								 AlignLeft | AlignVCenter, GetLabel(i));
			}
			painter.drawLine(x0,y0,x0,rect().top());
		}
	
		void Ruler::DrawBottom()
		{
			mTicks = GetnTicks();
			int r = int(mForegroundColor.r);
			int g = int(mForegroundColor.g);
			int b = int(mForegroundColor.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(mFont);

			int x0 = rect().left();
			int x1;
			int y0 = rect().top()+8;
			int y1 = rect().top()+3;

			for(int i=1; i < mTicks; i++)
			{
				x1 = x0 + i*(width()/(mTicks-1));
				if(x1 >= rect().right()-mMaxLabelWidth/2) break;
				painter.drawLine(x1,y0,x1,y1);
				int x = x1-mMaxLabelWidth/2;
				painter.drawText(x, y0+2, mMaxLabelWidth, mLabelHeight,
								 AlignHCenter | AlignTop, GetLabel(i));
			}
			painter.drawLine(x0,y1,rect().right(),y1);
		}

		void Ruler::DrawTop()
		{
			mTicks = GetnTicks();
			int r = int(mForegroundColor.r);
			int g = int(mForegroundColor.g);
			int b = int(mForegroundColor.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(mFont);

			int x0 = rect().left();
			int x1;
			int y0 = rect().bottom()-3;
			int y1 = rect().bottom()-8;

			for(int i=1; i < mTicks; i++)
			{
				x1 = x0 + i*(width()/(mTicks-1));
				if(x1 >= rect().right()-mMaxLabelWidth/2) break;
				painter.drawLine(x1,y0,x1,y1);
				int x = x1-mMaxLabelWidth/2;
				painter.drawText(x, y1-mLabelHeight-2, mMaxLabelWidth, mLabelHeight,
								 AlignHCenter | AlignBottom, GetLabel(i));
			}
			painter.drawLine(x0,y0,rect().right(),y0);
		}

		int Ruler::GetnTicks()
		{
			int nTicks = TestnTicksForSpan();
			if(nTicks) return nTicks;
			switch(mPosition)
			{
				case CLAM::VM::Left:
				case CLAM::VM::Right:
					nTicks = height()/(mLabelHeight*2)+1;
					break;
				case CLAM::VM::Bottom:
				case CLAM::VM::Top:
					nTicks = width()/mMaxLabelWidth+1;
					break;
				default:
					break;
			}
			if(nTicks < MINTICKS) nTicks = MINTICKS;
			if(nTicks > MAXTICKS) nTicks = MAXTICKS;
			if(mSpan < 0.1) 
			{
				if(nTicks > 5) nTicks = 5;
			}
			else if(mSpan <= 0.2) 
			{
				if(nTicks > 11) nTicks = 11;
			}
			return nTicks;
		}

		QString Ruler::GetLabel(int tick)
		{
			double value = mMin + (double(tick)*mSpan)/double(mTicks-1);
			int tmp = int(value);
			double frac = value-double(tmp);
			QString label("");
			switch(mScale)
			{
				case EScale::eLinear:	   
					if(value)
					{ 
						if(!tmp)
						{
							if(fabs(frac) < 1E-2) value = 0;
						} 
						label =  QString::number(value,'f',2);
					}
					else
					{
						label =  QString::number(value,'f',0);
					}
					break;
				case EScale::eLog:
					label = QString::number(value,'e',1);
					break;
				default:
					break;
			}
			return label;
		}

		int Ruler::TestnTicksForSpan()
		{
			int nTicks = 0;
			if(mSpan < 0.05) nTicks =  MINTICKS; 
			return nTicks;
		}

		void Ruler::ComputeMaxLabelWidth()
		{
			int length_min = QString::number(mMin,'f',2).length();
			int length_max = QString::number(mMax,'f',2).length();

			QFontMetrics font_metrics(mFont);
	    
			if(length_min > length_max)
			{
				if(mPosition==CLAM::VM::Left || mPosition==CLAM::VM::Right)
				{
					mMaxLabelWidth = font_metrics.width(QString::number(mMin,'f',2)+"9");
				}
		
				if(mPosition==CLAM::VM::Top || mPosition==CLAM::VM::Bottom)
				{
					mMaxLabelWidth = font_metrics.width(QString::number(mMin,'f',2)+"99");
				}
		
			}
			else
			{
				if(mPosition==CLAM::VM::Left || mPosition==CLAM::VM::Right)
				{
					mMaxLabelWidth = font_metrics.width(QString::number(mMax,'f',2)+"9");
				}

				if(mPosition==CLAM::VM::Top || mPosition==CLAM::VM::Bottom)
				{
					mMaxLabelWidth = font_metrics.width(QString::number(mMax,'f',2)+"99");
				}	
			}
		}

		QFont& Ruler::Font()
		{
			return mFont;
		}

    }
}

// END

