#include "SMSFreqShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSFreqShiftConfigurator::mHelpText = "<html><body><h2>Frequency Shift</h2><p><strong>Usage:</strong> Frequency shift applied to all partials expressed in   Hz.(X axis = time)</p><p><strong>Explanation:</strong> All components are shifted the amount expressed in Hz in the score.</p></body></html>";

	SMSFreqShiftConfigurator::SMSFreqShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( -1000, 1000 );
		mEditorWidget->SetGridWidth( 0.1, 100 );
	
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSFreqShift" );
		mConfig.GetBPFAmount().Insert( 0.0, 0.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 0.0 );
		
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSFreqShiftConfigurator::~SMSFreqShiftConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSFreqShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText );
	}

	Fl_Widget* SMSFreqShiftConfigurator::GetParametersWidget()
	{

		return mEditorWidget;
	}
	
	void SMSFreqShiftConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );
		
		if ( conCfg.HasBPFAmount() )
		{
			conCfg.RemoveBPFAmount();
			conCfg.UpdateData();
		}
		
		conCfg.AddBPFAmount();
		conCfg.RemoveAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert( 0.0, 0.0 );
		conCfg.GetBPFAmount().Insert( 1.0, 0.0 );
	
	}
	
	void SMSFreqShiftConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
		mEditorWidget->Clear();
		
		if ( !mConfig.HasBPFAmount() )
			mEditorWidget->InitPoints( mConfig.GetAmount() );
		else
			mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
	}

	const CLAM::ProcessingConfig& SMSFreqShiftConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSFreqShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}


static SMSConfiguratorFactory::Registrator<SMSFreqShiftConfigurator> regtFreqShiftCfg( "SMSFreqShift" );



}

