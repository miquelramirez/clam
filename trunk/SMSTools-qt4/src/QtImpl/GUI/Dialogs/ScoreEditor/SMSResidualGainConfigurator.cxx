#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSResidualGainConfigurator.hxx"

namespace QtSMS
{
	const char* SMSResidualGainConfigurator::mHelpText = "<html><body><h2>Residual Gain</h2><p><strong>Usage:</strong> Introduce the gain in dB's you want to apply to the residual component. (X axis = time)</p><p><strong>Explanation:</strong> Gain is applied to residual component </p></body></html>";

	SMSResidualGainConfigurator::SMSResidualGainConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 1.0);
		mEditorWidget->SetVerticalRange(-24.0, 24.0);
		mEditorWidget->SetGridWidth(0.1, 3.0);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();

		mConfig.UpdateData();
		mConfig.SetType("SMSResidualGain");
		mConfig.GetBPFAmount().Insert(0.0, 1.0);
		mConfig.GetBPFAmount().Insert(1.0, 1.0);

		mEditorWidget->InitPoints( mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSResidualGainConfigurator::~SMSResidualGainConfigurator()
	{
		delete mEditorWidget;
		delete mHelpWidget;
	}

	QWidget* SMSResidualGainConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSResidualGainConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSResidualGainConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

		conCfg.GetBPFAmount().Insert(0.0, 1.0);
		conCfg.GetBPFAmount().Insert(1.0, 1.0);
	}

	void SMSResidualGainConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);

		if(!mConfig.HasBPFAmount())
		{
			double value = mConfig.GetAmount();

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert(0.0, value);
			mConfig.GetBPFAmount().Insert(1.0, value);
		}

		mEditorWidget->Clear();
		mEditorWidget->InitPoints( mConfig.GetBPFAmount());
	}

	const CLAM::ProcessingConfig& SMSResidualGainConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSResidualGainConfigurator> regtPitchShiftCfg("SMSResidualGain");
}

// END

