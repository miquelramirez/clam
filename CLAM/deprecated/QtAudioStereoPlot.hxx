#ifndef __QTAUDIOSTEREOPLOT__
#define __QTAUDIOSTEREOPLOT__

#include "MediaTime.hxx"
#include "PlayablePlot.hxx"
#include "MultiDisplayPlot.hxx"
#include "Slotv1.hxx"


using SigSlot::Slotv1;

namespace CLAM
{
	class Audio;
	namespace VM
	{
		class TimeSegmentLabelsGroup;

		/**
		 * Allows viewing audio stereo (two audio channels)
		 *
		 * @ingroup QTVM
		 */

		class QtAudioStereoPlot : public MultiDisplayPlot, public PlayablePlot
		{

			Q_OBJECT

		public:
			QtAudioStereoPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~QtAudioStereoPlot();

			void SetData(const Audio& leftChannel, const Audio& rightChannel);
			void SetForegroundColor(Color c);
			void SetRegionColor(Color c);

		protected:
			void CreateControllers();
			void CreateSurfaces();

			void SynchronizeDialPos();
			void SynchronizeInsertMark();
			void SynchronizeRemoveMark();
			void SynchronizeUpdateMark();
			void SynchronizeUpdateTag();

			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();

			void hideEvent(QHideEvent* e);
			void closeEvent(QCloseEvent* e);

		private slots:
			void muteLeftChannel();
			void muteRightChannel();
			void updateRegion(MediaTime);

		private:
			Ruler*                  mYRulerL;
			Ruler*                  mYRulerR;
			QPushButton*            mMuteL;
			QPushButton*            mMuteR;
			Slotv1<TData>           mSlotPlayingTimeReceived;
			Slotv1<TData>           mSlotStopPlayingReceived;
			TimeSegmentLabelsGroup* mLabelsGroup;

			enum { MASTER=0, SLAVE=1 };
			
			void InitAudioStereoPlot();

			void PlayingTime(TData time);
			void StopPlaying(TData time);

			void SynchronizeRegionTime();
			void SynchronizePlayingPos();

			void SetPlayerData(const Audio& leftChannel, const Audio& rightChannel);
		};
	}
}

#endif

