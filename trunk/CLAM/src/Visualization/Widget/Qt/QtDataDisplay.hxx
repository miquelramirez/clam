#ifndef __QtDataDisplay__
#define __QtDataDisplay__

#include <qmainwindow.h>
#include "Viewport.hxx"
class QSizePolicy;
class QScrollBar;
class QSlider;
class QwtScale;

namespace CLAMGUI
{

class QtGLPort;

class QtDataDisplay : public QWidget
{
	Q_OBJECT
public:
	QtDataDisplay( QtGLPort* display, QWidget* parent = 0, const char* name = 0  );

	QtDataDisplay( QWidget* parent = 0, const char* name = 0 );

	virtual ~QtDataDisplay();

	QSize sizeHint();
	
	QSize minimumSize();

	QSizePolicy sizePolicy();

public slots:

	void XAxisZoom( int value );

	void YAxisZoom( int value );

	void XAxisDisp( int value );

	void YAxisDisp( int value );

	void SetupDisplay();

signals:

	void RefreshPlot( const Viewport& );

public:
	 
	virtual void SetupContents( QtGLPort* );

protected:

	class ViewState
	{
	public:
		ViewState( );

		Viewport UpdateXZoom( int value, const Viewport& actualview );

		Viewport UpdateYZoom( int value, const Viewport& actualview );

		Viewport UpdateXScroll( int value, const Viewport& actualview );

		Viewport UpdateYScroll( int value, const Viewport& actualview );

		void SetView( const Viewport& v );

		int GetCurrentXSteps() const;

		int GetCurrentYSteps() const;

		void ResetDisplacement();

	private:
		int    mPrevXf;
		int    mPrevYf;
		float  original_width;
		float  original_height;
		float  actual_width;
		float  actual_height;
		int    currentXStepping;
		int    currentYStepping;

		Viewport mView;

	};


private:
	
	QtGLPort*        mDataPort;
	Viewport*        mView;
	QScrollBar*      mXDisp;
	QScrollBar*      mYDisp;
	QSlider*         mXZoom;
	QSlider*         mYZoom;
	QwtScale*        mXRuler;
	QwtScale*        mYRuler;

	ViewState        mCurrentZoom;


};

}

#endif //QtDataDisplay.hxx
