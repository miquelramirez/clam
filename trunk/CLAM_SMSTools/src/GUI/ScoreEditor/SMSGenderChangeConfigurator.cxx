#include "SMSGenderChangeConfigurator.hxx"
#include "Factory.hxx"
#include "Assert.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_Gender_Selector.hxx"

namespace CLAMVM
{
	const char* SMSGenderChangeConfigurator::mHelpText = "<html><body><h2>Gender Change</h2><p><strong>Usage:</strong> If amount is 0 it means from male to female. If it is 1 (or anything different than 0) it means from female to male. </p><p><strong>Explanation:</strong> Gender change works by making a pitch shift with timbre preservation an octave and then shifting the spectral shape.</p></body></html>";

	SMSGenderChangeConfigurator::SMSGenderChangeConfigurator()
		: mHelpWidget( NULL ), mpGenderSelector( NULL )
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		CLAM_ASSERT( mHelpWidget != NULL, "Allocation failed" );
		mHelpWidget->textsize( 12 );
		
		mpGenderSelector = new Fl_SMS_Gender_Selector( 0, 0, 100, 100 );
		CLAM_ASSERT( mpGenderSelector != NULL, "Allocation failed" );


		SetGender.Wrap( this, &SMSGenderChangeConfigurator::OnGenderSet ); 

		mpGenderSelector->GenderSelected.Connect( SetGender );
		GenderChanged.Connect( mpGenderSelector->SetSelectedGender );

		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.AddAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSGenderChange" );
		mConfig.SetAmount( 0 );
		GenderChanged.Emit( (int)mConfig.GetAmount() );
	}

	SMSGenderChangeConfigurator::~SMSGenderChangeConfigurator()
	{
		if ( mHelpWidget!=NULL && mHelpWidget->parent() != NULL )
			delete mHelpWidget;

		if ( mpGenderSelector!=NULL && mpGenderSelector->parent() != NULL )
			delete mpGenderSelector;
	}

	void SMSGenderChangeConfigurator::OnGenderSet( int gender )
	{
		mConfig.SetAmount( gender );
		ConfigurationChanged.Emit();
	}

	void SMSGenderChangeConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSGenderChangeConfigurator::GetParametersWidget()
	{
		return mpGenderSelector;
	}

	void SMSGenderChangeConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );
		
		conCfg.AddAmount();
		conCfg.UpdateData();

		conCfg.SetAmount( 0 );

	}

	void SMSGenderChangeConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
		
		GenderChanged.Emit( (int)mConfig.GetAmount() );

	}

	const CLAM::ProcessingConfig& SMSGenderChangeConfigurator::GetConfig()
	{
		return mConfig;
	}

	Fl_Widget* SMSGenderChangeConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSGenderChangeConfigurator> regtPitchShiftCfg( "SMSGenderChange" );

}



