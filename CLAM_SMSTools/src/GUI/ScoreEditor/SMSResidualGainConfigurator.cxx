#include "SMSResidualGainConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSResidualGainConfigurator::mHelpText = "<html><body><h2>Residual Gain</h2><p><strong>Usage:</strong> Introduce the gain in dB's you want to apply to the residual component. (X axis = time)</p><p><strong>Explanation:</strong> Gain is applied to residual component </p></body></html>";

	SMSResidualGainConfigurator::SMSResidualGainConfigurator()
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
		mConfig.SetType( "SMSResidualGain" );
		mConfig.GetBPFAmount().Insert( 0.0, 1.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 1.0 );

		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSResidualGainConfigurator::~SMSResidualGainConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSResidualGainConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSResidualGainConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSResidualGainConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

		conCfg.GetBPFAmount().Insert( 0.0, 1.0 );
		conCfg.GetBPFAmount().Insert( 1.0, 1.0 );
		
	}

	void SMSResidualGainConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);

		if ( !mConfig.HasBPFAmount() )
		{
			double value = mConfig.GetAmount();

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0.0, value );
			mConfig.GetBPFAmount().Insert( 1.0, value );

		}

		mEditorWidget->Clear();
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );


	}

	const CLAM::ProcessingConfig& SMSResidualGainConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSResidualGainConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSResidualGainConfigurator> regtPitchShiftCfg( "SMSResidualGain" );

}


