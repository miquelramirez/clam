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

#include <FL/Fl.H>
#include "NotGeneratedUserInterface.hxx"
#include "SMSTransformPanel.hxx"
#include "SMSTools.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "AudioPlayer.hxx"
#include <FL/fl_message.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Help_Dialog.H>

using namespace CLAM;
using namespace CLAMGUI;

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mSMS->mGlobalConfig);
	configurator->SetApplyCallback(makeMemberFunctor0((*this), UserInterface, Update ));
	configurator->show();	
}

void UserInterface::Update()
{
	mSMS->SetHaveConfig(true);
 	mSMS->InitConfigs();
	// check if LoadSound operation could be carried
	if ( !LoadSound() )
		return;
	ApplyInitialState();
	ApplyReadyToAnalyzeState();
	mSMS->mExplorer.CloseAll();
	mSMS->mExplorer.SetFundFreqRangeHint( mSMS->mGlobalConfig.GetAnalysisLowestFundFreq(),
					      mSMS->mGlobalConfig.GetAnalysisHighestFundFreq());
	mWindow->redraw();
}

void UserInterface::LoadConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		std::string inputXMLFileName(str);

		mSMS->LoadConfig(inputXMLFileName);
		if (! LoadSound() )
			return;	
		ApplyReadyToAnalyzeState();
	
		if (mSMS->mHaveAnalysis &&	mSMS->mHaveConfig)
			ApplyAnalysisAvailableState();

		mSMS->mExplorer.CloseAll();
		mSMS->mExplorer.SetFundFreqRangeHint( mSMS->mGlobalConfig.GetAnalysisLowestFundFreq(),
						      mSMS->mGlobalConfig.GetAnalysisHighestFundFreq());

		mWindow->redraw();
	}		
}

bool UserInterface::LoadSound(void)
{
	mSMS->LoadInputSound();
	if ( !mSMS->mHaveAudioIn )
	{
		fl_alert( "Unable to open the Input sound file!");
		ApplyInitialState();
		return false;
	}

	mSMS->mExplorer.NewInputAudio( mSMS->mOriginalSegment.GetAudio());
	
	return true;
}

void UserInterface::StoreConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
		std::string inputXMLFileName(str);
		mSMS->StoreConfig(inputXMLFileName);
	}		
}

void UserInterface::LoadTransformation(void)
{
	char* str = fl_file_chooser("Please, select a transformation score","*.xml","");
	if ( str )
	{
		//mTransformationFileText->value(str);
		std::string inputXMLFileName(str);
		mSMS->LoadTransformationScore(inputXMLFileName);
		mSMS->mHaveTransformationScore=true;
		ApplyTransformationReadyState();
		mWindow->redraw();
	}
}

void UserInterface::EditTransformScore( )
{
	mScoreEditor->Show();
}

void UserInterface::SaveTransformScore()
{
	char* str = fl_file_chooser( "Please, select where to save the transformation score", "*.xml", "" );

	if ( str )
	{
		std::string outputXMLFilename( str );
		mSMS->StoreTransformationScore( outputXMLFilename );
	}
}


void UserInterface::LoadAnalysisData(void)
{

	if ( !mSMS->DoLoadAnalysis(  ) )
		return;
	mSMS->mHaveAnalysis = true;
	// @todo: Check this is true...
	mSMS->mHaveConfig = true;
	ApplyAnalysisAvailableState();
	DeactivateFrameDataMenuItems();
	mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
	// @todo: determine what has to do the UserInterface for obtaining frame data when it is being loaded
	// so that one cannot rely on the fact that it is available in the segment object
	// mSMS->mExplorer.NewFrame( mSMS->mSegment.GetFramesArray()[0]);
	mFrameDataAvailable = false;
	mWindow->redraw();
}

bool UserInterface::FrameDataAvailable()
{
	return mFrameDataAvailable;
}

void UserInterface::Analyze(void)
{
	mSMS->Analyze();
	if (mSMS->mHaveAnalysis)
	{
		ApplyInitialState();
		ApplyReadyToAnalyzeState();
		ApplyAnalysisAvailableState();
		mFrameDataAvailable = true;
		mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
		mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
					  FrameDataAvailable());
		mWindow->redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mSMS->Synthesize();
	if (mSMS->mHaveAudioOut)
	{
		ApplySynthesisAvailableState();
	}
	mSMS->mExplorer.NewSynthesizedAudio(mSMS->mAudioOut);
	mSMS->mExplorer.NewSynthesizedSinusoidal(mSMS->mAudioOutSin);
	mSMS->mExplorer.NewSynthesizedResidual( mSMS->mAudioOutRes );
	mWindow->redraw();
}


void UserInterface::Exit(void)
{
	//delete mWindow;
	mWindow->hide();
}

void UserInterface::StoreAnalysisData(void)
{
	mSMS->DoStoreAnalysis();
}

void UserInterface::StoreOutputSound(void)
{
	mSMS->StoreOutputSound();
}

void UserInterface::StoreOutputSoundResidual(void)
{
	mSMS->StoreOutputSoundResidual();
}

void UserInterface::StoreOutputSoundSinusoidal(void)
{
	mSMS->StoreOutputSoundSinusoidal();
}

void UserInterface::AnalyzeMelody(void)
{
	mSMS->ExecuteMelodyAnalysis();
	ApplyMelodyAvailableState();
}

void UserInterface::StoreMelody(void)
{
	mSMS->StoreMelody();
}

void UserInterface::Transform(void)
{

	mSMS->Transform();
	ApplyTransformationPerformedState();
	mSMS->mExplorer.CloseAll();
	mSMS->mExplorer.NewSegment( mSMS->mTransformedSegment );
	mSMS->mExplorer.NewFrame( mSMS->mTransformedSegment.GetFramesArray()[0],
												   FrameDataAvailable() );
	mWindow->redraw();

}

void UserInterface::UndoTransform()
{
	mSMS->mHaveTransformation = false;
	ApplyInitialState();
	ApplyReadyToAnalyzeState();
	ApplyAnalysisAvailableState();
	mFrameDataAvailable = true;
	
	mSMS->mExplorer.NewSegment( mSMS->mOriginalSegment );
	mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
													   FrameDataAvailable());

	mWindow->redraw();
}

void UserInterface::ChangeFrame()
{
	int nframe = (int) mCounter->value();

	if ( mFrameDataAvailable )
	{
		if(mSMS->mHaveTransformation)
			mSMS->mExplorer.NewFrame( mSMS->mTransformedSegment.GetFramesArray()[nframe],
													   FrameDataAvailable() );
		else
			mSMS->mExplorer.NewFrame( mSMS->mOriginalSegment.GetFramesArray()[nframe],
													   FrameDataAvailable() );
	}
}

void UserInterface::ChangeTimeTag( TTime tag )
{
}

void UserInterface::Init(  )
{
	mSMS->SetCanvas( mSmartTile );
	mScoreEditor = new SMSScoreEditor;
	mSMS->ScoreChanged.Connect( mScoreEditor->SetTransformationScore );
	mScoreEditor->TransformationChainChanged.Connect( mSMS->SetScore );
	ApplyInitialState();
	mFrameDataAvailable = false;
}

void UserInterface::DisplayInputSound()
{
	mSMS->mExplorer.ShowInputAudio();
	mWindow->redraw();
}

void UserInterface::DisplaySpectrumAndPeaks()
{
	mSMS->mExplorer.ShowSpectrumAndPeaks();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalSpectrum()
{
	mSMS->mExplorer.ShowSinusoidalSpectrum();
	mWindow->redraw();
}

void UserInterface::DisplayResidualSpectrum()
{
	mSMS->mExplorer.ShowResidualSpectrum();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedAudio()
{
	mSMS->mExplorer.ShowSynthesizedAudio();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedSinusoidal()
{
	mSMS->mExplorer.ShowSynthesizedSinusoidal();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedResidual()
{
	mSMS->mExplorer.ShowSynthesizedResidual();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalTracks()
{
	mSMS->mExplorer.ShowSinTracks();
	mWindow->redraw();

}

void UserInterface::DisplayFundFreqTrajectory()
{
	mSMS->mExplorer.ShowFundFreq();
	mWindow->redraw();
}

void UserInterface::ApplyInitialState()
{
	mSMS->mExplorer.CloseAll();
	

	mFileMenuItem->activate();
	mConfigurationOpsMenuItem->activate();
	mEditCfgMenuItem->activate();
	mLoadCfgMenuItem->activate();
	mStoreCfgMenuItem->activate();
	mAnalysisFileOpsMenuItem->activate();
	mLoadAnalysisMenuItem->activate();
	mStoreAnalysisMenuItem->deactivate();
	mMelodyFileOpsMenuItem->activate();
	mStoreMelodyMenuItem->deactivate();
	mSMSTransFileOpsMenuItem->activate();
	mLoadSMSTransScoreMenuItem->activate();
	mSMSSynthesisFileOpsMenuItem->activate();
	mStoreSMSSynthSoundMenuItem->deactivate();
	mStoreSMSSynthSinusoidalMenuItem->deactivate();
	mStoreSMSSynthResidualMenuItem->deactivate();
	mSMSAnalysisMenuItem->activate();
	mDoSMSAnalysisMenuItem->deactivate();
	mMelodyExtractionMenuItem->deactivate();
	mSMSTransformationMenuItem->activate();
	mDoSMSTransMenuItem->deactivate();
	mUndoTransMenuItem->deactivate();
	mSMSSynthesisMenuItem->activate();
	mDoSMSSynthesisMenuItem->deactivate();
	mViewMenuItem->activate();
	mShowOriginalAudioMenuItem->deactivate();
	mShowAnalysisResultsMenuItem->deactivate();
	mShowSinTracksMenuItem->deactivate();
	mShowFundFreqMenuItem->deactivate();
	mViewFrameDataMenuItem->deactivate();
	mShowSinusoidalSpectrumMenuItem->deactivate();
	mShowSpectrumAndPeaksMenuItem->deactivate();
	mShowResidualSpectrumMenuItem->deactivate();
	mViewSynthesisResultsMenuItem->deactivate();
	mShowSynthesizedAudioMenuItem->deactivate();
	mShowSynthesizedSinusoidalMenuItem->deactivate();
	mShowSynthesizedResidualMenuItem->deactivate();
	mAppExitMenuItem->activate();
	mHelpMenuItem->activate();
	mCounter->deactivate();
	mWindow->redraw();
}

void UserInterface::ApplyReadyToAnalyzeState()
{
	mEditCfgMenuItem->activate();
	mStoreCfgMenuItem->activate();
	mDoSMSAnalysisMenuItem->activate();
	mShowOriginalAudioMenuItem->activate();
	mStoreAnalysisMenuItem->deactivate();
	mDoSMSSynthesisMenuItem->deactivate();
	mWindow->redraw();
}

void UserInterface::ApplyAnalysisAvailableState()
{
	ApplyReadyToAnalyzeState();
	mCounter->activate();
	mCounter->range( 0, mSMS->mOriginalSegment.GetnFrames() );
	mCounter->step( 1 );
	mCounter->lstep( mSMS->mOriginalSegment.GetnFrames()/10 );

	mStoreAnalysisMenuItem->activate();
	mMelodyExtractionMenuItem->activate();
	mDoSMSSynthesisMenuItem->activate();
	
	mShowAnalysisResultsMenuItem->activate();
	mShowSinTracksMenuItem->activate();
	mShowFundFreqMenuItem->activate();
	mViewFrameDataMenuItem->activate();
	mShowSpectrumAndPeaksMenuItem->activate();
	mShowResidualSpectrumMenuItem->activate();

	if ( mSMS->mHaveTransformationScore )
		mDoSMSTransMenuItem->activate();

	mWindow->redraw();

}

void UserInterface::DeactivateFrameDataMenuItems()
{
	mViewFrameDataMenuItem->deactivate();
	mShowSpectrumAndPeaksMenuItem->deactivate();
	mShowResidualSpectrumMenuItem->deactivate();
	mWindow->redraw();
}

void UserInterface::ApplyMelodyAvailableState()
{
	mStoreMelodyMenuItem->activate();
	mWindow->redraw();
}

void UserInterface::ApplyTransformationReadyState()
{
	if ( mSMS->mHaveAnalysis )
		mDoSMSTransMenuItem->activate();
	mWindow->redraw();
}

void UserInterface::ApplySynthesisAvailableState()
{
	mStoreSMSSynthSoundMenuItem->activate();
	mStoreSMSSynthSinusoidalMenuItem->activate();
	mStoreSMSSynthResidualMenuItem->activate();

	mViewSynthesisResultsMenuItem->activate();
	mShowSynthesizedAudioMenuItem->activate();
	mShowSynthesizedSinusoidalMenuItem->activate();
	mShowSynthesizedResidualMenuItem->activate();
	mWindow->redraw();
}

void UserInterface::ApplyTransformationPerformedState()
{
	mUndoTransMenuItem->activate();
	mWindow->redraw();
}

/*This will evolve towards something more sophisticated*/
void composeFilename( const std::string& key, std::string& filename )
{
	if ( key=="Application_License" )
		filename="doc/en/license.html";
	else
		filename="";
}

void UserInterface::DisplayLicense()
{
	std::string urlToLoad;
	std::string key = "Application_License";

	composeFilename( key, urlToLoad );

	Fl_Help_Dialog* helpViewer = new Fl_Help_Dialog(  );
	helpViewer->load( urlToLoad.c_str() );
	helpViewer->show();
	
}
