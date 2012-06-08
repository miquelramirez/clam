#ifndef __SMSTIMEMULTIDISPLAY__
#define __SMSTIMEMULTIDISPLAY__

#include <CLAM/Segment.hxx>
#include <CLAM/MediaTime.hxx>
#include <CLAM/PlayablePlot.hxx>
#include <CLAM/MultiDisplayPlot.hxx>
#include <CLAM/Slotv1.hxx>

using SigSlot::Slotv1;

class QFrame;
class QComboBox;
class QLayout;
class QLabel;

namespace CLAM
{
	namespace VM
	{
		class VScrollGroup;
		class TimeSegmentLabelsGroup;
		class ColorScale;

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
			void startPlaying();
			void stopPlaying();

		public slots:
			void setCurrentTime(double);
			void colorSonogram();
			void blackAndWhiteSonogram();

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
			void spectrogram(bool);
			void updateRegion(MediaTime);
			void setCurrentPlayer(int);
			void selectedXPos(double);
			void updateLabels(QString, QString, QString);
			void updateColorScale(int);

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
			bool                      mIsPlaying;
			bool                      mColorSonogram;

			// cpntainers
			QFrame*  mAudioDisplaysContainer;
			QFrame*  mFreqDisplaysContainer;

			// player holes /////////////
			QFrame* leftHole;
			QFrame* middleHole;
			QFrame* rightHole;
			/////////////////////////////

			// spectrogram panel //////
			QFrame*     leftGroup;
			QFrame*     rightGroup;
			QLabel*     freqTxtLabel;
			QLabel*     mFrequency;
			QLabel*     decibelTxtLabel;
			QLabel*     mDecibels;
			QLabel*     sliceTxtLabel;
			QLabel*     mSlice;
			QLabel*     mFFTSize;
			QLabel*     mTotalSlices;
			QLabel*     mSpectralRange;
			QLabel*     leftTag;
			QLabel*     rightTag;
			ColorScale* mColorScale;
			/////////////////////////////

			std::vector<std::string>  mPlayList;
		   
			enum { MASTER=0, SYNTHESIZED=1, SINUSOIDAL=2, RESIDUAL=3, FUNDAMENTAL=4, SINTRACKS=5, SONOGRAM=6 };
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

		    void CreateAudioDisplays();
			void CreateFrequencyDisplays();

			QLayout* CreatePlayer();
			QLayout* CreateSpectrogramPanel();

			void ShowPlayer();
			void HidePlayer();

			void ShowSpectrogramPanel();
			void HideSpectrogramPanel();


			void InitPlayList();
			void AddAudioItem(int id);
			void AddFundFreqItem(int id);
			void RemovePlayListItem(int id);

			int GetPlayDataId(const QString& item);

			void FillRightGroupLabels();

		};
	}
}

#endif

