/*
 * V2MPianoRoll.hxx
 * Interface for V2MPianoRoll class
 *
 * Widget for Piano Roll view.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MPIANOROLL_
#define _V2MPIANOROLL_

#include <qwidget.h>
#include <CLAM/Melody.hxx>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/Slotv0.hxx>
#include "SharedTypes.hxx"
#include "V2MPREditDisplay.hxx"

class Q3Frame;
class QScrollBar;
class QPushButton;
class QLabel;
class Q3PopupMenu;

using CLAM::Melody;
using CLAM::MIDIMelody;
using CLAM::PitchNote;
using CLAM::EPitch;
using CLAM::TSize;
using SigSlot::Slotv0;

class V2MPianoRoll : public QWidget
{
	enum dType{dtAudio=0,dtEnergy,dtFund};

	typedef struct
	{
      int pointNumber;
	  int noteNumber;
	  int key;
	  int veloc;
	  PitchNote pitch;
	  bool shared;
	} XCoord;

	typedef struct 
	{
		Array<Point> notePoints;        // note points
		Array<int> octave;              // octaves of the notes
		Array<int> veloc;               // velocity of the notes
		Array<bool> availablePoints;    // points (notes) available
		TData energyMax;				// max value of energy 
		TData freqMax;					// max value of frequency
		int width;						// width of the area for data
		TSize size;						// audio size in samples 
		TData sampleRate;				// sample rate
		Array<XCoord> xInfo;			// info of xcoords x (vertical lines)
	} DataInfo;

	typedef struct
	{
		float xCoords[3];       // 0 -> prior, 1 -> current, 2 -> next
		int xIndex[3];          // 0 -> prior, 1 -> current, 2 -> next
		bool availX[3];         // 0 -> prior, 1 -> current, 2 -> next
		bool shared[3];         // 0 -> prior, 1 -> current, 2 -> next
		float pX[4];            // 0 -> prior2, 1 ->prior1, 2 -> current, 3 -> next
		int pIndex[4];          // 0 -> prior2, 1 ->prior1, 2 -> current, 3 -> next
		bool availP[4];         // 0 -> prior2, 1 ->prior1, 2 -> current, 3 -> next
		bool flag;              // 0 -> back
	} State; // store states in edition

	typedef std::list<V2MDataEditDisplay*> DisplayList;
	typedef std::list<DisplayData> DispData;
	typedef std::list<State> DState;

	Q_OBJECT

public:
	V2MPianoRoll(QWidget* parent);

	void SetData(const Melody& melody,
		         const MIDIMelody& midiMelody,
				 DispData analysisData,
				 bool first=false);

	MIDIMelody GetMIDIMelody();
	bool IsModified();
	void Synchronize(float xcoord);
	void SyncUndo();
	void SyncSave();
	void SyncDiscard();
	void SyncScroll(int);
	
signals:
	void requestSaveMelody();
	void sendXCoord(float);
	void sendUndo();
	void sendSave();
	void sendDiscard();
	void scrollValue(int);

private slots:
	void undo();
	void save();
	void discard();

	void show_signal();
	void show_energy();
	void show_fund();

	void updateViewport(int);

private:
	Q3Frame *displayContainer,*leftPanel,*bottomPanel;
	QScrollBar *scrollBar;
	DisplayList _displayList;
	DispData _displayData;
	Melody _melody;
	MIDIMelody _midiMelody;
	DataInfo dataInfo;
	V2MPREditDisplay *pianoRoll;
	Slotv1<float> mReceivedXCoord;
	Slotv0 mRightMousePressed;
	int _current;
	TData _left_scroll_x;
	bool _modified,_hit;
	DState _state;
	HBounds bound;

	QPushButton *showSignalButton,*showEnergyButton,*showFundButton;
	QPushButton *saveButton;

	QLabel *C,*Db,*D,*Eb,*E,*F,*Gb,*G,*Ab,*A,*Bb,*B;
	QLabel *amplitudeLabel,*energyLabel,*pitchLabel,
		   *midiKeyLabel,*velocityLabel,*midiPitchLabel,
		   *time1Label,*time2Label,*durationLabel;

	Q3PopupMenu *popupMenu;

	void InitLeftPanel();
	void InitBottomPanel();

	void InitDisplayContainer(int w,int h);
	void InitDisplayList();
	void ShowAudioDisplay(dType d);
	void SetAnalysisData(DispData analysisData);
	const DataArray GetSegmentationInfo();

	int GetFrameWidth(int wt,int wref);
	void FillDataStructure(int wt,int wref);
	void ComputeNotesInfo();
	void AddXCoordInfo();
	TData GetYCoord(const EPitch& pitch,TData step);

	void SetSizeHorCursor();
	void SetArrowCursor();	

	void UpdateVLine(float xcoord);
	unsigned GetPixel(float xcoord);
	int Hit(float xcoord);
	void SetCurrent(int index,float xcoord);
	void Update(bool prior = false);
	int GetSharedPointIndex(int index);
	void PurgeXCoord(int index);
	void SetDisplaysMouseTracking(bool mt);
	int FindIndex(float xcoord);
	void UpdateHit(int index,float xcoord);

	void InitPopupMenu();
	void ShowPopupMenu();
	void EnablePopupMenuItems();
	void DisablePopupMenuItems();
	void InitState();
	void PushState(int index);
	void PopState();
	void DiscardChanges();

	void UpdatePanel(TData xcoord,int index);
	void UpdateLabels(TData xcoord,int index,bool back = false);
	void ResetLabels();
	const QString GetPitchStr(const EPitch& pitch);
	void InitBounds();
	void SetBounds();
};

#endif

