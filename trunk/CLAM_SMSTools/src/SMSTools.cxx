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
#include <fstream>
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_file_chooser.H>
#include <FL/fl_ask.H>
#include "Assert.hxx"
#ifdef  GetClassName
#undef GetClassName
#endif


namespace CLAMGUI
{
	static bool gFirstTimeErrorLogOpened = false;
	static bool gFirstTimeWarningLogOpened = false;

	void RedirectAssertToLogFile( const char* message, const char* filename, int lineNumber)
	{
		std::ofstream log;
		if ( !gFirstTimeErrorLogOpened )
		{
			log.open( "SMSTools2_errors.log", std::ios_base::out );
			gFirstTimeErrorLogOpened = true;
		}
		else
			log.open( "SMSTools2_errors.log", std::ios_base::out | std::ios_base::app );

		log << "##########################################################" << std::endl;
		log << "################### ASSERTION FAILED #####################" << std::endl;
		log << "##########################################################" << std::endl;
		log << "At file " << filename << " line " << lineNumber << std::endl;
		log << message << std::endl;
		
		log.close();

		Fl::lock();
		fl_message( "Sorry, but SMSTools has crashed. You may send us a bug-report through\n"
			    "our web (http://www.iua.upf.es/mtg/clam). Please, do not forget to send\n"
			    "us the SMSTools2_errors.log file that can be located in SMSTools 2 folder.\n");
		Fl::unlock();
	}

	void RedirectWarningToLogFile( const char* message, const char* filename, int lineNumber)
	{
		std::ofstream log;

		if ( !gFirstTimeWarningLogOpened )
		{
			log.open( "SMSTools2_warnings.log", std::ios_base::out );
			gFirstTimeWarningLogOpened = true;
		}
		else
			log.open( "SMSTools2_warnings.log", std::ios_base::out|std::ios_base::app  );

		log << "##########################################################" << std::endl;
		log << "######################## WARNING #########################" << std::endl;
		log << "##########################################################" << std::endl;
		log << "At file " << filename << " line " << lineNumber << std::endl;
		log << message << std::endl;

		log.close();
	}

	SMSTools::SMSTools()
		: mUI( NULL )
	{
		TimeSelected.Wrap( this, &SMSTools::OnNewTime );
		mExplorer.SelectedTime.Connect( TimeSelected );
		SetScore.Wrap( this, &SMSTools::OnNewScore );

		CLAM::SetAssertFailedHandler( RedirectAssertToLogFile );
		CLAM::SetWarningHandler( RedirectWarningToLogFile );

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
		
		// FLTK visual settings
		Fl::visual(FL_DOUBLE|FL_INDEX);

		// FLTK Tooltip settings

		Fl_Tooltip::delay( 0.5 );
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

		Fl::lock(); // To enable FLTK thread-safe mode
		Fl::run();
	}

	void SMSTools::OnNewScore( const CLAM::SMSTransformationChainConfig& cfg )
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
		CLAM::TTime time( value  );
		CLAM::TIndex nframe;
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
			Fl::wait(0.5);
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
	
	bool SMSTools::DoLoadAnalysis()
	{

		char* fileName = fl_file_chooser("Choose file to load...", "*.xml|*.sdif", "");

		if ( !fileName )
			return false;


		mAnalysisInputFile = fileName;

		mCurrentWaitMessage = CreateWaitMessage("Loading analysis data, please wait");


		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, LoadAnalysis ) );
	
		DestroyWaitMessage();

		return true;
	}
	
	void SMSTools::AnalyzeMelody()
	{
		
		SMSBase::AnalyzeMelody();


	}

	void SMSTools::ExecuteMelodyAnalysis( )
	{
		mCurrentWaitMessage = CreateWaitMessage("Analyzing melody, please wait");

		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, AnalyzeMelody ) );
		
		DestroyWaitMessage();

	}

	void SMSTools::StoreMelody(void)
	{
		const char* filename = fl_file_chooser( "Choose file to store the extracted melody...",
												"*.xml", "" );

		if ( !filename )
			return;

		std::string melodyFilename;

		melodyFilename = filename;

		CLAM::XMLStorage x;
		x.UseIndentation(true);
		x.Dump(mMelody,"Analyzed_Melody",melodyFilename);
	}

	bool SMSTools::DoStoreAnalysis()
	{
		char* fileName = fl_file_chooser("Choose file to store on...", "*.xml|*.sdif", "");

		if ( !fileName )
			return false;

		mAnalysisOutputFile = fileName;

		mCurrentWaitMessage = CreateWaitMessage("Storing analysis data, please wait");


		ExecuteMethodOnThreadKeepingScreenUpToDate( 
			makeMemberFunctor0( *this, SMSTools, StoreAnalysis ) );

		DestroyWaitMessage();
	

		return true;
	}

	bool SMSTools::LoadAnalysis()
	{
		mSerialization.DoSerialization( mSerialization.Load, mOriginalSegment, mAnalysisInputFile.c_str() );

		mHaveTransformation=false;

		return true;
	}

	void SMSTools::StoreAnalysis()
	{

	mSerialization.DoSerialization( mSerialization.Store, mOriginalSegment, mAnalysisOutputFile.c_str() );	

		
	}

	void SMSTools::StoreSound(const CLAM::Audio& audio)
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
