#ifndef __SMSTIMEMULTIDISPLAY__
#define __SMSTIMEMULTIDISPLAY__

#include "Segment.hxx"
#include "MediaTime.hxx"
#include "PlayablePlot.hxx"
#include "MultiDisplayPlot.hxx"
#include "Slotv1.hxx"

using SigSlot::Slotv1;

class QFrame;
class QComboBox;
class QLayout;

namespace CLAM
{
	namespace VM
	{
		class VScrollGroup;
		class TimeSegmentLabelsGroup;

		class SMSTimeMultiDisplay : public MultiDisplayPlot, public PlayablePlot
		{
			Q_OBJECT
		public:
			SMSTimeMultiDisplay(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~SMSTimeMultiDisplay();

			void SetOriginalAudio(const Audio& audio);
			void SetSynthesizedAudio(const Audio& audio);
			void SetSynthesizedSinusoidal(const Audio& audio);
			void SetSynthesizedResidual(const Audio& audio);
			void SetAnalyzedSegment(const Segment& segment);

			void SetForegroundColor(Color c);
			void SetRegionColor(Color c);

			void ShowDisplay(int id);
			void HideDisplay(int id);

			void ShowDisplayOnNewData(bool show);

			void Flush();

		signals:
			void currentTime(float);
			void dataType(QString);

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
			void setMaxAudioVScroll(int);
			void setMaxFrequencyVScroll(int);
			void originalAudio(bool);
			void synthesizedAudio(bool);
			void sinusoidalAudio(bool);
			void residualAudio(bool);
			void fundamentalFreq(bool);
			void sinusoidalTracks(bool);
			void updateRegion(MediaTime);
			void setCurrentPlayer(int);
			void selectedXPos(double);

		private:
			std::vector<const Audio*> mAudioPtrs;
			std::vector<Ruler*>       mYRulers;
			VScrollGroup*             mAudioVScroll;
			VScrollGroup*             mFrequencyVScroll;
			Slotv1<TData>             mSlotPlayingTimeReceived;
			Slotv1<TData>             mSlotStopPlayingReceived;
			TimeSegmentLabelsGroup*   mLabelsGroup;
			QComboBox*                mCBPlayList;
			bool                      mShowOnNewData;
			bool                      mHasMasterData;
			bool                      mHasAudioData;
			bool                      mHasFundamentalData;
			bool                      mHasEnqueuedPlayListItem;
			int                       mEnqueuedDataId;
			int	                      mEnqueuedPlayerId;

			// player holes /////////////
			QFrame* leftHole;
			QFrame* middleHole;
			QFrame* rightHole;
			/////////////////////////////

			std::vector<std::string>  mPlayList;
		   
			enum { MASTER=0, SYNTHESIZED=1, SINUSOIDAL=2, RESIDUAL=3, FUNDAMENTAL=4, SINTRACKS=5 };
			enum { AUDIO_PLAYER=0, FUND_PLAYER=1 };

			void InitSMSTimePlot();

			void SynchronizeVZoom();
			void CheckVisibility();
			void SetYRulersWidth(int w);

			void ConnectVScrollGroup();
			void SynchronizeYRulers();
			void SyncAudioYRuler(int id);
			void SyncFrequencyYRuler(int id);
			void HideDisplays();
			void ConnectDataTypeSignal();

			void PlayingTime(TData time);
			void StopPlaying(TData time);

			void SynchronizeRegionTime();
			void SynchronizePlayingPos();

			void InitAudioPtrs();

			void CheckPlayerVisibility();
			void CheckAudioVScrollVisibility();
			void CheckFrequencyVScrollVisibility();

			QLayout* CreateAudioDisplays();
			QLayout* CreateFrequencyDisplays();
			QLayout* CreatePlayer();

			void ShowPlayer();
			void HidePlayer();

			void InitPlayList();
			void AddAudioItem(int id);
			void AddFundFreqItem(int id);
			void RemovePlayListItem(int id);

			int GetPlayDataId(const QString& item);

		};
	}
}

#endif

