#include "CLAMGL.hxx"
#include "vm_grid.hxx"

namespace CLAM
{
	namespace VM
	{
		Grid::Grid()
			: rd_show_grid(false)
			, rd_snap_to_grid(false)
			, rd_grid_xstep(1.0)
			, rd_grid_ystep(1.0)
			, rd_grid_color(0,0,0)
		{
		}

		Grid::~Grid()
		{
		}

		void Grid::show_grid(bool show)
		{
			rd_show_grid = show;
			emit requestRefresh();
		}

		void Grid::snap_to_grid(bool snap)
		{
			rd_snap_to_grid = snap;
		}

		void Grid::set_grid_steps(double xstep, double ystep)
		{
			rd_grid_xstep = xstep;
			rd_grid_ystep = ystep;
			emit requestRefresh();
		}

		void Grid::set_grid_color(const Color& color)
		{
			rd_grid_color = color;
			emit requestRefresh();
		}

		bool Grid::show_grid() const
		{
			return rd_show_grid;
		}

		bool Grid::snap_to_grid() const
		{
			return rd_snap_to_grid;
		}

		double Grid::xstep() const
		{
			return rd_grid_xstep;
		}

		double Grid::ystep() const
		{
			return rd_grid_ystep;
		}

		void Grid::render()
		{
			if(!rd_enabled) return;
			if(!rd_show_grid) return;
			
			glColor3ub(rd_grid_color.r,rd_grid_color.g,rd_grid_color.b);
			glLineWidth(1);
			glBegin(GL_LINES);
			// vertical lines
			double pos = rd_xrange.min;
			while(pos < rd_view.right)
			{
				if(pos > rd_view.left)
				{
					glVertex2d(pos,rd_view.bottom);
					glVertex2d(pos,rd_view.top);
				}
				pos += rd_grid_xstep;
			}
			// horizontal lines
			pos = rd_yrange.min;
			while(pos < rd_view.top)
			{
				if(pos > rd_view.bottom)
				{
					glVertex2d(rd_view.left,pos);
					glVertex2f(rd_view.right,pos);
				}
				pos += rd_grid_ystep;
			}
			glEnd();
		}
	}
}

// END
