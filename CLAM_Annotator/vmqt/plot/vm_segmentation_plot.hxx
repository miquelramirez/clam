#ifndef __VMQT_SEGMENTATION_PLOT_H__
#define __VMQT_SEGMENTATION_PLOT_H__

#include "vm_wplot.hxx"
#include "vm_data_types.hxx"

class QGridLayout;

namespace CLAM
{
	class Segmentation;

	namespace VM
	{
		class Ruler;
		class Plot2D;
		class ScrollGroup;

		class SegmentationPlot : public WPlot
		{
			Q_OBJECT
		public:
			SegmentationPlot(QWidget* parent=0);
			virtual ~SegmentationPlot();
			
			virtual void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			virtual void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);			
			virtual void set_zoom_steps(int hsteps, int vsteps);

			void set_segmentation(Segmentation* s);
			
		signals:
			void selectedRegion(double, double);
			void segmentOnsetChanged(unsigned, double);
			void segmentOffsetChanged(unsigned, double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged(unsigned);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

			void updateLocator(double);
			void updateLocator(double, bool);
			
		private slots:
			void setMaxVScroll(int);
			void setMaxHScroll(int);

		protected:
			QGridLayout* wp_layout;    
			Plot2D*      wp_plot;
			Ruler*       wp_xruler;
			Ruler*       wp_yruler;

		private:
			ScrollGroup* wp_hscroll;
			ScrollGroup* wp_vscroll;

			void init_segmentation_plot();
			void adjust_yruler_width(double min, double max);
		};
	}
}

#endif

