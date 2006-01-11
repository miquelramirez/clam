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
			
			virtual void set_xrange(double xmin, double xmax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);
			virtual void set_yrange(double ymin, double ymax, CLAM::VM::EScale scale=CLAM::VM::eLinearScale);			
			virtual void set_zoom_steps(int hsteps, int vsteps);

			void set_segmentation(CLAM::Segmentation* s);
			
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
			
		private slots:
			void setMaxVScroll(int);
			void setMaxHScroll(int);

		protected:
			QGridLayout*      wp_layout;    
			CLAM::VM::Plot2D* wp_plot;

		private:
			CLAM::VM::Ruler*       wp_xruler;
			CLAM::VM::Ruler*       wp_yruler;
			CLAM::VM::ScrollGroup* wp_hscroll;
			CLAM::VM::ScrollGroup* wp_vscroll;

			void init_segmentation_plot();
			void adjust_yruler_width(double min, double max);
		};
	}
}

#endif

