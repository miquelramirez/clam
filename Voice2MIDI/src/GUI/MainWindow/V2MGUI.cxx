/*
 *	V2MGUI.cxx
 *	Implementation for the V2MGUI class.
 *
*	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <qnamespace.h>
#include <qapplication.h>
#include <q3buttongroup.h>
#include <q3frame.h>
#include <qevent.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <q3filedialog.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <q3groupbox.h>
#include <qstatusbar.h>
#include <q3progressdialog.h>
#include "V2MGUI.hxx"
#include "V2MAnalysisDisplay.hxx"
#include "V2MSegmentationEditor.hxx"
#include "V2MPianoRoll.hxx"
#include "V2MIconData.hxx"
#include "V2MHelpViewer.hxx"

V2MGUI::V2MGUI()
	: Q3MainWindow(0,"main window",Qt::WStyle_Customize|Qt::WStyle_Title|Qt::WStyle_SysMenu|Qt::WStyle_Minimize|Qt::WDestructiveClose)
{
    setFixedSize(640,480);
    setGeometry(100,100,640,480);
    setCaption("Voice2MIDI");

	// set icon ///////////////////////////////
	QPixmap iconApp((const char**)icon_data);
	setIcon(iconApp);

	////// INIT WIDGETS ////////////////////////
	InitMenu();
	InitPushButtons();
    InitRadioGroup();
      
    wLine = new Q3Frame(this,"line_separator");
    wLine->setGeometry(430,35,2,415); 
    wLine->setFrameStyle(Q3Frame::VLine | Q3Frame::Sunken);

    InitAnalysisProgressDialog();
    
    ///////// FACADE ///////////////////////////////////////
    mStopSlot.Wrap(this,&V2MGUI::stop);
    mUpdateBarSlot.Wrap(this,&V2MGUI::UpdateAnalysisProgressBar);
	mDynamicReceptor.Wrap(this,&V2MGUI::UpdateDynamicDisplay);
    v2mFacade = new V2MFacade(mStopSlot,mUpdateBarSlot,mDynamicReceptor);

    ///////// INIT VARS //////////////////////////
	hasMIDISupport = false;
    hasAnalysis = false;
	hasMelody = false;
	recording = false;
	playingAudio = false;

	InitAnalysisDisplay();
	InitSegmentationEditor();
	InitPianoRoll();

	connect(segmentationEditor,SIGNAL(requestSaveMelody()),this,SLOT(saveMelody()));
	connect(segmentationEditor,SIGNAL(sendXCoord(float)),this,SLOT(syncPianoRoll(float)));
	connect(segmentationEditor,SIGNAL(sendUndo()),this,SLOT(syncPRUndo()));
	connect(segmentationEditor,SIGNAL(sendSave()),this,SLOT(syncPRSave()));
	connect(segmentationEditor,SIGNAL(sendDiscard()),this,SLOT(syncPRDiscard()));
	connect(segmentationEditor,SIGNAL(scrollValue(int)),this,SLOT(syncPRScroll(int)));

	connect(pianoRoll,SIGNAL(sendXCoord(float)),this,SLOT(syncSegmentationEditor(float)));
	connect(pianoRoll,SIGNAL(sendUndo()),this,SLOT(syncSegUndo()));
	connect(pianoRoll,SIGNAL(sendSave()),this,SLOT(syncSegSave()));
	connect(pianoRoll,SIGNAL(sendDiscard()),this,SLOT(syncSegDiscard()));
	connect(pianoRoll,SIGNAL(scrollValue(int)),this,SLOT(syncSegScroll(int)));
	
	InitMIDIGroup();
	InitStatusBar();

	UpdateEnabled(eInit);

	helpViewer = NULL; 
}

V2MGUI::~V2MGUI()
{ 
}

void V2MGUI::load_audio()
{
    QString fn = Q3FileDialog::getOpenFileName(QString::null,"Audio files (*.wav *.mp3 *.ogg)",this);
    if (!fn.isEmpty())
    {
			int err = v2mFacade->LoadAudio((fn));
			if(err)
			{
				Message(QMessageBox::Critical,"file error","File error. Perhaps this is not a mono audio file.");
			}
			else
			{
				hasAnalysis = false;
				hasMelody = false;
				UpdateEnabled(eLoadAudio);
				analysisDisplay->InitData();
				std::list<DisplayData> aux = v2mFacade->GetDisplayData(); // audio data for graphics
				// display audio loaded
				analysisDisplay->SetAudioData(aux);
				analysisDisplay->PlotOriginalAudio();
				analysisDisplay->show();
			}
    }
    
}

void V2MGUI::save_audio()
{
    QString fn = Q3FileDialog::getSaveFileName("audio_out.wav","Audio files (*.wav *.mp3 *.ogg)",this);
    if(!fn.isEmpty()) 
    {
			FILE* fp;
			if((fp=fopen((fn),"w"))==NULL)
			{
				Message(QMessageBox::Warning,"file error","Unable to write the file in this medium.");
			}
			else
			{
				fclose(fp);
				int err = v2mFacade->SaveAudio((fn));
				if(err)
					Message(QMessageBox::Warning,"file error","Unable to write the file.");
				else
					UpdateEnabled(eSaveAudio);
			}
    }
}

void V2MGUI::save_melody()
{
    QString fn = Q3FileDialog::getSaveFileName("melody_out.xml","*.xml",this);
    if(!fn.isEmpty()) 
    {
			FILE* fp;
			if((fp=fopen((fn),"w"))==NULL)
			{
				Message(QMessageBox::Warning,"file error","Unable to write the file in this medium.");
			}
			else
			{
				fclose(fp);
				if(segmentationEditor->IsModified()) segmentationEditor->SaveMelody();
				v2mFacade->StoreMelody(fn.ascii());
				UpdateEnabled(eSaveMelody);
			}
    }
}

void V2MGUI::save_midi_melody()
{
    QString fn = Q3FileDialog::getSaveFileName("midi_melody_out.xml","*.xml",this);
    if(!fn.isEmpty()) 
    {
			FILE* fp;
			if((fp=fopen((fn),"w"))==NULL)
			{
				Message(QMessageBox::Warning,"file error","Unable to write the file in this medium.");
			}
			else
			{
				fclose(fp);
				if(pianoRoll->IsModified()) segmentationEditor->SaveMelody();
				v2mFacade->StoreMIDIMelody(fn.ascii());
				UpdateEnabled(eSaveMIDIMelody);
			}
    }
}

void V2MGUI::save_midi()
{
    QString fn = Q3FileDialog::getSaveFileName("midi_out.mid","*.mid",this);
    if(!fn.isEmpty()) 
    {
			FILE* fp;
			if((fp=fopen((fn),"w"))==NULL)
			{
				Message(QMessageBox::Warning,"file error","Unable to write the file in this medium.");
			}
			else
			{
				fclose(fp);
				if(pianoRoll->IsModified()) segmentationEditor->SaveMelody();
				v2mFacade->SaveMIDI(fn.ascii(),MIDIPrograms->currentItem());
				UpdateEnabled(eSaveMIDI);
			}
    }
}

void V2MGUI::analyze()
{
	progress->setGeometry(this->x()+((this->width()-progress->width())/2),
						  this->y()+((this->height()-progress->height())/2),
						  250,60);
    progress->setTotalSteps(v2mFacade->GetAnalysisSteps());
    progress->show();
	StatusBarLabel->setText("Analising...");
    v2mFacade->Analyze();
    progress->hide();
    // get display data
	if(v2mFacade->GetAnalysisSteps() == 0)
	{
		view->setItemEnabled(0,false);
		return;
	}
	else
	{
		hasAnalysis = true;
		UpdateEnabled(eAnalyze);
	}
    std::list<DisplayData> aux = v2mFacade->GetDisplayData(false);
	analysisDisplay->SetAnalysisData(aux);
	StatusBarLabel->setText("Ready");
}

void V2MGUI::analyze_melody()
{
    v2mFacade->AnalyzeMelody();
	if(v2mFacade->GetMelody().GetNumberOfNotes() > 0)
	{	
		segmentationEditor->SetData(v2mFacade->GetMelody(),
			                        analysisDisplay->GetAnalysisData(),
									true);

		pianoRoll->SetData(v2mFacade->GetMelody(),
			               v2mFacade->GetMIDIMelody(),
						   analysisDisplay->GetAnalysisData(),
						   true);
		UpdateEnabled(eAnalyzeMelody);
		hasMelody = true;
		Message(QMessageBox::Information,"melody","Melody extraction successfully.");
	}
	else
	{
		analysis->setItemEnabled(1,false);
		Message(QMessageBox::Warning,"melody","No melody for extract.");
	}
}

void V2MGUI::plot_audio()
{
	segmentationEditor->hide();
	pianoRoll->hide();
	analysisDisplay->show();
	analysisDisplay->SetInfoAnalysisPanelEnabled(false);
	analysisDisplay->PlotOriginalAudio();
	if(playingAudio) analysisDisplay->InitDisplayForDynamicBehavior();
	view->setItemEnabled(0,false);
	if(hasAnalysis)
	{
		view->setItemEnabled(1,true);
		viewAnalysis->setItemEnabled(0,true);
		viewAnalysis->setItemEnabled(1,true);
	}
	if(hasMelody)
	{
		view->setItemEnabled(2,true);
		view->setItemEnabled(3,true);
	}
}

void V2MGUI::plot_analysis()
{
	segmentationEditor->hide();
	pianoRoll->hide();
	analysisDisplay->show();
	analysisDisplay->SetInfoAnalysisPanelEnabled(true);
    analysisDisplay->ShowCurrentFrame();
	view->setItemEnabled(0,true);
	viewAnalysis->setItemEnabled(0,true);
	viewAnalysis->setItemEnabled(1,false);
	if(hasMelody) 
	{
		view->setItemEnabled(2,true);
		view->setItemEnabled(3,true);
	}
}

void V2MGUI::plot_analysis_full()
{
	segmentationEditor->hide();
	pianoRoll->hide();
	analysisDisplay->show();
	analysisDisplay->SetInfoAnalysisPanelEnabled(true);
    analysisDisplay->ShowAllData();
	view->setItemEnabled(0,true);
	viewAnalysis->setItemEnabled(0,false);
	viewAnalysis->setItemEnabled(1,true);
	if(hasMelody) 
	{
		view->setItemEnabled(2,true);
		view->setItemEnabled(3,true);
	}
}

void V2MGUI::plot_segmentation()
{
	analysisDisplay->hide();
	pianoRoll->hide();
	segmentationEditor->show();
	view->setItemEnabled(2,false);
	view->setItemEnabled(0,true);
	view->setItemEnabled(1,true);
	viewAnalysis->setItemEnabled(0,true);
	viewAnalysis->setItemEnabled(1,true);
	view->setItemEnabled(3,true);
}

void V2MGUI::piano_roll()
{
	analysisDisplay->hide();
	segmentationEditor->hide();
	pianoRoll->show();
	view->setItemEnabled(3,false);
	view->setItemEnabled(0,true);
	view->setItemEnabled(1,true);
	viewAnalysis->setItemEnabled(0,true);
	viewAnalysis->setItemEnabled(1,true);
	view->setItemEnabled(2,true);
}

void V2MGUI::about()
{
    AboutBox();
}

void V2MGUI::rec()
{
	StatusBarLabel->setText("Recording...");
    UpdateEnabled(eRec);
    hasAnalysis = false;
	hasMelody = false;
	recording = true;
	analysisDisplay->show();
	analysisDisplay->ShowAudioDisplay();
	analysisDisplay->InitDisplayForDynamicBehavior();
    v2mFacade->Rec();
	analysisDisplay->InitData();
}

void V2MGUI::stop()
{
	UpdateEnabled(eStop);
    v2mFacade->Stop(); 
	if(recording)
	{
		std::list<DisplayData> aux = v2mFacade->GetDisplayData();
		analysisDisplay->SetAudioData(aux);
		recording = false;
		audio->setItemEnabled(0,true);
	}
}

void V2MGUI::play()
{
    pMode mode;
    
    UpdateEnabled(ePlay);
	
	if(analysisDisplay->IsAudioDisplayVisible() && !RadioPlayMIDI->isChecked())
	{
		analysisDisplay->HideEnergyAndFundDisplays();
		analysisDisplay->InitDisplayForDynamicBehavior();
	}

    if(RadioPlayAudio->isEnabled() && RadioPlayAudio->isChecked())
	{
		StatusBarLabel->setText("Playing original audio...");
		playingAudio = true;
		mode = pmAudio;
	}

    if(RadioPlayMIDI->isEnabled() && RadioPlayMIDI->isChecked())
	{
		StatusBarLabel->setText("Playing MIDI...");

		v2mFacade->SetMIDIDevice(MIDIDevices->currentItem());
		v2mFacade->SetMIDIProgram(MIDIPrograms->currentItem());

		if(pianoRoll->IsModified())
		{
			if(pianoRoll->GetMIDIMelody().GetNumberOfNotes() > 0)
			{
				v2mFacade->SetMIDIMelodyTmp(pianoRoll->GetMIDIMelody());
				mode = pmMIDI;
			}
			else
			{
				mode = pmNone;
				UpdateEnabled(eStop);
			}

		}
		else
		{
			v2mFacade->NeedUpdatedMelody(false);
			if(v2mFacade->GetMIDIMelody().GetNumberOfNotes() > 0)
			{
				mode = pmMIDI;
			}
			else
			{
				mode = pmNone;
				UpdateEnabled(eStop);
			}
		}
	}

    if(RadioPlayFundamental->isEnabled() && RadioPlayFundamental->isChecked())
	{
		StatusBarLabel->setText("Playing f0 (analysis)...");
		playingAudio = true;
		mode = pmFundamental;
	}

	if(RadioPlayFundamentalSeg->isEnabled() && RadioPlayFundamentalSeg->isChecked())
	{
		playingAudio = true;
		StatusBarLabel->setText("Playing f0 (melody)...");

		if(segmentationEditor->IsModified())
		{
			if(segmentationEditor->GetMelody().GetNumberOfNotes() > 0)
			{
				v2mFacade->SetMelodyTmp(segmentationEditor->GetMelody());
				mode = pmFundamentalSeg;
			}
			else
			{
				mode = pmNone;
				UpdateEnabled(eStop);
			}

		}
		else
		{
			v2mFacade->NeedUpdatedMelody(false);

			if(v2mFacade->GetMelody().GetNumberOfNotes() > 0)
			{
				mode = pmFundamentalSeg;
			}
			else
			{
				mode = pmNone;
				UpdateEnabled(eStop);
			}
		}
	}

    v2mFacade->Play(mode);
}

void V2MGUI::UpdateEnabled(EUpdateEnabled event)
{
    switch(event)
    {
		case	      eInit:
								audio->setItemEnabled(0,false);          // save audio	
								midi->setItemEnabled(0,false);			 // save midi
								melody->setItemEnabled(0,false);         // save melody
								melody->setItemEnabled(1,false);         // save midi melody
								analysis->setItemEnabled(0,false);       // analyze
								analysis->setItemEnabled(1,false);	     // analyze melody
								view->setItemEnabled(0,false);           // plot audio
								view->setItemEnabled(1,false);           // plot analysis
								view->setItemEnabled(2,false);           // segmentation
								view->setItemEnabled(3,false);           // piano roll
								RadioPlayAudio->setEnabled(false);       // play audio
								RadioPlayAudio->setChecked(true);        // checked audio play
								RadioPlayMIDI->setEnabled(false);        // play midi
								RadioPlayFundamental->setEnabled(false);    // play fundamental
								RadioPlayFundamentalSeg->setEnabled(false); // play fundamental after segmentation
								ButtonPlay->setEnabled(false);              // play
								ButtonStop->setEnabled(false);              // stop  
								analysisDisplay->SetInfoAnalysisPanelEnabled(false);
								pianoRoll->hide();
                                segmentationEditor->hide();
								StatusBarLabel->setText("Ready");  // message on the status bar
								break;
	case     eLoadAudio:
								audio->setItemEnabled(0,true);     // save audio 
								melody->setItemEnabled(0,false);   // save melody
								melody->setItemEnabled(1,false);   // save midi melody
								midi->setItemEnabled(0,false);     // save midi
								analysis->setItemEnabled(0,true);  // analyze
								analysis->setItemEnabled(1,false); // analyze melody
								view->setItemEnabled(0,false);     // plot audio
								view->setItemEnabled(1,false);     // plot analysis
								view->setItemEnabled(2,false);
								view->setItemEnabled(3,false);     // piano roll
								ButtonPlay->setEnabled(true);      // play
								RadioPlayAudio->setEnabled(true);  // play audio
								RadioPlayFundamental->setEnabled(false); // play fundamental
								RadioPlayFundamentalSeg->setEnabled(false); // play fundamental after segmentation
								RadioPlayMIDI->setEnabled(false);           // play midi
								RadioPlayAudio->setChecked(true);  // checked
								analysisDisplay->SetInfoAnalysisPanelEnabled(false);
								analysisDisplay->ResetAnalysisLabels();  // clear labels
								pianoRoll->hide();                     // piano rool 
								segmentationEditor->hide();
								break;
	case     eSaveAudio:
								audio->setItemEnabled(0,false);     // save audio
								break;
	case    eSaveMelody:
								melody->setItemEnabled(0,false);     // save melody
								break;
	case    eSaveMIDIMelody:
								melody->setItemEnabled(1,false);     // salve midi melody
								break;
	case    eSaveMIDI:
								midi->setItemEnabled(0,false);     // salve midi
								break;
	case       eAnalyze:
								RadioPlayFundamental->setEnabled(true); // play fundamental
								analysis->setItemEnabled(0,false);      // analyze 
								analysis->setItemEnabled(1,true);       // extract melody
								view->setItemEnabled(1,true);           // plot analysis
								viewAnalysis->setItemEnabled(0,true);   // analysis full
								viewAnalysis->setItemEnabled(1,true);   // analysis step by step
								break;
	case eAnalyzeMelody:
								analysis->setItemEnabled(1,false);      // extract melody
								melody->setItemEnabled(0,true);         // save melody
								melody->setItemEnabled(1,true);  		// save midi melody
								midi->setItemEnabled(0,true);           // save midi
								view->setItemEnabled(2,true);           // segmentation
								view->setItemEnabled(3,true);           // piano roll 
								RadioPlayFundamentalSeg->setEnabled(true); // play fundamental after segmentation
								RadioPlayMIDI->setEnabled(hasMIDISupport); // play MIDI
								break;
	case          ePlay:
								ButtonPlay->setEnabled(false);          // playing
								ButtonRec->setEnabled(false);           // capturing
								ButtonStop->setEnabled(true);           // stop
								audio->setEnabled(false);
								break;
	case          eStop:
								ButtonStop->setEnabled(false);           // stop
								ButtonPlay->setEnabled(true);            // playing
								ButtonRec->setEnabled(true);             // capturing
								RadioPlayAudio->setEnabled(true);        // play audio 
								audio->setEnabled(true);                 // audio group
								if(!hasAnalysis) 
									analysis->setItemEnabled(0,true);
								view->setItemEnabled(0,true);            // plot original audio
								StatusBarLabel->setText("Ready");		 // menssage on the status bar
								if(playingAudio) playingAudio = false;
								break;
	case           eRec:        
								ButtonRec->setEnabled(false);             // capturing
								ButtonPlay->setEnabled(false);            // playing
								ButtonStop->setEnabled(true);             // stop
								RadioPlayFundamental->setEnabled(false);  // play fundamental
								RadioPlayFundamentalSeg->setEnabled(false); // play fundamental after segmentation
								RadioPlayMIDI->setEnabled(false);           // play midi
								RadioPlayAudio->setChecked(true);         // checked
								audio->setItemEnabled(0,false);           // save audio
								melody->setItemEnabled(0,false);          // save melody
								melody->setItemEnabled(1,false);          // save midi melody
								midi->setItemEnabled(0,false);            // save midi file
								analysis->setItemEnabled(0,false);        // analyze 
								analysis->setItemEnabled(1,false);        // extract melody
								view->setItemEnabled(0,false);            // plot audio
								view->setItemEnabled(1,false);            // plot analysis
								view->setItemEnabled(2,false);
								view->setItemEnabled(3,false);            // piano roll
								analysisDisplay->SetInfoAnalysisPanelEnabled(false);
								analysisDisplay->ResetAnalysisLabels();   // create labels   
								audio->setEnabled(false);                 // audio group
								segmentationEditor->hide();
								pianoRoll->hide();
								break;

	case		eMMelody:       
								if(!melody->isItemEnabled(0))
									melody->setItemEnabled(0,true);   // save melody
								if(!melody->isItemEnabled(1))
									melody->setItemEnabled(1,true);   // save midi melody
								if(!midi->isItemEnabled(0))
									midi->setItemEnabled(0,true);     // save midi file
								break;

		default:
					break;
	}
}

void V2MGUI::InitAnalysisProgressDialog()
{
	progress = new Q3ProgressDialog(this,"progress",true,Qt::WStyle_Customize|Qt::WStyle_Title);
	progress->setFixedSize(250,60);
	progress->setCaption("Please Wait");
	progress->setLabelText("Analysing...");
	progress->setCancelButton(NULL);
	QPixmap iconApp((const char**)icon_data);
  	progress->setIcon(iconApp);
	progress->hide();
}

void V2MGUI::UpdateAnalysisProgressBar(int value)
{
    progress->setProgress(value);
}

void V2MGUI::InitRadioGroup()
{
	////////////// RADIO BUTTONS ////////////////////////////// 
    PlayButtonGroup = new Q3ButtonGroup(this,"PlayButtonGroup");
    PlayButtonGroup->setGeometry(446,height()-140,180,110); 
    PlayButtonGroup->setTitle("Play");

    RadioPlayAudio = new QRadioButton(PlayButtonGroup,"RadioPlayAudio");
    RadioPlayAudio->setGeometry(10,20,60,21); 
    RadioPlayAudio->setText("Audio");

    RadioPlayMIDI = new QRadioButton(PlayButtonGroup,"RadioPlayMIDI");
    RadioPlayMIDI->setGeometry(10,40,51,20); 
    RadioPlayMIDI->setText("MIDI");

    RadioPlayFundamental = new QRadioButton(PlayButtonGroup,"RadioPlayFundamental");
    RadioPlayFundamental->setGeometry(10,60,90,21); 
    RadioPlayFundamental->setText("Fundamental");

	RadioPlayFundamentalSeg = new QRadioButton(PlayButtonGroup,"RadioPlayFundamental");
    RadioPlayFundamentalSeg->setGeometry(10,80,110,21); 
    RadioPlayFundamentalSeg->setText("Fundamental SEG");
    ///////////// END RADIO BUTTONS ///////////////////////////////////////
}

void V2MGUI::InitPushButtons()
{
	/////////////// PUSH BUTTONS /////////////////////////
    ButtonRec = new QPushButton(this,"ButtonRec");
    ButtonRec->setGeometry(175,height()-60,71,31); 
    ButtonRec->setText("Rec");

    ButtonPlay = new QPushButton(this,"ButtonPlay");
    ButtonPlay->setGeometry(15,height()-60,71,31); 
    ButtonPlay->setText("Play");

    ButtonStop = new QPushButton(this,"ButtonStop");
    ButtonStop->setGeometry(95,height()-60,71,31); 
    ButtonStop->setText("Stop");

	//////////////// CONNECTIONS ////////////////////////////
    connect(ButtonRec,SIGNAL(clicked()),this,SLOT(rec()));
    connect(ButtonStop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ButtonPlay,SIGNAL(clicked()),this,SLOT(play()));

    ////////////// END PUSH BUTTONS //////////////////////////
}

#define CHECK_PTR(x) CLAM_ASSERT(NULL != x, "V2MGUI: allocation failure")

void V2MGUI::InitMenu()
{
	///////////// MENU //////////////////////////////////////
    audio = new Q3PopupMenu(this);
    CHECK_PTR(audio);
    audio->insertItem("Load...",this,SLOT(load_audio()),Qt::CTRL+Qt::Key_L);
    audio->insertItem("Save...",this,SLOT(save_audio()),Qt::CTRL+Qt::Key_S,0);

    midi = new Q3PopupMenu(this);
    CHECK_PTR(midi);
    midi->insertItem("Save MIDI file...",this,SLOT(save_midi()),Qt::CTRL+Qt::Key_M,0);

    melody = new Q3PopupMenu(this);
    CHECK_PTR(melody);
    melody->insertItem("Save melody...",this,SLOT(save_melody()),Qt::CTRL+Qt::Key_Y,0);
	melody->insertItem("Save MIDI melody...",this,SLOT(save_midi_melody()),Qt::CTRL+Qt::Key_D,1);

    file = new Q3PopupMenu(this);
    CHECK_PTR(file);
    file->insertItem("Audio",audio);
    file->insertSeparator();
	file->insertItem("MIDI",midi);
	file->insertSeparator();
    file->insertItem("Melody",melody);
    file->insertSeparator();
    file->insertItem("Exit",qApp,SLOT(quit()),Qt::CTRL+Qt::Key_E);
	

    analysis = new Q3PopupMenu(this);
    CHECK_PTR(analysis);
    analysis->insertItem("Analyze",this,SLOT(analyze()),Qt::CTRL+Qt::Key_A,0);
    analysis->insertSeparator();
    analysis->insertItem("Extract melody\t",this,SLOT(analyze_melody()),Qt::CTRL+Qt::Key_X,1);

	viewAnalysis = new Q3PopupMenu(this);
    CHECK_PTR(viewAnalysis);
    viewAnalysis->insertItem("Full",this,SLOT(plot_analysis_full()),Qt::CTRL+Qt::Key_F,0);
    viewAnalysis->insertItem("Step by Step",this,SLOT(plot_analysis()),Qt::CTRL+Qt::Key_N,1);

    view = new Q3PopupMenu(this);
    CHECK_PTR(view);
    view->insertItem("Original audio",this,SLOT(plot_audio()),Qt::CTRL+Qt::Key_O,0);
	view->insertItem("Analysis",viewAnalysis,1);
	view->insertItem("Segmentation",this,SLOT(plot_segmentation()),Qt::CTRL+Qt::Key_G,2);
	view->insertItem("Piano Roll",this,SLOT(piano_roll()),Qt::CTRL+Qt::Key_P,3);

    help = new Q3PopupMenu(this);
    CHECK_PTR(help);
	help->insertItem("Help...",this,SLOT(show_help()));
	help->insertSeparator();
    help->insertItem("About...",this,SLOT(about()));

    menu = new QMenuBar(this);
    CHECK_PTR(menu);
    menu->insertItem("&File",file);
    menu->insertItem("&Analysis",analysis);
    menu->insertItem("&View",view);
    menu->insertSeparator();
    menu->insertItem("&?",help);
    menu->setSeparator(QMenuBar::InWindowsStyle);
    /////////////// END MENU /////////////////////////////
}

void V2MGUI::InitAnalysisDisplay()
{
	analysisDisplay = new V2MAnalysisDisplay(this);
}

void V2MGUI::UpdateDynamicDisplay(DataArray& data)
{
	analysisDisplay->UpdateDynamicDisplay(data);
}

void V2MGUI::InitSegmentationEditor()
{
	segmentationEditor = new V2MSegmentationEditor(this);
}

void V2MGUI::InitPianoRoll()
{
	pianoRoll = new V2MPianoRoll(this);
}

void V2MGUI::saveMelody()
{
	v2mFacade->SetMelody(segmentationEditor->GetMelody());
	UpdateEnabled(eMMelody);
}

void V2MGUI::syncSegmentationEditor(float xcoord)
{
	segmentationEditor->Synchronize(xcoord);
	UpdateEnabled(eMMelody);
}

void V2MGUI::syncPianoRoll(float xcoord)
{
	pianoRoll->Synchronize(xcoord);
	UpdateEnabled(eMMelody);
}

void V2MGUI::syncSegUndo()
{
	segmentationEditor->SyncUndo();
}

void V2MGUI::syncSegSave()
{
	segmentationEditor->SyncSave();
}

void V2MGUI::syncSegDiscard()
{
	segmentationEditor->SyncDiscard();
}

void V2MGUI::syncPRUndo()
{
	pianoRoll->SyncUndo();
}

void V2MGUI::syncPRSave()
{
	pianoRoll->SetData(v2mFacade->GetMelody(),
			           v2mFacade->GetMIDIMelody(),
					   analysisDisplay->GetAnalysisData());
	pianoRoll->SyncSave();
}

void V2MGUI::syncPRDiscard()
{
	pianoRoll->SyncDiscard();
}

void V2MGUI::syncSegScroll(int value)
{
	segmentationEditor->SyncScroll(value);
}

void V2MGUI::syncPRScroll(int value)
{
	pianoRoll->SyncScroll(value);
}

void V2MGUI::InitMIDIGroup()
{
	std::list<std::string> lst;
	std::string str;
	int index = 0;
	MIDIGroup = new Q3GroupBox(this,"MIDIGroup");
    MIDIGroup->setGeometry(446,PlayButtonGroup->y()-200,180,160); 
    MIDIGroup->setTitle("MIDI");

	m_dev_label = new QLabel(MIDIGroup,"midi_dev_label");
	m_dev_label->setGeometry(10,25,70,20);
	m_dev_label->setText("MIDI Devices");
	MIDIDevices = new QComboBox(MIDIGroup,"midi_devices");
	MIDIDevices->setGeometry(10,m_dev_label->y()+20,160,25);

	m_prog_label = new QLabel(MIDIGroup,"midi_prog_label");
	m_prog_label->setGeometry(10,MIDIDevices->y()+35,90,20);
	m_prog_label->setText("MIDI Instruments");
	MIDIPrograms = new QComboBox(MIDIGroup,"midi_programs");
	MIDIPrograms->setGeometry(10,m_prog_label->y()+20,160,25);

	// get and insert items for devices
	lst = v2mFacade->GetMIDIDeviceNames();
	hasMIDISupport = lst.size() > 0;
	if(hasMIDISupport)
	{
		std::list<std::string>::iterator it = lst.begin();
		for(;it != lst.end();it++,index++)
		{
			str = (*it);
			QString s(str.c_str());
			MIDIDevices->insertItem(s,index);
		}

		
		// get and insert items for programs
		index = 0;
		lst = v2mFacade->GetAvailableMIDIPrograms();
		it = lst.begin();
		for(;it != lst.end();it++,index++)
		{
			str = (*it);
			QString s(str.c_str());
			MIDIPrograms->insertItem(s,index);
		}
	}
	else
	{
		Message(QMessageBox::Warning,"MIDI support not available","MIDI support is not available in your system. MIDI group will be disabled.");
		MIDIGroup->setEnabled(false);
	}
}

void V2MGUI::InitStatusBar()
{
	StatusBarLabel = new QLabel(statusBar(),"status_bar_label");
	StatusBarLabel->setAlignment(Qt::AlignLeft);
    statusBar()->addWidget(StatusBarLabel,1);
}

void V2MGUI::Message(QMessageBox::Icon icon,const std::string& title,const std::string& m)
{
	QMessageBox message(title.c_str(),
		                m.c_str(),
						icon,
						QMessageBox::Ok,
						Qt::NoButton,
						Qt::NoButton); 

	QPixmap iconApp((const char**)icon_data);
	//QMessageBox::Icon newIcon(iconApp);
	//message.setIcon(newIcon);
	//aboutBox.setIconPixmap(iconApp);
	message.exec();
}

void V2MGUI::AboutBox()
{
	std::string m("Voice2MIDI 0.3\n Under CLAM 0.90.0\n");
	m += "Voice2MIDI converter.\n";
	m += "PFC - UPF June - 2004\n";
	m += "ETIS\n\n";
	m += "Author: Ismael Mosquera Rivera\n";
	m += "Director: Xavier Amatriain Rubio\n";
	m += "Tutor: Alex Loscos Mira\n\n";
	m += "Collaborators: MTG (Music Technology Group - IUA - UPF)\n\n";
	m += "Special thanks to Maarten de Boer (MTG)\n";
	m += "for his contribution in the MIDI out CLAM support.\n"; 

	QMessageBox aboutBox("About Voice2MIDI",
		                m.c_str(),
						QMessageBox::Information,
						QMessageBox::Ok,
						QMessageBox::NoButton,
						QMessageBox::NoButton); 

	QPixmap iconApp((const char**)icon_data);
	//aboutBox.setIcon(iconApp);
	aboutBox.setIconPixmap(iconApp);
	aboutBox.exec();
}

void V2MGUI::show_help()
{
	if(!helpViewer)
	{
		FILE* fp = NULL;
		if((fp=fopen("doc/index.htm","r"))==NULL)
		{
			Message(QMessageBox::Critical,"help not found","Voice2MIDI  help  not  found.");
		}
		else
		{
			fclose(fp);
			helpViewer = new V2MHelpViewer();
			connect(helpViewer,SIGNAL(closed()),this,SLOT(destroyHelpWindow()));
			helpViewer->show();
		}
	}
}

void V2MGUI::destroyHelpWindow()
{
	delete helpViewer;
	helpViewer = NULL;
}

void V2MGUI::closeEvent(QCloseEvent *e)
{
	if(helpViewer) delete helpViewer;
	e->accept();
}

// END

