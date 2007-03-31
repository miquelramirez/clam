/*
 *  V2MGUI.hxx
 *	Interface for V2MGUI class
 *
 *	The GUI.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MGUI_
#define _V2MGUI_

#include <string>
#include <list>
#include <q3mainwindow.h>
#include <qmessagebox.h>
#include "V2MFacade.hxx"

class QMenuBar;
class Q3PopupMenu;
class Q3ButtonGroup;
class Q3Frame;
class QPushButton;
class QRadioButton;
class QComboBox;
class QLabel;
class Q3GroupBox;
class Q3ProgressDialog;
	
class V2MAnalysisDisplay;
class V2MSegmentationEditor;
class V2MPianoRoll;
class V2MHelpViewer;

class V2MGUI : public Q3MainWindow
{ 
	enum EUpdateEnabled {	eInit=0,
							eLoadAudio,
							eSaveAudio,
							eSaveMelody,
							eSaveMIDIMelody,
							eSaveMIDI,
							eAnalyze,
							eAnalyzeMelody,
							ePlay,
							eStop,
							eRec,
							eMMelody};
	
	Q_OBJECT

public:
	V2MGUI();
	~V2MGUI();

protected:
	void closeEvent(QCloseEvent *);

private slots:
	// menu
	void load_audio();
	void save_audio();
	void analyze();
	void analyze_melody();
	void save_melody();
	void save_midi_melody();
	void save_midi();
	void plot_audio();  
	void plot_analysis();
	void plot_analysis_full();
	void plot_segmentation();
	void piano_roll();
	void show_help();
	void about();

	// buttons
	void rec();
	void stop();
	void play();

	// save editing melody 
	void saveMelody();

	// synchronization of piano roll and segmentation views
	void syncSegmentationEditor(float xcoord);
	void syncPianoRoll(float xcoord);
	void syncSegUndo();
	void syncSegSave();
	void syncSegDiscard();
	void syncPRUndo();
	void syncPRSave();
	void syncPRDiscard();
	void syncSegScroll(int);
	void syncPRScroll(int);

	void destroyHelpWindow();

private:
	QMenuBar* menu;
	Q3PopupMenu *file,*audio,*midi,*melody,*analysis,*view,*viewAnalysis,*help;

	QPushButton* ButtonRec;
	QPushButton* ButtonStop;
	QPushButton* ButtonPlay;
	
	Q3ButtonGroup* PlayButtonGroup;
	QRadioButton* RadioPlayAudio;
	QRadioButton* RadioPlayMIDI;
	QRadioButton* RadioPlayFundamental;
	QRadioButton* RadioPlayFundamentalSeg;
    
	Q3Frame* wLine;

	Q3GroupBox* MIDIGroup;
	QComboBox* MIDIDevices;
	QComboBox* MIDIPrograms;
	QLabel* m_dev_label;
	QLabel* m_prog_label;
	
	QLabel* StatusBarLabel;    // etiqueta para la barra de estado

	Q3ProgressDialog* progress; // barra de progreso para analisis
	
	V2MFacade* v2mFacade;
	Slotv0 mStopSlot;
	Slotv1<int> mUpdateBarSlot;
	Slotv1<DataArray> mDynamicReceptor;
	
	V2MAnalysisDisplay* analysisDisplay;
	V2MSegmentationEditor* segmentationEditor;
	V2MPianoRoll* pianoRoll;
	V2MHelpViewer* helpViewer;

	bool hasMIDISupport;
	bool hasAnalysis;
	bool hasMelody;
	bool recording;
	bool playingAudio;

	void InitMenu();
	void InitRadioGroup();
	void InitPushButtons();
	void UpdateEnabled(EUpdateEnabled event);
	
	void InitAnalysisProgressDialog();
    void UpdateAnalysisProgressBar(int value);

	void InitAnalysisDisplay();
	void UpdateDynamicDisplay(DataArray& data);

	void InitSegmentationEditor();
	void InitPianoRoll();

	void InitMIDIGroup();
	
	void InitStatusBar();

	void Message(QMessageBox::Icon icon,const std::string& title,const std::string& m);
	void AboutBox();

};

#endif 

