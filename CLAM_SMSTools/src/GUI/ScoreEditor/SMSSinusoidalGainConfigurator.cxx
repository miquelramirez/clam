#include "SMSSinusoidalGainConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSSinusoidalGainConfigurator::mHelpText = "<html><body><h2>Sinusoidal Gain</h2><p><strong>Usage:</strong> Introduce the gain in dB's you want to apply to the sinusoidal component. (X axis = time)</p><p><strong>Explanation:</strong> Gain is applied to sinusoidal component </p></body></html>";

	SMSSinusoidalGainConfigurator::SMSSinusoidalGainConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );

		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( -24, 24 );
		mEditorWidget->SetGridWidth( 0.1, 3 );
		
		SetHelpWidgetText();
		
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSSinusoidalGain" );
		mConfig.GetBPFAmount().Insert( 0.0, 0.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 0.0 );
		
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSSinusoidalGainConfigurator::~SMSSinusoidalGainConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSSinusoidalGainConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSSinusoidalGainConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSSinusoidalGainConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );
		
		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert( 0.0, 0.0 );
		conCfg.GetBPFAmount().Insert( 1.0, 0.0 );

	}
	
	void SMSSinusoidalGainConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);

		if ( !mConfig.HasBPFAmount() )
		{
			double value =  mConfig.GetAmount();
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0.0, value );
			mConfig.GetBPFAmount().Insert( 1.0, value );
		
		}

		mEditorWidget->Clear();
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
	
	}

	const CLAM::ProcessingConfig& SMSSinusoidalGainConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSSinusoidalGainConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSSinusoidalGainConfigurator> regtPitchShiftCfg( "SMSSinusoidalGain" );

}


