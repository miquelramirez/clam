/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __QT_NETWORKPRESENTATION_HXX__
#define __QT_NETWORKPRESENTATION_HXX__

#include <string>
#include <qwidget.h>
#include "NetworkPresentation.hxx"
#include "Signalv1.hxx"
#include "Signalv0.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class ProcessingController;
	class ConnectionAdapter;
}

namespace NetworkGUI
{

class Qt_PortConnectionPresentation;
class Qt_ControlConnectionPresentation;
class Qt_InPortPresentation;
class Qt_OutPortPresentation;
class Qt_InControlPresentation;
class Qt_OutControlPresentation;

class Qt_NetworkPresentation :  public QWidget, public NetworkPresentation
{
public:
	Qt_NetworkPresentation( QWidget *parent = 0, const char *name = 0);
	virtual void Show();
	virtual void Hide();
protected:
	void AttachConnectionToPortPresentations( Qt_PortConnectionPresentation * );
	void AttachConnectionToControlPresentations( Qt_ControlConnectionPresentation * );
	virtual void SetName(const std::string& name); 
	virtual void CreateProcessingPresentation( const std::string &, CLAMVM::ProcessingController * );
	virtual void SetPortConnection(CLAMVM::ConnectionAdapter* );
	virtual void SetControlConnection(CLAMVM::ConnectionAdapter* );

	virtual void SetInPortClicked( Qt_InPortPresentation *);
	virtual void SetOutPortClicked( Qt_OutPortPresentation *);
	virtual void SetInControlClicked( Qt_InControlPresentation *);
	virtual void SetOutControlClicked( Qt_OutControlPresentation *);

	void paintEvent( QPaintEvent * );
	void mouseMoveEvent( QMouseEvent *);
	void mouseReleaseEvent( QMouseEvent *m);
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
	
	Qt_InPortPresentation* mInPortSelected;
	Qt_OutPortPresentation* mOutPortSelected;
	Qt_InControlPresentation* mInControlSelected;
	Qt_OutControlPresentation* mOutControlSelected;
	QPoint mMousePos;

public: // slots
	SigSlot::Slotv1< Qt_InPortPresentation * > SlotSetInPortClicked;
	SigSlot::Slotv1< Qt_OutPortPresentation * > SlotSetOutPortClicked;
	SigSlot::Slotv1< Qt_InControlPresentation * > SlotSetInControlClicked;
	SigSlot::Slotv1< Qt_OutControlPresentation * > SlotSetOutControlClicked;
	// signals
	SigSlot::Signalv1< const QPoint & > SignalAcquireOutPortAfterClickInPort;
	SigSlot::Signalv1< const QPoint & > SignalAcquireInPortAfterClickOutPort;
	SigSlot::Signalv1< const QPoint & > SignalAcquireOutControlAfterClickInControl;
	SigSlot::Signalv1< const QPoint & > SignalAcquireInControlAfterClickOutControl;
	SigSlot::Signalv1< const std::string& > SignalSendNewMessageToStatus;
	SigSlot::Signalv0 SignalProcessingCreated;


private:
	const std::string GetCompleteNameFromInPortSelected();
	const std::string GetCompleteNameFromOutPortSelected();
	const std::string GetCompleteNameFromInControlSelected();
	const std::string GetCompleteNameFromOutControlSelected();

};

} // namespace NetworkGUI

#endif //__QT_NETWORKPRESENTATION_HXX__
