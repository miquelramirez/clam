#include "SMSTimeStretchConfigurator.hxx"
#include "Factory.hxx"
#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_View.H>
#include "Fl_SMS_BPF_Editor.hxx"

namespace CLAMVM
{
	const char* SMSTimeStretchConfigurator::mHelpText = "<html><body><h2>Time Stretch</h2><p><strong>Usage:</strong> Introduce relative increase/decrease in speed of resulting sound. 0.8 means decreasing speed by a 20% factor (X axis = time).</p><p><strong>Explanation:</strong> The resulting spectral represantion of the original sound is obtained by interpolating neiboughring frames or skipping others.</p></body></html>";

	SMSTimeStretchConfigurator::SMSTimeStretchConfigurator()
	{
		mHelpWidget = new Fl_Help_View( 0, 0, 100, 100 );
		mHelpWidget->textsize( 12 );
		
		mEditorWidget = new Fl_SMS_BPF_Editor( 0, 0, 100, 100 );
		mEditorWidget->SetHorizontalRange( 0.0, 1.0 );
		mEditorWidget->SetVerticalRange( 0.5, 2.0 );
		mEditorWidget->SetGridWidth( 0.1, 0.15 );

		SetHelpWidgetText();

		mConfig.AddType();
		mConfig.RemoveAmount();
		mConfig.AddBPFAmount();
		mConfig.UpdateData();
		mConfig.SetType( "SMSTimeStretch" );
		mConfig.GetBPFAmount().Insert( 0.0, 1.0 );
		mConfig.GetBPFAmount().Insert( 1.0, 1.0 );
		
		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );
		mEditorWidget->PointsChanged.Connect( UserEditedParameters );
	}

	SMSTimeStretchConfigurator::~SMSTimeStretchConfigurator()
	{
		if ( mHelpWidget->parent() == NULL )
			delete mHelpWidget;
		if ( mEditorWidget->parent() == NULL )
			delete mEditorWidget;
	}

	void SMSTimeStretchConfigurator::SetHelpWidgetText()
	{
		mHelpWidget->value( mHelpText);
	}

	Fl_Widget* SMSTimeStretchConfigurator::GetParametersWidget()
	{
		return mEditorWidget;
	}

	void SMSTimeStretchConfigurator::Initialize( CLAM::ProcessingConfig& cfg )
	{
		CLAM::SMSTimeStretchConfig& conCfg = static_cast< CLAM::SMSTimeStretchConfig& >( cfg );
		conCfg.RemoveAmount();
		conCfg.AddBPFAmount();
		conCfg.UpdateData();

		conCfg.GetBPFAmount().Insert( 0, 1.0 );
		conCfg.GetBPFAmount().Insert( 1, 1.0 );

	}

	void SMSTimeStretchConfigurator::SetConfig( const CLAM::ProcessingConfig& cfg )
	{
		mConfig = static_cast<const CLAM::SMSTimeStretchConfig& >(cfg);
		mEditorWidget->Clear();

		if ( !mConfig.HasBPFAmount() )
		{
			double value = mConfig.GetAmount();
			
			if ( value < 0.5 ) 
				value = 0.5;
			else if ( value > 2.0 )
				value = 2.0;

			mConfig.AddBPFAmount();
			mConfig.RemoveAmount();
			mConfig.UpdateData();
			mConfig.GetBPFAmount().Insert( 0, value );
			mConfig.GetBPFAmount().Insert( 1, value );
		}
		else
		{
			for ( int i = 0; i < mConfig.GetBPFAmount().Size(); i++ )
			{
				if ( mConfig.GetBPFAmount().GetValueFromIndex( i ) < 0.5 )
					mConfig.GetBPFAmount().SetValue( i, 0.5 );
				else if ( mConfig.GetBPFAmount().GetValueFromIndex(i) > 2.0 )
					mConfig.GetBPFAmount().SetValue( i, 2.0 );
			}

			if ( mConfig.GetBPFAmount().Size() == 0 )
			{
				mConfig.GetBPFAmount().Insert( 0, 1.0 );
				mConfig.GetBPFAmount().Insert( 1, 1.0 );

			}

		}

		mEditorWidget->InitPoints( mConfig.GetBPFAmount() );

	}

	const CLAM::ProcessingConfig& SMSTimeStretchConfigurator::GetConfig()
	{
		mEditorWidget->InsertPointsIntoBPF( mConfig.GetBPFAmount() );

		return mConfig;
	}

	Fl_Widget* SMSTimeStretchConfigurator::GetHelpWidget()
	{
		return mHelpWidget;
	}
	
	static SMSConfiguratorFactory::Registrator<SMSTimeStretchConfigurator> regtPitchShiftCfg( "SMSTimeStretch" );

}
