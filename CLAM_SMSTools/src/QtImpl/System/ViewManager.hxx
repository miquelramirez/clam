#ifndef __VIEWMANAGER__
#define __VIEWMANAGER__

#include <vector>
#include <qobject.h>
#include <qstring.h>
#include "Segment.hxx"

class QWidget;
class QFrame;
class QBoxLayout;

namespace CLAM
{
	namespace VM
	{
		class MultiDisplayPlot;
		class Navigator;
	}
}

namespace QtSMS
{
	enum eView 
	{ 
		ORIGINAL_AUDIO        = 0
		, SYNTH_AUDIO         = 1 
		, SYNTH_SINUSOIDAL    = 2 
		, SYNTH_RESIDUAL      = 3 
		, FUND_FREQ           = 4 
		, SIN_TRACKS          = 5 
		, SONOGRAM            = 6
		, SPEC_AND_PEAKS      = 0 
		, SIN_SPEC            = 1 
		, RES_SPEC            = 2 
		, TIME_GROUP_VIEW     = 0
		, SPECTRUM_GROUP_VIEW = 1 
	};

	class ViewManager : public QObject
	{
		typedef std::vector<CLAM::VM::MultiDisplayPlot*> PlotList;

		Q_OBJECT

	public:
		~ViewManager();

		static ViewManager* Instance();

		void SetAudio(eView id);
		void SetAnalyzedData();
		void SetTransformedData();
		void SetSynthesizedData();
		
		void ShowConcreteView(eView group_id, eView view_id);
		void HideConcreteView(eView group_id, eView view_id);

		void SetBackgroundBlack();
		void SetBackgroundWhite();

		void ColorSonogram();
		void BlackAndWhiteSonogram();

		void Flush();

		QWidget* GetView(QWidget* parent);

		void Reset();
		void HideDisplays();
		
	signals:
		void dataType(QString);
		void sampleRateDuration(QString);

	private slots:
		void onNewTime(float);
		void onNewFrame(int);
		void onStartPlaying();
		void onStopPlaying();
		void onTimeViewFocusIn();
		void onSpectrumViewFocusIn();
		
	private:
		ViewManager();

		static ViewManager* mInstance;
		
		PlotList             mPlotList;
		CLAM::VM::Navigator* mFrameNavigator;
		float                mCurrentTime;
		int                  mLastFrame;
		bool                 mSentinel;
		bool                 mHasTimeViewFocus;
		bool                 mHasSpecViewFocus;
		bool                 mHasTransformedSegment;

		QFrame* hole;
		QFrame* mTimeViewContainer;
		QFrame* mSpecViewContainer;

		QBoxLayout* timeViewLayout;
		QBoxLayout* specViewLayout;

		int GetFrameNumber();
		void UpdateSpectrumView(int frameNumber, bool update=true);
		void SetNavigatorVisible(bool visible);
		void SetNavigatorEnabled(bool enabled);
		
		int GetHoleWidth();
		QString GetSampleRateDuration(bool hasAudio);
		QString TimeToStr(float time);
	};
}

#endif

