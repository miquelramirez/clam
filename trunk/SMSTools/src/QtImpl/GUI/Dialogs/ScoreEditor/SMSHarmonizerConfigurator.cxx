#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSHarmonizerConfigurator.hxx"

namespace QtSMS
{
	const char* SMSHarmonizerConfigurator::mHelpText = "<html><body><h2>Harmonizer</h2><p><strong>Usage:</strong> Each point in the fuction defines a new voice added to the harmonization. The X value is the gain in relation to the original one and the Y value the pitch transposition factor.</p><p><strong>Explanation:</strong> Pitch transposed (with timbre preservation) versions of the original are added applying the gain factor specified.</p></body></html>";

	SMSHarmonizerConfigurator::SMSHarmonizerConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(-12.0, 12.0);
		mEditorWidget->SetVerticalRange(0.5, 2.0);
		mEditorWidget->SetGridWidth(3.0, 0.15);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSHarmonizer");

		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSHarmonizerConfigurator::~SMSHarmonizerConfigurator()
	{
		delete mHelpWidget;
		delete mEditorWidget;
	}

	QWidget* SMSHarmonizerConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSHarmonizerConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSHarmonizerConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);		

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();
	}

	void SMSHarmonizerConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);

		if(!mConfig.HasBPFAmount())
		{
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}
	
		mEditorWidget->Clear();
		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
	}

	const CLAM::ProcessingConfig& SMSHarmonizerConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

	static SMSConfiguratorFactory::Registrator<SMSHarmonizerConfigurator> regtPitchShiftCfg("SMSHarmonizer");
}

// END

