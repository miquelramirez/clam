#include "UserInterface.hxx"
#include "AnalysisSynthesisExampleGUI.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"
#include "CBL.hxx"

using namespace CLAM;
using namespace CLAMGUI;

void UserInterface::EditConfiguration(void)
{
	CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
	configurator->SetConfig(mAnalysisSynthesisExample->mGlobalConfig);
	configurator->SetApplyCallback(makeMemberFunctor0((*this), UserInterface, Update ));
	configurator->show();
}



void UserInterface::Update()
{
	mAnalysisSynthesisExample->SetHaveConfig(true);
	mAnalysisSynthesisExample->InitConfigs();
	LoadSound();
}


void UserInterface::LoadConfiguration(void)
{
	
	char* str = fl_file_chooser("Select configuration file","*.xml","");
	
	if ( str )
	{
		mConfigurationText->value(str);
		std::string inputXMLFileName(str);

		mAnalysisSynthesisExample->LoadConfig(inputXMLFileName);
		LoadSound();	
	
		if (mAnalysisSynthesisExample->mHaveAnalysis &&	mAnalysisSynthesisExample->mHaveConfig)
			mSynthesize->activate();
		Fl::redraw();
	}		
}

void UserInterface::LoadSound(void)
{
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
		{
			mAnalyze->deactivate();
			mDisplayInSM->deactivate();
			mDisplayInSound->deactivate();
			mPlayInputSound->deactivate();
			mAnalysisSynthesisExample->mHaveConfig=false;
		}
		mSmartTile->equalize();

		mSynthesize->deactivate();
		mOutputSM->deactivate();
	}
	Fl::redraw();
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
	if (mAudioInputDisplay==NULL)
		mAudioInputDisplay = Attach( "Audio Input" , &mAnalysisSynthesisExample->mAudioIn );
}

void UserInterface::DisplayInputSpectrum(void)
{
	Geometry g(0, 0, 860, 490);
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
	if (mAudioOutputDisplay==NULL)
		mAudioOutputDisplay = Attach( "Audio Output" , &mAnalysisSynthesisExample->mAudioOut );
}

void UserInterface::DisplayOutputSoundResidual(void)
{
	if (mAudioOutputResidualDisplay==NULL)
		mAudioOutputResidualDisplay = Attach( "Residual" , &mAnalysisSynthesisExample->mAudioOutRes );
}

void UserInterface::DisplayOutputSoundSinusoidal(void)
{
	if (mAudioOutputSinusoidalDisplay==NULL)
		mAudioOutputSinusoidalDisplay = Attach( "Sinusoidal" , &mAnalysisSynthesisExample->mAudioOutSin );
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

void UserInterface::_Detach(Fl_Window *w,UserInterface* ui)
{
	ui->Detach(w);
}

void UserInterface::Detach(Fl_Window *w)
{
	PresentationWindow* p = dynamic_cast<PresentationWindow*>(w);

	View* v = p->GetPresentation()->GetLinkedView();
	delete p->GetPresentation();
	delete v;
		
	if (w==mAudioInputDisplay) 
		mAudioInputDisplay = NULL;
	else if (w==mAudioOutputDisplay)
		mAudioOutputDisplay = NULL;
	else if (w==mAudioOutputResidualDisplay)
		mAudioOutputResidualDisplay = NULL;
	else if (w==mAudioOutputSinusoidalDisplay)
		mAudioOutputSinusoidalDisplay = NULL;
}

Fl_Window* UserInterface::Attach(const char* title, CLAM::Audio* data)
{
	printf("ATTACH %s\n",title);
	ProcDataView<Audio>* localView;
	ProcDataPresentation<Audio>* localPresentation;

	localView = new ProcDataView<Audio>;
	localView->BindTo( data );

	Geometry g(0, 0, mSmartTile->w(), mSmartTile->h());

	localPresentation = new ProcDataPresentation<Audio>(g, title);
	localPresentation->LinkWithView( localView );

	mSmartTile->add_titled( localPresentation->GetWindow() );
	
	localPresentation->Show();

	mSmartTile->equalize();

	localPresentation->GetWindow()->callback((Fl_Callback*) _Detach,this);

	localView->Refresh();
	
	return localPresentation->GetWindow();
}

void UserInterface::Init()
{
	mAudioInputDisplay=NULL; 
	mAudioOutputDisplay=NULL; 
	mAudioOutputResidualDisplay=NULL; 
	mAudioOutputSinusoidalDisplay=NULL; 
}
