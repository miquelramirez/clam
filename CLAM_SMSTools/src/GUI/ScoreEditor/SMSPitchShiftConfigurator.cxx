#include "SMSPitchShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"
#include "SMS_ScoreEditor_Helper.hxx"

namespace CLAMVM
{
	const char* SMSPitchShiftConfigurator::mHelpText = "<html><body><h2>Pitch Shift with Timbre Preservation</h2><p><strong>Usage:</strong> Factor to apply to current pitch. The default 1 value means no change, a factor of 0.5 means an octave lower. (X axis = time)</p><p><strong>Explanation:</strong> Pitch shift with timbre preservation works by first extracting spectral envelope of the sinusoidal component. Then peaks are moved multiplying their original frequency by the value of the transformation. After that, the original spectral envelope is applied back. Residual component is just comb-filtered using a filter with maximums at new fundamental and harmonics.</p></body></html>";

	SMSPitchShiftConfigurator::SMSPitchShiftConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );

		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( 0.5, 2.0 );
		mEditorWidget->SetGridWidth( 0.1, 0.1 );
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSPitchShift" );
		mConfig.GetBPFAmount().Insert( 0.0, 1.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 1.0 );
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSPitchShiftConfigurator::~SMSPitchShiftConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSPitchShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSPitchShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSPitchShiftConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
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

	void SMSPitchShiftConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast< const CLAM::SMSTransformationConfig& >( cfg );
		
		if ( !mConfig.HasBPFAmount() )
		{
			double oldAmount = mConfig.GetAmount();

			if ( oldAmount < 0.5 ) oldAmount = 0.5;
			else if ( oldAmount > 2.0 ) oldAmount = 2.0;

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();

			mConfig.GetBPFAmount().Insert( 0.0, oldAmount );
			mConfig.GetBPFAmount().Insert( 1.0, oldAmount );
		}
		else
			clampBPFValues( mConfig.GetBPFAmount(), 0.5, 2.0 );

		mEditorWidget->Clear();

		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

	}

	const CLAM::ProcessingConfig& SMSPitchShiftConfigurator::GetConfig()
	{

		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSPitchShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}
	
	static SMSConfiguratorFactory::Registrator<SMSPitchShiftConfigurator> regtPitchShiftCfg( "SMSPitchShift" );


}


