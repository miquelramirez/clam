#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSFreqShiftConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSFreqShiftConfigurator> regtFreqShiftCfg("SMSFreqShift");

	const char* SMSFreqShiftConfigurator::mHelpText = "<html><body><h2>Frequency Shift</h2><p><strong>Usage:</strong> Frequency shift applied to all partials expressed in   Hz.(X axis = time)</p><p><strong>Explanation:</strong> All components are shifted the amount expressed in Hz in the score.</p></body></html>";

	SMSFreqShiftConfigurator::SMSFreqShiftConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 1.0);
		mEditorWidget->SetVerticalRange(-1000.0, 1000.0);
		mEditorWidget->SetGridWidth(0.1, 100.0);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSFreqShift");
		mConfig.GetBPFAmount().Insert(0.0, 0.0);
		mConfig.GetBPFAmount().Insert(1.0, 0.0);
		
		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSFreqShiftConfigurator::~SMSFreqShiftConfigurator()
	{
		delete mHelpWidget;
		delete mEditorWidget;
	}

	QWidget* SMSFreqShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSFreqShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	
	void SMSFreqShiftConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);
		
		if(conCfg.HasBPFAmount())
		{
			conCfg.RemoveBPFAmount();
			conCfg.UpdateData();
		}
		
		conCfg.AddBPFAmount();
		conCfg.RemoveAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert(0.0, 0.0);
		conCfg.GetBPFAmount().Insert(1.0, 0.0);
	}
	
	void SMSFreqShiftConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig& >(cfg);
		mEditorWidget->Clear();
		
		if(!mConfig.HasBPFAmount())
		{
			mEditorWidget->InitPoints(mConfig.GetAmount());
		}
		else
		{
			mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		}

		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
	}

	const CLAM::ProcessingConfig& SMSFreqShiftConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

}

// END

