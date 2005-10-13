#include <qfiledialog.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qvalidator.h>
#include "SMSConfigDlg.hxx"

namespace QtSMS
{
	SMSConfigDlg::SMSConfigDlg(CLAM::SMSAnalysisSynthesisConfig& config, QWidget* parent) 
		: SMSConfigDlgBase(parent)
		, mConfig(config)
	{
		InitConfigDlg();
	}

	SMSConfigDlg::~SMSConfigDlg(){}

	void SMSConfigDlg::Apply()
	{
		// file page
		if(!mCfgNameLineEdit->text().isEmpty())
		{
			mConfig.SetName(mCfgNameLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetName("");
		}
		if(!mInputSoundLineEdit->text().isEmpty())
		{
			mConfig.SetInputSoundFile(mInputSoundLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetInputSoundFile("");
		}
		if(!mOutputSoundLineEdit->text().isEmpty())
		{
			mConfig.SetOutputSoundFile(mOutputSoundLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetOutputSoundFile("");
		}
		if(!mInputAnalysisLineEdit->text().isEmpty())
		{
			mConfig.SetInputAnalysisFile(mInputAnalysisLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetInputAnalysisFile("");
		}
		if(!mOutputAnalysisLineEdit->text().isEmpty())
		{
			mConfig.SetOutputAnalysisFile(mOutputAnalysisLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetOutputAnalysisFile("");
		}
		if(!mMorphSoundFileLineEdit->text().isEmpty())
		{
			mConfig.SetMorphSoundFile(mMorphSoundFileLineEdit->text().ascii());
		}
		else
		{
			mConfig.SetMorphSoundFile("");
		}

		// analysis page
		if(!mAnalysisWindowSizeLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisWindowSize(CLAM::TSize(mAnalysisWindowSizeLineEdit->text().toInt()));
		}
		if(!mAnalysisHopSizeLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisHopSize(CLAM::TSize(mAnalysisHopSizeLineEdit->text().toInt()));
		}
		if(!mResAnalysisWindowSizeLineEdit->text().isEmpty())
		{
			mConfig.SetResAnalysisWindowSize(CLAM::TSize(mResAnalysisWindowSizeLineEdit->text().toInt()));
		}
		if(!mAnalysisZeroPaddingFactorLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisZeroPaddingFactor(CLAM::TSize(mAnalysisZeroPaddingFactorLineEdit->text().toInt()));
		}
		if(!mAnalysisPeakDetectMagThresholdLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisPeakDetectMagThreshold(CLAM::TData(mAnalysisPeakDetectMagThresholdLineEdit->text().toDouble()));
		}
		if(!mAnalysisPeakDetectMaxFreqLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisPeakDetectMaxFreq(CLAM::TSize(mAnalysisPeakDetectMaxFreqLineEdit->text().toInt()));
		}
		if(!mAnalysisSinTrackingFreqDeviationLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisSinTrackingFreqDeviation(CLAM::TData(mAnalysisSinTrackingFreqDeviationLineEdit->text().toDouble()));
		}
		if(!mAnalysisReferenceFundFreqLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisReferenceFundFreq(CLAM::TData(mAnalysisReferenceFundFreqLineEdit->text().toDouble()));
		}
		if(!mAnalysisLowestFundFreqLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisLowestFundFreq(CLAM::TData(mAnalysisLowestFundFreqLineEdit->text().toDouble()));
		}
		if(!mAnalysisHighestFundFreqLineEdit->text().isEmpty())
		{
			mConfig.SetAnalysisHighestFundFreq(CLAM::TData(mAnalysisHighestFundFreqLineEdit->text().toDouble()));
		}
		mConfig.SetAnalysisWindowType(CLAM::EWindowType::ValueTable()[mAnalysisWindowTypeComboBox->currentItem()].value);
		mConfig.SetResAnalysisWindowType(CLAM::EWindowType::ValueTable()[mResAnalysisWindowTypeComboBox->currentItem()].value);
		mConfig.SetAnalysisHarmonic(mHarmonicAnalysisCheckBox->isChecked());

		// tracks page
		if(!mCleanTracksMinLengthLineEdit->text().isEmpty())
		{
			mConfig.SetCleanTracksMinLength(CLAM::TSize(mCleanTracksMinLengthLineEdit->text().toInt()));
		}
		if(!mCleanTracksMaxDropOutLineEdit->text().isEmpty())
		{
			mConfig.SetCleanTracksMaxDropOut(CLAM::TSize(mCleanTracksMaxDropOutLineEdit->text().toInt()));
		}
		mConfig.SetDoCleanTracks(mCleanTracksCheckBox->isChecked());

		// synthesis page
		if(!mSynthFrameSizeLineEdit->text().isEmpty())
		{
			mConfig.SetSynthesisFrameSize(CLAM::TSize(mSynthFrameSizeLineEdit->text().toInt()));
		}
		mConfig.SetSynthesisWindowType(CLAM::EWindowType::ValueTable()[mSynthWindowTypeComboBox->currentItem()].value);
	}

	void SMSConfigDlg::browseInputSoundFile()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mInputSoundLineEdit->setText(filename);
	}

	void SMSConfigDlg::browseOutputSoundFile()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mOutputSoundLineEdit->setText(filename);
	}

	void SMSConfigDlg::browseInputAnalysisFile()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		mInputAnalysisLineEdit->setText(filename);
	}

	void SMSConfigDlg::browseOutputAnalysisFile()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		mOutputAnalysisLineEdit->setText(filename);
	}

	void SMSConfigDlg::browseMorphSoundFile()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mMorphSoundFileLineEdit->setText(filename);
	}

	void SMSConfigDlg::InitConfigDlg()
	{
		// init validators and fill dialog
		SetValidators();
		FillComboBoxes();

		// file page
		mInputSoundLineEdit->setText(mConfig.GetInputSoundFile());
		mOutputSoundLineEdit->setText(mConfig.GetOutputSoundFile());
		mInputAnalysisLineEdit->setText(mConfig.GetInputAnalysisFile());
		mOutputAnalysisLineEdit->setText(mConfig.GetOutputAnalysisFile());
		mMorphSoundFileLineEdit->setText(mConfig.GetMorphSoundFile());

		// analysis page
		mAnalysisWindowSizeLineEdit->setText(QString::number(mConfig.GetAnalysisWindowSize()));
		mAnalysisHopSizeLineEdit->setText(QString::number(mConfig.GetAnalysisHopSize()));
		mResAnalysisWindowSizeLineEdit->setText(QString::number(mConfig.GetResAnalysisWindowSize()));
		mAnalysisZeroPaddingFactorLineEdit->setText(QString::number(mConfig.GetAnalysisZeroPaddingFactor()));
		mAnalysisPeakDetectMagThresholdLineEdit->setText(QString::number(mConfig.GetAnalysisPeakDetectMagThreshold(),'f',0));
		mAnalysisPeakDetectMaxFreqLineEdit->setText(QString::number(mConfig.GetAnalysisPeakDetectMaxFreq()));
		mAnalysisSinTrackingFreqDeviationLineEdit->setText(QString::number(mConfig.GetAnalysisSinTrackingFreqDeviation(),'f',0));
		mAnalysisReferenceFundFreqLineEdit->setText(QString::number(mConfig.GetAnalysisReferenceFundFreq(),'f',0));
		mAnalysisLowestFundFreqLineEdit->setText(QString::number(mConfig.GetAnalysisLowestFundFreq(),'f',0));
		mAnalysisHighestFundFreqLineEdit->setText(QString::number(mConfig.GetAnalysisHighestFundFreq(),'f',0));
		mAnalysisWindowTypeComboBox->setCurrentItem(mConfig.GetAnalysisWindowType());
		mResAnalysisWindowTypeComboBox->setCurrentItem(mConfig.GetResAnalysisWindowType());
		mHarmonicAnalysisCheckBox->setChecked(mConfig.GetAnalysisHarmonic());

		// tracks page
		mCleanTracksMinLengthLineEdit->setText(QString::number(mConfig.GetCleanTracksMinLength()));
		mCleanTracksMaxDropOutLineEdit->setText(QString::number(mConfig.GetCleanTracksMaxDropOut()));
		mCleanTracksCheckBox->setChecked(mConfig.GetDoCleanTracks());

		// synthesis page
		mSynthFrameSizeLineEdit->setText(QString::number(mConfig.GetSynthesisFrameSize()));
		mSynthWindowTypeComboBox->setCurrentItem(mConfig.GetSynthesisWindowType());		
	}

	void SMSConfigDlg::SetValidators()
	{
		mAnalysisWindowSizeLineEdit->setValidator(new QIntValidator(this));
		mAnalysisHopSizeLineEdit->setValidator(new QIntValidator(this));
		mResAnalysisWindowSizeLineEdit->setValidator(new QIntValidator(this));
		mAnalysisZeroPaddingFactorLineEdit->setValidator(new QIntValidator(this));
		mAnalysisPeakDetectMaxFreqLineEdit->setValidator(new QIntValidator(this));

		mAnalysisPeakDetectMagThresholdLineEdit->setValidator(new QDoubleValidator(this));
		mAnalysisSinTrackingFreqDeviationLineEdit->setValidator(new QDoubleValidator(this));
		mAnalysisReferenceFundFreqLineEdit->setValidator(new QDoubleValidator(this));
		mAnalysisLowestFundFreqLineEdit->setValidator(new QDoubleValidator(this));
		mAnalysisHighestFundFreqLineEdit->setValidator(new QDoubleValidator(this));

		mCleanTracksMinLengthLineEdit->setValidator(new QIntValidator(this));
		mCleanTracksMaxDropOutLineEdit->setValidator(new QIntValidator(this));

		mSynthFrameSizeLineEdit->setValidator(new QIntValidator(this));
	}

	void SMSConfigDlg::FillComboBoxes()
	{
		
		for(int i=0; CLAM::EWindowType::ValueTable()[i].name != NULL; i++)
		{
			const char* item =  CLAM::EWindowType::ValueTable()[i].name;
			mAnalysisWindowTypeComboBox->insertItem(item);
			mResAnalysisWindowTypeComboBox->insertItem(item);
			mSynthWindowTypeComboBox->insertItem(item);
		}
	}
}

// END

