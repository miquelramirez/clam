#include "CLAMGL.hxx"
#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{

		Renderer2D::Renderer2D()
			: rd_catch_events(true)
			, rd_viewport(-1,-1,1,1)
			, rd_grid(0)
		{
		}

		Renderer2D::~Renderer2D()
		{
		}

		void Renderer2D::set_enabled(bool enabled)
		{
			Renderer::set_enabled(enabled);
			emit requestRefresh();
		}

		void Renderer2D::set_grid(CLAM::VM::Grid* grid)
		{
			rd_grid = grid;
		}

		void Renderer2D::set_xrange(double xmin, double xmax)
		{
			if(xmax <= xmin) return;
			rd_xrange.min = xmin;
			rd_xrange.max = xmax;
			emit requestUpdate();
		}

		void Renderer2D::set_yrange(double ymin, double ymax)
		{
			if(ymax <= ymin) return;
			rd_yrange. min = ymin;
			rd_yrange.max = ymax;
			emit requestUpdate();
		}

		void Renderer2D::set_hbounds(double left, double right)
		{
			rd_view.left = left;
			rd_view.right = right;
		}

		void Renderer2D::set_vbounds(double bottom, double top)
		{
			rd_view.bottom = bottom;
			rd_view.top = top;
		}

		void Renderer2D::mouse_press_event(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::mouse_release_event(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::mouse_move_event(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::mouse_doubleclick_event(double x, double y)
		{
			// do nothing by default
		}

		void Renderer2D::key_press_event(int key)
		{
			// do nothing by default
		}

		void Renderer2D::key_release_event(int key)
		{
			// do nothing by default
		}

		void Renderer2D::enter_event()
		{
			// do nothing by default
		}

		void Renderer2D::leave_event()
		{
			// do nothing by default
		}

		void Renderer2D::set_viewport(const CLAM::VM::GLViewport& v)
		{
			rd_viewport = v;
		}

		void Renderer2D::set_key(const QString& key)
		{
			rd_key = key;
		}

		void Renderer2D::catch_events(bool ce)
		{
			rd_catch_events = ce;
		}

		int Renderer2D::get_glList()
		{
			int gl_list = 1;
			while(glIsList(gl_list)) gl_list++;
			return gl_list;
		}
	}
}

// END
