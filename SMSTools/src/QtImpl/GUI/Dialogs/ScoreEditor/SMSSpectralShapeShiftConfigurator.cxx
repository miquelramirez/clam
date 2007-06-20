#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSSpectralShapeShiftConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSSpectralShapeShiftConfigurator> regtPitchShiftCfg("SMSSpectralShapeShift");
	const char* SMSSpectralShapeShiftConfigurator::mHelpText = "<html><body><h2>Spectral Shape Shift</h2><p><strong>Usage:</strong> Introduce frequency shift applied to the sinusoidal spectral shape expressed in Hz (X axis = time).</p><p><strong>Explanation:</strong> Spectral shape of the sinusoidal component is extracted and then shifted in frequency according to the value specified.</p></body></html>";

	SMSSpectralShapeShiftConfigurator::SMSSpectralShapeShiftConfigurator()
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
		mConfig.SetType("SMSSpectralShapeShift");
		mConfig.GetBPFAmount().Insert(0.0, 1.0);
		mConfig.GetBPFAmount().Insert(1.0, 1.0);

		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSSpectralShapeShiftConfigurator::~SMSSpectralShapeShiftConfigurator()
	{
		delete mEditorWidget;
		delete mHelpWidget;
	}

	QWidget* SMSSpectralShapeShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSSpectralShapeShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSSpectralShapeShiftConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);

		conCfg.RemoveBPFAmount();
		conCfg.RemoveAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert(0.0, 1.0);
		conCfg.GetBPFAmount().Insert(1.0, 1.0);	
	}

	void SMSSpectralShapeShiftConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);
		mEditorWidget->Clear();
		if(mConfig.HasBPFAmount())
		{
			mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		}
		else
		{
			if(mConfig.GetAmount() >= 0.5)
			{
				mEditorWidget->InitPoints(mConfig.GetAmount());
			}
			else
			{
				mEditorWidget->InitPoints(1.0);
			}
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}
	}

	const CLAM::ProcessingConfig& SMSSpectralShapeShiftConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

}

// END

