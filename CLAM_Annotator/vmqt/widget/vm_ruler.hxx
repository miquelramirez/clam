#ifndef __VMQT_RULER_H__
#define __VMQT_RULER_H__

#include <vector>
#include <QGLWidget>
#include "vm_data_types.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler : public QGLWidget
		{
			Q_OBJECT
		public:
			Ruler(QWidget* parent, ERulerPos pos, ERulerScale scale=CLAM::VM::eLinearScale);
			~Ruler();

			void set_range(double min, double max);
			void set_step(double step);
			void set_scale(ERulerScale scale);
			ERulerScale get_scale() const;
	    
			void set_bg_color(const Color& c);
			void set_fg_color(const Color& c);
	
			void show_floats();
			void show_integers();

			const QFont& get_font() const;

		signals:
			void valueChanged(double, double);
	    
		public slots:
			void updateRange(double, double);

		protected:
			void paintGL(); 
			void resizeEvent(QResizeEvent* e);

		private:
			int    rw_label_height;
			bool   rw_do_resize;
			bool   rw_show_floats;
			double rw_step;
			QFont  rw_font;
	
			ERulerPos   rw_position;
			ERulerScale rw_scale;
			GLViewport  rw_viewport;
			Range       rw_total_range;
			Range       rw_current_range;
			Color       rw_bg_color;
			Color       rw_fg_color;

			std::vector<double> rw_values_to_draw;

			enum { MINTICKS=3 };

			void rebuild();

			void draw();
			void draw_left();
			void draw_right();
			void draw_bottom();
			void draw_top();
			void draw_labels();

			int get_ticks();
			int test_ticks_for_span();
			int get_max_label_width();
			bool is_visible(double value);
			double round(double x);
			double ceil(double x);
			QString get_label(double value);
			std::pair<double,double> get_label_coords(double value);
		};
    }
}

#endif

