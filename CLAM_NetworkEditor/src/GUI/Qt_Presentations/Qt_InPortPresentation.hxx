
#ifndef __QT_INPORTPRESENTATION_HXX__
#define __QT_INPORTPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "InPortPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_InPortPresentation : public QWidget, public InPortPresentation
{
public:
	Qt_InPortPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_InPortPresentation();
	virtual void Show();
	virtual void Hide();
protected:
	virtual void OnNewName(const std::string& name);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);
public: // signals
	SigSlot::Signalv2< int , int >  AcquirePos;
	SigSlot::Signalv1< Qt_InPortPresentation * > AcquireInPortClicked;

// qt stuff
	int   mId;
};


} // namespace NetworkGUI

#endif // __QT_INPORTPRESENTATION_HXX__
