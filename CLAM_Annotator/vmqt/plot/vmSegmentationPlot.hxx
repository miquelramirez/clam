#ifndef __VMQT_SEGMENTATION_PLOT_H__
#define __VMQT_SEGMENTATION_PLOT_H__

#include "vmWPlot.hxx"
#include "vmDataTypes.hxx"

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
			
			virtual void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			virtual void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);			
			virtual void SetZoomSteps(int hsteps, int vsteps);

			void SetSegmentation(Segmentation* s);
			
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
			QGridLayout* mLayout;    
			Plot2D*      mPlot;
			Ruler*       mXRuler;
			Ruler*       mYRuler;
			Locator*       mLocator;
			SegmentEditor* mSegmentation;

		private:
			ScrollGroup* mHScroll;
			ScrollGroup* mVScroll;

			void InitSegmentationPlot();
			void AdjustYRulerWidth(double min, double max);
		};
	}
}

#endif

