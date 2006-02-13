#include "CLAMGL.hxx"
#include "vmGrid.hxx"

namespace CLAM
{
	namespace VM
	{
		Grid::Grid()
			: mShowGrid(false)
			, mSnapToGrid(false)
			, mGridXStep(1.0)
			, mGridYStep(1.0)
			, mGridColor(0,0,0)
		{
		}

		Grid::~Grid()
		{
		}

		void Grid::ShowGrid(bool show)
		{
			mShowGrid = show;
			emit requestRefresh();
		}

		void Grid::SnapToGrid(bool snap)
		{
			mSnapToGrid = snap;
		}

		void Grid::SetGridSteps(double xstep, double ystep)
		{
			mGridXStep = xstep;
			mGridYStep = ystep;
			emit requestRefresh();
		}

		void Grid::SetGridColor(const Color& color)
		{
			mGridColor = color;
			emit requestRefresh();
		}

		bool Grid::ShowGrid() const
		{
			return mShowGrid;
		}

		bool Grid::SnapToGrid() const
		{
			return mSnapToGrid;
		}

		double Grid::XStep() const
		{
			return mGridXStep;
		}

		double Grid::YStep() const
		{
			return mGridYStep;
		}

		void Grid::Render()
		{
			if(!mEnabled) return;
			if(!mShowGrid) return;
			
			glColor3ub(mGridColor.r,mGridColor.g,mGridColor.b);
			glLineWidth(1);
			glBegin(GL_LINES);
			// vertical lines
			double pos = mXRange.min;
			while(pos < mView.right)
			{
				if(pos > mView.left)
				{
					glVertex2d(pos,mView.bottom);
					glVertex2d(pos,mView.top);
				}
				pos += mGridXStep;
			}
			// horizontal lines
			pos = mYRange.min;
			while(pos < mView.top)
			{
				if(pos > mView.bottom)
				{
					glVertex2d(mView.left,pos);
					glVertex2f(mView.right,pos);
				}
				pos += mGridYStep;
			}
			glEnd();
		}
	}
}

// END
