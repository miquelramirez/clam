#include "UserInterface.hxx"
#include "AnalysisSynthesisExampleGUI.hxx"
#include <FL/fl_file_chooser.H>
#include <FL/Fl.H>
#include "FLTKConfigurator.hxx"
#include "DebugSnapshots.hxx"
#include "AudioSnapshot.hxx"
#include "AudioBrowser.hxx"
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
	DetachDisplays();
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
		DetachDisplays();

	}		
}

void UserInterface::DetachDisplays()
{
	if( mAudioInputDisplay!=NULL ){
//			mSmartTile->close( mAudioInputDisplay );
		Detach( mAudioInputDisplay );
//			mSmartTile->equalize();
	}
	if( mAudioOutputDisplay!=NULL )
		Detach( mAudioOutputDisplay );
	if( mAudioOutputResidualDisplay!=NULL )
		Detach( mAudioOutputResidualDisplay );
	if( mAudioOutputSinusoidalDisplay!=NULL )
		Detach( mAudioOutputSinusoidalDisplay );
	if( mInputSpectrum!=NULL )
		Detach( mInputSpectrum );
	if( mOutputSpectrum!=NULL )
		Detach( mOutputSpectrum );
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

		mCounter->deactivate();
		mCounter->value( 0 );

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

		mCounter->activate();
		mCounter->range( 0, mAnalysisSynthesisExample->mSegment.GetnFrames() );
		mCounter->step( 1 );
		mCounter->lstep( mAnalysisSynthesisExample->mSegment.GetnFrames()/10 );

		mPaintSignal.Emit( true );
		ChangeFrame();

		mDisplayInSpec->activate();

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
	if (mInputSpectrum == NULL)
		mInputSpectrum = Attach( "Input Spectrum" , &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ (int) mCounter->value() ].GetSpectrum(), 0 );
	else {
		List<Frame>& localFrames = mAnalysisSynthesisExample->mSegment.GetFramesArray();
		Frame& frame = localFrames[ mCounter->value() ];
		Spectrum& spectrum = frame.GetSpectrum();

		mInputSpectrumView->BindTo( &spectrum );
		mInputSpectrumView->Refresh();
		mSmartTile->equalize();
	}
}

void UserInterface::DisplayOutputSound(void)
{
	if (mAudioOutputDisplay==NULL)
		mAudioOutputDisplay = Attach( "Audio Output" , &mAnalysisSynthesisExample->mAudioOut );
}

void UserInterface::DisplayOutputSpectrum(void)
{
	if (mOutputSpectrum == NULL)
		mOutputSpectrum = Attach( "Output Spectrum" , &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ (int) mCounter->value() ].GetSpectrum(), 1 );
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
	Fl_Widget* w2 = w->parent();


	/* it should not be necesary to close w2 explicitely. instead, deleting
	** w2 should already take care of that (through a callback). mdeboer
	** should edit SmartTile to do so.
	*/
	mSmartTile->close( w2 );

	PresentationWindow* p = dynamic_cast<PresentationWindow*>(w);

	View* v = p->GetPresentation()->GetLinkedView();
	delete p->GetPresentation();
	delete v;
	delete w2;
		
	if (w==mAudioInputDisplay) 
		mAudioInputDisplay = NULL;
	else if (w==mAudioOutputDisplay)
		mAudioOutputDisplay = NULL;
	else if (w==mAudioOutputResidualDisplay)
		mAudioOutputResidualDisplay = NULL;
	else if (w==mAudioOutputSinusoidalDisplay)
		mAudioOutputSinusoidalDisplay = NULL;
 	else if (w==mInputSpectrum) 
		mInputSpectrum = NULL;
	else if (w==mOutputSpectrum)
		mOutputSpectrum = NULL;

	mSmartTile->equalize();
}

Fl_Window* UserInterface::Attach(const char* title, CLAM::Audio* data )
{
	ProcDataView<Audio>* localView;
	AudioBrowser* localPresentation;

	localView = new ProcDataView<Audio>;
	localView->BindTo( data );

	Geometry g(0, 0, mSmartTile->w(), mSmartTile->h());

	localPresentation = new AudioBrowser(g, title);
	localPresentation->LinkWithView( localView );

	//Link Signals with Slots
	mFrameSignal.Connect( *localPresentation->GetFrameSlot() );
	mPaintSignal.Connect( *localPresentation->GetPaintSlot() );
	localPresentation->GetSignal()->Connect( mSlot );

 	if (mAnalysisSynthesisExample->mHaveAnalysis)
 		localPresentation->setPainting( true );

	mSmartTile->add_titled( localPresentation->GetWindow() );
	localPresentation->Show();
	mSmartTile->equalize();
	localPresentation->GetWindow()->callback((Fl_Callback*) _Detach,this);
	localView->Refresh();

	return localPresentation->GetWindow();
}

Fl_Window* UserInterface::Attach(const char* title, CLAM::Spectrum* data, int type )
{
	Geometry g(0, 0, mSmartTile->w(), mSmartTile->h());

	ProcDataView<Spectrum>* mSpectrumView;

	mSpectrumView = new ProcDataView<Spectrum>;
	if( type == 0 )
		mInputSpectrumView = mSpectrumView;
	else
		mOutputSpectrumView = mSpectrumView;


	ProcDataPresentation<Spectrum> *localPresentation = 
		new ProcDataPresentation<Spectrum>(g, title);

	mSpectrumView->BindTo( data );
	localPresentation->LinkWithView( mSpectrumView );

	mSmartTile->add_titled( localPresentation->GetWindow() );
	localPresentation->Show();
	mSmartTile->equalize();
	localPresentation->GetWindow()->callback((Fl_Callback*) _Detach,this);
	mSpectrumView->Refresh();

	return localPresentation->GetWindow();
}

void UserInterface::Init()
{
	mAudioInputDisplay=NULL; 
	mAudioOutputDisplay=NULL; 
	mAudioOutputResidualDisplay=NULL; 
	mAudioOutputSinusoidalDisplay=NULL;
	mInputSpectrum=NULL;
	mInputSpectrumView=NULL;
	mOutputSpectrum=NULL;
	mOutputSpectrumView=NULL;
}

void UserInterface::ChangeFrame()
{
	DisplayInputSound();
	DisplayInputSpectrum();

	int nframe = (int) mCounter->value();

	List<Frame>& localFrames = mAnalysisSynthesisExample->mSegment.GetFramesArray();
	
	TData nextcursorpos = localFrames[ nframe ].GetCenterTime();
	
	//Notify SigSlotted class to change
	mFrameSignal.Emit( nextcursorpos );

	//Change Spectrum Displayer
	// InputSpectrum Displayer is yet cahnged
		
	if(mOutputSpectrumView != NULL)
	{
		mOutputSpectrumView->BindTo( &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
		mOutputSpectrumView->Refresh();
		mSmartTile->equalize();
	}

	Fl::redraw();
}

void UserInterface::ChangeTimeTag( double tag )
{
	//Change mCounter
	TTime time(tag);
	TIndex nframe = mAnalysisSynthesisExample->mSegment.FindFrame( time );

	mCounter->value( (int) nframe );

	//Notify other GLPorts
	TData nextcursorpos = mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetCenterTime();
	mFrameSignal.Emit( nextcursorpos );

	//Change Spectrum Displayer
	if(mInputSpectrumView != NULL)
	{
		mInputSpectrumView->BindTo( &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
		mInputSpectrumView->Refresh();
		mSmartTile->equalize();
	}
	if(mOutputSpectrumView != NULL)
	{
		mOutputSpectrumView->BindTo( &mAnalysisSynthesisExample->mSegment.GetFramesArray()[ nframe ].GetSpectrum() );
		mOutputSpectrumView->Refresh();
		mSmartTile->equalize();
	}

	Fl::redraw();
}
