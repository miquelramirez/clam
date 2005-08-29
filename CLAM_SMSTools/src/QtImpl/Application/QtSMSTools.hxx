#ifndef __QTSMSTOOLS__
#define __QTSMSTOOLS__

#include "SMSToolsBase.h"

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
		// menu help
		void showOnlineHelp();
		void showLicense();
		void showAboutBox();

	private:
		Engine* mEngine;

		void InitSMSTools();
		void InitialState();
		void UpdateState();
		void ShowIncomingAudio();

		void NotImplemented();
	};
}

#endif

