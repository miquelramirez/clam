#ifndef __QTSMSTOOLS__
#define __QTSMSTOOLS__

#include <CLAM/Thread.hxx>
#include "SMSToolsBase.h"
#include <CLAM/Slotv0.hxx>

class QLabel;

namespace QtSMS
{
	class Engine;

	class QtSMSTools : public SMSToolsBase
	{
		Q_OBJECT
	public:
		QtSMSTools();
		~QtSMSTools();

	private slots:
		// menu File  
		void loadConfiguration();
		void newConfiguration();
		void storeExtractedMelody();
		void loadAnalysisData();
		void storeAnalysisData();
		void saveSynthesizedAudio();
		void saveSynthesizedSinusoidal();
		void saveSynthesizedResidua();
		void loadTransformationScore();
		void newTransformationScore();
		// menu analysis
		void analyze();
		void melodyExtraction();
		// menu transformation
		void doTransformation();
		void undoTransformation();
		// menu synthesis
		void synthesize();
		// menu view
		void displayBWSonogram( bool );
		void displayColorSonogram( bool );
		void displayFundamentalFrequency( bool );
		void displayOriginalAudio( bool );
		void displayResidualSpectrum( bool );
		void displaySinusoidalSpectrum( bool );
		void displaySinusoidalTracks( bool );
		void displaySpectrumAndPeaks( bool );
		void displaySynthesizedSound( bool );
		void displaySynthesizedResidual( bool );
		void displaySynthesizedSinusoidal( bool );
		void backgroundBlack( bool );
		void backgroundWhite( bool );
		// menu help
		void showOnlineHelp();
		void showAboutBox();

		// status bar
		void setLeftSBLabelText(QString);
		void setRightSBLabelText(QString);

	private:
		Engine*       mEngine;
		CLAM::Thread  mThread;
		QLabel*       mLeftSBLabel;
		QLabel*       mRightSBLabel;
		bool          mShowOriginalAudio;

		SigSlot::Slotv0 mSlotAnalysisDataLoaded;

		void InitSMSTools();
		void InitialState();
		void UpdateState();
		void ShowIncomingAudio();

		void InitStatusBar();
		void InitMenuViewItems(bool flag=true);
		void ResetMenuViewAudioItems();
		void SendAnalyzedDataToViewManager();
		void SendTransformedDataToViewManager();
		void SendSynthesizedDataToViewManager();
		void LaunchMethodOnThread(CBL::Functor0 method);
		void OnAnalysisDataLoaded();

		void NotImplemented();
	};
}

#endif

