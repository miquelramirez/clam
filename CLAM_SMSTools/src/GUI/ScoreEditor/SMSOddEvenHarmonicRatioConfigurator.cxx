#include "SMSOddEvenHarmonicRatioConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSOddEvenHarmonicRatioConfigurator::mHelpText = "<html><body><h2>Odd/Even Harmonic Ratio</h2><p><strong>Usage:</strong> Indicate in dB's the gain applied to odd harmonics in relation to even. E.g. A value of 6 means that a 6dB difference will be introduced, thus, odd harmonics will be 3dB higher and even harmonic 3dB lower.(X axis = time)</p><p><strong>Explanation:</strong> Harmonics are modified according to their harmonic number. </p></body></html>";

	SMSOddEvenHarmonicRatioConfigurator::SMSOddEvenHarmonicRatioConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( -12, 12 );
		mEditorWidget->SetGridWidth( 0.1, 3 );

		SetHelpWidgetText();


		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSOddEvenHarmonicRatio" );
		mConfig.GetBPFAmount().Insert( 0.0, 0 );
		mConfig.GetBPFAmount().Insert( 1.0, 0 );
		
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSOddEvenHarmonicRatioConfigurator::~SMSOddEvenHarmonicRatioConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSOddEvenHarmonicRatioConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSOddEvenHarmonicRatioConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSOddEvenHarmonicRatioConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );

		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

		mConfig.GetBPFAmount().Insert( 0.0, 0 );
		mConfig.GetBPFAmount().Insert( 1.0, 0 );

		
	}

	void SMSOddEvenHarmonicRatioConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
		mEditorWidget->Clear();
		
		if ( !mConfig.HasBPFAmount() )
		{
			double value = mConfig.GetAmount();
			
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0, value );
			mConfig.GetBPFAmount().Insert( 1, value );
		}
			
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

	}

	const CLAM::ProcessingConfig& SMSOddEvenHarmonicRatioConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSOddEvenHarmonicRatioConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}
	
	static SMSConfiguratorFactory::Registrator<SMSOddEvenHarmonicRatioConfigurator> regtPitchShiftCfg( "SMSOddEvenHarmonicRatio" );

}



