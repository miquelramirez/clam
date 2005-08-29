#ifndef __VIEWMANAGER__
#define __VIEWMANAGER__

#include <vector>
#include <qobject.h>
#include "Segment.hxx"

class QWidget;

namespace CLAM
{
	namespace VM
	{
		class MultiDisplayPlot;
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
		void SetSynthesizedData();
		
		void ShowConcreteView(eView group_id, eView view_id);
		void HideConcreteView(eView group_id, eView view_id);

		void Flush();

		QWidget* GetView(QWidget* parent);

	private slots:
		void onNewTime(float);

	private:
		ViewManager();

		static ViewManager* mInstance;
		
		PlotList mPlotList;
		float    mCurrentTime;
		int      mLastFrame;

		int GetFrameNumber();
	};
}

#endif

