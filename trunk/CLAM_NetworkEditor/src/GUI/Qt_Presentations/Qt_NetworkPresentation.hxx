
#ifndef __QT_NETWORKPRESENTATION_HXX__
#define __QT_NETWORKPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include <qlabel.h>
#include "NetworkPresentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
	class ProcessingAdapter;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class Qt_ConnectionPresentation;
class Qt_InPortPresentation;
class Qt_OutPortPresentation;

class Qt_NetworkPresentation :  public QWidget, public NetworkPresentation
{
public:
	Qt_NetworkPresentation( QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_NetworkPresentation();
	virtual void Show();
	virtual void Hide();
protected:
	void AttachConnectionToPortPresentations( Qt_ConnectionPresentation * );
	virtual void OnNewName(const std::string& name); 
	virtual void OnNewProcessing(CLAMVM::ProcessingAdapter*, const std::string & name );
	virtual void OnNewConnection(CLAMVM::ConnectionAdapter* );
	virtual void OnNewInPortClicked( Qt_InPortPresentation *);
	virtual void OnNewOutPortClicked( Qt_OutPortPresentation *);
	void paintEvent( QPaintEvent * );
	void mouseMoveEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *m);
	QLabel mNameLabel;

	Qt_InPortPresentation* mInPortSelected;
	Qt_OutPortPresentation* mOutPortSelected;
	QPoint mMousePos;

public: // slots
	SigSlot::Slotv1< Qt_InPortPresentation * > SetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SetOutPortClicked;
	// signals
	SigSlot::Signalv1< const QPoint & > AcquireOutPortAfterClickInPort;
	SigSlot::Signalv1< const QPoint & > AcquireInPortAfterClickOutPort;
	SigSlot::Signalv1< const std::string& > SendNewMessageToStatus;


private:
	const std::string GetCompleteNameFromInPortSelected();
	const std::string GetCompleteNameFromOutPortSelected();
};

} // namespace NetworkGUI

#endif //__QT_NETWORKPRESENTATION_HXX__
