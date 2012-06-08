#include <CLAM/Factory.hxx>
#include "SMS_ScoreEditor_Helper.hxx"
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSPitchShiftConfigurator.hxx"

namespace QtSMS
{

	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSPitchShiftConfigurator> regtPitchShiftCfg("SMSPitchShift");

	const char* SMSPitchShiftConfigurator::mHelpText = "<html><body><h2>Pitch Shift with Timbre Preservation</h2><p><strong>Usage:</strong> Factor to apply to current pitch. The default 1 value means no change, a factor of 0.5 means an octave lower. (X axis = time)</p><p><strong>Explanation:</strong> Pitch shift with timbre preservation works by first extracting spectral envelope of the sinusoidal component. Then peaks are moved multiplying their original frequency by the value of the transformation. After that, the original spectral envelope is applied back. Residual component is just comb-filtered using a filter with maximums at new fundamental and harmonics.</p></body></html>";

	SMSPitchShiftConfigurator::SMSPitchShiftConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 1.0);
		mEditorWidget->SetVerticalRange(0.5, 2.0);
		mEditorWidget->SetGridWidth(0.1, 0.1);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSPitchShift");
		mConfig.GetBPFAmount().Insert(0.0, 1.0);
		mConfig.GetBPFAmount().Insert(1.0, 1.0);

		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSPitchShiftConfigurator::~SMSPitchShiftConfigurator()
	{
		delete mEditorWidget;
		delete mHelpWidget;
	}

	QWidget* SMSPitchShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSPitchShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSPitchShiftConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
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

	void SMSPitchShiftConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);
		
		if(!mConfig.HasBPFAmount())
		{
			double oldAmount = mConfig.GetAmount();

			if(oldAmount < 0.5) 
			{
				oldAmount = 0.5;
			}
			else if(oldAmount > 2.0) 
			{
				oldAmount = 2.0;
			}

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();

			mConfig.GetBPFAmount().Insert(0.0, oldAmount);
			mConfig.GetBPFAmount().Insert(1.0, oldAmount);
		}
		else
		{
			CLAMVM::clampBPFValues(mConfig.GetBPFAmount(), 0.5, 2.0);
		}

		mEditorWidget->Clear();
		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
	}

	const CLAM::ProcessingConfig& SMSPitchShiftConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

}

// END

