#ifndef __SMSCONFIGDLG__
#define __SMSCONFIGDLG__

#include <vector>
#include "SMSConfigDlgBase.h"
#include "SMSAnalysisSynthesisConfig.hxx"

namespace QtSMS
{
	class SMSConfigDlg : public SMSConfigDlgBase
	{
		Q_OBJECT
	public:
		SMSConfigDlg(CLAM::SMSAnalysisSynthesisConfig& config, QWidget* parent=0);
		~SMSConfigDlg();

		void Apply();
		
	private slots:
		void browseInputSoundFile();
		void browseOutputSoundFile();
		void browseInputAnalysisFile();
		void browseOutputAnalysisFile();
		void browseMorphSoundFile();

	private:
		CLAM::SMSAnalysisSynthesisConfig& mConfig;
		
		void SetValidators();
		void InitConfigDlg();
		void FillComboBoxes();
	};
}

#endif

