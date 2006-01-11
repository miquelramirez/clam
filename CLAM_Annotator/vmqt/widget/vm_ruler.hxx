#ifndef __VMQT_RULER_H__
#define __VMQT_RULER_H__

#include <QWidget>
#include "vm_data_types.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler : public QWidget
		{
			Q_OBJECT
		public:
			Ruler(QWidget* parent, CLAM::VM::ERulerPos pos, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);
			~Ruler();

			void set_scale(CLAM::VM::EScale scale);
			CLAM::VM::EScale get_scale() const;
	    
			void set_bg_color(const CLAM::VM::Color& c);
			void set_fg_color(const CLAM::VM::Color& c);
	
			const QFont& get_font() const;

		signals:
			void valueChanged(double, double);
	    
		public slots:
			void updateRange(double, double);

		protected:
			void paintEvent(QPaintEvent* pe);

		private:
			int   rw_ticks;
			int   rw_label_width;
			int   rw_label_height;
			QFont rw_font;
	
			CLAM::VM::ERulerPos rw_position;
			CLAM::VM::EScale    rw_scale;
			CLAM::VM::Range     rw_range;
			CLAM::VM::Color     rw_fg_color;
			
			enum { MINTICKS=3, MAXTICKS=17 };

			void draw_left();
			void draw_right();
			void draw_bottom();
			void draw_top();

			int get_ticks();
			int test_ticks_for_span();
			void compute_max_label_width();
			QString get_label(int tick);
		};
    }
}

#endif

