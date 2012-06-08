#include <string>
#include <qfiledialog.h>
#include <qaction.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include "Engine.hxx"
#include "ViewManager.hxx"
#include "SMSConfigDlg.hxx"
#include "ScoreEditorDlg.hxx"
#include "QtWaitMessage.hxx"
#include "QtSMSTools.hxx"
#include "AboutBox.hxx"
#include <CLAM/CLAMVersion.hxx>
#include "SMSToolsVersion.hxx"

#ifdef WIN32
#include <CLAM/CLAM_windows.h>
#else
#include <qprocess.h>
#endif

#ifndef DATA_EXAMPLES_PATH
#define DATA_EXAMPLES_PATH "example-data"
#endif

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
		mSlotAnalysisDataLoaded.Wrap(this,&QtSMSTools::OnAnalysisDataLoaded);

		mEngine = Engine::Instance();
		mEngine->ConnectSlotAnalysisDataLoaded(mSlotAnalysisDataLoaded);
		setCentralWidget(Engine::DisplayManager()->GetView(this));
		InitStatusBar();
		InitialState();
		InitMenuViewItems();
		
		connect(Engine::DisplayManager(),SIGNAL(dataType(QString)),SLOT(setLeftSBLabelText(QString)));
		connect(Engine::DisplayManager(),SIGNAL(sampleRateDuration(QString)),SLOT(setRightSBLabelText(QString)));
	}

	void QtSMSTools::loadConfiguration()
	{
		QString examplesPath;
#ifdef __APPLE__
		QDir dir(qApp->applicationDirPath()+"/../Resources/example-data/");
		examplesPath =QString(dir.absPath());
#else
		examplesPath = DATA_EXAMPLES_PATH;
#endif
		QString filename = QFileDialog::getOpenFileName(examplesPath, "(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		if(!mEngine->LoadConfiguration(filename.ascii()))
		{
			QMessageBox::critical(0,"SMS Tools 2","Load configuration failed.");
			return;
		}
		
		mShowOriginalAudio=true;
		Engine::DisplayManager()->Flush(); // we have a new config
		if(!mEngine->GetGlobalConfig().GetInputAnalysisFile().empty())
		{
			mEngine->RetrieveAudio(true);
			mEngine->LoadAnalysis(mEngine->GetGlobalConfig().GetInputAnalysisFile());
		}
		else
		{
			Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
			InitMenuViewItems();
			ShowIncomingAudio();
			UpdateState();
		}
	}

	void QtSMSTools::newConfiguration()
	{
		SMSConfigDlg configDlg(mEngine->GetGlobalConfig(),this);
		if( configDlg.exec() != QDialog::Accepted ) return;
		configDlg.Apply();
		if(!mEngine->HasValidAudioInput()) return;
		QString filename = QFileDialog::getSaveFileName("new_config.xml","*.xml",this);
		if(filename.isEmpty()) return;
		mShowOriginalAudio=true;
		mEngine->StoreConfiguration(filename.ascii());
		mEngine->LoadConfiguration(filename.ascii());
		Engine::DisplayManager()->Flush(); // we have a new config
		if(!mEngine->GetGlobalConfig().GetInputAnalysisFile().empty())
		{
			mEngine->RetrieveAudio(true);
			mEngine->LoadAnalysis(mEngine->GetGlobalConfig().GetInputAnalysisFile());
		}
		else
		{
			Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
			InitMenuViewItems();
			ShowIncomingAudio();
			UpdateState();
		}
	}

	void QtSMSTools::storeExtractedMelody()
	{
		QString filename = QFileDialog::getSaveFileName("extracted_melody_out.xml","*.xml",this);
		if(filename.isEmpty()) return;
		mEngine->StoreMelody(filename.ascii());
	}

	void QtSMSTools::loadAnalysisData()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		mShowOriginalAudio=false;
		Engine::DisplayManager()->Flush();
		InitMenuViewItems();
		mEngine->RetrieveAudio(false);
		mEngine->LoadAnalysis(filename.ascii());
	}

	void QtSMSTools::storeAnalysisData()
	{
		std::string fn = (!mEngine->GetGlobalConfig().GetOutputAnalysisFile().empty()) ? 
			mEngine->GetGlobalConfig().GetOutputAnalysisFile() : "outputAnalysis.xml";
		QString filename = QFileDialog::getSaveFileName(fn.c_str(),"(*.xml *.sdif)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreAnalysis(filename.ascii());
	}

	void QtSMSTools::saveSynthesizedAudio()
	{
		std::string fn = (!mEngine->GetGlobalConfig().GetOutputSoundFile().empty()) ? 
			mEngine->GetGlobalConfig().GetOutputSoundFile() : "outputSound.wav";
		QString filename = QFileDialog::getSaveFileName(fn.c_str(),"Audio (*.wav *.ogg *.mp3)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSound(filename.ascii());
	}

	void QtSMSTools::saveSynthesizedSinusoidal()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_sinusoidal_out.wav","Audio (*.wav *.ogg *.mp3)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundSinusoidal(filename.ascii());
	}

	void QtSMSTools::saveSynthesizedResidua()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_residual_out.wav","Audio (*.wav *.ogg *.mp3)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundResidual(filename.ascii());
	}

	void QtSMSTools::loadTransformationScore()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"*.xml *",this);
		if(filename.isEmpty()) return;
		mEngine->LoadTransformationScore(filename.ascii());	
		UpdateState();
	}

	void QtSMSTools::newTransformationScore()
	{
		ScoreEditorDlg scoreDlg(this);
		if (mEngine->GetState().GetHasTransformationScore())
		{
			scoreDlg.SetTransformationScore(mEngine->GetCurrentTransformationScore());
		}
		if (scoreDlg.exec() != QDialog::Accepted) return;
		if (!scoreDlg.Apply())
		{
			QMessageBox::information(0,"SMS Tools 2","The score has not been changed or it's empty.");
			return;
		}
		QString filename = QFileDialog::getSaveFileName("new_score.xml","*.xml",this);
		if (filename.isEmpty()) return;

		mEngine->SetCurrentTransformationScore(scoreDlg.GetTransformationChain());
		mEngine->StoreTransformationScore(filename.ascii());
		UpdateState();
	}

	void QtSMSTools::analyze()
	{
		mShowOriginalAudio=false;
		InitMenuViewItems(false);
		Engine::DisplayManager()->HideDisplays();
		Engine::DisplayManager()->Reset();
		mEngine->Analyze();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendAnalyzedDataToViewManager));
	}

	void QtSMSTools::melodyExtraction()
	{
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
	    const std::string url="http://www.iua.upf.es/mtg/clam/";
#ifndef WIN32
		QProcess sysCall(this);
    #if defined(Q_WS_MAC)
		sysCall.addArgument("open");
		sysCall.addArgument(url.c_str());
		if(!sysCall.start())
		{
			QMessageBox::critical(0,"SMS Tools 2","Unable to open a web browser.");
		}
    #else
		// try to open the url on a web browser between several common ones
		sysCall.addArgument("firefox");
		sysCall.addArgument(url.c_str());
		if(!sysCall.start())
		{
			sysCall.clearArguments();
			sysCall.addArgument("mozilla");
			sysCall.addArgument(url.c_str());
			if(!sysCall.start())
			{
				sysCall.clearArguments();
				sysCall.addArgument("netscape");
				sysCall.addArgument(url.c_str());
				if(!sysCall.start())
				{
					sysCall.clearArguments();
					sysCall.addArgument("konqueror");
					sysCall.addArgument(url.c_str());
					if(!sysCall.start())
					{
						QMessageBox::critical(0,"SMS Tools 2","Unable to open a web browser.");
					}
				}
			}
		}
    #endif
#else
		if((unsigned int)(ShellExecute(NULL,"open",url.c_str(),NULL,NULL,SW_SHOW)) <= 32)
		{
			QMessageBox::critical(0,"SMS Tools 2","Unable to open a web browser.");
		}
#endif
	}

	void QtSMSTools::showAboutBox()
	{
		AboutBox* aboutBox = new AboutBox(this);
		aboutBox->versionInfo->setText(tr(
			"<p><b>SMSTools version %1</b></p>"
			"<p>Based on CLAM version %2</p>")
			.arg(SMSTools::GetFullVersion())
			.arg(CLAM::GetFullVersion())
			);
		aboutBox->exec();
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
		if(mShowOriginalAudio)
		{
			Engine::DisplayManager()->HideDisplays();
			Engine::DisplayManager()->Reset();
			Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
			InitMenuViewItems();
			ShowIncomingAudio();
		}
//		CLAMGUI::QtWaitMessage* msg = new CLAMGUI::QtWaitMessage("Building Graphic Data....");
		Engine::DisplayManager()->SetAnalyzedData();
//		delete msg;
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

	void QtSMSTools::OnAnalysisDataLoaded()
	{
		InitMenuViewItems(false);
		Engine::DisplayManager()->HideDisplays();
		Engine::DisplayManager()->Reset();
		LaunchMethodOnThread(makeMemberFunctor0(*this,QtSMSTools,SendAnalyzedDataToViewManager));
	}

	void QtSMSTools::NotImplemented()
	{
		QMessageBox::information(0,"SMS Tools 2","Sorry, not implemented yet.");
	}
}

// END

