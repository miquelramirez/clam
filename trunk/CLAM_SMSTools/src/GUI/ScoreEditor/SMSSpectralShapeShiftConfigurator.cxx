#include "SMSSpectralShapeShiftConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSSpectralShapeShiftConfigurator::mHelpText = "<html><body><h2>Spectral Shape Shift</h2><p><strong>Usage:</strong> Introduce frequency shift applied to the sinusoidal spectral shape expressed in Hz (X axis = time).</p><p><strong>Explanation:</strong> Spectral shape of the sinusoidal component is extracted and then shifted in frequency according to the value specified.</p></body></html>";

	SMSSpectralShapeShiftConfigurator::SMSSpectralShapeShiftConfigurator()
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
		mConfig.SetType( "SMSSpectralShapeShift" );
		mConfig.GetBPFAmount().Insert( 0.0, 1.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 1.0 );
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSSpectralShapeShiftConfigurator::~SMSSpectralShapeShiftConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSSpectralShapeShiftConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSSpectralShapeShiftConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSSpectralShapeShiftConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );

		conCfg.RemoveBPFAmount();
		conCfg.RemoveAmount();
		conCfg.UpdateData();

		conCfg.GetBPFAmount().Insert( 0.0, 1.0 );
		conCfg.GetBPFAmount().Insert( 1.0, 1.0 );	
		
	}

	void SMSSpectralShapeShiftConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
		mEditorWidget->Clear();
		if ( mConfig.HasBPFAmount() )
		{
			mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		}
		else
		{
			if ( mConfig.GetAmount() >= 0.5 )
				mEditorWidget->InitPoints( mConfig.GetAmount() );
			else
				mEditorWidget->InitPoints( 1.0 );
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
		}

	}

	const CLAM::ProcessingConfig& SMSSpectralShapeShiftConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSSpectralShapeShiftConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSSpectralShapeShiftConfigurator> regtPitchShiftCfg( "SMSSpectralShapeShift" );
	
}


