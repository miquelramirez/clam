#ifndef __FLAXIS__
#define __FLAXIS__

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "Fl_Axis_Enumerations.hxx"
#include "Slotv2.hxx"


namespace CLAMVM
{
	using SigSlot::Slotv2;

inline void fl_axis_rect(double x, double y, double w, double h)
{
	fl_begin_loop();
	fl_vertex(x,y);
	fl_vertex(x+w,y);
	fl_vertex(x+w,y+h);
	fl_vertex(x,y+h);
	fl_end_loop();
}

inline void fl_axis_rectf(double x, double y, double w, double h)
{
	fl_begin_polygon();
	fl_vertex(x,y);
	fl_vertex(x+w,y);
	fl_vertex(x+w,y+h);
	fl_vertex(x,y+h);
	fl_end_polygon();
}

inline void fl_axis_loop(double x1, double y1, double x2, double y2, double x3, double y3)
{
	fl_begin_loop();
	fl_vertex(x1,y1); fl_vertex(x2,y2); fl_vertex(x3,y3);
	fl_end_loop();
}

inline void fl_axis_polygon(double x1, double y1, double x2, double y2, double x3, double y3)
{
	fl_begin_polygon();
	fl_vertex(x1,y1); fl_vertex(x2,y2); fl_vertex(x3,y3);
	fl_end_polygon();
}

 inline void fl_axis_loop(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
 {
	fl_begin_loop();
	fl_vertex(x1,y1); fl_vertex(x2,y2); fl_vertex(x3,y3); fl_vertex(x4,y4);
	fl_end_loop();
};

 inline void fl_axis_polygon(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
 {
	fl_begin_polygon();
	fl_vertex(x1,y1); fl_vertex(x2,y2); fl_vertex(x3,y3); fl_vertex(x4,y4);
	fl_end_polygon();
};

 inline void fl_axis_text(const char  *label, double x, double y)
 {
	fl_draw(label,(int)(x+.5),(int)(y+.5));
};

 inline void fl_axis_point(double x, double y)
 {
	fl_point((int)(x+.5),(int)(y+.5));
};

inline void fl_axis_filled_circle(double x, double y, double r)
{
	fl_begin_polygon();
	//fl_arc(x,y,r,0,360);
	fl_circle(x,y,r);
	fl_end_polygon();
};

inline void fl_axis_text(const char  *label, double x, double y, double w, double h, Fl_Align align)
{
	fl_draw(label, (int)(x+.5), (int)(y+.5), (int)(w+.5), (int)(h+.5), align);
};


class Fl_Axis : public Fl_Box
{
		
// members
protected:
		int scale_;
		bool valid_;
		double k_;
		double q_;
		const char *label_format_;
		int minor_grid_style_;
		int major_grid_style_;
		int label_grid_style_;
		int minor_grid_width_;
		int major_grid_width_;
		int label_grid_width_;
		char * minor_grid_dashes_;
		char * major_grid_dashes_;
		char * label_grid_dashes_;
		int grid_visible_;
		double tick_interval_;
		int tick_separation_;
		int tick_length_;
		int tick_width_;
		int major_step_;
		int label_step_;
		unsigned char  axis_align_;		
		int label_font_size_;
  		double min_;
		double max_;
		double proxy_min;
		double proxy_max;
		int min_pos_;
		int max_pos_;
		int border_;

		Fl_Color axis_color_;
		Fl_Color minor_grid_color_;
		Fl_Color major_grid_color_;
		Fl_Color label_grid_color_;
		Fl_Font label_font_face_;		

		

		// methods
protected:
		virtual int min_pos() const =0 ;
		virtual int max_pos() const =0 ;
		int update();
		int next_tick(int &tick_index, double &tick_value, int &tick_order, double &interval_);

		virtual void adjust_range( double offset, double scale );
	
public:
		Slotv2<double,double>   AdjustRange;

		double position(double);
		
		double value(double);
		
		int border() const
		{
				return border_;
		}
		
		void border(int border)
		{
				border_=border;
				damage(FL_AXIS_DAMAGE_ALL);
		}
		

		double minimum() const
		{
				return min_;
		}
		
		double maximum() const
		{
				return max_;
		}

		
		void minimum(double x);

		void maximum(double x);

		void clear()
		{
				valid_=0;
		}

		int cleared() const
		{
				return !valid_;
		}
		
		void rescale(int when, double x);

		void rescale_move(int when, double  x);

		void tick_interval(double interval) 
		{
				tick_interval_=interval; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		double tick_interval() const
		{
				return tick_interval_;
		}
		
		void tick_separation(int separation) 
		{
				tick_separation_=separation; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int tick_separation() const
		{
				return tick_separation_;
		}
		
		void tick_length(int length)
		{
				tick_length_=length; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int tick_length() const
		{
				return tick_length_;
		}
		
		void tick_width(int width)
		{
				tick_width_=width; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int tick_width() const
		{
				return tick_width_;
		}
		
		void major_step(int step)
		{
				major_step_=step;
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int major_step() const 
		{
				return major_step_;
		}
		
		void label_step(int step)
		{
				label_step_=step;
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int label_step() const
		{
				return label_step_;
		}
		
		void label_format(const char *format)
		{
				label_format_=format; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		const char* label_format() const
		{
				return label_format_;
		}
		
		void label_font(Fl_Font face)
		{
				label_font_face_=face; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		Fl_Font label_font() const
		{
				return label_font_face_;
		}
		
		void label_size(int size)
		{
				label_font_size_=size; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		int label_size() const 
		{
				return label_font_size_;
		}
		
		void axis_color(Fl_Color _axis_color)
		{
				axis_color_=_axis_color; 
				damage(FL_AXIS_DAMAGE_ALL);
		}
		
		void axis_align(unsigned char align)
		{
				axis_align_=align; 
				damage(FL_AXIS_DAMAGE_ALL);
		}

		unsigned char axis_align() const
		{
				return axis_align_;
		}
		
		void scale(int s)		
		{
				if(s!=scale_)				
						redraw(); 
						
				scale_=s;
		}

		int scale() const
		{
				return scale_;
		}
		
		Fl_Axis(int x, int y, int w, int h, const char * label=0);

		~Fl_Axis();

		// constants
		protected:
		static const int		CANVAS_BORDER;                //gap between the graphics and surrounding"box"
		static const int        AXIS_BORDER;                  //gap between axis drawing(i.e.axis line) and its "box"
		static const int        MINOR_INTERVAL;               //0 stands for automatic choice in default_*_intervals array
		static const int        MINOR_SEPARATION;            
		static const int        MAJOR_STEP;
		static const int        LABEL_STEP;
		static const int        LABEL_SIZE;
		static const Fl_Font    LABEL_FONT;
		
		
		static const int MAX_LABEL_FORMAT;
		static const int MAX_LABEL_LENGTH;
		
		
		static const int NO_LIN_DEFAULTS;
		static const double default_lin_intervals[];
		static const int default_lin_major_steps[];
		static const int default_lin_label_steps[];
		
		static const int NO_LOG_DEFAULTS;
		static const double default_log_intervals[];
		static const int default_log_major_steps[];
		static const int default_log_label_steps[];
		

		
};

}

#endif // Fl_Axis.hxx
