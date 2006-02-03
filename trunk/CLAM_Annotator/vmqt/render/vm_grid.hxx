#ifndef __VMQT_GRID_H__
#define __VMQT_GRID_H__

#include "vm_renderer2d.hxx"

namespace CLAM
{
	namespace VM
	{
		class Grid : public Renderer2D
		{
		public:
			Grid();
			~Grid();

			void show_grid(bool show);
			void snap_to_grid(bool snap);
			void set_grid_steps(double xstep, double ystep);
		    void set_grid_color(const Color& color);

			bool show_grid() const;
			bool snap_to_grid() const;

			double xstep() const;
			double ystep() const;

		protected:
			void render();

		private:
			bool   rd_show_grid;
			bool   rd_snap_to_grid;
			double rd_grid_xstep;
			double rd_grid_ystep;
			
			Color rd_grid_color;
		};
	}
}

#endif

