#include "SMSSineFilterConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSSineFilterConfigurator::mHelpText = "<html><body><h2>Sinusoidal Filter</h2><p><strong>Usage:</strong> Introduce the frequency response of the filter you want to apply to the sinusoidal component. (X axis = frequency, Y axis = gain in dB's)</p><p><strong>Explanation:</strong> Sinusoidal component is filtered according to   the frequency response introduced.</p></body></html>";

	SMSSineFilterConfigurator::SMSSineFilterConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );

		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 200.0 );
		mEditorWidget->SetVerticalRange( -24, 24 );
		mEditorWidget->SetGridWidth( 20, 3 );
		
		SetHelpWidgetText();
		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSSineFilter" );
		mConfig.GetBPFAmount().Insert( 0.0, 0.0 );
		mConfig.GetBPFAmount().Insert( 200.0, 0.0 );

		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSSineFilterConfigurator::~SMSSineFilterConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSSineFilterConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSSineFilterConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSSineFilterConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{

		CLAM::SMSTransformationConfig& conCfg = static_cast< CLAM::SMSTransformationConfig& >( cfg );
		
		conCfg.RemoveAmount();
		conCfg.RemoveBPFAmount();
		conCfg.UpdateData();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();
		conCfg.GetBPFAmount().Insert( 0.0, 0.0 );
		conCfg.GetBPFAmount().Insert( 200.0, 0.0 );
		
		
	}

	void SMSSineFilterConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTransformationConfig& >(cfg);
		mEditorWidget->Clear();		

		if ( !mConfig.HasBPFAmount() )
		{
			double value = mConfig.GetAmount();
			
			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0.0, value );
			mConfig.GetBPFAmount().Insert( 200.0, value );
		}


		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

	}

	const CLAM::ProcessingConfig& SMSSineFilterConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSSineFilterConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}

	static SMSConfiguratorFactory::Registrator<SMSSineFilterConfigurator> regtPitchShiftCfg( "SMSSineFilter" );

}


