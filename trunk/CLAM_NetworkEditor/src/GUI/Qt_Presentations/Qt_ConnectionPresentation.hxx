
#ifndef __QT_CONNECTIONPRESENTATION_HXX__
#define __QT_CONNECTIONPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include <qpointarray.h>
#include "ConnectionPresentation.hxx"
#include "Slotv2.hxx"

namespace NetworkGUI
{

class Qt_ConnectionPresentation :  public QWidget, public ConnectionPresentation
{
public:
	Qt_ConnectionPresentation( QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ConnectionPresentation();
	void Show();
	void Hide();
	void setAutoMask(bool b);

protected:    
	void updateMask();
	void paintEvent( QPaintEvent * );
	void OnNewOutPos(int x, int y);
	void OnNewInPos(int x, int y);
	void UpdatePosition();
	void UpdateBezierLine();


	void mousePressEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *);
	void keyPressEvent( QKeyEvent * );
	QPoint origin;
	QPoint end;
	bool mDown;
	QPointArray mPositions;

public: //slots
	SigSlot::Slotv2< int, int > SetOutPos;
	SigSlot::Slotv2< int, int > SetInPos;
};

} // namespace NetworkGUI

#endif //__QT_CONNECTIONPRESENTATION_HXX__
