#include "SMSPitchDiscretizationConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>


namespace CLAMVM
{
	const char* SMSPitchDiscretizationConfigurator::mHelpText = "<html><body><h2>Pitch Discretization to Temperate Scale</h2><p><strong>Usage:</strong> If active it rounds the pitch to nearest note according to temperate musical scale.</p><p><strong>Explanation:</strong> It is just a particular case of pitch shift with timbre preservation where the pitch is quantified to one of the 12 semitones of an octave.</p></body></html>";

	SMSPitchDiscretizationConfigurator::SMSPitchDiscretizationConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.UpdateData();
		mConfig.SetType( "SMSPitchDiscretization" );
	}

	SMSPitchDiscretizationConfigurator::~SMSPitchDiscretizationConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
	}

	void SMSPitchDiscretizationConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSPitchDiscretizationConfigurator::GetParametersWidget()
	{
		return NULL;
	}

	void SMSPitchDiscretizationConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
	}

	void SMSPitchDiscretizationConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
	}

	const CLAM::ProcessingConfig& SMSPitchDiscretizationConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSPitchDiscretizationConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSPitchDiscretizationConfigurator> regtPitchShiftCfg( "SMSPitchDiscretization" );

}


