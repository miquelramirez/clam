#ifndef __VMQT_BPF_EDITOR_H__
#define __VMQT_BPF_EDITOR_H__

#include "BPF.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class BPFEditor : public CLAM::VM::Renderer2D
		{
			Q_OBJECT
		public:
			BPFEditor();
			~BPFEditor();

			void set_data(CLAM::BPF* bpf);
			void set_colors(const CLAM::VM::Color& cline, const CLAM::VM::Color& chandler);
			void set_flags(int flags);

		signals:
			void xValueChanged(unsigned, double);
			void yValueChanged(unsigned, double);
			void elementAdded(unsigned, double, double);
			void elementRemoved(unsigned);
			
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

			CLAM::BPF* rd_bpf;
			
			CLAM::VM::Color rd_line_color;
			CLAM::VM::Color rd_handler_color;

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

			QString get_tooltip(double x, double y);
		};
	}
}

#endif

