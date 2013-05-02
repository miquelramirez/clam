#ifndef __SMSCONFIGDLG__
#define __SMSCONFIGDLG__

#include <vector>
#include "ui_SMSConfigDlg.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"

namespace QtSMS
{
	class SMSConfigDlg : public QDialog, public Ui::SMSConfigDlg
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

