#include <qfiledialog.h>
#include <qaction.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include "Message.hxx"
#include "Engine.hxx"
#include "ViewManager.hxx"
#include "SMSConfigDlg.hxx"
#include "ScoreEditorDlg.hxx"
#include "QtWaitMessage.hxx"
#include "QtSMSTools.hxx"

namespace QtSMS
{
	QtSMSTools::QtSMSTools()
		: SMSToolsBase(0,"SMSTools",WDestructiveClose)
	{
		InitSMSTools();
	}

	QtSMSTools::~QtSMSTools()
	{
		delete mEngine;
	}

	void QtSMSTools::InitSMSTools()
	{
		mEngine = Engine::Instance();
		setCentralWidget(Engine::DisplayManager()->GetView(this));
		InitStatusBar();
		InitialState();
		InitMenuViewItems();
		
		connect(Engine::DisplayManager(),SIGNAL(dataType(QString)),SLOT(setLeftSBLabelText(QString)));
		connect(Engine::DisplayManager(),SIGNAL(sampleRateDuration(QString)),SLOT(setRightSBLabelText(QString)));
	}

	void QtSMSTools::loadConfiguration()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		if(mEngine->LoadConfiguration((filename)))
		{
			Engine::DisplayManager()->Flush(); // we have a new config
			Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
			InitMenuViewItems();
			ShowIncomingAudio();
			UpdateState();
		}
		else
		{
			CLAM::VM::Message(QMessageBox::Critical,"SMS Tools 2","Load configuration failed.");
		}
	}

	void QtSMSTools::newConfiguration()
	{
		SMSConfigDlg* configDlg = new SMSConfigDlg(mEngine->GetGlobalConfig());
		if( configDlg->exec() == QDialog::Accepted )
		{
			configDlg->Apply();
			if(mEngine->HasValidAudioInput())
			{
				QString filename = QFileDialog::getSaveFileName("new_config.xml","*.xml",this);
				if(!filename.isEmpty())
				{
					mEngine->StoreConfiguration((filename));
					mEngine->LoadConfiguration((filename));
					Engine::DisplayManager()->Flush(); // we have a new config
					Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
					InitMenuViewItems();
					ShowIncomingAudio();
					UpdateState();
				}
			}
		}
		delete configDlg;
	}

	void QtSMSTools::storeExtractedMelody()
	{

		QString filename = QFileDialog::getSaveFileName("extracted_melody_out.xml","*.xml",this);
		if(filename.isEmpty()) return;
		mEngine->StoreMelody((filename));
	}

	void QtSMSTools::loadAnalysisData()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::storeAnalysisData()
	{
		QString filename = QFileDialog::getSaveFileName("analysis_data_out.xml","(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreAnalysis((filename));
	}

	void QtSMSTools::saveSynthesizedAudio()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_audio_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSound((filename));
	}

	void QtSMSTools::saveSynthesizedSinusoidal()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_sinusoidal_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundSinusoidal((filename));
	}

	void QtSMSTools::saveSynthesizedResidua()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_residual_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundResidual((filename));
	}

	void QtSMSTools::loadTransformationScore()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"*.xml *",this);
		if(!filename.isEmpty())
		{
			mEngine->LoadTransformationScore((filename));	
			UpdateState();
		}
	}

	void QtSMSTools::newTransformationScore()
	{
		ScoreEditorDlg* scoreDlg = new ScoreEditorDlg();
		if(mEngine->GetState().GetHasTransformationScore())
		{
			scoreDlg->SetTransformationScore(mEngine->GetCurrentTransformationScore());
		}
		if(scoreDlg->exec() == QDialog::Accepted)
		{
			if(scoreDlg->Apply())
			{
				QString filename = QFileDialog::getSaveFileName("new_score.xml","*.xml",this);
				if(!filename.isEmpty())
				{
					mEngine->SetCurrentTransformationScore(scoreDlg->GetTransformationChain());
					mEngine->StoreTransformationScore((filename));
					UpdateState();
				}
			}
			else
			{
				CLAM::VM::Message(QMessageBox::Information,"SMS Tools 2","The score has not been changed or it's empty.");
			}

		}
		delete scoreDlg;
	}

	void QtSMSTools::analyze()
	{
		InitMenuViewItems(false);
		Engine::DisplayManager()->HideDisplays();
		Engine::DisplayManager()->Reset();
		mEngine->Analyze();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendAnalyzedDataToViewManager));
	}

	void QtSMSTools::melodyExtraction()
	{
		NotImplemented();
		return;

		// there are something wrong
		mEngine->ExtractMelody();
		UpdateState();
	}

	void QtSMSTools::doTransformation()
	{
		InitMenuViewItems(false);
		Engine::DisplayManager()->HideDisplays();
		Engine::DisplayManager()->Reset();
		mEngine->Transform();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendTransformedDataToViewManager));
	}

	void QtSMSTools::undoTransformation()
	{
		mEngine->GetState().SetHasTransformation(false);
		InitMenuViewItems(false);
		Engine::DisplayManager()->HideDisplays();
		Engine::DisplayManager()->Reset();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendAnalyzedDataToViewManager));
	}
   
	void QtSMSTools::synthesize()
	{
		ResetMenuViewAudioItems();
		Engine::DisplayManager()->Reset();
		mEngine->Synthesize();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendSynthesizedDataToViewManager));
	}

	void QtSMSTools::displayBWSonogram(bool on)
	{
		if(on)
		{
			mMenuViewColorSonogram->setOn(false);
			Engine::DisplayManager()->BlackAndWhiteSonogram();
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SONOGRAM);
		}
		else
		{
			if(!mMenuViewColorSonogram->isOn())
			{
				Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SONOGRAM);
			}
		}
		
	}

	void QtSMSTools::displayColorSonogram(bool on)
	{
		if(on)
		{
			mMenuViewBWSonogram->setOn(false);
			Engine::DisplayManager()->ColorSonogram();
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SONOGRAM);
		}
		else
		{
			if(!mMenuViewBWSonogram->isOn())
			{
				Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SONOGRAM);
			}
		}
	}

	void QtSMSTools::displayFundamentalFrequency(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,FUND_FREQ);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,FUND_FREQ);
		}
	}

	void QtSMSTools::displayOriginalAudio(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,ORIGINAL_AUDIO);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,ORIGINAL_AUDIO);
		}
	}

	void QtSMSTools::displayResidualSpectrum(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,RES_SPEC);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,RES_SPEC);
		}
	}

	void QtSMSTools::displaySinusoidalSpectrum(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,SIN_SPEC);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,SIN_SPEC);
		}
	}

	void QtSMSTools::displaySinusoidalTracks(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SIN_TRACKS);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SIN_TRACKS);
		}
	}

	void QtSMSTools::displaySpectrumAndPeaks(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,SPEC_AND_PEAKS);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,SPEC_AND_PEAKS);
		}
	}

	void QtSMSTools::displaySynthesizedSound(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_AUDIO);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_AUDIO);
		}
	}

	void QtSMSTools::displaySynthesizedResidual(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_RESIDUAL);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_RESIDUAL);
		}
	}

	void QtSMSTools::displaySynthesizedSinusoidal(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_SINUSOIDAL);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_SINUSOIDAL);
		}
	}

	void QtSMSTools::backgroundBlack(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->SetBackgroundBlack();
			mBackgroundWhite->setOn(false);
		}
		else
		{
			if(!mBackgroundWhite->isOn()) mBackgroundBlack->setOn(true);
		}
	}

	void QtSMSTools::backgroundWhite(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->SetBackgroundWhite();
			mBackgroundBlack->setOn(false);
		}
		else
		{
			if(!mBackgroundBlack->isOn()) mBackgroundWhite->setOn(true);
		}
	}

	void QtSMSTools::showOnlineHelp()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::showLicense()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::showAboutBox()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::setLeftSBLabelText(QString str)
	{
		if(str.isEmpty())
		{
			mLeftSBLabel->setText("ready");
		}
		else
		{
			mLeftSBLabel->setText(str);
		}
	}

	void QtSMSTools::setRightSBLabelText(QString str)
	{
		if(str.isEmpty())
		{
			mRightSBLabel->setText("sr: --      dur: --:--,--- ");
		}
		else
		{
			mRightSBLabel->setText(str);
		}
	}

	void QtSMSTools::InitialState()
	{
		// menu File
		mMenuFileNewCfg->setEnabled(true);
		mMenuFileLoadCfg->setEnabled(true);
		mMenuFileLoadAnalysis->setEnabled(true);
		mMenuFileStoreAnalysis->setEnabled(false);
		mMenuFileSaveMelody->setEnabled(false);
		mMenuFileNewScore->setEnabled(true);
		mMenuFileLoadScore->setEnabled(true);
		mMenuFileSaveSynSound->setEnabled(false);
		mMenuFileSaveSynSinusoidal->setEnabled(false);
		mMenuFileSaveSynResidual->setEnabled(false);
		// menu SMS Analysis
		mMenuAnalysisAnalyze->setEnabled(false);
		mMenuAnalysisExtractMelody->setEnabled(false);
		// menu SMS Transformation
		mMenuTransformApply->setEnabled(false);
		mMenuTransformUndo->setEnabled(false);
		// menu SMS Synthesis
		mMenuSynthSinthesize->setEnabled(false);
		// menu View
		mMenuViewOriginalAudio->setEnabled(false);
		mMenuViewFundFreq->setEnabled(false);
		mMenuViewSinTracks->setEnabled(false);
		mMenuViewColorSonogram->setEnabled(false);
		mMenuViewBWSonogram->setEnabled(false);
		mMenuViewSpecPeaks->setEnabled(false);
		mMenuViewSinSpec->setEnabled(false);
		mMenuViewResSpec->setEnabled(false);
		mMenuViewSynAudio->setEnabled(false);
		mMenuViewSynSinusoidal->setEnabled(false);
		mMenuViewSynResidual->setEnabled(false);
		// menu Help
		mMenuOnlineHelp->setEnabled(true);
		mMenuLicense->setEnabled(true);
		mMenuAbout->setEnabled(true); 
	}

	void QtSMSTools::UpdateState()
	{
		InitialState();

		if(mEngine->GetState().GetHasConfig())
		{
			if(mEngine->GetState().GetHasAudioIn())
			{
				mMenuAnalysisAnalyze->setEnabled(true);
				mMenuViewOriginalAudio->setEnabled(true);
			}
			if(mEngine->GetState().GetHasAnalysis())
			{
				mMenuSynthSinthesize->setEnabled(true);
			}
		}

		if(mEngine->GetState().GetHasAnalysis())
		{
			mMenuFileStoreAnalysis->setEnabled(true);
			mMenuAnalysisExtractMelody->setEnabled(true);
			mMenuViewFundFreq->setEnabled(true);
			mMenuViewSinTracks->setEnabled(true);
			mMenuViewColorSonogram->setEnabled(true);
			mMenuViewBWSonogram->setEnabled(true);
			mMenuViewSpecPeaks->setEnabled(true);
			mMenuViewSinSpec->setEnabled(true);
			mMenuViewResSpec->setEnabled(true);
			if(mEngine->GetState().GetHasTransformationScore())
			{
				mMenuTransformApply->setEnabled(true);
			}
		}

		if(mEngine->GetState().GetHasMelody())
		{
			mMenuFileSaveMelody->setEnabled(true);
		}

		if(mEngine->GetState().GetHasAudioOut())
		{
			mMenuFileSaveSynSound->setEnabled(true);
			mMenuFileSaveSynSinusoidal->setEnabled(true);
			mMenuFileSaveSynResidual->setEnabled(true);
			mMenuViewSynAudio->setEnabled(true);
			mMenuViewSynSinusoidal->setEnabled(true);
			mMenuViewSynResidual->setEnabled(true);
		}

		if(mEngine->GetState().GetHasTransformation())
		{
			mMenuTransformUndo->setEnabled(true);
		}
	}

	void QtSMSTools::ShowIncomingAudio()
	{		
		mMenuViewOriginalAudio->setOn(true);
		displayOriginalAudio(true);
	}

	void QtSMSTools::InitStatusBar()
	{
		mLeftSBLabel = new QLabel(this);
		mLeftSBLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		mLeftSBLabel->setAlignment(AlignLeft);
		mLeftSBLabel->setText("ready");
		mRightSBLabel = new QLabel(this);
		mRightSBLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		mRightSBLabel->setAlignment(AlignLeft);
		mRightSBLabel->setText("sr: --      dur: --:--,--- ");
		statusBar()->addWidget(mLeftSBLabel,1);
		statusBar()->addWidget(mRightSBLabel,0);
	}

	void QtSMSTools::InitMenuViewItems(bool flag)
	{
		if(flag)
		{
			mMenuViewOriginalAudio->setOn(false);
		}
		mMenuViewFundFreq->setOn(false);
		mMenuViewSinTracks->setOn(false);
		mMenuViewColorSonogram->setOn(false);
		mMenuViewBWSonogram->setOn(false);
		mMenuViewSpecPeaks->setOn(false);
		mMenuViewSinSpec->setOn(false);
		mMenuViewResSpec->setOn(false);
		mMenuViewSynAudio->setOn(false);
		mMenuViewSynSinusoidal->setOn(false);
		mMenuViewSynResidual->setOn(false);
	}

	void QtSMSTools::ResetMenuViewAudioItems()
	{
		mMenuViewSynAudio->setOn(false);
		mMenuViewSynSinusoidal->setOn(false);
		mMenuViewSynResidual->setOn(false);
	}

	void QtSMSTools::SendAnalyzedDataToViewManager()
	{
		CLAMGUI::QtWaitMessage* msg = new CLAMGUI::QtWaitMessage("Building Graphic Data....");
		Engine::DisplayManager()->SetAnalyzedData();
		delete msg;
		UpdateState();
	}

	void QtSMSTools::SendTransformedDataToViewManager()
	{
		CLAMGUI::QtWaitMessage* msg = new CLAMGUI::QtWaitMessage("Building Graphic Data....");
		Engine::DisplayManager()->SetTransformedData();
		delete msg;
		UpdateState();
	}

	void QtSMSTools::SendSynthesizedDataToViewManager()
	{
		CLAMGUI::QtWaitMessage* msg = new CLAMGUI::QtWaitMessage("Building Graphic Data...");
		Engine::DisplayManager()->SetSynthesizedData();
		delete msg;
		UpdateState();
	}

	void QtSMSTools::LaunchMethodOnThread(CBL::Functor0 method)
	{
		mThread.SetThreadCode(method);
		mThread.Start();
	}

	void QtSMSTools::NotImplemented()
	{
		CLAM::VM::Message(QMessageBox::Information,"SMS Tools 2","Sorry, not implemented yet.");
	}
}

// END

