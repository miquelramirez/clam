#ifndef __VMQT_GRID_H__
#define __VMQT_GRID_H__

#include "vmRenderer2D.hxx"

namespace CLAM
{
	namespace VM
	{
		class Grid : public Renderer2D
		{
		public:
			Grid();
			~Grid();

			void ShowGrid(bool show);
			void SnapToGrid(bool snap);
			void SetGridSteps(double xstep, double ystep);
		    void SetGridColor(const Color& color);

			bool ShowGrid() const;
			bool SnapToGrid() const;

			double XStep() const;
			double YStep() const;

		protected:
			void Render();

		private:
			bool   mShowGrid;
			bool   mSnapToGrid;
			double mGridXStep;
			double mGridYStep;
			
			Color mGridColor;
		};
	}
}

#endif

