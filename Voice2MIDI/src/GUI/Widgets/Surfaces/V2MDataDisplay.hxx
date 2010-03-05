/*
 * V2MDataDisplay.hxx
 * Interface for V2MDataDisplay class
 *
 * Display surface for data viewing.
 * The rendering is performed through the Qt interface for the OpenGL API.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _V2MDATADISPLAY_
#define _V2MDATADISPLAY_

#include <QtOpenGL/QGLWidget>
#include <CLAM/Array.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>
#include "SharedTypes.hxx"

using CLAM::TData;
using CLAM::Array;
using CLAM::DataArray;
using CLAM::TSize;
using SigSlot::Signalv1;
using SigSlot::Slotv1;

class QMouseEvent;

class V2MDataDisplay : public QGLWidget
{
	typedef struct
	{
		TData left;
		TData right;
		TData bottom;
		TData top;
	} View;

	typedef struct
	{
		float r;
		float g;
		float b;
	} Color;
	
public:
	V2MDataDisplay(QWidget* parent);
	virtual ~V2MDataDisplay();

	void SetBounds(TData left,TData right);
	void SetView(TData leff,TData right,TData bottom,TData top);
	void SetData(const DataArray& data);
	void SetDrawVLineEnabled(bool s);
	bool NeedVLines();
	void SetGlobalXCoord(const DataArray& xcoords);
	void SetColor(float r,float g,float b);
	void SetXCoordSlot(Slotv1<float>& slot);
	void SetSubSamplingFactor(int factor);

protected:
	HBounds bound;
	View mView;
	float xcoord;
	DataArray globalXCoord;
	Array<bool> availableXCoord;
	Signalv1<float> mSendXCoord;

	virtual void initializeGL(); 
	virtual void resizeGL(int w,int h); 
	virtual void paintGL(); 

	virtual void mousePressEvent(QMouseEvent* e);

	void DrawVLines();

private:
	DataArray mData;
	int subSamplingFactor;
	Color color;
	bool needVLine;
	
	void InitBounds();
	void InitView();
	void SetDefaultColor();
};

#endif


