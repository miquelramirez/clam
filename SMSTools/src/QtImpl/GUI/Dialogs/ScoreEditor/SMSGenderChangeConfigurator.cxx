#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "Qt_SMS_Gender_Selector.hxx"
#include "SMSGenderChangeConfigurator.hxx"

namespace QtSMS
{
	const char* SMSGenderChangeConfigurator::mHelpText = "<html><body><h2>Gender Change</h2><p><strong>Usage:</strong> If amount is 0 it means from male to female. If it is 1 (or anything different than 0) it means from female to male. </p><p><strong>Explanation:</strong> Gender change works by making a pitch shift with timbre preservation an octave and then shifting the spectral shape.</p></body></html>";

	SMSGenderChangeConfigurator::SMSGenderChangeConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);
		
		mConfig.AddType();
		mConfig.AddAmount();
		mConfig.UpdateData();
		mConfig.SetType("SMSGenderChange");
		mConfig.SetAmount(0);

		mGenderSelector = new Qt_SMS_Gender_Selector;
		connect(mGenderSelector,SIGNAL(genderChanged(int)),SLOT(onGenderChanged(int)));
	}

	SMSGenderChangeConfigurator::~SMSGenderChangeConfigurator()
	{
		delete mHelpWidget;
		delete mGenderSelector;
	}

	QWidget* SMSGenderChangeConfigurator::GetParametersWidget()
	{
		return mGenderSelector;
	}

	QWidget* SMSGenderChangeConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSGenderChangeConfigurator::Initialize(CLAM::ProcessingConfig& cfg)
	{
		CLAM::SegmentTransformationConfig& conCfg = static_cast<CLAM::SegmentTransformationConfig&>(cfg);
		
		conCfg.AddAmount();
		conCfg.UpdateData();
		conCfg.SetAmount(0);
	}

	void SMSGenderChangeConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);
	}

	const CLAM::ProcessingConfig& SMSGenderChangeConfigurator::GetConfig()
	{
		return mConfig;
	}
		
	void SMSGenderChangeConfigurator::onGenderChanged(int gender)
	{
		mConfig.SetAmount(gender);
		emit configurationChanged();
	}

	static SMSConfiguratorFactory::Registrator<SMSGenderChangeConfigurator> regtPitchShiftCfg("SMSGenderChange");
}

// END

