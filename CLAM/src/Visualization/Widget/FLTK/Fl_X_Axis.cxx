#include "Fl_X_Axis.hxx"
#include <Fl/Fl_Group.H>
#include <cstdio>

namespace CLAMVM
{

int Fl_X_Axis::min_pos() const
{
	if(scale_&FL_AXIS_REV)
		return x()+w()-border()+Fl::box_dx(box())-Fl::box_dw(box());

	else
		return x()+border()+Fl::box_dx(box());

}

int Fl_X_Axis::max_pos() const
{
	if(scale_&FL_AXIS_REV)
		return x()+border()+Fl::box_dx(box());

	else
		return x()+w()-border()+Fl::box_dx(box())-Fl::box_dw(box());
		
}
		

void Fl_X_Axis::draw()
{
		int tick_index=-1;
		double tick_value;
		int tick_order;//, tick_number;
		double _interval=0;
		const char * label_format=label_format_;
		
		if(damage()|FL_DAMAGE_ALL)
				draw_label();
		if (damage()&(FL_DAMAGE_ALL|FL_AXIS_DAMAGE_ALL))
		{
				update();
				if (box()==FL_NO_BOX)
				{
						fl_color(parent()->color());
						fl_rectf(x(),y(),w(),h());
				}
				else
						draw_box();

				if(!valid_) 
						return;
				
				fl_font(label_font_face_,label_font_size_);
				int a, b, l1, l2, m1, m2, l ,_y,_w,_h; //temporary coordinates for ticks
				double _pos,_x;
				fl_clip(x()+Fl::box_dx(box()),y()+Fl::box_dy(box()),w()-Fl::box_dw(box()),h()-Fl::box_dh(box()));
				fl_color(axis_color_);
				a=y()+Fl::box_dh(box())+border_;
				b=a+h()-Fl::box_dh(box())-2*border_;
				
				switch(axis_align_ & FL_AXIS_ALIGNMENT)
				{
				case FL_AXIS_BOTTOM:
						l=l1=m1=a;
						if(axis_align_&FL_AXIS_NO_TICS)
								m2=m1;
						else
								if (tick_length_)
										m2=m1+tick_length_;
								else
										m2=m1+label_font_size_;
						l2=(m1+m2)/2;
						break;
				case FL_AXIS_TOP:
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

				if(axis_align_ & FL_AXIS_LINE)
				{
						fl_begin_line();
						fl_vertex(min_pos(),l);
						fl_vertex(max_pos(),l);
						fl_vertex(Fl::box_dx(box()),l);
						fl_vertex(Fl::box_dx(box())-Fl::box_dw(box()),l);
						//fl_vertex(x()+border_,l);
						//fl_vertex(x()+w()-border_,l);
						fl_end_line();
				}
				while(next_tick(tick_index, tick_value, tick_order, _interval))
				{
						_pos=position(tick_value);
						if(scale_&FL_AXIS_REV)
						{
								if(_pos<max_pos_) 
										break;
								if(_pos>min_pos_) 
										continue;
						}
						else
						{
								if(_pos<min_pos_) 
										continue;
								if(_pos>max_pos_) 
										break;
						}

						if(!(axis_align_&FL_AXIS_NO_TICS))
						{
								if(tick_index % major_step_)
								{
										fl_begin_loop();
										fl_vertex(_pos,l1);
										fl_vertex(_pos,l2);
										fl_end_loop();
								}
								else
								{
										fl_begin_loop();
										fl_vertex(_pos,m1);
										fl_vertex(_pos,m2);
										fl_end_loop();
								}
						}
						
						if(!((tick_index % label_step_)|(axis_align_&FL_AXIS_NO_LABELS)))
						{


							char*  label = new char[Fl_Axis::MAX_LABEL_LENGTH];
							char* _label_format= new char[Fl_Axis::MAX_LABEL_FORMAT];

							if(!label_format)
							{
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
							_x=_pos-_w/double(2);
							switch (axis_align_ & FL_AXIS_ALIGNMENT)
							{
							case FL_AXIS_TOP:
									_y=m1-_h/3;
									break;
							case FL_AXIS_BOTTOM:
									_y=m2+_h;
									break;
							case FL_AXIS_CENTER:
									_y=l+_h/3;
									Fl_Color _color=fl_color();
									fl_color(color());
									fl_axis_rectf(_x-_h/6,l-_h/2,_w+_h/3,_h);
									fl_color(_color);
									break;
							}
							fl_axis_text(label,_x,_y);
							delete [] label;
							delete [] _label_format;
						}
				}
				fl_line_style(0,0);
				fl_pop_clip();
		}
};

Fl_X_Axis::Fl_X_Axis(int x, int y, int w, int h, const char *label)
		: Fl_Axis(x, y, w,  h,  label)
{
};


Fl_X_Axis::~Fl_X_Axis()
{
}
   
}
