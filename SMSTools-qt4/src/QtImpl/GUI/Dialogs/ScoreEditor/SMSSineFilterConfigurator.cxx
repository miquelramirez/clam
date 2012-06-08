#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSSineFilterConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSSineFilterConfigurator> regtPitchShiftCfg("SMSSineFilter");
	const char* SMSSineFilterConfigurator::mHelpText = "<html><body><h2>Sinusoidal Filter</h2><p><strong>Usage:</strong> Introduce the frequency response of the filter you want to apply to the sinusoidal component. (X axis = frequency, Y axis = gain in dB's)</p><p><strong>Explanation:</strong> Sinusoidal component is filtered according to   the frequency response introduced.</p></body></html>";

	SMSSineFilterConfigurator::SMSSineFilterConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 200.0);
		mEditorWidget->SetVerticalRange(-24.0, 24.0);
		mEditorWidget->SetGridWidth(20.0, 3.0);

		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSSineFilter");
		mConfig.GetBPFAmount().Insert(0.0, 0.0);
		mConfig.GetBPFAmount().Insert(200.0, 0.0);

		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSSineFilterConfigurator::~SMSSineFilterConfigurator()
	{
		delete mEditorWidget;
		delete mHelpWidget;
	}

	QWidget* SMSSineFilterConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSSineFilterConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSSineFilterConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);
		
		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert(0.0, 0.0);
		conCfg.GetBPFAmount().Insert(200.0, 0.0);
	}

	void SMSSineFilterConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig& >(cfg);
		mEditorWidget->Clear();		

		if(!mConfig.HasBPFAmount())
		{
			double value = mConfig.GetAmount();
			
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert(0.0, value);
			mConfig.GetBPFAmount().Insert(200.0, value);
		}

		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
	}

	const CLAM::ProcessingConfig& SMSSineFilterConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}


}

// END

