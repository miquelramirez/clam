
#ifndef __QT_OUTPORTPRESENTATION_HXX__
#define __QT_OUTPORTPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "OutPortPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_OutPortPresentation : public QWidget, public OutPortPresentation
{
public:
	Qt_OutPortPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_OutPortPresentation();
	virtual void Show();
	virtual void Hide();
protected:
	virtual void OnNewName(const std::string& name);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);

public: // signals
	SigSlot::Signalv2< int , int >  AcquirePos;
	SigSlot::Signalv1< Qt_OutPortPresentation * > AcquireOutPortClicked;
// qt stuff
	int   mId;
	bool        mDown;
};


} // namespace NetworkGUI

#endif // __QT_OUTPORTPRESENTATION_HXX__
