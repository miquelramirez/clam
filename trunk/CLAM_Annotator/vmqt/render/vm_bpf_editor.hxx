#ifndef __VMQT_BPF_EDITOR_H__
#define __VMQT_BPF_EDITOR_H__

#include "BPF.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFEditor : public Renderer2D
		{
			Q_OBJECT
		public:
			BPFEditor();
			~BPFEditor();

			void set_data(BPF* bpf);
			void set_colors(const Color& cline, const Color& chandler);
			void set_flags(int flags);
			int get_flags() const;
			const BPF& get_data() const;

		signals:
			void xValueChanged(QString, unsigned, double);
			void yValueChanged(QString, unsigned, double);
			void elementAdded(QString, unsigned, double, double);
			void elementRemoved(QString, unsigned);
			
		protected:
			void render();
			void set_hbounds(double left, double right);
			void mouse_press_event(double x, double y);
			void mouse_release_event(double x, double y);
			void mouse_move_event(double x, double y);
			void key_press_event(int key);
			void key_release_event(int key);		   
			void leave_event();

		private:
			int        rd_glList;
			int        rd_flags;
			long       rd_current_index;
			bool       rd_rebuild_glList;

			BPF* rd_bpf;
			
			Color rd_line_color;
			Color rd_handler_color;

			enum { LINE_WIDTH=2, POINT_SIZE=5, TOLERANCE=3 };

			enum EditionMode
			{
				Idle,
				DraggingPoint
			};

			EditionMode rd_edition_mode;

			void remove();
			long add(double x, double y);
			void update(double x, double y);

			long pickPoint(double x, double y);
			long get_bound(double search_value);
			std::pair<long, long> get_bounds();

			double round(double x);
			QString get_tooltip(double x, double y);

			void choose_current_point(long index);
			void move_current_point_dx(double dx);
			void move_current_point_dy(double dy);

			double get_xstep();
			double get_ystep();
		};
	}
}

#endif

