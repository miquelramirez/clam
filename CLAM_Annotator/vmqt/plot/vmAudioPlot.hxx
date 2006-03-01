#ifndef __VMQT_AUDIO_PLOT_H__
#define __VMQT_AUDIO_PLOT_H__

#include "Audio.hxx"
#include "vmSegmentationPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class Grid;
		class DataArrayRenderer;

		class AudioPlot : public SegmentationPlot
		{
		public:
			AudioPlot(QWidget* parent=0);
			virtual ~AudioPlot();

			virtual void SetData(const Audio& audio, bool update=false);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

		private:
			void InitAudioPlot();
			std::pair<int,int> GetZoomSteps(TSize size);

			void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void SetZoomSteps(int hsteps, int vsteps);
		private:
			Grid * mGridRenderer;
			DataArrayRenderer * mAudioRenderer;
		};
	}
}

#endif

