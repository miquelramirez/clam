#include "UserInterface.hxx"
#include "AnalysisSynthesisExampleGUI.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"

using namespace CLAM;
using namespace CLAMGUI;

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mAnalysisSynthesisExample->mGlobalConfig);
	configurator->show();
	Fl::run();
}

void UserInterface::LoadConfiguration(void)
{
	
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		mConfigurationText->value(str);
		
		std::string inputXMLFileName(str);
		
		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		
		if (mAnalysisSynthesisExample->mHaveConfig)
		{	
			mAnalysisSynthesisExample->LoadInputSound();
			if (mAnalysisSynthesisExample->mHaveAudioIn)
			{
				mAnalyze->activate();
				mDisplayInSM->activate();
				mDisplayInSound->activate();
				mPlayInputSound->activate();
			}
			else 
				mAnalysisSynthesisExample->mHaveConfig=false;
			for(int i=0;i<4;i++){
				if(mAttachedPresentations[i]!=NULL){
					if( mAttachedPresentations[i]->GetWindow()->shown() ) 
						mAttachedPresentations[i]->GetWindow()->hide();
				}
			}
		}
		
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			mSynthesize->activate();
		Fl::redraw();
	}		
}

void UserInterface::StoreConfiguration(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
		mConfigurationText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->StoreConfig(inputXMLFileName);
	}		
}

void UserInterface::LoadTransformation(void)
{
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	if ( str )
	{
		//mTransformationFileText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadTransformationScore(inputXMLFileName);
		mAnalysisSynthesisExample->mHaveTransformationScore=true;
		if (mAnalysisSynthesisExample->mHaveAnalysis)
			mDoTransformation->activate();
		Fl::redraw();
	}
}
void UserInterface::LoadAnalysisData(void)
{
	char* str = fl_file_chooser("Select analysis data file","{*.xml|*.sdif}","");
	if (str)
	{
		//mAnalysisDataText->value(str);
		std::string inputXMLFileName(str);
		mAnalysisSynthesisExample->LoadAnalysis(inputXMLFileName);
		mStoreAnalysisData->deactivate();
		if (
			mAnalysisSynthesisExample->mHaveAnalysis &&
			mAnalysisSynthesisExample->mHaveConfig)
		{
			mSynthesize->activate();
		}
		Fl::redraw();
	}
}

void UserInterface::Analyze(void)
{
	mAnalysisSynthesisExample->Analyze();
	if (mAnalysisSynthesisExample->mHaveAnalysis)
	{
		mDisplayInSpec->activate();
		mSynthesize->activate();
		mMelodySM->activate();
		mStoreAnalysisData->activate();
		if(mAnalysisSynthesisExample->mHaveTransformationScore)
			mDoTransformation->activate();
		Fl::redraw();
	}
}

void UserInterface::Synthesize(void)
{
	mAnalysisSynthesisExample->Synthesize();
	if (mAnalysisSynthesisExample->mHaveAudioOut)
	{
		mSynthesize->activate();
		mOutputSM->activate();
		mPlayOutputs->activate();
		Fl::redraw();
	}
}

void UserInterface::Exit(void)
{
	delete mWindow;
}

void UserInterface::StoreAnalysisData(void)
{
	mAnalysisSynthesisExample->StoreAnalysis();
}

void UserInterface::DisplayInputSound(void)
{
	if(mAttachedPresentations[0]==NULL){
		Attach(0, &mAnalysisSynthesisExample->mAudioIn); 
	}
 	else{
		//MRJ: Don't forget to always refresh associated views!
		mAttachedViews[0]->Refresh();
		if( mAttachedPresentations[0]->GetWindow()->shown() ) {
			mAttachedPresentations[0]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[0]->GetWindow()->show();
		}
 	}
	Fl::redraw();
}

void UserInterface::DisplayInputSpectrum(void)
{
	Geometry g(0, 0, 890, 490);
	ProcDataView<Spectrum> *view = new ProcDataView<Spectrum>;
	ProcDataPresentation<Spectrum> *presentation = 
		new ProcDataPresentation<Spectrum>(g,"Input Audio Spectrum");

	view->BindTo( &mAnalysisSynthesisExample->mSegment.GetFramesArray()[1].GetSpectrum() );
	presentation->LinkWithView( view );

//	Attach(presentation->GetWindow());

	presentation->Show();
	view->Refresh();

	Fl::redraw();
}

void UserInterface::DisplayOutputSound(void)
{
	if(mAttachedPresentations[1]==NULL){
		Attach(1, &mAnalysisSynthesisExample->mAudioOut); 
	}
	else{
		mAttachedViews[1]->Refresh();
		if( mAttachedPresentations[1]->GetWindow()->shown() ) {
			mAttachedPresentations[1]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[1]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	if(mAttachedPresentations[2]==NULL){
		Attach(2, &mAnalysisSynthesisExample->mAudioOutRes); 
	}
	else{
		mAttachedViews[2]->Refresh();
		if( mAttachedPresentations[2]->GetWindow()->shown() ) {
			mAttachedPresentations[2]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[2]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	if(mAttachedPresentations[3]==NULL){
		Attach(3, &mAnalysisSynthesisExample->mAudioOutSin);
	}
	else{
		mAttachedViews[3]->Refresh();
		if( mAttachedPresentations[3]->GetWindow()->shown() ) {
			mAttachedPresentations[3]->GetWindow()->hide();
		}
		else {
			mAttachedPresentations[3]->GetWindow()->show();
		}
	}
	Fl::redraw();
}

void UserInterface::StoreOutputSound(void)
{
	mAnalysisSynthesisExample->StoreOutputSound();
}

void UserInterface::StoreOutputSoundResidual(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundResidual();
}

void UserInterface::StoreOutputSoundSinusoidal(void)
{
	mAnalysisSynthesisExample->StoreOutputSoundSinusoidal();
}

void UserInterface::AnalyzeMelody(void)
{
	mAnalysisSynthesisExample->AnalyzeMelody();
	mMelodyStore->activate();
}

void UserInterface::StoreMelody(void)
{
	mAnalysisSynthesisExample->StoreMelody();
}

void UserInterface::Transform(void)
{
	mAnalysisSynthesisExample->Transform();
}

void UserInterface::PlayInputSound(void)
{

	mAnalysisSynthesisExample->PlayInputSound();
}

void UserInterface::PlayOutputSound(void)
{

	mAnalysisSynthesisExample->PlayOutputSound();
}

void UserInterface::PlaySinusoidal(void)
{

	mAnalysisSynthesisExample->PlaySinusoidal();
}

void UserInterface::PlayResidual(void)
{

	mAnalysisSynthesisExample->PlayResidual();
}


void UserInterface::Attach(int i, Audio* obj)
{
	Geometry g(0, 0, 860, 490);
	mAttachedViews[i] = new ProcDataView<Audio>;
	mAttachedPresentations[i] = new ProcDataPresentation<Audio>(g, "");
	
	mAttachedViews[i]->BindTo( obj );
	mAttachedPresentations[i]->LinkWithView( mAttachedViews[i] );
	
	mAttachedPresentations[i]->GetWindow()->resizable();
	
	mSmartTile->add(mAttachedPresentations[i]->GetWindow());
	mSmartTile->equalize();
	mAttachedPresentations[i]->GetWindow()->show();
	
	mAttachedPresentations[i]->Show();
	mAttachedViews[i]->Refresh();
}


