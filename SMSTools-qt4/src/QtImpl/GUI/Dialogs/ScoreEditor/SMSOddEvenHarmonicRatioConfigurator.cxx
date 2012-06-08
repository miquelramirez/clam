#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSOddEvenHarmonicRatioConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSOddEvenHarmonicRatioConfigurator> regtPitchShiftCfg("SMSOddEvenHarmonicRatio");
	const char* SMSOddEvenHarmonicRatioConfigurator::mHelpText = "<html><body><h2>Odd/Even Harmonic Ratio</h2><p><strong>Usage:</strong> Indicate in dB's the gain applied to odd harmonics in relation to even. E.g. A value of 6 means that a 6dB difference will be introduced, thus, odd harmonics will be 3dB higher and even harmonic 3dB lower.(X axis = time)</p><p><strong>Explanation:</strong> Harmonics are modified according to their harmonic number. </p></body></html>";

	SMSOddEvenHarmonicRatioConfigurator::SMSOddEvenHarmonicRatioConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 1.0);
		mEditorWidget->SetVerticalRange(-12.0, 12.0);
		mEditorWidget->SetGridWidth(0.1, 3.0);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSOddEvenHarmonicRatio" );
		mConfig.GetBPFAmount().Insert( 0.0, 0.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 0.0 );
		
		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSOddEvenHarmonicRatioConfigurator::~SMSOddEvenHarmonicRatioConfigurator()
	{
		delete mHelpWidget;
		delete mEditorWidget;
	}

	QWidget* SMSOddEvenHarmonicRatioConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSOddEvenHarmonicRatioConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSOddEvenHarmonicRatioConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert(0.0, 0.0);
		conCfg.GetBPFAmount().Insert(1.0, 0.0);
	}

	void SMSOddEvenHarmonicRatioConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);
		mEditorWidget->Clear();
		
		if(!mConfig.HasBPFAmount())
		{
			double value = mConfig.GetAmount();
			
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert(0.0, value);
			mConfig.GetBPFAmount().Insert(1.0, value);
		}
			
		mEditorWidget->InitPoints(mConfig.GetBPFAmount());
	}

	const CLAM::ProcessingConfig& SMSOddEvenHarmonicRatioConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}


}

// END

