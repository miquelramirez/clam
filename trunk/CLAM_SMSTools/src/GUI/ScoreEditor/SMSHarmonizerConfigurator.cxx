#include "SMSHarmonizerConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSHarmonizerConfigurator::mHelpText = "<html><body><h2>Harmonizer</h2><p><strong>Usage:</strong> Each point in the fuction defines a new voice added to the harmonization. The X value is the gain in relation to the original one and the Y value the pitch transposition factor.</p><p><strong>Explanation:</strong> Pitch transposed (with timbre preservation) versions of the original are added applying the gain factor specified.</p></body></html>";

	SMSHarmonizerConfigurator::SMSHarmonizerConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( -12, 12 );
		mEditorWidget->SetVerticalRange( 0.5, 2.0 );
		mEditorWidget->SetGridWidth( 3, 0.15 );

		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSHarmonizer" );


		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSHarmonizerConfigurator::~SMSHarmonizerConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSHarmonizerConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSHarmonizerConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );		

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

	}

	
	void SMSHarmonizerConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);

		if ( !mConfig.HasBPFAmount() )
		{
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}
	
		mEditorWidget->Clear();
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
	}

	const CLAM::ProcessingConfig& SMSHarmonizerConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );
		return mConfig;
	}

	Fl_Widget* SMSHarmonizerConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSHarmonizerConfigurator> regtPitchShiftCfg( "SMSHarmonizer" );


}


