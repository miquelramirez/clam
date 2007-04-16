/*
 * V2MAnalysisDisplay.hxx
 * Interface for V2MAnalysisDisplay class
 * This class implements the widget for viewing the shape of the wave of the original
 * audio, frame to frame of the results obtained during the analysis, and dynamic
 * viewing on playing and capturing.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MANALYSISDISPLAY_
#define _V2MANALYSISDISPLAY_

#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <Q3Frame>
#include "SharedTypes.hxx"
#include <CLAM/Slotv1.hxx>

class Q3Frame;
class QPushButton;
class QLabel;
class QLCDNumber;

class V2MDataDisplay;

using CLAM::DataArray;
using SigSlot::Slotv1;

class V2MAnalysisDisplay : public QWidget
{
	typedef std::list<V2MDataDisplay*> DisplayList;
	typedef std::list<DisplayData> DispData;

	typedef struct
	{
	    int nSamples;
	    TData sampleRate;
	    DataArray data;
	    TData dur;
	    int pos;
	    int lastPos;
	    int frameSize;
	    int rest;
	    int totalFrames;
	    int nFrame;
	}FramesInfo;

	Q_OBJECT

public:
	V2MAnalysisDisplay(QWidget* parent);
	~V2MAnalysisDisplay();

	void InitData();
	void SetAudioData(std::list<DisplayData> data);
	void SetAnalysisData(std::list<DisplayData> data);
	void PlotOriginalAudio();
	void ShowCurrentFrame();
	void ShowAudioDisplay();
	void ShowAllData();
	bool IsAudioDisplayVisible();
	void SetInfoAnalysisPanelEnabled(bool en);
	void HideEnergyAndFundDisplays();
	void InitDisplayForDynamicBehavior();
	void ResetAnalysisLabels();
	void UpdateDynamicDisplay(DataArray& data);	
	const std::list<DisplayData> GetAnalysisData();

public slots:
	void hide();

private slots:
	// navigation buttons
	void FirstFrame();
	void PriorFrame();
	void NextFrame();
	void LastFrame();

private:
	FramesInfo framesInfo; // structure for info frame

	Q3Frame* infoAnalysisPanel; // panel
	
	QLabel *timeLabel,*totalTimeLabel,*amplitudeLabel,*energyLabel,*fundLabel; // labels of the panel
	QPushButton *first,*last,*prior,*next; 
	QLCDNumber *frameNum; // frame number

	Slotv1<float> mReceivedXCoord; // aim to draw vertical line

	// data and display lists
	DisplayList _displayList;
	DispData _displayData;

	float _lastX;

	bool hasAnalysis;
	bool isAudioDisplayVisible;
	bool buttonsEnabled;
	bool vline;

	void InitDisplay();
	void InitFramesInfo(const DispData::iterator it);
	void DisplayFrames();
	void DisplayCurrentFrameAndVLines(float xcoord);
	void UpdateAnalysisPlots(float xcoord);
	void SetAudioDisplayVisible(bool v);
	void ResetLabels();
	void EnableSpeedButtons();
	void DisableSpeedButtons();
	void UpdatePanelAndVLine(float xcoord,bool full = false);
	DataArray GetXUpdated(bool full = false);
};

#endif

