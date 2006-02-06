#include <QResizeEvent>
#include "vm_ruler.hxx"

namespace CLAM
{
	namespace VM
	{
		Ruler::Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale)
			: QGLWidget(parent)
			, rw_do_resize(false)
			, rw_show_floats(true)
			, rw_step(0.0)
			, rw_position(pos)
			, rw_scale(scale)
		{
			setAutoBufferSwap(false);

			rw_font.setFamily("fixed");
			rw_font.setPointSize(9);
			rw_font.setBold(true);
			
			QFontMetrics font_metrics(rw_font);
			rw_label_height = font_metrics.height();

			set_range(0.0,1.0);
			setFocusPolicy(Qt::NoFocus);
		}

		Ruler::~Ruler()
		{
		}

		void Ruler::set_range(double min, double max)
		{
			if(min >= max) return;
			rw_total_range.min = min;
			rw_total_range.max = max;
			rw_current_range.min = rw_total_range.min-1.0;
			rw_current_range.max = rw_total_range.max+1.0;
			updateRange(rw_total_range.min,rw_total_range.max);
		}

		void Ruler::set_step(double step)
		{
			if(step < 0) return;
			rw_step = step;
			rebuild();
			updateGL();
		}

		void Ruler::set_scale(ERulerScale scale)
		{
			rw_scale = scale;
			updateGL();
		}

		ERulerScale Ruler::get_scale() const
		{
			return rw_scale;
		}

		void Ruler::set_bg_color(const Color& c)
		{
			rw_bg_color = c;
			updateGL();
		}

		void Ruler::set_fg_color(const Color& c)
		{
			rw_fg_color = c;
			updateGL();
		}

		void Ruler::show_floats()
		{
			rw_show_floats = true;
			rebuild();
			updateGL();
		}

		void Ruler::show_integers()
		{
			rw_show_floats = false;
			rebuild();
			updateGL();
		}

		const QFont& Ruler::get_font() const
		{
			return rw_font;
		}

		void Ruler::updateRange(double min, double max)
		{
			if((rw_current_range.min==min) && (rw_current_range.max==max)) return;
			if(min >= max) return;
			rw_current_range.min = min;
			rw_current_range.max = max;
			rebuild();
			updateGL();
			emit valueChanged(rw_current_range.min,rw_current_range.max);
		}
	
		void Ruler::paintGL()
		{
			if(rw_do_resize)
			{
				glViewport(0,0,rw_viewport.w,rw_viewport.h);
				rw_do_resize = false;
			}
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			(rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight)
				? glOrtho(double(rw_viewport.x),double(rw_viewport.w),rw_current_range.min,rw_current_range.max,-1.0,1.0)
				: glOrtho(rw_current_range.min,rw_current_range.max,double(rw_viewport.y),double(rw_viewport.h),-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glShadeModel(GL_FLAT);
			glClearColor(double(rw_bg_color.r)/255.0, 
						 double(rw_bg_color.g)/255.0, 
						 double(rw_bg_color.b)/255.0, 
						 double(rw_bg_color.a)/255.0);
			glClear(GL_COLOR_BUFFER_BIT);
			draw(); 
			swapBuffers();
		}

		void Ruler::resizeEvent(QResizeEvent* e)
		{
			rw_viewport.x = 0;
			rw_viewport.y = 0;
			rw_viewport.w = e->size().width();
			rw_viewport.h = e->size().height();
			rebuild();
			rw_do_resize = true;
		}

		void Ruler::rebuild()
		{
			rw_values_to_draw.clear();
			int nTicks = get_ticks();
			if(nTicks <= 0) return;
			if(rw_step)
			{
				double div = round(rw_current_range.span()/rw_step);
				if(!div) return;
				int tick;
				double value;
				// first tick
				if(rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight)
				{
					tick = rw_viewport.h/(nTicks-1);
					value =  double(tick)*rw_current_range.span()/double(rw_viewport.h)+rw_current_range.min;
				}
				else
				{
					tick = rw_viewport.w/(nTicks-1);
					value = double(tick)*rw_current_range.span()/double(rw_viewport.w)+rw_current_range.min;;
				}
				rw_values_to_draw.push_back(round((value-rw_total_range.min)/rw_step)*rw_step+rw_total_range.min);
				if(nTicks == 1) return;
				double step = ceil(div/double(nTicks-1))*rw_step;
				for(int i=1; i < nTicks-1; i++)
				{
					value = rw_values_to_draw[0]+double(i)*step;
					if(!is_visible(value)) break;
					rw_values_to_draw.push_back(value);
				}
			}
			else
			{
				for(int i=1; i < nTicks-1; i++)
				{
					int tick;
					double value;
					if(rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight)
					{
						tick = i*(rw_viewport.h/(nTicks-1));
						value = double(tick)*rw_current_range.span()/double(rw_viewport.h)+rw_current_range.min;
					}
					else
					{
						tick = i*(rw_viewport.w/(nTicks-1));
						value = double(tick)*rw_current_range.span()/double(rw_viewport.w)+rw_current_range.min;;
					}
					rw_values_to_draw.push_back(value);
				}
			}
		}

		void Ruler::draw()
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
			double x0 = double(rw_viewport.w)-5.0;
			double x1 = double(rw_viewport.w)-1.0;
			glLineWidth(1);
			glColor3ub(rw_fg_color.r,rw_fg_color.g,rw_fg_color.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < rw_values_to_draw.size(); i++)
			{
				glVertex2d(x0,rw_values_to_draw[i]);
				glVertex2d(x1,rw_values_to_draw[i]);
			}
			// drqw axis
			glVertex2d(x1,rw_current_range.min);
			glVertex2d(x1,rw_current_range.max);
			glEnd();
			draw_labels();
		}

		void Ruler::draw_right()
		{
			double x0 = 0.0;
			double x1 = 5.0;
			glLineWidth(1);
			glColor3ub(rw_fg_color.r,rw_fg_color.g,rw_fg_color.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < rw_values_to_draw.size(); i++)
			{
				glVertex2d(x0,rw_values_to_draw[i]);
				glVertex2d(x1,rw_values_to_draw[i]);
			}
			// drqw axis
			glVertex2d(x0,rw_current_range.min);
			glVertex2d(x0,rw_current_range.max);
			glEnd();		 
			draw_labels();
		}
	
		void Ruler::draw_bottom()
		{
			double y0 = double(rw_viewport.h)-6.0;
			double y1 = double(rw_viewport.h)-1.0;
			glLineWidth(1);
			glColor3ub(rw_fg_color.r,rw_fg_color.g,rw_fg_color.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < rw_values_to_draw.size(); i++)
			{
				glVertex2d(rw_values_to_draw[i],y0);
				glVertex2d(rw_values_to_draw[i],y1);
			}
			// drqw axis
			glVertex2d(rw_current_range.min,y1);
			glVertex2d(rw_current_range.max,y1);
			glEnd();			
			draw_labels();
		}

		void Ruler::draw_top()
		{
			double y0 = 0.0;
			double y1 = 5.0;
			glLineWidth(1);
			glColor3ub(rw_fg_color.r,rw_fg_color.g,rw_fg_color.b);
			glBegin(GL_LINES);
			// draw ticks
			for(unsigned i=0; i < rw_values_to_draw.size(); i++)
			{
				glVertex2d(rw_values_to_draw[i],y0);
				glVertex2d(rw_values_to_draw[i],y1);
			}
			// drqw axis
			glVertex2d(rw_current_range.min,y0);
			glVertex2d(rw_current_range.max,y0);
			glEnd();		   
			draw_labels();
		}

		void Ruler::draw_labels()
		{
			glColor3ub(rw_fg_color.r,rw_fg_color.g,rw_fg_color.b);
			for(unsigned i=0; i < rw_values_to_draw.size(); i++)
			{
				std::pair<double,double> coords = get_label_coords(rw_values_to_draw[i]);
				renderText(coords.first,coords.second,-1.0,get_label(rw_values_to_draw[i]),rw_font);
			}
		}

		int Ruler::get_ticks()
		{
			int nTicks = test_ticks_for_span();
			if(nTicks) return nTicks;
			int label_span = (rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight) 
				? rw_label_height+6
				: get_max_label_width()+6;
			nTicks = (rw_position==CLAM::VM::eLeft || rw_position==CLAM::VM::eRight) 
				? rw_viewport.h/label_span
				: rw_viewport.w/label_span;
			return nTicks;
		}

		int Ruler::test_ticks_for_span()
		{
			int nTicks = 0;
			if(rw_current_range.span() < 0.05) nTicks =  MINTICKS; 
			return nTicks;
		}

		int Ruler::get_max_label_width()
		{
			QFontMetrics fm(rw_font);
			if(rw_scale==CLAM::VM::eLogScale) return fm.width("-0.0e+00");
			if(rw_show_floats)
			{
				int length_min = QString::number(rw_current_range.min,'f',2).length();
				int length_max = QString::number(rw_current_range.max,'f',2).length();
				int length = (length_min > length_max)
					? fm.width(QString::number(rw_current_range.min,'f',2))
					: fm.width(QString::number(rw_current_range.max,'f',2));
				return length;
			}
			else
			{
				int length_min = QString::number(rw_current_range.min,'f',0).length();
				int length_max = QString::number(rw_current_range.max,'f',0).length();
				int length = (length_min > length_max)
					? fm.width(QString::number(rw_current_range.min,'f',0))
					: fm.width(QString::number(rw_current_range.max,'f',0));
				return length;
			}
		}

		bool Ruler::is_visible(double value)
		{
			bool visible = false;
			switch(rw_position)
			{
				case CLAM::VM::eLeft:
				case CLAM::VM::eRight:
				{
					double margin = double(rw_label_height)*rw_current_range.span()/double(rw_viewport.h);
					if(value+margin < rw_current_range.max) visible = true;
				}
				break;
				case CLAM::VM::eBottom:
				case CLAM::VM::eTop:
				{
					QFontMetrics fm(rw_font);
					int label_width = (rw_scale==CLAM::VM::eLogScale)
						? fm.width("-0.0e+00")
						: (rw_show_floats) 
						? fm.width(QString::number(value,'f',2)) 
						: fm.width(QString::number(value,'f',0));
					double margin = double(label_width/2)*rw_current_range.span()/double(rw_viewport.w);
					if(value+margin < rw_current_range.max) visible = true;
				}
				break;
				default:
					break;
			}
			return visible;
		}

		double Ruler::round(double x) 
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac >= 0.5) ? i+1.0 : i;
		}
	
		double Ruler::ceil(double x)
		{
			double i=double(int(x));
			double frac=x-i;
			return (frac) ? i+1.0 : i;
		}

		QString Ruler::get_label(double value)
		{
			if(rw_scale==CLAM::VM::eLinearScale)
			{
				if(rw_show_floats) 
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

		std::pair<double,double> Ruler::get_label_coords(double value)
		{
			QFontMetrics font_metrics(rw_font);
			int label_width = (rw_show_floats) 
				? font_metrics.width(QString::number(value,'f',2))
				: font_metrics.width(QString::number(value,'f',0));
			double x=0.0, y=0.0;
			switch(rw_position)
			{
				case CLAM::VM::eLeft:
				{
					double adjust = double(rw_label_height/4)*rw_current_range.span()/double(rw_viewport.h);
					x = rw_viewport.w - label_width-8;
					y = value - adjust;
				}
				break;
				case CLAM::VM::eRight:
				{
					double adjust = double(rw_label_height/4)*rw_current_range.span()/double(rw_viewport.h);
					x = 8;
					y = value - adjust;
				}
				break;
				case CLAM::VM::eBottom:
				{
					double adjust = double(label_width/2)*rw_current_range.span()/double(rw_viewport.w);
					x = value - adjust;
					y = rw_viewport.h-(rw_label_height/2+8);
				}
				break;
				case CLAM::VM::eTop:
				{
					double adjust = double(label_width/2)*rw_current_range.span()/double(rw_viewport.w);
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

