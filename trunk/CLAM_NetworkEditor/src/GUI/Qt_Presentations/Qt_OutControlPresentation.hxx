
#ifndef __QT_OUTCONTROLPRESENTATION_HXX__
#define __QT_OUTCONTROLPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include <qregion.h>
#include "OutControlPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_OutControlPresentation : public QWidget, public OutControlPresentation
{
public:
	Qt_OutControlPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_OutControlPresentation();
	virtual void Show();
	virtual void Hide();
	void updatePosition();
	QRegion GetRegion();
protected:
	virtual void OnNewName(const std::string& name);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);


public: // signals
	SigSlot::Signalv2< int , int >  AcquirePos;
	SigSlot::Signalv1< Qt_OutControlPresentation * > AcquireOutControlClicked;

protected:
// qt stuff
	int   mId;
	QRegion mReg;
};


} // namespace NetworkGUI

#endif // __QT_OUTCONTROLPRESENTATION_HXX__
