#include <qlabel.h>
#include <CLAM/Factory.hxx>
#include "Qt_Help_View.hxx"
#include "SMSPitchDiscretizationConfigurator.hxx"

namespace QtSMS
{
	static CLAM::FactoryRegistrator<SMSConfiguratorFactory, SMSPitchDiscretizationConfigurator> regtPitchShiftCfg("SMSPitchDiscretization");
	const char* SMSPitchDiscretizationConfigurator::mHelpText = "<html><body><h2>Pitch Discretization to Temperate Scale</h2><p><strong>Usage:</strong> If active it rounds the pitch to nearest note according to temperate musical scale.</p><p><strong>Explanation:</strong> It is just a particular case of pitch shift with timbre preservation where the pitch is quantified to one of the 12 semitones of an octave.</p></body></html>";

	SMSPitchDiscretizationConfigurator::SMSPitchDiscretizationConfigurator()
	{
		mHelpWidget = new Qt_Help_View;
		mHelpWidget->SetText(mHelpText);

		mLabelMsg = new QLabel("Non-editable configuration",0);
		mLabelMsg->setFont(QFont("Sans",12));
		mLabelMsg->setAlignment(Qt::AlignCenter);

		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSPitchDiscretization" );
	}

	SMSPitchDiscretizationConfigurator::~SMSPitchDiscretizationConfigurator()
	{
		delete mHelpWidget;
	}

	QWidget* SMSPitchDiscretizationConfigurator::GetParametersWidget()
	{
		return mLabelMsg;
	}

	QWidget* SMSPitchDiscretizationConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	void SMSPitchDiscretizationConfigurator::Initialize(CLAM::ProcessingConfig&)
	{
		/* empty body */
	}

	void SMSPitchDiscretizationConfigurator::SetConfig(const CLAM::ProcessingConfig& cfg)
	{
		mConfig = static_cast<const CLAM::SegmentTransformationConfig&>(cfg);
	}

	const CLAM::ProcessingConfig& SMSPitchDiscretizationConfigurator::GetConfig()
	{
		return mConfig;
	}


}

// END

