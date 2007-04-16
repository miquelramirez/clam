/*
 * V2MSegmentationEditor.hxx
 * Interface for V2MSegmentationEditor class
 *
 * Widget for the segmentation view.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MSEGMENTATIONEDITOR_
#define _V2MSEGMENTATIONEDITOR_

#include <list>
#include <CLAM/Melody.hxx>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/Slotv0.hxx>
#include "SharedTypes.hxx"
#include "V2MDataEditDisplay.hxx"

class QScrollBar;
class Q3Frame;
class QLabel;
class QPushButton;
class Q3PopupMenu;

using CLAM::Melody;
using CLAM::MIDIMelody;
using CLAM::TSize;

using CLAM::Array;
using CLAM::TData;
using CLAM::DataArray;
using SigSlot::Slotv0;

class V2MSegmentationEditor : public QWidget
{
	typedef struct
	{
      int pointNumber;
	  int noteNumber;
	  TData energy;
	  TData freq;
	  bool shared;
	} XCoord;

	typedef struct 
	{
		Array<Point> EnergyPoints;   // energy data
		Array<Point> FundPoints;     // f0 data
		Array<bool> availablePoints; // available points
		TData energyMax;             // max value of energy 
		TData freqMax;               // max value of frequency
		int width;                   // width of the area of data
		TSize size;                  // audio size in samples 
		TData sampleRate;            // sample rate
		Array<XCoord> xInfo;         // xcoords info (vertical lines)
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
	} State; // store the state in edition

	typedef std::list<V2MDataEditDisplay*> DisplayList;
	typedef std::list<DisplayData> DispData;
	typedef std::list<State> DState;

	Q_OBJECT

public:
	V2MSegmentationEditor(QWidget* parent);
	~V2MSegmentationEditor();

	void SetData(const Melody& melody,DispData analysisData,bool first=false);
	void Synchronize(float xcoord);
	void SyncUndo();
	void SyncSave();
	void SyncDiscard();
	void SyncScroll(int);
	bool IsModified();
	Melody GetMelody();
	void SaveMelody();

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

	void updateViewport(int);

private:
	Q3Frame *displayContainer,*panel;
	QScrollBar* scrollBar;
	DisplayList _displayList;
	DispData _displayData;
	Melody _melody;
	DataInfo dataInfo;
	Slotv1<float> mReceivedXCoord;
	int _current;
	TData _left_scroll_x;
	bool _modified,_hit;
	DState _state;
	Slotv0 mRightMousePressed;
	HBounds bound;

	QLabel *time1Label,*time2Label,*durationLabel,
		   *amplitudeLabel,*energy1Label,*pitch1Label,
		   *energy2Label,*pitch2Label,*segLabel; // etiquetas del panel

	QPushButton *saveButton;
	Q3PopupMenu *popupMenu;

	void InitPanel();
	int GetFrameWidth(int wt,int wref);
	void InitDisplayContainer(int w,int h);
	void SetAnalysisData(DispData analysisData);
	void FillDataStructure(int wt,int wref);
	void InitDisplayList();
	void InitPopupMenu();
	void ShowPopupMenu();
	void EnablePopupMenuItems();
	void DisablePopupMenuItems();
	void InitState();
	const DataArray GetSegmentationInfo(); 
	void UpdateVLine(float xcoord);
	
	void ComputeFundAndEnergyPoints();
	void AddXCoordInfo();	
	
	unsigned GetPixel(float xcoord);
	int Hit(float xcoord);
	void SetCurrent(int index,float xcoord);
	void Update(bool prior = false);
	int GetSharedPointIndex(int index);
	void PurgeXCoord(int index);
	void SetDisplaysMouseTracking(bool mt);
	void SetSizeHorCursor();
	void SetArrowCursor();	
    int FindIndex(float xcoord);
	void UpdateHit(int index,float xcoord);
	void UpdatePanel(TData xcoord,int index);
	void UpdateLabels(TData xcoord,int index,bool back = false);
	void ResetLabels();
	void PushState(int index);
	void PopState();
	void UpdateChanges();
	void DiscardChanges();

	void InitBounds();
	void SetBounds();
};

#endif

