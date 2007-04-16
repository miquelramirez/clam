/*
 * V2MDataEditDisplay.hxx
 * Interface for V2MDataEditDisplay class
 *
 * This class inherits from V2MDataDisplay and incorporates functionality for editing
 * and viewing the segments of the notes obtained on melody extraction.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MDATAEDITDISPLAY_
#define _V2MDATAEDITDISPLAY_

#include "V2MDataDisplay.hxx"
#include <CLAM/Point.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv0.hxx>

using CLAM::Array;
using CLAM::Point;
using SigSlot::Signalv0;
using SigSlot::Slotv0;

enum dataType { dtAudio = 0, dtFundEnergy };

class V2MDataEditDisplay : public V2MDataDisplay
{
public:
	V2MDataEditDisplay(QWidget* parent);
	virtual ~V2MDataEditDisplay();

	void SetDataType(dataType dt);
	void SetPoints(const Array<Point>& points);
	int GetnXCoords();
	TData GetXCoord(int index);
	void UpdateXCoord(int index,TData value);
	void DeleteXCoord(int index,bool dd = false);
	TData GetPointX(int index);
	void UpdatePoint(int pointNumber,TData xcoord);
	void DeletePoint(int index);
	bool IsAvailableXCoord(int index);
	int GetNextXCoordAvailableIndex(int index);
	int GetPriorXCoordAvailableIndex(int index);
	void WakeUpXCoord(int index);
	void WakeUpPoint(int pointNumber);
	void RightButtonMousePressedSlot(Slotv0& slot);
	const Array<Point> GetPoints();
	const Array<int> GetPointsIndex();

protected:
	Array<Point> _points;
	Array<bool> availablePoint;

	virtual void paintGL(); 

	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent * e);

	int GetDrawMode(int pIndex);

private:
	dataType _dataType;
	Signalv0 _sendRightButtonPressed;

	void DrawDataLines();	
	void DrawHLine(int pIndex,int mode);
};

#endif

