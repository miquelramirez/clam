#include <QPainter>
#include "CLAM_Math.hxx"
#include "vm_ruler.hxx"

namespace CLAM
{
	namespace VM
	{
		Ruler::Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale)
			: QWidget(parent) 
			  , rw_position(pos)
			  , rw_scale(scale)
			  , rw_range(-1.0,1.0)
		{
			rw_font.setFamily("fixed");
			rw_font.setPointSize(9);
			rw_font.setBold(true);
			
			QFontMetrics font_metrics(rw_font);
			rw_label_width = font_metrics.width("9.99");
			rw_label_height = font_metrics.height();

			updateRange(0.0,1.0);
			setFocusPolicy(Qt::NoFocus);
		}

		Ruler::~Ruler()
		{
		}

		void Ruler::set_scale(ERulerScale scale)
		{
			rw_scale = scale;
		}

		ERulerScale Ruler::get_scale() const
		{
			return rw_scale;
		}

		void Ruler::set_bg_color(const Color& c)
		{
			setPalette(QPalette(QColor(int(c.r),int(c.g),int(c.b))));
		}

		void Ruler::set_fg_color(const Color& c)
		{
			rw_fg_color = c;
		}

		void Ruler::updateRange(double min, double max)
		{
			if(min==rw_range.min && max==rw_range.max) return;
			if(min >= max) return;
			rw_range.min = min;
			rw_range.max = max;
			compute_max_label_width();
			update();
			emit valueChanged(rw_range.min,rw_range.max);
		}
	
		void Ruler::paintEvent(QPaintEvent* pe)
		{
			switch(rw_position)
			{
				case CLAM::VM::eLeft:
					draw_left();
					break;
				case CLAM::VM::eRight:
					draw_right();
					break;
				case CLAM::VM::eBottom:
					draw_bottom();
					break;
				case CLAM::VM::eTop:
					draw_top();
					break;
				default:
					break;
			}
		}

		void Ruler::draw_left()
		{
			rw_ticks = get_ticks();
			int r = int(rw_fg_color.r);
			int g = int(rw_fg_color.g);
			int b = int(rw_fg_color.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(rw_font);

			int x0 = rect().right()-5;
			int x1 = rect().right();
			int y0 = rect().bottom();
			int y1;

			for(int i=1; i < rw_ticks-1; i++)
			{
				y1 = y0 - i*(height()/(rw_ticks-1));
				painter.drawLine(x0,y1,x1,y1); // draw ticks
				int y = y1 - rw_label_height/2;
				painter.drawText(rect().left()+2, y, rect().right()-8, rw_label_height,
								 Qt::AlignRight | Qt::AlignVCenter, get_label(i));  // draw text
			}
			painter.drawLine(x1,y0,x1,rect().top()); // draw axis
		}

		void Ruler::draw_right()
		{
			rw_ticks = get_ticks();
			int r = int(rw_fg_color.r);
			int g = int(rw_fg_color.g);
			int b = int(rw_fg_color.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(rw_font);

			int x0 = rect().left();
			int x1 = rect().left()+5;
			int y0 = rect().bottom();
			int y1;

			for(int i=1; i < rw_ticks-1; i++)
			{
				y1 = y0 - i*(height()/(rw_ticks-1));
				painter.drawLine(x0,y1,x1,y1);
				int y = y1 - rw_label_height/2;
				painter.drawText(rect().left()+8, y, rect().right()-2, rw_label_height,
								 Qt::AlignLeft | Qt::AlignVCenter, get_label(i));
			}
			painter.drawLine(x0,y0,x0,rect().top());
		}
	
		void Ruler::draw_bottom()
		{
			rw_ticks = get_ticks();
			int r = int(rw_fg_color.r);
			int g = int(rw_fg_color.g);
			int b = int(rw_fg_color.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(rw_font);

			int x0 = rect().left();
			int x1;
			int y0 = rect().top()+5;
			int y1 = rect().top();

			for(int i=1; i < rw_ticks; i++)
			{
				x1 = x0 + i*(width()/(rw_ticks-1));
				if(x1 >= rect().right()-rw_label_width/2) break;
				painter.drawLine(x1,y0,x1,y1);
				int x = x1-rw_label_width/2;
				painter.drawText(x, y0+2, rw_label_width, rw_label_height,
								 Qt::AlignHCenter | Qt::AlignTop, get_label(i));
			}
			painter.drawLine(x0,y1,rect().right(),y1);
		}

		void Ruler::draw_top()
		{
			rw_ticks = get_ticks();
			int r = int(rw_fg_color.r);
			int g = int(rw_fg_color.g);
			int b = int(rw_fg_color.b);
	    
			QColor c(r,g,b);
			QPen pen(c);

			QPainter painter(this);
			painter.setPen(pen);
			painter.setFont(rw_font);

			int x0 = rect().left();
			int x1;
			int y0 = rect().bottom();
			int y1 = rect().bottom()-5;

			for(int i=1; i < rw_ticks; i++)
			{
				x1 = x0 + i*(width()/(rw_ticks-1));
				if(x1 >= rect().right()-rw_label_width/2) break;
				painter.drawLine(x1,y0,x1,y1);
				int x = x1-rw_label_width/2;
				painter.drawText(x, y1-rw_label_height-2, rw_label_width, rw_label_height,
								 Qt::AlignHCenter | Qt::AlignBottom, get_label(i));
			}
			painter.drawLine(x0,y0,rect().right(),y0);
		}

		int Ruler::get_ticks()
		{
			int nTicks = test_ticks_for_span();
			if(nTicks) return nTicks;
			switch(rw_position)
			{
				case CLAM::VM::eLeft:
				case CLAM::VM::eRight:
					nTicks = height()/(rw_label_height*2)+1;
					break;
				case CLAM::VM::eBottom:
				case CLAM::VM::eTop:
					nTicks = width()/rw_label_width+1;
					break;
				default:
					break;
			}
			if(nTicks < MINTICKS) nTicks = MINTICKS;
			if(nTicks > MAXTICKS) nTicks = MAXTICKS;
			if(rw_range.span() < 0.1) 
			{
				if(nTicks > 5) nTicks = 5;
			}
			else if(rw_range.span() <= 0.2) 
			{
				if(nTicks > 11) nTicks = 11;
			}
			return nTicks;
		}

		QString Ruler::get_label(int tick)
		{
			double value = rw_range.min + (double(tick)*rw_range.span())/double(rw_ticks-1);
			int tmp = int(value);
			double frac = value-double(tmp);
			QString label("");
			switch(rw_scale)
			{
				case CLAM::VM::eLinearScale:	   
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
				case CLAM::VM::eLogScale:
					label = QString::number(value,'e',1);
					break;
				default:
					break;
			}
			return label;
		}

		int Ruler::test_ticks_for_span()
		{
			int nTicks = 0;
			if(rw_range.span() < 0.05) nTicks =  MINTICKS; 
			return nTicks;
		}

		void Ruler::compute_max_label_width()
		{
			int length_min = QString::number(rw_range.min,'f',2).length();
			int length_max = QString::number(rw_range.max,'f',2).length();

			QFontMetrics font_metrics(rw_font);
	    
			if(length_min > length_max)
			{
				if(rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight)
				{
					rw_label_width = font_metrics.width(QString::number(rw_range.min,'f',2)+"9");
				}
		
				if(rw_position==CLAM::VM::eTop || rw_position==CLAM::VM::eBottom)
				{
					rw_label_width = font_metrics.width(QString::number(rw_range.min,'f',2)+"99");
				}
		
			}
			else
			{
				if(rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight)
				{
					rw_label_width = font_metrics.width(QString::number(rw_range.max,'f',2)+"9");
				}

				if(rw_position==CLAM::VM::eTop || rw_position==CLAM::VM::eBottom)
				{
					rw_label_width = font_metrics.width(QString::number(rw_range.max,'f',2)+"99");
				}	
			}
		}

		const QFont& Ruler::get_font() const
		{
			return rw_font;
		}
	}
}

// END

