
#ifndef __QT_INCONTROLPRESENTATION_HXX__
#define __QT_INCONTROLPPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "InControlPresentation.hxx"
#include "Signalv2.hxx"
#include "Signalv1.hxx"

namespace NetworkGUI
{

class Qt_InControlPresentation : public QWidget, public InControlPresentation
{
public:
	Qt_InControlPresentation( int id, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_InControlPresentation();
	virtual void Show();
	virtual void Hide();
protected:
	virtual void OnNewName(const std::string& name);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent *);
public: // signals
	SigSlot::Signalv2< int , int >  AcquirePos;
	SigSlot::Signalv1< Qt_InControlPresentation * > AcquireInControlClicked;

// qt stuff
	int   mId;
};


} // namespace NetworkGUI

#endif // __QT_INCONTROLPRESENTATION_HXX__
