#ifndef __VMQT_AUDIO_PLOT_H__
#define __VMQT_AUDIO_PLOT_H__

#include "Audio.hxx"
#include "vm_segmentation_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		class AudioPlot : public SegmentationPlot
		{
		public:
			AudioPlot(QWidget* parent=0);
			virtual ~AudioPlot();

			virtual void set_data(const Audio& audio, bool update=false);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

		private:
			void init_audio_plot();
			std::pair<int,int> get_zoom_steps(TSize size);

			void set_xrange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_yrange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);
			void set_zoom_steps(int hsteps, int vsteps);
		};
	}
}

#endif

