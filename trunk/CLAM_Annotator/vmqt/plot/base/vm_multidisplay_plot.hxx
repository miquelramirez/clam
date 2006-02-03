#ifndef __VMQT_MULTIDISPLAY_PLOT_H__
#define __VMQT_MULTIDISPLATPLOT_H__

#include <vector>
#include "vm_wplot.hxx"

namespace CLAM
{
	namespace VM
	{
		class Ruler;
		class Plot2D;
		class ScrollGroup;

		class MultiDisplayPlot : public WPlot
		{
			Q_OBJECT
		public:
			MultiDisplayPlot(QWidget* parent=0);
			virtual ~MultiDisplayPlot();

		protected:
			Ruler*       wp_xruler;
			ScrollGroup* wp_hscroll;

			std::vector<Plot2D*> wp_display;

			virtual void create_display()=0;

			void init_multidisplay_plot(int master_id);
			void set_master_id(int id);

		private slots:
			void setMaxHScroll(int);

		private:
			int wp_master_id;
		};
	}
}

#endif

