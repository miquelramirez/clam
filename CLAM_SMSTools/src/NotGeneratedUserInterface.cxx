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
#include "SMSAppState.hxx"
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
	
	mSMS->GetState().SetHasConfig(true);
 	mSMS->InitConfigs();
	// check if LoadSound operation could be carried
	if(LoadSound())
	{
		//This should go to SMSAppState class
		bool hadTransScore=mSMS->GetState().GetHasTransformationScore();
		bool hadMorphSound=mSMS->GetState().GetHasAudioMorph();
		mSMS->GetState().DefaultInit();
		mSMS->GetState().SetHasAudioIn(true);
		mSMS->GetState().SetHasTransformationScore(hadTransScore);
		mSMS->GetState().SetHasAudioMorph(hadMorphSound);
	}
	mSMS->GetState().SetHasConfig(true);
	UpdateState();
	mSMS->SegmentExplorer().CloseAll();
	mSMS->SegmentExplorer().SetFundFreqRangeHint( mSMS->mGlobalConfig.GetAnalysisLowestFundFreq(),
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
		LoadSound();
		UpdateState();
	
		if (mSMS->GetState().GetHasAnalysis() &&	mSMS->GetState().GetHasConfig())
		{
			InitCounter();
		}

		mSMS->SegmentExplorer().CloseAll();
		mSMS->SegmentExplorer().SetFundFreqRangeHint( mSMS->mGlobalConfig.GetAnalysisLowestFundFreq(),
						      mSMS->mGlobalConfig.GetAnalysisHighestFundFreq());

		mWindow->redraw();
	}		
}

bool UserInterface::LoadSound(void)
{
	mSMS->LoadInputSound();
	if ( !mSMS->GetState().GetHasAudioIn() )
	{
		return false;
	}

	mSMS->SegmentExplorer().NewInputAudio( mSMS->mOriginalSegment.GetAudio());
	
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
		mSMS->GetState().SetHasTransformationScore(true);
		UpdateState();
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
	mSMS->GetState().SetHasAnalysis (true);
	UpdateState();
	InitCounter();
	DeactivateFrameDataMenuItems();
	mSMS->SegmentExplorer().NewSegment( mSMS->mOriginalSegment );
	// @todo: determine what has to do the UserInterface for obtaining frame data when it is being loaded
	// so that one cannot rely on the fact that it is available in the segment object
	// mSMS->SegmentExplorer().NewFrame( mSMS->mSegment.GetFramesArray()[0]);
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
	if (mSMS->GetState().GetHasAnalysis())
	{
		UpdateState();
		InitCounter();
		mFrameDataAvailable = true;
		mSMS->SegmentExplorer().NewSegment( mSMS->mOriginalSegment );
		mSMS->SegmentExplorer().NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
					  FrameDataAvailable());
		mWindow->redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mSMS->Synthesize();
	if (mSMS->GetState().GetHasAudioOut())
	{
		UpdateState();
	}
	mSMS->SegmentExplorer().NewSynthesizedAudio(mSMS->mAudioOut);
	mSMS->SegmentExplorer().NewSynthesizedSinusoidal(mSMS->mAudioOutSin);
	mSMS->SegmentExplorer().NewSynthesizedResidual( mSMS->mAudioOutRes );
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
	UpdateState();
}

void UserInterface::StoreMelody(void)
{
	mSMS->StoreMelody();
}

void UserInterface::Transform(void)
{

	mSMS->Transform();
	UpdateState();
	mSMS->SegmentExplorer().CloseAll();
	mSMS->SegmentExplorer().NewSegment( mSMS->mTransformedSegment );
	mSMS->SegmentExplorer().NewFrame( mSMS->mTransformedSegment.GetFramesArray()[0],
												   FrameDataAvailable() );
	mWindow->redraw();

}

void UserInterface::UndoTransform()
{
	mSMS->GetState().SetHasTransformation (false);
	UpdateState();
	InitCounter();
	mFrameDataAvailable = true;
	
	mSMS->SegmentExplorer().NewSegment( mSMS->mOriginalSegment );
	mSMS->SegmentExplorer().NewFrame( mSMS->mOriginalSegment.GetFramesArray()[0],
													   FrameDataAvailable());

	mWindow->redraw();
}

void UserInterface::ChangeFrame()
{
	int nframe = (int) mCounter->value();

	if ( mFrameDataAvailable )
	{
		if(mSMS->GetState().GetHasTransformation())
			mSMS->SegmentExplorer().NewFrame( mSMS->mTransformedSegment.GetFramesArray()[nframe],
													   FrameDataAvailable() );
		else
			mSMS->SegmentExplorer().NewFrame( mSMS->mOriginalSegment.GetFramesArray()[nframe],
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
	mSMS->SegmentExplorer().ShowInputAudio();
	mWindow->redraw();
}

void UserInterface::DisplaySpectrumAndPeaks()
{
	mSMS->SegmentExplorer().ShowSpectrumAndPeaks();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalSpectrum()
{
	mSMS->SegmentExplorer().ShowSinusoidalSpectrum();
	mWindow->redraw();
}

void UserInterface::DisplayResidualSpectrum()
{
	mSMS->SegmentExplorer().ShowResidualSpectrum();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedAudio()
{
	mSMS->SegmentExplorer().ShowSynthesizedAudio();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedSinusoidal()
{
	mSMS->SegmentExplorer().ShowSynthesizedSinusoidal();
	mWindow->redraw();

}

void UserInterface::DisplaySynthesizedResidual()
{
	mSMS->SegmentExplorer().ShowSynthesizedResidual();
	mWindow->redraw();

}

void UserInterface::DisplaySinusoidalTracks()
{
	mSMS->SegmentExplorer().ShowSinTracks();
	mWindow->redraw();

}

void UserInterface::DisplayFundFreqTrajectory()
{
	mSMS->SegmentExplorer().ShowFundFreq();
	mWindow->redraw();
}

void UserInterface::ApplyInitialState()
{
	mSMS->SegmentExplorer().CloseAll();
	

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

void UserInterface::InitCounter()
{
	mCounter->activate();
	mCounter->range( 0, mSMS->mOriginalSegment.GetnFrames() );
	mCounter->step( 1 );
	mCounter->lstep( mSMS->mOriginalSegment.GetnFrames()/10 );
}

void UserInterface::DeactivateFrameDataMenuItems()
{
	mViewFrameDataMenuItem->deactivate();
	mShowSpectrumAndPeaksMenuItem->deactivate();
	mShowResidualSpectrumMenuItem->deactivate();
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

void UserInterface::UpdateState()
{
	mSMS->SegmentExplorer().CloseAll();
	
	SMSAppState appState=mSMS->GetState();

	ApplyInitialState();

	if(appState.GetHasConfig())
	{
		if(appState.GetHasAudioIn())
		{
			mDoSMSAnalysisMenuItem->activate();
			mShowOriginalAudioMenuItem->activate();
		}
		if(appState.GetHasAnalysis())
			mDoSMSSynthesisMenuItem->activate();
	}
	if(appState.GetHasAnalysis())
	{
		mStoreAnalysisMenuItem->activate();
		mShowAnalysisResultsMenuItem->activate();
		mShowSinTracksMenuItem->activate();
		mShowFundFreqMenuItem->activate();
		mViewFrameDataMenuItem->activate();
		mShowSinusoidalSpectrumMenuItem->activate();
		mShowSpectrumAndPeaksMenuItem->activate();
		mShowResidualSpectrumMenuItem->activate();
		mMelodyExtractionMenuItem->activate();
		if(appState.GetHasTransformationScore())
			mDoSMSTransMenuItem->activate();
		if(mFrameDataAvailable)
		{
			mViewFrameDataMenuItem->activate();
			mShowSpectrumAndPeaksMenuItem->activate();
			mShowResidualSpectrumMenuItem->activate();
		}
	}
	if(appState.GetHasMelody())
		mStoreMelodyMenuItem->activate();
	if(appState.GetHasAudioOut())
	{
		mViewSynthesisResultsMenuItem->activate();
		mStoreSMSSynthSoundMenuItem->activate();
		mStoreSMSSynthSinusoidalMenuItem->activate();
		mStoreSMSSynthResidualMenuItem->activate();
		mShowSynthesizedAudioMenuItem->activate();
		mShowSynthesizedSinusoidalMenuItem->activate();
		mShowSynthesizedResidualMenuItem->activate();
	
	}
	
	if(appState.GetHasTransformation())
		mUndoTransMenuItem->activate();
	
		
	mWindow->redraw();
}
