/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SMSTools.hxx"
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_file_chooser.H>
#ifdef  GetClassName
#undef GetClassName
#endif



namespace CLAMGUI
{
	SMSTools::SMSTools()
		: mUI( NULL )
	{
		TimeSelected.Wrap( this, &SMSTools::OnNewTime );
		mExplorer.SelectedTime.Connect( TimeSelected );
		SetScore.Wrap( this, &SMSTools::OnNewScore );
	}

	SMSTools::~SMSTools()
	{
		delete mUI;
	}

	void SMSTools::Run(void)
	{
		// FLTK look and feel  settings

		Fl::get_system_colors();
		
		Fl::set_boxtype(FL_UP_BOX,FL_THIN_UP_BOX);
		Fl::set_boxtype(FL_DOWN_BOX,FL_THIN_DOWN_BOX);
		Fl::set_boxtype(FL_UP_FRAME,FL_THIN_UP_FRAME);
		Fl::set_boxtype(FL_DOWN_FRAME,FL_THIN_DOWN_FRAME);
		
		mUI = new UserInterface;
		mUI->mSMS = this;
		mUI->Init(  );
		
		// FLTK Tooltip settings

		Fl_Tooltip::delay( 0.01 );
		Fl_Tooltip::size( 10 );
		Fl_Tooltip::enable();

		mUI->mWindow->show();
		/* MDB: TODO we would rather have the window manager position
		** the mWindow, but we want to center the AboutWindow on top
		** of it, and at this point window manager has not placed the
		** window yet, so the centering will fail. The real solution
		** would be do wait until the mWindow has been mapped.. How do
		** we do that? */
		mUI->mWindow->position(20,40);
		mUI->AboutWindow();

		Fl::run();
	}

	void SMSTools::OnNewScore( const SMSTransformationChainConfig& cfg )
	{
		mTransformationScore = cfg;
		mHaveTransformationScore = true;
		ScoreChanged.Emit( mTransformationScore );

		mUI->ApplyTransformationReadyState();
	}

	void SMSTools::LoadTransformationScore( const std::string& inputFilename )
	{
		SMSBase::LoadTransformationScore( inputFilename );
		ScoreChanged.Emit( mTransformationScore );
	}

	
	void SMSTools::OnNewTime( double value )
	{
		if (!mHaveAnalysis)
			return;
		//Change mCounter
		TTime time( value  );
		TIndex nframe;
		if(mHaveTransformation)
			nframe = mTransformedSegment.FindFrame( time );
		else
			nframe = mOriginalSegment.FindFrame( time );
		
		mUI->mCounter->value( (int) nframe );
		if(mHaveTransformation)
			mExplorer.NewFrame( mTransformedSegment.GetFramesArray()[nframe],mUI->FrameDataAvailable() );
		else
			mExplorer.NewFrame( mOriginalSegment.GetFramesArray()[nframe],mUI->FrameDataAvailable() );
	}

	Progress* SMSTools::CreateProgress(const char* title,float from,float to) 
	{
		ProgressGUI* tmp = new ProgressGUI(title,from,to);
		return tmp;
	}

	WaitMessage* SMSTools::CreateWaitMessage(const char* title) 
	{
		WaitMessageGUI* tmp = new WaitMessageGUI(title);
		return tmp;
	}

	void SMSTools::ExecuteMethodOnThreadKeepingScreenUpToDate( 
		CBL::Functor0 method )
	{
		mThread.SetThreadCode( method );

		mThread.Start();

		while ( mThread.IsRunning() )
		{
			Fl::wait( 0.1 );
		}
	}

	void SMSTools::SetCanvas( Fl_Smart_Tile* canvas )
	{
		mExplorer.SetCanvas( canvas );
	}

	void SMSTools::DoAnalysis()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, AnalysisProcessing ) );
	}

	void SMSTools::DoTracksCleanup()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, TracksCleanupProcessing ) );
	}
	
	void SMSTools::DoMorphAnalysis()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, MorphAnalysisProcessing ) );
	}

	void SMSTools::DoMorphTracksCleanup()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, MorphTracksCleanupProcessing ) );
	}


	void SMSTools::DoSynthesis()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, SynthesisProcessing ) );
	}

	void SMSTools::DoTransformation()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, TransformProcessing ) );
	}
	
	void SMSTools::DoLoadAnalysis()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, LoadAnalysis ) );

	}

	void SMSTools::DoStoreAnalysis()
	{
		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, StoreAnalysis ) );

	}

	bool SMSTools::LoadAnalysis()
	{
		char* fileName = fl_file_chooser("Choose file to load...", "*.xml|*.sdif", "");

		if ( !fileName )
			return false;

		return SMSBase::LoadAnalysis(fileName);
	}

	void SMSTools::StoreAnalysis()
	{
		char* fileName = fl_file_chooser("Choose file to store on...", "*.xml|*.sdif", "");

		if ( !fileName )
			return;
		
		SMSBase::StoreAnalysis(fileName);

	}

	void SMSTools::StoreSound(const Audio& audio)
	{
		char* fileName = fl_file_chooser("Choose file to store on...", "*.wav", "");

		if ( !fileName )
			return;
		
		SMSBase::StoreSound(fileName,audio);
	}

	void SMSTools::StoreOutputSound()
	{
		StoreSound(mAudioOut);
	}

	void SMSTools::StoreOutputSoundResidual()
	{
		StoreSound(mAudioOutRes);
	}

	void SMSTools::StoreOutputSoundSinusoidal()
	{
		StoreSound(mAudioOutSin);
	}

}
