#include "SMS_DataExplorer.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "Assert.hxx"

#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "Fl_SMS_Spectrum.hxx"
#include "Fl_SMS_SinTracks_Browser.hxx"
#include "Fl_SMS_SpectrumAndPeaks.hxx"
#include "Fl_SMS_FundFreq_Browser.hxx"
#include "Fl_Smart_Tile.hxx"

#include <iostream>

namespace CLAMVM
{

	SMS_DataExplorer::SMS_DataExplorer()
		: mpOriginalAudioWidget( NULL ), mpSynthesizedAudioWidget( NULL ), mpSynthesizedResidualWidget( NULL ),
		  mpSynthesizedSinusoidalWidget( NULL ), mpSpectrumAndPeaksWidget( NULL ), mpSinusoidalSpectrum( NULL ),
		  mpResidualSpectrum( NULL ), mpSegmentSinTracks( NULL ), mpFundFreqBrowser( NULL ),
		  mpCanvas( NULL ), mCurrentFrameCenterTime( 0 ), mHigherF0Hint( 0 ), mLowerF0Hint( 0 )
	{
		// Slot wrapping
		NewSegment.Wrap( this, &SMS_DataExplorer::OnNewSegment );
		NewFrame.Wrap( this, &SMS_DataExplorer::OnNewFrame );
		SelectedTimeChanged.Wrap( this, &SMS_DataExplorer::OnSelectedTimeChanged );
		NewInputAudio.Wrap( this, &SMS_DataExplorer::OnNewInputAudio );
		NewSynthesizedAudio.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedAudio );
		NewSynthesizedSinusoidal.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedSinusoidal );
		NewSynthesizedResidual.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedResidual );
		ShowInputAudio.Wrap( this, &SMS_DataExplorer::OnShowInputAudio );
		ShowSinTracks.Wrap( this, &SMS_DataExplorer::OnShowSinTracks );
		ShowFundFreq.Wrap( this, &SMS_DataExplorer::OnShowFundFreq );
		ShowSpectrumAndPeaks.Wrap( this, &SMS_DataExplorer::OnShowSpectrumAndPeaks );
		ShowSinusoidalSpectrum.Wrap( this, &SMS_DataExplorer::OnShowSinusoidalSpectrum );
		ShowResidualSpectrum.Wrap( this, &SMS_DataExplorer::OnShowResidualSpectrum );
		ShowSynthesizedAudio.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedAudio );
		ShowSynthesizedSinusoidal.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedSinusoidal );
		ShowSynthesizedResidual.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedResidual );
		SetFundFreqRangeHint.Wrap( this, &SMS_DataExplorer::OnNewFundFreqRangeHint );
	}
	
	SMS_DataExplorer::~SMS_DataExplorer()
	{
	}

	void SMS_DataExplorer::OnNewFundFreqRangeHint( CLAM::TData lower, CLAM::TData higher )
	{
		mHigherF0Hint = higher;
		mLowerF0Hint = lower;

		if ( mpFundFreqBrowser )
			mpFundFreqBrowser->NewFreqRangeHint( lower, higher );

	}

	void SMS_DataExplorer::SetCanvas( Fl_Smart_Tile* pCanvas )
	{
		CLAM_ASSERT( pCanvas != NULL, "The provided canvas was null!" );
		mpCanvas = pCanvas;
	}

	void SMS_DataExplorer::OnNewSegment( const CLAM::Segment& segment )
	{
		mSinusoidalTracksAdapter.BindTo( segment );
		mFundFreqAdapter.BindTo( segment );

		// first we create the necessary widgets 
		if ( !mpSegmentSinTracks )
		{
			mpSegmentSinTracks = new Fl_SMS_SinTracks_Browser( 0,0,100,100, "Sinusoidal Tracks");
			CLAM_ASSERT( mpSegmentSinTracks != NULL, "Unable to create widget");
			mpSegmentSinTracks->AttachTo( mSinusoidalTracksAdapter );
			mpSegmentSinTracks->SetSelectedXValue( mCurrentFrameCenterTime );
			mpSegmentSinTracks->SelectedXValue.Connect( SelectedTimeChanged );

		}
		if ( !mpFundFreqBrowser )
		{
			mpFundFreqBrowser = new Fl_SMS_FundFreq_Browser( 0,0,100,100, "Fundamental Frequency");
			CLAM_ASSERT( mpFundFreqBrowser != NULL, "Unable to create widget");
			mFundFreqAdapter.TrajectoryExtracted.Connect( mpFundFreqBrowser->NewTrajectory );
			mFundFreqAdapter.TimeSpanChanged.Connect( mpFundFreqBrowser->NewTimeSpan );
			mpFundFreqBrowser->SetSelectedXValue( mCurrentFrameCenterTime );
			mpFundFreqBrowser->SelectedXValue.Connect( SelectedTimeChanged );
			
		}

		mSinusoidalTracksAdapter.Publish();
		mFundFreqAdapter.Publish();
	}
	
	void SMS_DataExplorer::OnNewFrame( const CLAM::Frame& frame, bool fullAnalysisData )
	{

		if ( fullAnalysisData )
		{
			CLAM_ASSERT( frame.HasSpectrum(), "The given frame did not have the Spectrum field");
			mSpectrumAdapter.BindTo( frame.GetSinusoidalAnalSpectrum() );
			// @todo: AnalysisSynthesis does not keep a SinusoidalSpec in the Frames
			//CLAM_ASSERT( frame.HasSinusoidalSpec(), "The given frame did not have the Sinusoidal Spectrum field" );
			//mSinusoidalSpectrumAdapter.BindTo( frame.GetSinusoidalSpec() );
			if ( !mpSpectrumAndPeaksWidget )
			{
				mpSpectrumAndPeaksWidget = new Fl_SMS_SpectrumAndPeaks( 0,0,100,100, "Frame spectrum and spectral peaks" );
				CLAM_ASSERT( mpSpectrumAndPeaksWidget!=NULL, "The widget could not be created" );
				mpSpectrumAndPeaksWidget->AttachTo( mSpectrumAdapter, mPeakArrayAdapter );
			}
			if ( !mpSinusoidalSpectrum )
			{
				mpSinusoidalSpectrum = new Fl_SMS_Spectrum( 0,0,100,100, "Sinusoidal Spectrum ");
				CLAM_ASSERT( mpSinusoidalSpectrum!=NULL, "The widget could not be created");
				mpSinusoidalSpectrum->AttachTo( mSinusoidalSpectrumAdapter );
			}
			mSpectrumAdapter.Publish();
			mSinusoidalSpectrumAdapter.Publish();

		}

		CLAM_ASSERT( frame.HasResidualSpec(), "The given frame did not have the Residual Spectrum field");
		mResidualSpectrumAdapter.BindTo( frame.GetResidualSpec() );
		CLAM_ASSERT( frame.HasSpectralPeakArray(), "The given frame did not have the SpectralPeakArray field" );
		mPeakArrayAdapter.BindTo( frame.GetSpectralPeakArray() );
		
		// now we create the necessary widgets
		if ( !mpResidualSpectrum )
		{
			mpResidualSpectrum = new Fl_SMS_Spectrum( 0,0,100,100, "Residual Spectrum");
			CLAM_ASSERT( mpResidualSpectrum!=NULL, "The widget could not be created");
			mpResidualSpectrum->AttachTo( mResidualSpectrumAdapter );
		}
		
		mResidualSpectrumAdapter.Publish();
		mPeakArrayAdapter.Publish();
		
		mCurrentFrameCenterTime = frame.GetCenterTime();
		if ( mpOriginalAudioWidget )
		{
			mpOriginalAudioWidget->SetSelectedXValue( frame.GetCenterTime() );
		}
		if ( mpSegmentSinTracks )
		{
			mpSegmentSinTracks->SetSelectedXValue( frame.GetCenterTime() );
		}
		if ( mpFundFreqBrowser )
		{
			mpFundFreqBrowser->SetSelectedXValue( frame.GetCenterTime() );
		}
		if ( mpSynthesizedAudioWidget )
		{
			mpSynthesizedAudioWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}
		if ( mpSynthesizedSinusoidalWidget )
		{
			mpSynthesizedSinusoidalWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}
		if ( mpSynthesizedResidualWidget )
		{
			mpSynthesizedResidualWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}

	}

	void SMS_DataExplorer::OnSelectedTimeChanged( double timeval )
	{
		SelectedTime.Emit( timeval );
	}

	void SMS_DataExplorer::OnNewInputAudio( const CLAM::Audio& audio )
	{
		mOriginalAudioAdapter.BindTo( audio );
		
		if ( !mpOriginalAudioWidget )
		{
			mpOriginalAudioWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,100,100, "Input Audio");
			CLAM_ASSERT( mpOriginalAudioWidget!=NULL, "The widget could not be created");
			mpOriginalAudioWidget->AttachTo( mOriginalAudioAdapter );
			mpOriginalAudioWidget->SetSelectedXValue( mCurrentFrameCenterTime );
			mpOriginalAudioWidget->SelectedXValue.Connect( SelectedTimeChanged );
		}

		mOriginalAudioAdapter.Publish();
	}

	void SMS_DataExplorer::OnNewSynthesizedAudio( const CLAM::Audio& audio )
	{
		mSynthesizedAudioAdapter.BindTo( audio );
		
		if ( !mpSynthesizedAudioWidget )
		{
			mpSynthesizedAudioWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,100,100, "Synthesized Audio");
			CLAM_ASSERT( mpSynthesizedAudioWidget!=NULL, "The widget could not be created");
			mpSynthesizedAudioWidget->AttachTo( mSynthesizedAudioAdapter );
			mpSynthesizedAudioWidget->SetSelectedXValue( mCurrentFrameCenterTime );
			mpSynthesizedAudioWidget->SelectedXValue.Connect( SelectedTimeChanged );
		}

		mSynthesizedAudioAdapter.Publish();
	}
	

	void SMS_DataExplorer::OnNewSynthesizedSinusoidal( const CLAM::Audio& audio )
	{
		mSynthesizedSinusoidalAdapter.BindTo( audio );
		
		if ( !mpSynthesizedSinusoidalWidget )
		{
			mpSynthesizedSinusoidalWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,100,100, "Synthesized Sinusoidal Component");
			CLAM_ASSERT( mpSynthesizedSinusoidalWidget!=NULL, "The widget could not be created");
			mpSynthesizedSinusoidalWidget->AttachTo( mSynthesizedSinusoidalAdapter );
			mpSynthesizedSinusoidalWidget->SetSelectedXValue( mCurrentFrameCenterTime );
			mpSynthesizedSinusoidalWidget->SelectedXValue.Connect( SelectedTimeChanged );
		}

		mSynthesizedSinusoidalAdapter.Publish();

	}

	void SMS_DataExplorer::OnNewSynthesizedResidual( const CLAM::Audio& audio )
	{
		mSynthesizedResidualAdapter.BindTo( audio );
		
		if ( !mpSynthesizedResidualWidget )
		{
			mpSynthesizedResidualWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,100,100, "Synthesized Residual Component");
			CLAM_ASSERT( mpSynthesizedResidualWidget!=NULL, "The widget could not be created");
			mpSynthesizedResidualWidget->AttachTo( mSynthesizedResidualAdapter );
			mpSynthesizedResidualWidget->SetSelectedXValue( mCurrentFrameCenterTime );
			mpSynthesizedResidualWidget->SelectedXValue.Connect( SelectedTimeChanged );
		}

		mSynthesizedResidualAdapter.Publish();

	}

	void SMS_DataExplorer::OnShowInputAudio()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		// already in the canvas
		if ( mpCanvas->contains( mpOriginalAudioWidget ) )
			return;
     		     
		mpOriginalAudioWidget->callback( (Fl_Callback*)sDetachCb, this );

		mpCanvas->add_adjust( mpOriginalAudioWidget );
		//TODO: this is a HACK!
		mpOriginalAudioWidget->hide();
		Fl::flush();
		mpOriginalAudioWidget->show();
		Fl::flush();

		mpCanvas->redraw();
		mpOriginalAudioWidget->redraw();
	}

	void SMS_DataExplorer::OnShowSinTracks( )
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSegmentSinTracks ) ) // already in the canvas
			return;
		     
		mpSegmentSinTracks->callback( (Fl_Callback*)sDetachCb, this );

		mpCanvas->add_adjust ( mpSegmentSinTracks );
		//TODO: this is a HACK!
		mpSegmentSinTracks->hide();
		Fl::flush();
		mpSegmentSinTracks->show();
		Fl::flush();


		mpSegmentSinTracks->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowFundFreq( )
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpFundFreqBrowser ) ) // already in the canvas
			return;
		     
		mpFundFreqBrowser->callback( (Fl_Callback*)sDetachCb, this );

		mpCanvas->add_adjust ( mpFundFreqBrowser );
		//TODO: this is a HACK!
		mpFundFreqBrowser->hide();
		Fl::flush();
		mpFundFreqBrowser->show();
		Fl::flush();

		if ( mHigherF0Hint != mLowerF0Hint )
			mpFundFreqBrowser->NewFreqRangeHint( mLowerF0Hint, mHigherF0Hint );

		mpFundFreqBrowser->Show();
		mpCanvas->redraw();

	}


	void SMS_DataExplorer::OnShowSpectrumAndPeaks()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSpectrumAndPeaksWidget ) ) // already in the canvas
			return;
		     
		mpSpectrumAndPeaksWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add_adjust( mpSpectrumAndPeaksWidget );

		//TODO: this is a HACK!
		mpSpectrumAndPeaksWidget->hide();
		Fl::flush();
		mpSpectrumAndPeaksWidget->show();
		Fl::flush();

		mpSpectrumAndPeaksWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSinusoidalSpectrum()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");
/*
		if ( mpCanvas->contains( mpSinusoidalSpectrum ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSinusoidalSpectrum->resize( 0, 0, widgetWidth, widgetHeight );
		mpSinusoidalSpectrum->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSinusoidalSpectrum );
		mpSinusoidalSpectrum->Show();
		mpCanvas->redraw();
*/
	}

	void SMS_DataExplorer::OnShowResidualSpectrum()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpResidualSpectrum ) )// already in the canvas
			return;
		     
		mpResidualSpectrum->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add_adjust( mpResidualSpectrum );
		//TODO: this is a HACK!
		mpResidualSpectrum->hide();
		Fl::flush();
		mpResidualSpectrum->show();
		Fl::flush();

		mpResidualSpectrum->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSynthesizedAudio()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedAudioWidget ) ) // already in the canvas
			return;
		     
		mpSynthesizedAudioWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add_adjust(mpSynthesizedAudioWidget );
		//TODO: this is a HACK!
		mpSynthesizedAudioWidget->hide();
		Fl::flush();
		mpSynthesizedAudioWidget->show();
		Fl::flush();

		mpSynthesizedAudioWidget->Show();
		mpCanvas->redraw();

	}
	
	void SMS_DataExplorer::OnShowSynthesizedSinusoidal()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedSinusoidalWidget ) ) // already in the canvas
			return;
		     
		mpSynthesizedSinusoidalWidget->callback( (Fl_Callback*)sDetachCb, this );

		mpCanvas->add_adjust(mpSynthesizedSinusoidalWidget );
		//TODO: this is a HACK!
		mpSynthesizedSinusoidalWidget->hide();
		Fl::flush();
		mpSynthesizedSinusoidalWidget->show();
		Fl::flush();
		
		mpSynthesizedSinusoidalWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSynthesizedResidual()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedResidualWidget ) )// already in the canvas
			return;
		     
		mpSynthesizedResidualWidget->callback( (Fl_Callback*)sDetachCb, this );


		mpCanvas->add_adjust(mpSynthesizedResidualWidget );
		//TODO: this is a HACK!
		mpSynthesizedResidualWidget->hide();
		Fl::flush();
		mpSynthesizedResidualWidget->show();
		Fl::flush();

		mpSynthesizedResidualWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::Detach()
	{
	}

	void SMS_DataExplorer::sDetachCb( Fl_Widget* w, void* cbData )
	{
		SMS_DataExplorer* pExplorer = ( SMS_DataExplorer* )cbData;
		w->hide();
	}

	void SMS_DataExplorer::CloseAll()
	{
		if ( mpOriginalAudioWidget )
		{
			if ( mpCanvas->contains(mpOriginalAudioWidget) )
			{
				mpOriginalAudioWidget->Hide();
				mpCanvas->remove( mpOriginalAudioWidget );
			}
		}
		if ( mpSynthesizedAudioWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedAudioWidget) )
			{
				mpSynthesizedAudioWidget->Hide();
				mpCanvas->remove( mpSynthesizedAudioWidget );
			}
		}
		if ( mpSynthesizedResidualWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedResidualWidget) )
			{
				mpSynthesizedResidualWidget->Hide();
				mpCanvas->remove( mpSynthesizedResidualWidget );
			}
		}

		if ( mpSynthesizedSinusoidalWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedSinusoidalWidget) )
			{
				mpSynthesizedSinusoidalWidget->Hide();
				mpCanvas->remove( mpSynthesizedSinusoidalWidget );
			}
		}
		if ( mpSpectrumAndPeaksWidget )
		{
			if ( mpCanvas->contains(mpSpectrumAndPeaksWidget) )
			{
				mpSpectrumAndPeaksWidget->Hide();
				mpCanvas->remove( mpSpectrumAndPeaksWidget );
			}
		}
		if ( mpSinusoidalSpectrum )
		{
			if ( mpCanvas->contains(mpSinusoidalSpectrum) )
			{
				mpSinusoidalSpectrum->Hide();
				mpCanvas->remove( mpSinusoidalSpectrum );
			}
		}
		if ( mpResidualSpectrum )
		{
			if ( mpCanvas->contains(mpResidualSpectrum) )
			{
				mpResidualSpectrum->Hide();
				mpCanvas->remove( mpResidualSpectrum );
			}
		}
		if ( mpSegmentSinTracks )
		{
			if ( mpCanvas->contains(mpSegmentSinTracks) )
			{
				mpSegmentSinTracks->Hide();
				mpCanvas->remove( mpSegmentSinTracks );
			}
		}
		if ( mpFundFreqBrowser )
		{
			if ( mpCanvas->contains(mpFundFreqBrowser) )
			{
				mpFundFreqBrowser->Hide();
				mpCanvas->remove( mpFundFreqBrowser );
			}
		}

		if ( mpCanvas->children() > 0 )
		  mpCanvas->redraw();
		else
		  {
		    mpCanvas->hide();
		    mpCanvas->show();
		  }

	}
	
}
