#ifndef __VMQT_AUDIO_STEREO_PLOT_H__
#define __VMQT_AUDIO_STEREO_PLOT_H__

#include "Audio.hxx"
#include "vmMultiDisplayPlot.hxx"

class QGridLayout;

namespace CLAM
{
	namespace VM
	{
		class AudioStereoPlot : public MultiDisplayPlot
		{
			Q_OBJECT
		public:
			AudioStereoPlot(QWidget* parent=0);
			virtual ~AudioStereoPlot();

			virtual void SetData(const Audio& chn0, const Audio& chn1, bool update=false);

		signals:
			void selectedRegion(double,double);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

			void updateLocator(double);
			void updateLocator(double,bool);

		protected:
			QGridLayout* mLayout;
			
			enum { MASTER=0, SLAVE=1 };

		private slots:
			void setMaxVScroll(int);

		private:
			Ruler*       mYRuler0;
			Ruler*       mYRuler1;
			ScrollGroup* mVScroll;

			void CreateDisplay();
			void InitAudioStereoPlot();
			std::pair<int,int> GetZoomSteps(TSize size);
		};
	}
}

#endif

