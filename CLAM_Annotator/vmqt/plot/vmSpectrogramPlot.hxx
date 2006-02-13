#ifndef __VMQT_SPECTROGRAM_PLOT_H__
#define __VMQT_SPECTROGRAM_PLOT_H__

#include "Segment.hxx"
#include "vmSegmentationPlot.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class SpecgramPanel;

		class SpectrogramPlot : public SegmentationPlot
		{
			Q_OBJECT
		public:
			SpectrogramPlot(QWidget* parent=0);
			~SpectrogramPlot();

			void SetData(const Segment& segment, bool update=false);

		public slots:
			void colorSpecgram();
			void blackWhiteSpecgram();

		private slots:
			void updateColorScale(int);

		private:
			int            mSonogramColormap;
			int            mColorScaleWidth;
			SpecgramPanel* mPanel;
		   
			enum { COLOR_SONOGRAM=0, BW_SONOGRAM=1 };

			void InitSpectrogramPlot();
		   
			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetZoomSteps(int hsteps, int vsteps);

			std::pair<int,int> GetZoomSteps(TSize size, TData yrange);
			Array<Spectrum> GetSpecMatrix(const Segment& in);
		};
	}
}

#endif

