
#ifndef __QT_PROCESSINGPRESENTATION_HXX__
#define __QT_PROCESSINGPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "ProcessingPresentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include "QTConfigurator.hxx"


namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class InPortAdapter;
	class OutPortAdapter;
}

namespace NetworkGUI
{

class Qt_InPortPresentation; 
class Qt_OutPortPresentation; 

class Qt_ProcessingPresentation : public QWidget, public ProcessingPresentation
{
public:
	Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingPresentation();
	virtual void Show();
	virtual void Hide();
	void EmitPositionOfPorts();
protected:
	virtual void OnNewObservedClassName(const std::string& name);
	virtual void OnNewInPort( CLAMVM::InPortAdapter* );
	virtual void OnNewOutPort( CLAMVM::OutPortAdapter* );	
	virtual void OnNewInPortClicked( Qt_InPortPresentation *);
	virtual void OnNewOutPortClicked( Qt_OutPortPresentation *);
	virtual void OnNewOutPortAfterClickInPort(const QPoint &);
	virtual void OnNewInPortAfterClickOutPort(const QPoint &);

	void paintEvent( QPaintEvent * );
	void mousePressEvent( QMouseEvent * );
	void mouseReleaseEvent( QMouseEvent * );
	void mouseMoveEvent( QMouseEvent * );
	void keyPressEvent( QKeyEvent * );

// qt stuff
	bool        mDown;
	QPoint     mClickPos;

public: // signals
	SigSlot::Signalv1< Qt_InPortPresentation * > AcquireInPortClicked;
	SigSlot::Signalv1< Qt_OutPortPresentation * > AcquireOutPortClicked;
	SigSlot::Signalv1< CLAM::ProcessingConfig * > EditConfiguration;
public: // slots
	SigSlot::Slotv1< Qt_InPortPresentation * > SetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SetOutPortClicked;
	SigSlot::Slotv1< const QPoint & > SetOutPortAfterClickInPort;
	SigSlot::Slotv1< const QPoint & > SetInPortAfterClickOutPort;
};


} // namespace NetworkGUI

#endif // __QT_PROCESSINGPRESENTATION_HXX__
