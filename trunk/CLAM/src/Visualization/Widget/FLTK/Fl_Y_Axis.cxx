#include "Fl_Y_Axis.hxx"
#include <FL/Fl_Group.H>
#include <cmath>
#include <cstdio>

namespace CLAMVM
{

	int Fl_Y_Axis::min_pos() const
	{ 
		if (scale_&FL_AXIS_REV)
			return y()+border()+Fl::box_dy(box());
		else
			return y()+h()-border()+Fl::box_dy(box())-Fl::box_dh(box());		
	}

	int Fl_Y_Axis::max_pos() const
	{
		if (scale_&FL_AXIS_REV)
			return y()+h()-border()+Fl::box_dy(box())-Fl::box_dh(box());

		else
			return y()+border()+Fl::box_dy(box());				
	}

	void Fl_Y_Axis::draw()
	{
		int tick_index=-1;
		double tick_value;
		int tick_order;//,tick_number;
		double _interval=0;
		const char * label_format=label_format_;
		if (damage()&(FL_DAMAGE_ALL|FL_AXIS_DAMAGE_ALL)){
			update();
			if (box()==FL_NO_BOX){
				fl_color(parent()->color());
				fl_rectf(x(),y(),w(),h());
			}else
				draw_box();
			if(!valid_) return;
			fl_font(label_font_face_,label_font_size_);
			int a = 0, b = 0, l1 = 0, l2 = 0, m1 = 0;
			int m2 = 0, l = 0, _x = 0, _w = 0 , _h = 0; //temporary coordinates for ticks
			double _pos,_y;
			fl_clip(x()+Fl::box_dx(box()),y()+Fl::box_dy(box()),w()-Fl::box_dw(box()),h()-Fl::box_dh(box()));
			fl_color(axis_color_);
			a=x()+Fl::box_dx(box())+border_;
			b=a+w()-Fl::box_dw(box())-2*border_;
			switch(axis_align_ & FL_AXIS_ALIGNMENT){
			case FL_AXIS_RIGHT:
				l=l1=m1=a;
				if(axis_align_&FL_AXIS_NO_TICS)
					m2=m1;
				else
					if (tick_length_)
						m2=m1+tick_length_;
					else
						m2=m1+label_font_size_;
				l2=(l1+m2)/2;
				break;
			case FL_AXIS_LEFT:
				l=l2=m2=b-1;
				if(axis_align_&FL_AXIS_NO_TICS)
					m1=m2;
				else
					if (tick_length_)
						m1=m2-tick_length_;
					else
						m1=m2-label_font_size_;
				l1=(m1+m2)/2;
				break;
			case FL_AXIS_CENTER:
				m1=a;
				m2=b;
				l=(a+b)/2;
				l1=(a+l)/2;
				l2=(l+b)/2;
				break;
			}
			fl_line_style(FL_SOLID|FL_CAP_FLAT,tick_width_);
//		double minp,maxp;
		
			if(axis_align_ & FL_AXIS_LINE){
				fl_begin_line();
				fl_vertex(l,Fl::box_dy(box()));
				fl_vertex(l,Fl::box_dy(box())-Fl::box_dh(box()));
				//fl_vertex(x()+border_,l);
				//fl_vertex(x()+w()-border_,l);
				fl_end_line();
			}
			while(next_tick(tick_index, tick_value, tick_order, _interval)){
				_pos=position(tick_value);
				if(scale_&FL_AXIS_REV){
					if(_pos<min_pos_) continue;
					if(_pos>max_pos_) break;
				}else{
					if(_pos<max_pos_) break;
					if(_pos>min_pos_) continue;
				}
				if(!(axis_align_&FL_AXIS_NO_TICS)){
					fl_begin_loop();
					if(tick_index % major_step_){
						fl_vertex(l1,_pos);
						fl_vertex(l2,_pos);
					}else{
						fl_vertex(m1,_pos);
						fl_vertex(m2,_pos);
					}
					fl_end_loop();
				}
				if(!((tick_index % label_step_)|(axis_align_&FL_AXIS_NO_LABELS)))
				{
					// KLUDGE by MRJ: seems that we have found another VC++ bug
					// declaring the array below as
					// char label[MAX_LABEL_LENGTH];
					// char _label_format[MAX_LABEL_FORMAT];
					// produced error C2057, arguing that MAX_LABEL_LENGTH and
					// MAX_LABEL_FORMAT arent constant expression, although they
					// are declared as static const in the Fl_Axis base class.
					// Is there some detail I am missing?
					// 
					char* label = new char[MAX_LABEL_LENGTH];
					char* _label_format= new char[MAX_LABEL_FORMAT];
                
					if(!label_format){
						int _tick_order;
						if (tick_order>=0)
							_tick_order=0;
						else
							_tick_order=-tick_order;
						sprintf(_label_format,"%s.%if","%",_tick_order);
					}
					else
						strcpy(_label_format,label_format);

					if ( tick_value < 1e-10 && tick_value > -1e-10) 
						sprintf( label, "%d", 0 );
					else
						sprintf(label, _label_format,tick_value);
					fl_measure(label,_w,_h);
					_y=_pos+_h/3;
					switch (axis_align_ & FL_AXIS_ALIGNMENT){
					case FL_AXIS_LEFT:
						_x=m1-_h/3-_w;
						break;
					case FL_AXIS_RIGHT:
						_x=m2+_h/3;
						break;
					case FL_AXIS_CENTER:
						_x=(m1+m2)/2-_w/2;
						Fl_Color _color=fl_color();
						fl_color(color());
						fl_axis_rectf(_x-_h/6,_pos-_h/2,_w+_h/3,_h);
						fl_color(_color);
						break;
					}
					fl_axis_text(label,_x,_y);
					delete[] label;
					delete[] _label_format;
				}
			}
			fl_line_style(0);
			fl_pop_clip();
		}
	};


	Fl_Y_Axis::Fl_Y_Axis(int x, int y, int w, int h, const char * label)
		: Fl_Axis(x, y, w,  h,  label){
		axis_align(FL_AXIS_LEFT|FL_AXIS_LINE);
	};

	Fl_Y_Axis::~Fl_Y_Axis()
	{
	}

}
