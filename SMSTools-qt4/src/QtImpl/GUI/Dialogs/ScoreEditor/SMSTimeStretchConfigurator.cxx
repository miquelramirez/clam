#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_BPF_Editor.hxx"
#include "SMSTimeStretchConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSTimeStretchConfigurator> regtPitchShiftCfg("SegmentSMSTimeStretch");
	const char* SMSTimeStretchConfigurator::mHelpText = "<html><body><h2>Time Stretch</h2><p><strong>Usage:</strong> Introduce relative increase/decrease in speed of resulting sound. 0.8 means decreasing speed by a 20% factor (X axis = time).</p><p><strong>Explanation:</strong> The resulting spectral represantion of the original sound is obtained by interpolating neiboughring frames or skipping others.</p></body></html>";

	SMSTimeStretchConfigurator::SMSTimeStretchConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mEditorWidget = new Qt_SMS_BPF_Editor;
		mEditorWidget->SetHorizontalRange(0.0, 1.0);
		mEditorWidget->SetVerticalRange(0.5, 2.0);
		mEditorWidget->SetGridWidth(0.1, 0.15);
	
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSTimeStretch");
		mConfig.GetBPFAmount().Insert(0.0, 1.0);
		mConfig.GetBPFAmount().Insert(1.0, 1.0);
		
		mEditorWidget->InitPoints( mConfig.GetBPFAmount());
		connect(mEditorWidget,SIGNAL(pointsChanged()),this,SIGNAL(configurationChanged()));
	}

	SMSTimeStretchConfigurator::~SMSTimeStretchConfigurator()
	{
		delete mEditorWidget;
		delete mHelpWidget;
	}

	QWidget* SMSTimeStretchConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	QWidget* SMSTimeStretchConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSTimeStretchConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SegmentSMSTimeStretchConfig& conCfg = static_cast<CLAM::SegmentSMSTimeStretchConfig&>(cfg);
		conCfg.RemoveAmount();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

		conCfg.GetBPFAmount().Insert(0.0, 1.0);
		conCfg.GetBPFAmount().Insert(1.0, 1.0);
	}

	void SMSTimeStretchConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SegmentSMSTimeStretchConfig&>(cfg);
		mEditorWidget->Clear();

		if(!mConfig.HasBPFAmount())
		{
			double value = mConfig.GetAmount();
			
			if(value < 0.5)
			{ 
				value = 0.5;
			}
			else if(value > 2.0)
			{
				value = 2.0;
			}

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0, value);
			mConfig.GetBPFAmount().Insert( 1, value);
		}
		else
		{
			for(int i = 0; i < mConfig.GetBPFAmount().Size(); i++)
			{
				if(mConfig.GetBPFAmount().GetValueFromIndex( i ) < 0.5)
				{
					mConfig.GetBPFAmount().SetValue(i, 0.5);
				}
				else if(mConfig.GetBPFAmount().GetValueFromIndex(i) > 2.0)
				{
					mConfig.GetBPFAmount().SetValue(i, 2.0);
				}
			}

			if(mConfig.GetBPFAmount().Size() == 0)
			{
				mConfig.GetBPFAmount().Insert(0.0, 1.0);
				mConfig.GetBPFAmount().Insert(1.0, 1.0);
			}
		}

		mEditorWidget->InitPoints(mConfig.GetBPFAmount());

	}

	const CLAM::ProcessingConfig& SMSTimeStretchConfigurator::GetConfig()
	{
		mConfig.GetBPFAmount()=mEditorWidget->GetBPF();
		return mConfig;
	}

}

// END

